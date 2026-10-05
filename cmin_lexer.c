/*
 * Cmin Lexical Analyzer
 *
 * Input : <filename>.cmin
 * Output: <same filename>.txt
 *
 * The token numbers are intentionally fixed so that the sample
 * required by the proposal is produced:
 *   NUMBER     = 10
 *   IDENTIFIER = 11
 *   PLUS       = 21
 *   DIVIDE     = 24
 *   LPAREN     = 25
 *   RPAREN     = 26
 *   EOF        = -1
 *
 * Cmin characteristics implemented from the proposal:
 * - identifiers: [a-zA-Z_][a-zA-Z0-9_]*
 * - Cmin keywords/reserved words
 * - integer and floating-point numeric literals
 * - character and string literals
 * - arithmetic, comparison, assignment, bitwise and logical operators
 * - (, ), {, }, [, ], ,, ., :, ?
 * - C-style line comments and block comments
 * - whitespace is ignored
 * - Cmin normally does not require semicolons; a semicolon is ignored
 *   by the lexer when encountered.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEXEME 1024

/* Token codes. The values 10, 11, 21, 24, 25 and 26 match the
   example in the proposal/request. */
enum TokenType {
    TOK_NUMBER = 10,
    TOK_IDENTIFIER = 11,

    TOK_IF = 12,
    TOK_ELSE = 13,
    TOK_WHILE = 14,
    TOK_CONTINUE = 15,
    TOK_BREAK = 16,
    TOK_RETURN = 17,
    TOK_TRY = 18,
    TOK_CATCH = 19,

    TOK_MINUS = 20,
    TOK_PLUS = 21,
    TOK_MULTIPLY = 22,
    TOK_MODULO = 23,
    TOK_DIVIDE = 24,

    TOK_LPAREN = 25,
    TOK_RPAREN = 26,
    TOK_LBRACE = 27,
    TOK_RBRACE = 28,
    TOK_LBRACKET = 29,
    TOK_RBRACKET = 30,

    TOK_ASSIGN = 31,
    TOK_EQUAL = 32,
    TOK_NOT_EQUAL = 33,
    TOK_LESS = 34,
    TOK_GREATER = 35,
    TOK_LESS_EQUAL = 36,
    TOK_GREATER_EQUAL = 37,

    TOK_INCREMENT = 38,
    TOK_DECREMENT = 39,
    TOK_EXPONENT = 40,

    TOK_PLUS_EQUAL = 41,
    TOK_MINUS_EQUAL = 42,
    TOK_MULTIPLY_EQUAL = 43,
    TOK_DIVIDE_EQUAL = 44,
    TOK_MODULO_EQUAL = 45,
    TOK_EXPONENT_EQUAL = 46,

    TOK_LEFT_SHIFT = 47,
    TOK_RIGHT_SHIFT = 48,
    TOK_LEFT_SHIFT_EQUAL = 49,
    TOK_RIGHT_SHIFT_EQUAL = 50,

    TOK_BIT_AND = 51,
    TOK_BIT_OR = 52,
    TOK_XOR = 53,
    TOK_LOGICAL_AND = 54,
    TOK_LOGICAL_OR = 55,
    TOK_NOT = 56,

    TOK_COMMA = 57,
    TOK_DOT = 58,
    TOK_COLON = 59,
    TOK_QUESTION = 60,

    TOK_STRING = 61,
    TOK_CHAR = 62,

    TOK_ERROR = 99,
    TOK_EOF = -1
};

typedef struct {
    const char *word;
    int token;
} Keyword;

static const Keyword keywords[] = {
    {"if", TOK_IF},
    {"else", TOK_ELSE},
    {"while", TOK_WHILE},
    {"continue", TOK_CONTINUE},
    {"break", TOK_BREAK},
    {"return", TOK_RETURN},
    {"try", TOK_TRY},
    {"catch", TOK_CATCH},

    /* Cmin reserved words / data types / modifiers */
    {"int", TOK_IDENTIFIER},
    {"char", TOK_IDENTIFIER},
    {"string", TOK_IDENTIFIER},
    {"float", TOK_IDENTIFIER},
    {"double", TOK_IDENTIFIER},
    {"short", TOK_IDENTIFIER},
    {"long", TOK_IDENTIFIER},
    {"bool", TOK_IDENTIFIER},
    {"struct", TOK_IDENTIFIER},
    {"signed", TOK_IDENTIFIER},
    {"unsigned", TOK_IDENTIFIER},
    {"const", TOK_IDENTIFIER},
    {"global", TOK_IDENTIFIER},
    {"typedef", TOK_IDENTIFIER},
    {"anon", TOK_IDENTIFIER},
    {"include", TOK_IDENTIFIER}
};

