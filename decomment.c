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

int output;
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
            break;
        case SLASH:
            if (prevState == START) {
                state = COMMENT1;
            } else if (prevState == ENDCOMMENT1) {
                output = 0;
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
            if (state == QUOTE) {
                printf("%c", c);
                state = SKIP;
            }
            break;
        case QUOTATION:
            if (prevState != COMMENT2 && prevState != QUOTE && prevState != ENDCOMMENT1) {
                state = QUOTE;
            }
            break;
        case D_QUOTATION:
            if (state != COMMENT2 && prevState != QUOTE && prevState != ENDCOMMENT1) {
                state = QUOTE;
            }
            break;
        default:
            state = START;
            printf("%c", c);
    }
    return state;
}

enum StateType inComment(int c) {
    enum StateType state;
    switch(c) {
        case STAR:
            state = ENDCOMMENT1;
            break;
        default:
            output += c;
            state = COMMENT2;
    }
    return state;
}

int main() {
    enum StateType state = START;
    c = (int) getchar();
    state = checkNextState(c);
    while(c != EOF) {
        prevState = state;
        c = (int) getchar();
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
                c = (int) getchar();
                printf("%c", c);
                c = (int) getchar();
                state = checkNextState(c);
                break;
            case ERROR:
                break;
        }
    }
    if (state == ERROR) {
        printf("%s", output);
        fprintf(stderr, "Error: line %d: unterminated comment\n", lineNum);
        return 1;
    }
    return 0;
}