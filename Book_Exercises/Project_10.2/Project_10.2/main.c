//
//  main.c
//  Project_10.2
//
//  Created by Noah Turnquist on 4/2/24.
//

#include <stdio.h>
#include <string.h>

void DeleteLast(char* str);
void DeleteFirst(char* str);
void DeleteTrailingSpaces(char* str);
void DeleteLeadingSpaces(char* str);

int main(int argc, const char * argv[]) {
    char testStr[] = "This is a string";
    printf("%s\n", testStr);
    DeleteLast(testStr);
    printf("%s\n", testStr);
    
    char testStr2[] = "This is a string";
    printf("%s\n", testStr2);
    DeleteFirst(testStr2);
    printf("%s\n", testStr2);
    
    char testStr3[] = "This is a string          ";
    printf("%s\n", testStr3);
    DeleteTrailingSpaces(testStr3);
    printf("%s\n", testStr3);
    
    char testStr4[] = "              This is a string";
    printf("%s\n", testStr4);
    DeleteLeadingSpaces(testStr4);
    printf("%s\n", testStr4);
    
    return 0;
}

void DeleteLast(char* str) {
    int len = (int) strlen(str);
    str[len - 1] = '\0';
}

void DeleteFirst(char* str) {
    int len = (int) strlen(str);
    char tempStr[len + 1];
    strcpy(tempStr, str);
    char* pTempStr = tempStr;
    pTempStr++;
    strcpy(str, pTempStr);
}

void DeleteTrailingSpaces(char* str) {
    char* locationOfLastChar = str;
    for (char* itr = str; itr < str + strlen(str); itr++) {
        if (*itr != ' ' || *itr != '\n') {
            locationOfLastChar = itr;
        }
    }
    for (char* itr = locationOfLastChar; itr < str + strlen(str); itr++) {
        *itr = '\0';
    }
}

void DeleteLeadingSpaces(char* str) {
    int idx = (int) strcspn(str, " ");
    printf("idx: %d\n", idx);
    char* ptr = str;
    ptr += idx;
    strncpy(str, ptr, strlen(str));
}
