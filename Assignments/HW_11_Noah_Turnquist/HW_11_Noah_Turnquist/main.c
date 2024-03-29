//HW #11, Noah Turnquist

//TODO: CHECK TO MAKE SURE PROGRAM DOESN'T CONTAIN ANY STRCHR(), STRRCHR(), or MEMCHR() calls.
//TODO: Email Ken: ask about fpurge()
//TODO: Email Ken: See if I should discard newline character at the end string when reading it in from fgets()
//TODO: Email Ken: Had to use type long for location array to get rid of some compiler warning. Is this okay?
//TODO: Make loops more efficient using pointer notation?
//TODO: Keep or discard newline character in userString?
//TODO: Break up program into more discrete functions for better readability and maintanability

#include <stdio.h>
#include <string.h>

#define SIZE 41
#define MAXMATCHES 10
#define FLUSH while(getchar() != '\n')

int CharIsAt(char* pStr, char ch, long loc[], int mLoc);
int FMin(int x, int y);

int main(void) {
    char userString[SIZE];
    long locations[MAXMATCHES];
    char searchCh;
    int matches;
    
    printf("HW #11, Noah Turnquist\n");
    printf("Enter string to be searched. (Blank line to quit): ");
    while (*fgets(userString, SIZE, stdin) != '\n') {
        fpurge(stdin); //TODO: Ask prof is this is an okay way to clear the buffer
        
        //Forcing the second to last element in string to be a newline...
        //If the user enters more than SIZE - 1 characters.
        //Ending of string will look like [..., \n, \0]
        if (strlen(userString) >= SIZE - 1) {
            userString[SIZE - 2] = '\n';
        }
        
        printf("Enter character to search for: ");
        searchCh = getc(stdin);
        FLUSH;
        
        matches = CharIsAt(userString, searchCh, locations, MAXMATCHES);
        
        printf("The character '%c' was found %d times.\n", searchCh, matches);
        
        int matchesToPrint = FMin(matches, MAXMATCHES);
        printf("The first %d matches are at: ", matchesToPrint);
        for (int i = 0; i < matchesToPrint; i++) {
            printf("%2ld ", locations[i]);
        }
        printf("in the string:\n");
        printf("%s", userString);
        
        int lenUserString = (int) strlen(userString);
        char underline[lenUserString + 1];
        for (int i = 0; i < lenUserString; i++) {
            underline[i] = ' ';
        }
        
        for (int i = 0; i < matchesToPrint; i++) {
            long matchIndex = locations[i];
            underline[matchIndex] = '^';
        }
        
        printf("%s\n\n", underline);
        
        printf("Enter string to be searched. (Blank line to quit): ");
    }
    
    return 0;
}

int CharIsAt(char* pStr, char ch, long loc[], int mLoc) {
    int matches = 0;
    
    for (char* itr = pStr; *itr != '\0'; itr++) {
        if (*itr == ch) {
            if (matches < mLoc) {
                loc[matches] = (long) ((long) itr - (long) pStr);
            }
            matches++;
        }
    }
    return matches;
}

int FMin(int x, int y) {
    return x < y ? x : y;
}


//VERIFICATION
/*
 1. Check under normal conditions
    String = "hello" Char = 'l'
    String = "This is fun" Char = 'h'
 2. Check with no matching characters
    String = "Green" Char = 'a'
 3. Check with more than MAXMATCHES characters
    String = "aaaaaaaaaaaaaaaaaaaaaaaaaaa", char = 'a'
 4. Check with userString overflow
    String = 'adadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadada', char = 'd'
 5. Check with ending program on first loop iteration.
    String = "\n"
 6. Check with entering some numbers.
    String = "12456678", Char = '6'
 */
