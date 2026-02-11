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

/*This keeps track of the total line number*/
int lineNum = 1;

/*This keep strack of the line where a comment starts, it updates everytime for new comment*/
int commentLine;

/*This stores which symbol ' or " started the quote*/
char startQuote;


/* Checks the next state of the program. It takes in the current character (int), the previousState (enum StateType)
and if its in a comment or quote (int), and it outputs state (enum StateType)*/

enum StateType checkNextState(int c, char commentState, char quoteState, enum StateType prevState) {
    enum StateType state;
    if (prevState == COMMENT1 && c != STAR) {
        putchar(SLASH);
    } 

    /*the following if else statement is only in effect when the program is in a comment or quote to account for edgecases*/
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
                } else {
                    state = QUOTE;
                }
                putchar(c);
                break;
            case D_QUOTATION:
                if (startQuote == c) {
                    state = START;
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

    /*this switch statement takes care of the general case when we are not in a comment
    or quote*/
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
            break;
        case D_QUOTATION:
            startQuote = c;
            state = QUOTE;
            putchar(c);
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


/*Given a file, it will remove all comments within the file. It will return an error only when there is 
an unterminated comment and will given the line where that comment started. It will ignore unterminated 
quotes*/

int main() {
    int EXIT_FAILURE = 1;
    int EXIT_SUCCESS = 0;
    char commentState = 0;
    char quoteState = 0;
    enum StateType prevState = START;
    int c;

    enum StateType state = START;
    c = getchar();
    state = checkNextState(c, commentState, quoteState, prevState);
    while(c != EOF) {
        prevState = state;
        c = getchar();
        switch (state) {
            case START:
                commentState = 0;
                quoteState = 0;
                state = checkNextState(c, commentState, quoteState, prevState);
                break;
            case COMMENT1:
                state = checkNextState(c, commentState, quoteState, prevState);
                break;
            case COMMENT2:
                commentState = 1;
                state = checkNextState(c, commentState, quoteState, prevState);
                break;
            case ENDCOMMENT1:
                state = checkNextState(c, commentState, quoteState, prevState);
                break;
            case QUOTE:
                quoteState = 1;
                state = checkNextState(c, commentState, quoteState, prevState);
                break;
            case SKIP:
                if (c != EOF) {
                    putchar(c);
                }
                c = getchar();
                state = checkNextState(c, commentState, quoteState, prevState);
                break;
            case ERROR:
                break;
            case END:
                break;
        }
    }
    if (state == ERROR) {
        fprintf(stderr, "Error: line %d: unterminated comment\n", commentLine);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}