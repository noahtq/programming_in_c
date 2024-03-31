//HW #11, Noah Turnquist

#include <stdio.h>
#include <string.h>

#define SIZE 41
#define MAXMATCHES 10
#define FLUSH while(getchar() != '\n')

int CharIsAt(char* pStr, char ch, int loc[], int mLoc);
int FMin(int x, int y);

int main(void) {
    char userString[SIZE];
    int locations[MAXMATCHES];
    char searchCh;
    int matches;
    
    printf("HW #11, Noah Turnquist\n");
    printf("Enter string to be searched. (Blank line to quit): ");
    while (*fgets(userString, SIZE, stdin) != '\n') {
        
        //Check if there are any newline characters in string.
        //If newline character found set foundIt to true and exit loop
        int foundIt = 0;
        for (char* itr = userString; itr < userString + (SIZE - 1) && !foundIt; itr++) {
            if (*itr == '\n') {
                foundIt = 1;
            }
        }
        //If no newline character in userString, flush the buffer
        //and add a newline character at the end of the userString
        //end of array is [..., \n, \0]
        if (!foundIt) {
            userString[SIZE - 2] = '\n';
            FLUSH;
        }
        
        printf("Enter character to search for: ");
        searchCh = getchar();
        FLUSH;
        
        matches = CharIsAt(userString, searchCh, locations, MAXMATCHES);
        
        printf("The character '%c' was found %d times.\n", searchCh, matches);
        
        int matchesToPrint = FMin(matches, MAXMATCHES);
        printf("The first %d matches are at: ", matchesToPrint);
        for (int i = 0; i < matchesToPrint; i++) {
            printf("%2d ", locations[i]);
        }
        printf("in the string:\n");
        printf("%s", userString);
        
        int lenUserString = (int) strlen(userString);
        char underline[lenUserString + 1];
        for (int i = 0; i < lenUserString; i++) {
            underline[i] = ' ';
        }
        underline[lenUserString] = '\0';
        
        for (int i = 0; i < matchesToPrint; i++) {
            int matchIndex = locations[i];
            underline[matchIndex] = '^';
        }
        
        printf("%s\n\n", underline);
        
        printf("Enter string to be searched. (Blank line to quit): ");
    }
    
    return 0;
}

int CharIsAt(char* pStr, char ch, int loc[], int mLoc) {
    int matches = 0;
    
    for (char* itr = pStr; *itr != '\0'; itr++) {
        if (*itr == ch) {
            if (matches < mLoc) {
                loc[matches] = (int) (itr - pStr);
            }
            matches++;
        }
    }
    return matches;
}

int FMin(int x, int y) {
    return x < y ? x : y;
}


//HW #11, Noah Turnquist
//Enter string to be searched. (Blank line to quit): hello
//Enter character to search for: l
//The character 'l' was found 2 times.
//The first 2 matches are at:  2  3 in the string:
//hello
//  ^^
//
//Enter string to be searched. (Blank line to quit): This is fun
//Enter character to search for: i
//The character 'i' was found 2 times.
//The first 2 matches are at:  2  5 in the string:
//This is fun
//  ^  ^
//
//Enter string to be searched. (Blank line to quit): Green
//Enter character to search for: a
//The character 'a' was found 0 times.
//The first 0 matches are at: in the string:
//Green
//      
//
//Enter string to be searched. (Blank line to quit): aaaaaaaaaaaaaaaaaaaa
//Enter character to search for: a
//The character 'a' was found 20 times.
//The first 10 matches are at:  0  1  2  3  4  5  6  7  8  9 in the string:
//aaaaaaaaaaaaaaaaaaaa
//^^^^^^^^^^
//
//Enter string to be searched. (Blank line to quit): adadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadadad
//Enter character to search for: d
//The character 'd' was found 19 times.
//The first 10 matches are at:  1  3  5  7  9 11 13 15 17 19 in the string:
//adadadadadadadadadadadadadadadadadadada
// ^ ^ ^ ^ ^ ^ ^ ^ ^ ^
//
//Enter string to be searched. (Blank line to quit): 123456678
//Enter character to search for: 6
//The character '6' was found 2 times.
//The first 2 matches are at:  5  6 in the string:
//123456678
//     ^^
//
//Enter string to be searched. (Blank line to quit):
//Program ended with exit code: 0


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
