//HW #11, Noah Turnquist

//TODO: CHECK TO MAKE SURE PROGRAM DOESN'T CONTAIN ANY STRCHR(), STRRCHR(), or MEMCHR() calls.
//TODO: See if I should discard newline character at the end string when reading it in from fgets()
//TODO: Fix warning about long to int
//TODO: Make loops more efficient using pointer notation
//TODO: Find a way to discard new line character in userString
//TODO: Replace scanf for character with something more appropriate

#include <stdio.h>
#include <string.h>

#define SIZE 41
#define MAXMATCHES 10
#define FLUSH while(getchar() != '\n')

void ClearBuffer(void);
int CharIsAt(char* pStr, char ch, int loc[], int mLoc);
int FMin(int x, int y);

int main(void) {
    char userString[SIZE];
    int locations[MAXMATCHES];
    char searchCh;
    int matches;
    
    //TODO: Fix issue of input buffer not being flushed as necessary
    printf("HW #11, Noah Turnquist\n");
    printf("Enter string to be searched. (Blank line to quit):\n");
    while (*fgets(userString, SIZE - 1, stdin) != '\n') {
        ClearBuffer();
        printf("Enter character to search for: ");
        scanf(" %c", &searchCh);
        
        matches = CharIsAt(userString, searchCh, locations, MAXMATCHES);
        
        printf("The character '%c' was found %d times.\n", searchCh, matches);
        
        int matchesToPrint = FMin(matches, MAXMATCHES);
        printf("The first %d matches are at: ", matchesToPrint);
        for (int i = 0; i < matchesToPrint; i++) {
            printf("%2d ", locations[i]);
        }
        printf("in the string:\n");
        printf("%s", userString);
        for (int i = 0; i < strlen(userString); i++) { //TODO: Could make slightly more efficient by terminating once we've reached matches
            int match = 0;
            for (int j = 0; j < matchesToPrint; j++) {
                if (i == locations[j]) {
                    putchar('^');
                    match = 1;
                }
            }
            if (match == 0) putchar(' ');
        }
        printf("\n\n");
        
        printf("Please enter a string:\n");
        FLUSH;
    }
    
    return 0;
}

int CharIsAt(char* pStr, char ch, int loc[], int mLoc) {
    int matches = 0;
    
    for (char* itr = pStr; *itr != '\0'; itr++) {
        if (*itr == ch) {
            if (matches < mLoc) {
                loc[matches] = itr - pStr;
            }
            matches++;
        }
    }
    return matches;
}

int FMin(int x, int y) {
    return x < y ? x : y;
}

void ClearBuffer(void) {
    char ch;
    while( (ch = getchar()) != '\n' && ch != EOF );
}
