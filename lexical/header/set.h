#include <stdio.h>

#define MAX_LEXEME   1024
#define MAX_FILELEN 32767
#define MAX_PATH    32767

#define LETTER    0
#define DIGIT     1
#define DOT       2
#define HASH      3
#define NEWLINE   4
#define LINEFEED  5
#define TAB       6

#define IDENT    10
#define CONSTANT 11
#define IF       12
#define ELSE     13
#define WHILE    14
#define CONTINUE 15
#define BREAK    16
#define TRY      17
#define BUT      18
#define CONST_   19
#define GLOBAL   20
#define INT_     21
#define DEC      22
#define STRING   23
#define NUL      24
#define STRUCT   25
#define BOOL     26
#define TRUE_    27
#define FALSE_   28
#define RETURN   29
#define INCLUDE  30
#define MAIN     31
#define ADD_OP   32
#define SUB_OP   33
#define MULT_OP  34
#define DIV_OP   35
#define MODULO   36
#define INCREM   37
#define DECREM   38
#define BITNOT   39
#define BITAND   40
#define BITOR    41
#define BITXOR   42
#define BITLFT   43
#define BITRGHT  44
#define EQL      45
#define PLSEQL   46
#define MINEQL   47
#define TIMEQL   48
#define DIVEQL   49
#define LESS     50
#define GREAT    51
#define LSSEQL   52
#define GRTEQL   53
#define EQLEQL   54
#define COMAND   55
#define COMOR    56
#define NOTEQL   57
#define O_PAREN  58
#define C_PAREN  59
#define O_BRACK  60
#define C_BRACK  61
#define O_SQRL   62
#define C_SQRL   63
#define COMMA    64
#define S_QUOTE  65
#define D_QUOTE  66
#define S_COMME  67
#define O_COMME  68
#define C_COMME  69
#define INTER    70
#define ESCCHAR  71
#define CHARLIT  72

#define UNKNOWN  99
#define EOF_     -1

void getChar();
void addChar();
void getNonBlank();
int analyzer();
const char* symTab(int nextToken) {
    switch(nextToken) {
        case 0: return "LETTER";
        case 1: return "DIGIT";
        case 2: return "DOT";
        case 3: return "HASH";
        case 4: return "NEWLINE";
        case 10: return "IDENT";
        case 11: return "CONSTANT";
        case 12: return "IF";
        case 13: return "ELSE";
        case 14: return "WHILE";
        case 15: return "CONTINUE";
        case 16: return "BREAK";
        case 17: return "TRY";
        case 18: return "BUT";
        case 19: return "CONST";
        case 20: return "GLOBAL";
        case 21: return "INT";
        case 22: return "DEC";
        case 23: return "STRING";
        case 24: return "NUL";
        case 25: return "STRUCT";
        case 26: return "BOOL";
        case 27: return "TRUE";
        case 28: return "FALSE";
        case 29: return "RETURN";
        case 30: return "INCLUDE";
        case 31: return "MAIN";
        case 32: return "ADD_OP";
        case 33: return "SUB_OP";
        case 34: return "MULT_OP";
        case 35: return "DIV_OP";
        case 36: return "MODULO";
        case 37: return "INCREM";
        case 38: return "DECREM";
        case 39: return "BITNOT";
        case 40: return "BITAND";
        case 41: return "BITOR";
        case 42: return "BITXOR";
        case 43: return "BITLFT";
        case 44: return "BITRGHT";
        case 45: return "EQL";
        case 46: return "PLSEQL";
        case 47: return "MINEQL";
        case 48: return "TIMEQL";
        case 49: return "DIVEQL";
        case 50: return "LESS";
        case 51: return "GREAT";
        case 52: return "LSSEQL";
        case 53: return "GRTEQL";
        case 54: return "EQLEQL";
        case 55: return "COMAND";
        case 56: return "COMOR";
        case 57: return "NOTEQL";
        case 58: return "O_PAREN";
        case 59: return "C_PAREN";
        case 60: return "O_BRACK";
        case 61: return "C_BRACK";
        case 62: return "O_SQRL";
        case 63: return "C_SQRL";
        case 64: return "COMMA";
        case 65: return "S_QUOTE";
        case 66: return "D_QUOTE";
        case 67: return "S_COMME";
        case 68: return "O_COMME";
        case 69: return "C_COMME";
        case 70: return "INTER";
        case 71: return "ESCCHAR";
        case 72: return "CHARLIT";

        case 99: return "UNKNOWN";
        case -1: return "EOF_";

    }
}

char fileName[MAX_FILELEN];
char lexeme[MAX_LEXEME];
char nextChar;
short int* tokenArray;
short int nextToken, charClass;
unsigned short int lexLen;
char workPath[MAX_PATH] = "..\\..\\programs\\";
char symtblPath[MAX_PATH] = "..\\..\\lexical\\bin\\";
bool debugVal = 1;

FILE *fptr;
FILE *fptr2;
FILE *fptrErr;