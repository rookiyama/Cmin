#ifndef _SET
#define _SET
#include "../header/set.h"
#endif

#include <ctype.h>

int keyword() {
    switch(lexeme[0]) {
        case 'b':
        if (lexeme[1] == 'r' &&
            lexeme[2] == 'e' &&
            lexeme[3] == 'a' &&
            lexeme[4] == 'k' &&
            !lexeme[5])
            return BREAK;
        else if (lexeme[1] == 'u' &&
                 lexeme[2] == 't' &&
                 !lexeme[3])
            return BUT;
        else if (lexeme[1] == 'o' &&
                 lexeme[2] == 'o' &&
                 lexeme[3] == 'l' &&
                 !lexeme[4])
            return BOOL;
        return IDENT;
        case 'c':
        if (lexeme[1] == 'h' &&
            lexeme[2] == 'a' &&
            lexeme[3] == 'r' &&
            !lexeme[4])
            return CHARLIT;
        else if (lexeme[1] == 'o' &&
            lexeme[2] == 'n') {
            if (lexeme[3] == 't' &&
                lexeme[4] == 'i' &&
                lexeme[5] == 'n' &&
                lexeme[6] == 'u' &&
                lexeme[7] == 'e' &&
                !lexeme[8])
                return CONTINUE;
            else if (lexeme[3] == 's' &&
                     lexeme[4] == 't' &&
                     !lexeme[5])
                return CONST_;
        } return IDENT;
        case 'd':
        if (lexeme[1] == 'e' &&
            lexeme[2] == 'c' &&
            !lexeme[3])
            return DEC;
        return IDENT;
        case 'e':
        if (lexeme[1] == 'l' &&
            lexeme[2] == 's' &&
            lexeme[3] == 'e' &&
            !lexeme[4])
            return ELSE;
        return IDENT;
        case 'f':
        if (lexeme[1] == 'a' &&
            lexeme[2] == 'l' &&
            lexeme[3] == 's' &&
            lexeme[4] == 'e' &&
            !lexeme[5])
            return FALSE_;
        return IDENT;
        case 'g':
        if (lexeme[1] == 'l' &&
            lexeme[2] == 'o' &&
            lexeme[3] == 'b' &&
            lexeme[4] == 'a' &&
            lexeme[5] == 'l' &&
            !lexeme[6])
            return GLOBAL;
        return IDENT;
        case 'i':
        if (lexeme[1] == 'f' &&
            !lexeme[2])
            return IF;
        else if (lexeme[1] == 'n' &&
                 lexeme[2] == 't' &&
                 !lexeme[3])
            return INT_;
        return IDENT;
        case 'm':
        if (lexeme[1] == 'a' &&
            lexeme[2] == 'i' &&
            lexeme[3] == 'n' &&
            !lexeme[4])
            return MAIN;
        return IDENT;
        case 'n':
        if (lexeme[1] == 'u' &&
            lexeme[2] == 'l' &&
            lexeme[3] == 'l' &&
            !lexeme[4])
            return NUL;
        return IDENT;
        case 'r':
        if (lexeme[1] == 'e' &&
            lexeme[2] == 't' &&
            lexeme[3] == 'u' &&
            lexeme[4] == 'r' &&
            lexeme[5] == 'n' &&
            !lexeme[6])
            return RETURN;
        return IDENT;
        case 's':
        if (lexeme[1] == 't' &&
            lexeme[2] == 'r') {
            if (lexeme[3] == 'u' &&
                lexeme[4] == 'c' &&
                lexeme[5] == 't' &&
                !lexeme[6])
                return STRUCT;
            else if (lexeme[3] == 'i' &&
                     lexeme[4] == 'n' &&
                     lexeme[5] == 'g' &&
                     !lexeme[6])
                return STRING;
        } return IDENT;
        case 't':
        if (lexeme[1] == 'r') {
            if (lexeme[2] == 'u' &&
                lexeme[3] == 'e' &&
                !lexeme[4])
                return TRUE_;
            else if (lexeme[2] == 'y' &&
                     !isalpha(lexeme[3]))
                return TRY;
        } return IDENT;
        case 'w':
        if (lexeme[1] == 'h' &&
            lexeme[2] == 'i' &&
            lexeme[3] == 'l' &&
            lexeme[4] == 'e' &&
            !lexeme[5])
            return WHILE;
        return WHILE;
        default:
        return IDENT;
    }
}