#define KEYWORD_COUNT (sizeof(keywords) / sizeof(keywords[0]))

static int is_cmin_reserved(const char *lexeme) {
    size_t i;

    for (i = 0; i < KEYWORD_COUNT; ++i) {
        if (strcmp(lexeme, keywords[i].word) == 0) {
            return 1;
        }
    }
    return 0;
}

static int get_keyword_token(const char *lexeme) {
    size_t i;

    for (i = 0; i < KEYWORD_COUNT; ++i) {
        if (strcmp(lexeme, keywords[i].word) == 0) {
            /* The proposal calls these words reserved, while the
               executable keyword subset is represented by 12-19.
               Type names/modifiers are still emitted as identifiers
               here only if the project wants one generic identifier
               class. Change this function if separate token classes
               are required later. */
            if (keywords[i].token != TOK_IDENTIFIER)
                return keywords[i].token;
            return TOK_IDENTIFIER;
        }
    }

    return TOK_IDENTIFIER;
}

static void print_token(FILE *out, int token, const char *lexeme) {
    fprintf(out, "Next token is: %d Next lexeme is %s\n", token, lexeme);
}

static int read_string_or_char(FILE *in, char *lexeme, int opening) {
    int c;
    int escaped = 0;
    size_t i = 0;

    lexeme[i++] = (char)opening;

    while ((c = fgetc(in)) != EOF) {
        if (i < MAX_LEXEME - 2)
            lexeme[i++] = (char)c;

        if (escaped) {
            escaped = 0;
            continue;
        }

        if (c == '\\') {
            escaped = 1;
        } else if (c == opening) {
            lexeme[i] = '\0';
            return opening == '"' ? TOK_STRING : TOK_CHAR;
        } else if (c == '\n') {
            break;
        }
    }

    lexeme[i] = '\0';
    return TOK_ERROR;
}

