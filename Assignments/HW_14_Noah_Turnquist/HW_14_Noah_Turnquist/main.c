//HW #14, Noah Turnquist

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define FLUSH while(getchar() != '\n')
#define MAXTITLE 50
#define MAXAUTHOR 50
#define MAXYEAR 4

typedef struct {
    int bookId;
    char title[MAXTITLE + 1];
    char author[MAXAUTHOR + 1];
    char published[MAXYEAR + 1];
} BOOK;

typedef struct {
    BOOK data;
    struct NODE* link;
} NODE;

typedef struct {
    NODE* pHead;
    int numBooks;
} HEADER;

enum MENUOPTIONS { INITORDELETELIST = 1, APPEND, WRITETOBINARY, PRINTBOOK, EXIT };

int GetMenuOptionFromUser(HEADER list);

int main(void) {
    int userChoice;
    HEADER list = { NULL, 0 };
    
    printf("HW #14, Noah Turnquist\n");
    
    do {
        userChoice = GetMenuOptionFromUser(list);
        printf("%d\n", userChoice);
    } while(userChoice != EXIT);
    
    return 0;
}

int GetMenuOptionFromUser(HEADER list) {
    int validSelection = 0;
    char chUserChoice;
    int userChoice = -1;
    
    do {
        printf("Please select one of the options by entering the cooresponding number.\n");
        if (list.pHead == NULL) {
            printf("Initialize the list: (%d)\n", INITORDELETELIST);
        } else {
            printf("Delete the list: (%d)\n", INITORDELETELIST);
            printf("Append data from file to the list: (%d)\n", APPEND);
            printf("Write the list out to a binary file: (%d)\n", WRITETOBINARY);
            printf("Get info on a book: (%d)\n", PRINTBOOK);
        }
        printf("Exit the program: (%d)\n", EXIT);
        chUserChoice = getchar();
        if (isdigit(chUserChoice)) {
            userChoice = chUserChoice - '0';
            if (list.pHead == NULL && (userChoice == INITORDELETELIST || userChoice == EXIT)) {
                validSelection = 1;
            } else if (list.pHead && (userChoice >= INITORDELETELIST && userChoice <= EXIT)) {
                validSelection = 1;
            } else {
                printf("Please pick from one of the menu option.\n");
            }
        } else {
            printf("Please enter a number.\n");
        }
        FLUSH;
    } while(!validSelection);
    
    return userChoice;
}


/*
 TEST PLAN
 
 TEST - GetMenuOptionFromUser()
 
 ---Unitialized list---
 
 1. Run the program. The list should be uninitialized by default.
 - The only menu options are: Initialize the list, exit
 
 2. Run the program. The list should be uninitialized by default.
    Select the option to initialize the list, '1'. Should exit function and return userChoice.
 
 3. Run the program. The list should be uninitialized by default.
    Select the option to exit the program, '5'. Should exit function function and then exit the program.
 
 4. Run the program. The list should be uninitialized by default.
    Enter a non digit, 'c'. Should give error message and then ask you to try again.
 
 5. Run the program. The list should be uninitialized by default.
    Enter an invalid list option, '2' . Should give error message and then ask you to try again.
 
 6. Run the program. The list should be uninitialized by default.
    Enter too many characters , 'hello' . Should give error message and then ask you to try again.
 
 7. Run the program. The list should be uninitialized by default.
    Enter valid character and then invalid characters , '1awdada' . Should exit function and return userChoice.
 
 8. Run the program. The list should be uninitialized by default.
    Enter invalid character and then valid character , 'adwadawda1' . Should give error message and then ask you to try again.
 
 ---Initialized list---
 
 1. Run the program. Hit 1 to initialize the list.
 - The menu option should be to: delete, append, write, get a book, and exit.
 
 2. Run the program. Hit 1 to initialize the list.
 - Select a valid option, '2'. Should exit function and return userChoice.
 
 3. Run the program. Hit 1 to initialize the list.
    Select the option to exit the program, '5'. Should exit function function and then exit the program.
 
 4. Run the program. Hit 1 to initialize the list.
    Enter a non digit, 'c'. Should give error message and then ask you to try again.
 
 5. Run the program. Hit 1 to initialize the list.
    Enter an invalid list option, '7' . Should give error message and then ask you to try again.
 
 6. Run the program. Hit 1 to initialize the list.
    Enter too many characters , 'hello' . Should give error message and then ask you to try again.
 
 7. Run the program. Hit 1 to initialize the list.
    Enter valid character and then invalid characters , '3awdada' . Should exit function and return userChoice.
 
 8. Run the program. Hit 1 to initialize the list.
    Enter invalid character and then valid character , 'adwadawda1' . Should give error message and then ask you to try again.
 
 
 
 TEST -
 
 */
