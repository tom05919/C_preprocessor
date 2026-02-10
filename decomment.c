#include <stdio.h>

enum StateType {
    START, 
    COMMENT1, 
    COMMENT2, 
    ENDCOMMENT1, 
    QUOTE, 
    SKIP, 
    ERROR
};

enum charOfInterest {
    SLASH = '/',
    STAR = '*',
    B_SLASH = '\\',
    QUOTATION = '\'',
    D_QUOTATION = '"',
    NEW_LINE = '\n'
};

enum StateType prevState;
int c;
int lineNum;
int commentLine;
int errorLine;

enum StateType checkNextState(int c) {
    enum StateType state;
    switch (c) {
        case NEW_LINE:
            lineNum++;
            putchar(c);
            if (prevState == ENDCOMMENT1) {
                state = COMMENT2;
            } else {
                state = prevState;
            }
            break;
        case SLASH:
            if (prevState == START) {
                state = COMMENT1;
            } else if (prevState == ENDCOMMENT1) {
                putchar(' ');
                state = START;
            } else {
                state = prevState;
            }
            break;
        case STAR:
            if (prevState == COMMENT1) {
                state = COMMENT2;
                commentLine = lineNum;
            } else if (prevState == COMMENT2) {
                state = ENDCOMMENT1;
            } else {
                state = prevState;
            }
            break;
        case B_SLASH:
            if (prevState == QUOTE) {
                putchar(c);
                state = SKIP;
            }
            break;
        case QUOTATION:
            if (prevState != COMMENT2 && prevState != QUOTE && prevState != ENDCOMMENT1) {
                state = QUOTE;
                putchar(c);
            } else if (prevState == QUOTE) {
                state = START;
                putchar(c);
            }
            break;
        case D_QUOTATION:
            if (prevState != COMMENT2 && prevState != QUOTE && prevState != ENDCOMMENT1) {
                state = QUOTE;
                putchar(c);
            } else if (prevState == QUOTE) {
                state = START;
                putchar(c);
            }
            break;
        case EOF:
            break;
        default:
            state = START;
            putchar(c);
    }
    return state;
}

enum StateType inComment(int c) {
    enum StateType state;
    switch(c) {
        case STAR:
            state = ENDCOMMENT1;
            break;
        case EOF:
            putchar('  ');
            state = ERROR;
            errorLine = commentLine;
        default:
            state = COMMENT2;
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
                state = inComment(c);
                break;
            case ENDCOMMENT1:
                state = checkNextState(c);
                break;
            case QUOTE:
                state = checkNextState(c);
                break;
            case SKIP:
                c = getchar();
                putchar(c);
                c = getchar();
                state = checkNextState(c);
                break;
            case ERROR:
                fprintf(stderr, "Error: line %d: unterminated comment\n", errorLine);
                return 0;
        }
    }
    return 1;
}