static int get_next_token(FILE *in, char *lexeme, int *line) {
    int c, next;
    size_t i;

    lexeme[0] = '\0';

    /* Skip whitespace and comments. */
    while (1) {
        c = fgetc(in);

        if (c == EOF)
            return TOK_EOF;

        if (c == ' ' || c == '\t' || c == '\r') {
            continue;
        }

        if (c == '\n') {
            (*line)++;
            continue;
        }

        if (c == ';') {
            /* Cmin does not require semicolons. */
            continue;
        }

        if (c == '/') {
            next = fgetc(in);

            if (next == '/') {
                /* Inline comment. */
                while ((c = fgetc(in)) != EOF && c != '\n')
                    ;
                if (c == '\n')
                    (*line)++;
                continue;
            }

            if (next == '*') {
                /* Block comment. */
                int previous = 0;
                int closed = 0;

                while ((c = fgetc(in)) != EOF) {
                    if (c == '\n')
                        (*line)++;

                    if (previous == '*' && c == '/') {
                        closed = 1;
                        break;
                    }

                    previous = c;
                }

                if (!closed) {
                    strcpy(lexeme, "/*");
                    return TOK_ERROR;
                }

                continue;
            }

            ungetc(next, in);
            strcpy(lexeme, "/");
            return TOK_DIVIDE;
        }

        break;
    }

    /* Identifier / keyword:
       [a-zA-Z_][a-zA-Z0-9_]* */
    if (isalpha((unsigned char)c) || c == '_') {
        i = 0;

        do {
            if (i < MAX_LEXEME - 1)
                lexeme[i++] = (char)c;

            c = fgetc(in);
        } while (isalnum((unsigned char)c) || c == '_');

        lexeme[i] = '\0';

        if (c != EOF)
            ungetc(c, in);

        if (is_cmin_reserved(lexeme)) {
            int token = get_keyword_token(lexeme);

            /* Keep Cmin data types/modifiers as reserved tokens.
               This prevents them from being mistaken for user identifiers. */
            if (token == TOK_IDENTIFIER) {
                /* Assign a stable generic reserved-word token range. */
                if (strcmp(lexeme, "int") == 0) return 70;
                if (strcmp(lexeme, "char") == 0) return 71;
                if (strcmp(lexeme, "string") == 0) return 72;
                if (strcmp(lexeme, "float") == 0) return 73;
                if (strcmp(lexeme, "double") == 0) return 74;
                if (strcmp(lexeme, "short") == 0) return 75;
                if (strcmp(lexeme, "long") == 0) return 76;
                if (strcmp(lexeme, "bool") == 0) return 77;
                if (strcmp(lexeme, "struct") == 0) return 78;
                if (strcmp(lexeme, "signed") == 0) return 79;
                if (strcmp(lexeme, "unsigned") == 0) return 80;
                if (strcmp(lexeme, "const") == 0) return 81;
                if (strcmp(lexeme, "global") == 0) return 82;
                if (strcmp(lexeme, "typedef") == 0) return 83;
                if (strcmp(lexeme, "anon") == 0) return 84;
                if (strcmp(lexeme, "include") == 0) return 85;
            }

            return token;
        }

        return TOK_IDENTIFIER;
    }

    /* Numeric literals: integers and decimals. */
    if (isdigit((unsigned char)c)) {
        int has_dot = 0;
        i = 0;

        do {
            if (i < MAX_LEXEME - 1)
                lexeme[i++] = (char)c;

            c = fgetc(in);

            if (c == '.' && !has_dot) {
                has_dot = 1;
                if (i < MAX_LEXEME - 1)
                    lexeme[i++] = (char)c;
                c = fgetc(in);
            }
        } while (isdigit((unsigned char)c));

        lexeme[i] = '\0';

        if (c != EOF)
            ungetc(c, in);

        return TOK_NUMBER;
    }

    /* String / character literals. */
    if (c == '"' || c == '\'')
        return read_string_or_char(in, lexeme, c);

    /* Multi-character operators. */
    switch (c) {
        case '+':
            next = fgetc(in);
            if (next == '+') { strcpy(lexeme, "++"); return TOK_INCREMENT; }
            if (next == '=') { strcpy(lexeme, "+="); return TOK_PLUS_EQUAL; }
            if (next != EOF) ungetc(next, in);
            strcpy(lexeme, "+");
            return TOK_PLUS;

        case '-':
            next = fgetc(in);
            if (next == '-') { strcpy(lexeme, "--"); return TOK_DECREMENT; }
            if (next == '=') { strcpy(lexeme, "-="); return TOK_MINUS_EQUAL; }
            if (next != EOF) ungetc(next, in);
            strcpy(lexeme, "-");
            return TOK_MINUS;

        case '*':
            next = fgetc(in);
            if (next == '*') {
                int third = fgetc(in);
                if (third == '=') {
                    strcpy(lexeme, "**=");
                    return TOK_EXPONENT_EQUAL;
                }
                if (third != EOF)
                    ungetc(third, in);
                strcpy(lexeme, "**");
                return TOK_EXPONENT;
            }
            if (next == '=') { strcpy(lexeme, "*="); return TOK_MULTIPLY_EQUAL; }
            if (next != EOF) ungetc(next, in);
            strcpy(lexeme, "*");
            return TOK_MULTIPLY;

        case '%':
            next = fgetc(in);
            if (next == '=') { strcpy(lexeme, "%="); return TOK_MODULO_EQUAL; }
            if (next != EOF) ungetc(next, in);
            strcpy(lexeme, "%");
            return TOK_MODULO;

        case '=':
            next = fgetc(in);
            if (next == '=') { strcpy(lexeme, "=="); return TOK_EQUAL; }
            if (next != EOF) ungetc(next, in);
            strcpy(lexeme, "=");
            return TOK_ASSIGN;

        case '!':
            next = fgetc(in);
            if (next == '=') { strcpy(lexeme, "!="); return TOK_NOT_EQUAL; }
            if (next != EOF) ungetc(next, in);
            strcpy(lexeme, "!");
            return TOK_NOT;

        case '<':
            next = fgetc(in);
            if (next == '=') { strcpy(lexeme, "<="); return TOK_LESS_EQUAL; }
            if (next == '<') {
                int third = fgetc(in);
                if (third == '=') {
                    strcpy(lexeme, "<<=");
                    return TOK_LEFT_SHIFT_EQUAL;
                }
                if (third != EOF) ungetc(third, in);
                strcpy(lexeme, "<<");
                return TOK_LEFT_SHIFT;
            }
            if (next != EOF) ungetc(next, in);
            strcpy(lexeme, "<");
            return TOK_LESS;

        case '>':
            next = fgetc(in);
            if (next == '=') { strcpy(lexeme, ">="); return TOK_GREATER_EQUAL; }
            if (next == '>') {
                int third = fgetc(in);
                if (third == '=') {
                    strcpy(lexeme, ">>=");
                    return TOK_RIGHT_SHIFT_EQUAL;
                }
                if (third != EOF) ungetc(third, in);
                strcpy(lexeme, ">>");
                return TOK_RIGHT_SHIFT;
            }
            if (next != EOF) ungetc(next, in);
            strcpy(lexeme, ">");
            return TOK_GREATER;

        case '&':
            next = fgetc(in);
            if (next == '&') { strcpy(lexeme, "&&"); return TOK_LOGICAL_AND; }
            if (next != EOF) ungetc(next, in);
            strcpy(lexeme, "&");
            return TOK_BIT_AND;

        case '|':
            next = fgetc(in);
            if (next == '|') { strcpy(lexeme, "||"); return TOK_LOGICAL_OR; }
            if (next != EOF) ungetc(next, in);
            strcpy(lexeme, "|");
            return TOK_BIT_OR;

        case '^':
            strcpy(lexeme, "^");
            return TOK_XOR;

        case '(':
            strcpy(lexeme, "(");
            return TOK_LPAREN;

        case ')':
            strcpy(lexeme, ")");
            return TOK_RPAREN;

        case '{':
            strcpy(lexeme, "{");
            return TOK_LBRACE;

        case '}':
            strcpy(lexeme, "}");
            return TOK_RBRACE;

        case '[':
            strcpy(lexeme, "[");
            return TOK_LBRACKET;

        case ']':
            strcpy(lexeme, "]");
            return TOK_RBRACKET;

        case ',':
            strcpy(lexeme, ",");
            return TOK_COMMA;

        case '.':
            strcpy(lexeme, ".");
            return TOK_DOT;

        case ':':
            strcpy(lexeme, ":");
            return TOK_COLON;

        case '?':
            strcpy(lexeme, "?");
            return TOK_QUESTION;

        default:
            lexeme[0] = (char)c;
            lexeme[1] = '\0';
            return TOK_ERROR;
    }
}

