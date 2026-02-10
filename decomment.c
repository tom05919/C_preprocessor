#include <stdio.h>

enum StateType {
    START, 
    COMMENT1, 
    COMMENT2, 
    ENDCOMMENT1, 
    QUOTE, 
    SKIP, 
    ERROR,
    END
};

enum charOfInterest {
    SLASH = '/',
    STAR = '*',
    B_SLASH = '\\',
    QUOTATION = '\'',
    D_QUOTATION = '"',
    NEW_LINE = '\n'
};

int EXIT_FAILURE = 1;
int EXIT_SUCCESS = 0;

enum StateType prevState;
int c;
int lineNum = 1;
int commentLine;
int errorLine;
char commentState = 0;
char quoteState = 0;
char startQuote;

enum StateType checkNextState(int c) {
    enum StateType state;
    if (prevState == COMMENT1 && c != STAR) {
        putchar(SLASH);
    } 
    if (commentState == 1) {
         switch (c) {
            case NEW_LINE:
                lineNum++;
                putchar(c);
                state = COMMENT2;
                break;
            case SLASH:
               if (prevState == ENDCOMMENT1) {
                    state = START;
                    commentState = 0;
                } else {
                    state = COMMENT2;
                }
                break;
            case STAR:
                if (prevState == COMMENT2 || prevState == ENDCOMMENT1) {
                    state = ENDCOMMENT1;
                } else {
                    state = COMMENT2;
                }
                break;
            case EOF:
                state = ERROR;
                errorLine = commentLine;
                break;
            default:
                state = COMMENT2;
         }
         return state;
    } else if (quoteState == 1) {
        switch(c) {
            case QUOTATION:
               if (startQuote == c) {
                    state = START;
                    quoteState = 0;
                } else {
                    state = QUOTE;
                }
                putchar(c);
                break;
            case D_QUOTATION:
                if (startQuote == c) {
                    state = START;
                    quoteState = 0;
                } else {
                    state = QUOTE;
                }
                putchar(c);
                break;
            case B_SLASH:
                putchar(c);
                state = SKIP;
                break;
            case NEW_LINE:
                lineNum++;
                putchar(c);
                state = QUOTE;
                break;
            case EOF:
                state = END;
                break;
            default:
                putchar(c);
                state = QUOTE;
        }
        return state;
    }

    switch (c) {
        case NEW_LINE:
            lineNum++;
            putchar(c);
            state = (prevState == QUOTE) ? QUOTE : START;
            break;
        case SLASH:
            if (prevState == START) {
                state = COMMENT1;
            } else if (prevState != COMMENT1) {
                putchar(c);
                state = prevState;
            } else {
                state = prevState;
            }
            break;
        case STAR:
            if (prevState == COMMENT1) {
                putchar(' ');
                state = COMMENT2;
                commentLine = lineNum;
                commentState = 1;
            } else {
                putchar(c);
                state = prevState;
            }
            break;
        case B_SLASH:
            putchar(c);
            state = prevState;
            break;
        case QUOTATION:
            startQuote = c;
            state = QUOTE;
            putchar(c);
            quoteState = 1;
            break;
        case D_QUOTATION:
            startQuote = c;
            state = QUOTE;
            putchar(c);
            quoteState = 1;
            break;
        case EOF:
            state = END;
            break;
        default:
            state = START;
            putchar(c);
    }
    return state;
}

int main() {
    enum StateType state = START;
    c = getchar();
    state = checkNextState(c);
    while(c != EOF) {
        prevState = state;
        c = getchar();
        switch (state) {
            case START:
                state = checkNextState(c);
                break;
            case COMMENT1:
                state = checkNextState(c);
                break;
            case COMMENT2:
                state = checkNextState(c);
                break;
            case ENDCOMMENT1:
                state = checkNextState(c);
                break;
            case QUOTE:
                state = checkNextState(c);
                break;
            case SKIP:
                if (c != EOF) {
                    putchar(c);
                }
                c = getchar();
                state = checkNextState(c);
                break;
            case ERROR:
                break;
            case END:
                break;
        }
    }
    if (state == ERROR) {
        fprintf(stderr, "Error: line %d: unterminated comment\n", errorLine);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}