int opNoPrefix() {
    switch(lexeme[0]) {
        case '\'':
        return S_QUOTE;
        case '"':
        return D_QUOTE;
        case ',':
        return COMMA;
        case '%':
        return MODULO;
        case '{':
        return O_SQRL;
        case '}':
        return C_SQRL;
        case '[':
        return O_BRACK;
        case ']':
        return C_BRACK;
        case '(':
        return O_PAREN;
        case ')':
        return C_PAREN;
        case '.':
        return DOT;
        case '$':
        return INTER;
        case '^':
        return BITXOR;
        case '~':
        return BITNOT;
        case '+':
        return ADD_OP;
        case '-':
        return SUB_OP;
        case '*':
        return MULT_OP;
        case '/':
        return DIV_OP;
        case '&':
        return BITAND;
        case '|':
        return BITOR;
        case '!':
        return BITNOT;
        case '<':
        return LESS;
        case '>':
        return GREAT;
        case '=':
        return EQL;
        default:
        return UNKNOWN;
    }
}

int opWPrefix() {
    switch(lexeme[0]) {
        case '+':
        switch(lexeme[1]) {
            case '+':
            return INCREM;
            case '=':
            return PLSEQL;
            default:
            if (lexeme[1] < 33) {
                lexeme[1] = '\0';
                return ADD_OP;
            }
            else return UNKNOWN;
        }
        case '-':
        switch(lexeme[1]) {
            case '-':
            return DECREM;
            case '=':
            return MINEQL;
            default:
            if (lexeme[1] < 33) {
                lexeme[1] = '\0';
                return SUB_OP;
            }
            else return UNKNOWN;
        }
        case '*':
        switch(lexeme[1]) {
            case '/':
            return C_COMME;
            case '=':
            return TIMEQL;
            default:
            if (lexeme[1] < 33) {
                lexeme[1] = '\0';
                return MULT_OP;
            }
            else return UNKNOWN;
        }
        case '/':
        switch(lexeme[1]) {
            case '*':
            return O_COMME;
            case '/':
            return S_COMME;
            case '=':
            return DIVEQL;
            default:
            if (lexeme[1] < 33) {
                lexeme[1] = '\0';
                return DIV_OP;
            }
            else return UNKNOWN;
        }
        case '&':
        if (lexeme[1] == '&')
            return COMAND;
        else if (lexeme[1] < 33) {
            lexeme[1] = '\0';
            return BITAND;
        } else return UNKNOWN;
        case '|':
        if (lexeme[1] == '|')
            return COMOR;
        else if (lexeme[1] < 33) {
            lexeme[1] = '\0';
            return BITOR;
        } else return UNKNOWN;
        case '!':
        if (lexeme[1] == '=')
            return NOTEQL;
        else if (lexeme[1] < 33) {
            lexeme[1] = '\0';
            return BITNOT;
        } else return UNKNOWN;
        case '<':
        switch(lexeme[1]) {
            case '<':
            return BITLFT;
            case '=':
            return LSSEQL;
            default:
            if (lexeme[1] < 33) {
                lexeme[1] = '\0';
                return LESS;
            }
            else return UNKNOWN;
        }
        case '>':
        switch(lexeme[1]) {
            case '>':
            return BITRGHT;
            case '=':
            return GRTEQL;
            default:
            if (lexeme[1] < 33) {
                lexeme[1] = '\0';
                return GREAT;
            }
            else return UNKNOWN;
        }
        case '=':
        if (lexeme[1] == '=')
            return EQLEQL;
        else if (lexeme[1] < 33) {
            lexeme[1] = '\0';
            return EQL;
        } else return UNKNOWN;
        default:
        return UNKNOWN;
    }
}