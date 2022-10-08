/*
Program that counts the physical lines and the source lines of code of a file(s)
Author: Augusto Escudero
*/

#include <stdio.h>
#include <string.h>

struct IntTuple { //tuple structure
    int a;
    int b;
};

struct IntTuple slocCounter(FILE *fp) {

    enum CharStates{SLASH, REVERSE_SLASH, ASTERISK, SEMICOLON, SINGLE_QUOTE, DOUBLE_QUOTE, DEFAULT};
    enum EvalStates{SLOC, SINGLE_COMMENT, MULTI_COMMENT, STRING};

    char ch;
    int slocCount = 0;
    int physLineCount = 0;
    int charCount = 0;
    enum CharStates charState = DEFAULT;
    enum EvalStates evalState = SLOC;

    struct IntTuple returnResult; //the function will return two integer values contained within the tuple structure.

    if (fp == NULL) {
        perror("Error: ");
        returnResult.a = -1;
        returnResult.b = -1;
        return returnResult;
    }

    ch = getc(fp);

    while (ch!=EOF) {
        charCount++;

        switch(ch) {
            case '\n': // char is a new line
                if (evalState == SINGLE_COMMENT) {
                    evalState = SLOC;
                }
                physLineCount++;
                break;

            case '/': // char is a slash
                if (evalState == SLOC) {
                    if (charState == DEFAULT) {
                        charState = SLASH;
                    } else if (charState == SLASH) { // double slash -> comment
                        evalState = SINGLE_COMMENT;
                    }
                } else if (evalState == MULTI_COMMENT) {
                    if (charState == ASTERISK) { // slash followed by an asterisk while in multi line comment mode
                        charState = DEFAULT;
                        evalState = SLOC;
                    }
                }
                break;

            case '\\': //char is reverse slash
                if (evalState == STRING) {
                    charState = REVERSE_SLASH;
                }
                break;

            case '*': //char is asterisk
                if (evalState == SLOC) {
                    if (charState == SLASH) { // slash followed by asterisk -> comment
                        evalState = MULTI_COMMENT;
                    }
                }
                charState = ASTERISK;  
                break;
            
            case '\'': //char is single quote
                if (evalState == SLOC) {
                    evalState = STRING;
                    charState = SINGLE_QUOTE;
                } else if (evalState == STRING) {
                    if (charState == SINGLE_QUOTE) {
                        evalState = SLOC;
                        charState = DEFAULT;
                    } else if (charState == REVERSE_SLASH) {
                        charState = SINGLE_QUOTE;
                    }
                    
                }
                break;

            case '"': //char is double quote
                if (evalState == SLOC) {
                    evalState = STRING;
                    charState = DOUBLE_QUOTE;
                } else if (evalState == STRING) {
                    if (charState == DOUBLE_QUOTE) {
                        evalState = SLOC;
                        charState = DEFAULT;
                    } else if (charState == REVERSE_SLASH) {
                        charState = DOUBLE_QUOTE;
                    }
                    
                }
                break;
            
             case ';': //char is a semicolon
                if (evalState == SLOC) {
                    slocCount++;
                }
                break;
        }

        ch = getc(fp);
    }

    fclose(fp);
    if (charCount>0) {
        if (physLineCount == 0) {
            physLineCount = 1;
        }
    }
    
    returnResult.a = physLineCount;
    returnResult.b = slocCount;
    return returnResult;
}

int main(int argc, char *argv[]) {

    FILE *fp;
    int totalPhys = 0;
    int totalSloc = 0;
    struct IntTuple result;

    if (argc <= 2) {

        char * filename = "";

        if (argc == 1) { // read from standard input
            fp = stdin;
        } else if (argc == 2) { // read from specified file
            fp = fopen(argv[1], "r");
            filename = argv[1];
        }
        result = slocCounter(fp);
        totalPhys = result.a;
        totalSloc = result.b;
        if (totalPhys == -1) {
                printf("Aborting program.\n");
                return -1;
            } 
    
        printf("%d %4d %s\n", result.b, result.a, filename);
        


    } else if (argc > 2) { // read from multiple files
        
        for (int i=1; i<argc; i++) {
            fp = fopen(argv[i], "r");
            result = slocCounter(fp);
            totalPhys+=result.a;
            totalSloc+=result.b;
            if (totalPhys == -1) {
                printf("Aborting program.\n");
                return -1;
            } 
            printf("%d %4d %s\n", result.b, result.a, argv[i]);
            fclose(fp);
        }
        printf("%d %4d Total", totalSloc, totalPhys);
    }

    return 0;
  
}
