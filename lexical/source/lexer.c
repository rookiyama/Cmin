#ifndef _TOKEN
#define _TOKEN
#include "../source/token.c"
#endif
#ifndef _SOURCE
#define _SOURCE
#include "../source/scan.c"
#endif

void main() {
    fileSearch();
    if ((fptr = fopen(fileName, "r")) == NULL)
        printf("ERROR - file not found\n");
    else {
        if (!debugVal) {
            printf("ERROR - unknown file extension found\n");
            fptrErr = fopen("../bin/err.txt", "w");
            fprintf(fptrErr, "ERROR - unknown file extension found\n");
            fclose(fptrErr);
            return;
        }
        remove("../bin/err.txt");
        fptr2 = fopen("../bin/symbol_table.txt", "w");
        fprintf(fptr2, "%-15s%-15s%-15s\n\n", "Token Type", "Lexeme", "Token Value");
        getChar();
        do {
            analyzer();
        } while(nextToken != EOF_);
    }
    fclose(fptr2);
    fclose(fptr);
    return;
}

void getChar() {
    if ((nextChar = getc(fptr)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else if (nextChar == 10)
            charClass = NEWLINE;
        else if (nextChar == '#')
            charClass = HASH;
        else if (nextChar == '\\')
            charClass = ESCCHAR;
        else
            charClass = UNKNOWN;
    } else
        charClass = EOF;
}

void addChar() {
    if (lexLen <= 1024) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else
        printf("ERROR - lexeme too long\n");
}

void getNonBlank() {
    while(nextChar < 33 && nextChar != 10 && nextChar != EOF) {
        getChar();
    }
}

int analyzer() {
    lexLen = 0;
    getNonBlank();
    switch(charClass) {
        case LETTER:
        do {
            addChar();
            getChar();
        } while (charClass == LETTER || charClass == DIGIT || nextChar == '_');
        nextToken = keyword();
        break;
        case ESCCHAR: //last issue
            addChar();
            getChar();
        case DIGIT:
        bool dotCounter = 0;
        do {
            addChar();
            getChar();
            if (nextChar == '.' && !dotCounter)
                dotCounter = 1;
            else if (nextChar == '.' && dotCounter)
                break;
        } while (charClass == DIGIT || nextChar == '.');
        if (isspace(nextChar)) {
            nextToken = CONSTANT;
            break;
        } do {
            addChar();
            getChar();
        } while (!isspace(nextChar));
        nextToken = UNKNOWN;
        break;
        case NEWLINE:
        addChar();
        getChar();
        nextToken = NEWLINE;
        lexeme[0] = '\\';
        lexeme[1] = 'n';
        break;
        case HASH:
        do {
            addChar();
            getChar();
        } while (charClass == LETTER);
        if (lexeme[1] == 'i' && lexeme[2] == 'n' &&
            lexeme[3] == 'c' && lexeme[4] == 'l' &&
            lexeme[5] == 'u' && lexeme[6] == 'd' &&
            lexeme[7] == 'e') {
            nextToken = INCLUDE;
        } else {
            nextToken = UNKNOWN;
        } break;
        case UNKNOWN:
        addChar();
        if ((nextToken = opNoPrefix()) != UNKNOWN) {
            getChar();
            if (isspace(nextChar) || isalnum(nextChar) || EOF) break;
            addChar();
            if ((nextToken = opWPrefix()) != UNKNOWN) {
                getChar();
                break;
            }
        } else if (nextToken == UNKNOWN) {
            do {
                getChar();
                if (nextChar == EOF) break;
                addChar();
            } while (nextChar > 32);
        }
        getChar();
        break;
        case EOF_:
        nextToken = EOF_;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        break;
    }
    printf("Next token is : %d, next lexeme is %s\n", nextToken, lexeme);
    fprintf(fptr2, "%-15s%-15s%-15d", symTab(nextToken), lexeme, nextToken);
    for (size_t i = 0; i < lexLen; i++){
        lexeme[i] = '\0';
    }
    fprintf(fptr2, "\n");
    return nextToken;
}