static int has_cmin_extension(const char *filename) {
    const char *dot = strrchr(filename, '.');
    return dot != NULL && strcmp(dot, ".cmin") == 0;
}

static void make_output_filename(const char *input, char *output, size_t size) {
    const char *dot = strrchr(input, '.');

    if (dot != NULL && strcmp(dot, ".cmin") == 0) {
        size_t base_len = (size_t)(dot - input);
        if (base_len + 5 >= size)
            base_len = size - 5;

        memcpy(output, input, base_len);
        output[base_len] = '\0';
        strcat(output, ".txt");
    } else {
        snprintf(output, size, "%s.txt", input);
    }
}

int main(int argc, char *argv[]) {
    FILE *input;
    FILE *output;
    char output_name[1024];
    char lexeme[MAX_LEXEME];
    int token;
    int line = 1;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file.cmin>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!has_cmin_extension(argv[1])) {
        fprintf(stderr, "Error: input file must have a .cmin extension.\n");
        return EXIT_FAILURE;
    }

    input = fopen(argv[1], "r");
    if (input == NULL) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    make_output_filename(argv[1], output_name, sizeof(output_name));

    output = fopen(output_name, "w");
    if (output == NULL) {
        perror("Error creating output file");
        fclose(input);
        return EXIT_FAILURE;
    }

    fprintf(output, "Cmin Lexical Analyzer Symbol Table\n");
    fprintf(output, "Input file: %s\n\n", argv[1]);

    do {
        lexeme[0] = '\0';
        token = get_next_token(input, lexeme, &line);

        if (token == TOK_EOF) {
            print_token(output, TOK_EOF, "EOF");
            break;
        }

        if (token == TOK_ERROR) {
            fprintf(output,
                    "Lexical error on line %d: invalid lexeme \"%s\"\n",
                    line, lexeme);
            fclose(input);
            fclose(output);
            fprintf(stderr, "Lexical error on line %d. See %s\n",
                    line, output_name);
            return EXIT_FAILURE;
        }

        print_token(output, token, lexeme);
    } while (1);

    fclose(input);
    fclose(output);

    printf("Lexical analysis complete.\n");
    printf("Output written to: %s\n", output_name);

    return EXIT_SUCCESS;
}