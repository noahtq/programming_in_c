//HW #14, Noah Turnquist

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FLUSH while(getchar() != '\n')
#define MAXTITLE 50
#define MAXAUTHOR 50
#define MAXYEAR 4
#define DEFAULTFILEPATH "/Users/noahturnquist/Documents/College/Spring_2024/Programming_in_C/Assignments/HW_14_Noah_Turnquist/HW_14_Noah_Turnquist/"
#define INPUTFILENAME "HW14DataMac.txt"
#define MAXIDLENGTH 5

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
HEADER InitializeOrDeleteLinkedList(HEADER list);
HEADER DeleteLinkedList(HEADER list);
HEADER AppendLinkedList(char* fileName, HEADER list);
FILE* OpenFileInReadMode(char* fileName);
void AppendFileNameToFilePath(const char* fileName, const char* filepath, char* fullPath, int stringLength);
BOOK GetBookFromFile(FILE* fp, int bookId);
int FileToStringWithoutNewline(FILE* fp, char* str, int maxChars);
void DestroyNewlinesAndCarriageReturns(FILE* fp);
void PrintBookById(HEADER list);
int GetBookIdFromUser(HEADER list);
void PrintBookInfo(BOOK bookNode);

int main(void) {
    int userChoice;
    HEADER list = { NULL, 0 };
    
    printf("HW #14, Noah Turnquist\n");
    
    do {
        userChoice = GetMenuOptionFromUser(list);
        switch (userChoice) {
            case INITORDELETELIST:
                list = InitializeOrDeleteLinkedList(list);
                break;
            case APPEND:
                list = AppendLinkedList(INPUTFILENAME, list);
                break;
            case PRINTBOOK:
                PrintBookById(list);
                break;
            case EXIT:
                printf("Exiting program.\n");
                break;
        }
        
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

HEADER InitializeOrDeleteLinkedList(HEADER list) {
    HEADER newHeader;
    
    if (list.pHead == NULL) {
        newHeader = AppendLinkedList(INPUTFILENAME, list);
        printf("Initialized list.\n");
    } else {
        newHeader = DeleteLinkedList(list);
        printf("Deleted list.\n");
    }
    
    return newHeader;
}

HEADER DeleteLinkedList(HEADER list) {
    NODE* pCur = list.pHead;
    NODE* pNext;
    
    while(pCur != NULL) {
        pNext = pCur->link;
        free(pCur);
        pCur = pNext;
    }
    
    HEADER newHeader = { NULL, 0 };
    return newHeader;
}

HEADER AppendLinkedList(char* fileName, HEADER list) {
    FILE* fp;
    HEADER newHeader;
    BOOK tempBook;
    int bookId = list.numBooks;
    NODE* pHead = list.pHead;
    NODE* pPrev = list.pHead;
    NODE* pNew;
    
    if ((fp = OpenFileInReadMode(fileName)) == NULL) {
        printf("Error. Couldn't open file.\n");
        newHeader.pHead = NULL;
        newHeader.numBooks = 0;
    } else {
        while (!feof(fp)) {
            tempBook = GetBookFromFile(fp, bookId);
            if (tempBook.bookId == -1) {
                printf("Error reading book.\n");
            } else {
                pNew = (NODE*) malloc(sizeof(NODE));
                if (pNew) {
                    pNew->data = tempBook;
                    if (pPrev == NULL) {
                        pHead = pNew;
                    } else {
                        pNew->link = NULL;
                        pPrev->link = pNew;
                    }
                    pPrev = pNew;
                    bookId++;
                } else {
                    printf("Error couldn't allocate memory for node.\n");
                }
            }
            
            //Don't try to keep reading if there's extra newlines at the end of a line or the file
            DestroyNewlinesAndCarriageReturns(fp);
        }
        
        newHeader.pHead = pHead;
        newHeader.numBooks = bookId;
    }
    
    return newHeader;
}

FILE* OpenFileInReadMode(char* fileName) {
    FILE* fp;
    int stringLength = (int) strlen(DEFAULTFILEPATH) + (int) strlen(fileName);
    char fullFilePath[stringLength + 1];
    
    AppendFileNameToFilePath(fileName, DEFAULTFILEPATH, fullFilePath, stringLength);
    
    if ((fp = fopen(fullFilePath, "r")) == NULL) {
        printf("Couldn't open file at %s\n", fullFilePath);
    }
    
    return fp;
}

void AppendFileNameToFilePath(const char* fileName, const char* filepath, char* fullPath, int stringLength) {
    /*
     Add together a filename, filepath, and extension.
     Ensure that total filepath size does not exceed stringLength
     */
    
    //Copy the file path. After copy subtract the characters written from
    //stringLength
    strncpy(fullPath, filepath, stringLength);
    stringLength -= strlen(filepath);
    
    //Concatenate file name to fullPath. After Concatination subtract the length
    //of fileName from stringLength.
    strncat(fullPath, fileName, stringLength);
    stringLength -= strlen(fileName);
}

BOOK GetBookFromFile(FILE* fp, int bookId) {
    BOOK tempBook;
    char title[MAXTITLE + 1];
    char author[MAXAUTHOR + 1];
    char published[MAXYEAR + 1];
    
    tempBook.bookId = -1;
    if (!FileToStringWithoutNewline(fp, title, MAXTITLE)) {
        printf("Error: couldn't read title.\n");
        return tempBook;
    }
    if (!FileToStringWithoutNewline(fp, author, MAXAUTHOR)) {
        printf("Error: couldn't read author.\n");
        return tempBook;
    }
    if (!FileToStringWithoutNewline(fp, published, MAXYEAR)) {
        printf("Error: couldn't read year published.\n");
        return tempBook;
    }
    
    tempBook.bookId = bookId;
    strncpy(tempBook.title, title, MAXTITLE);
    strncpy(tempBook.author, author, MAXAUTHOR);
    strncpy(tempBook.published, published, MAXYEAR);
    
    return tempBook;
}

int FileToStringWithoutNewline(FILE* fp, char* str, int maxChars) {
    char* newline;
    char* carriageReturn;
    
    DestroyNewlinesAndCarriageReturns(fp);
    
    if (fgets(str, maxChars + 1, fp)) {
        
        //Remove all newlines and carriage returns from string
        while ((newline = strchr(str, '\n')) != NULL) {
            *newline = '\0';
        }
        
        while ((carriageReturn = strchr(str, '\r')) != NULL) {
            *carriageReturn = '\0';
        }
        
        return 1;
    } else {
        return 0;
    }
}

void DestroyNewlinesAndCarriageReturns(FILE* fp) {
    // When reading files from different operating systems,
    // we might have extra newlines '\n' or carriage returns '\r'.
    // This code discards those to make sure we can read everything as expected.
    
    char ch;
    while((ch = getc(fp)) == '\n' || ch == '\r');
    ungetc(ch, fp);
}

void PrintBookById(HEADER list) {
    int bookId;
    NODE* pWalker = list.pHead;
    NODE* bookNode = NULL;
    
    bookId = GetBookIdFromUser(list);
    
    while (pWalker != NULL && bookNode == NULL) {
        if (pWalker->data.bookId == bookId) {
            bookNode = pWalker;
        } else {
            pWalker = pWalker->link;
        }
    }
    
    if (bookNode == NULL) {
        printf("Couldn't find the requested book in the list.\n");
    } else {
        PrintBookInfo(bookNode->data);
    }
}

int GetBookIdFromUser(HEADER list) {
    int validSelection = 0;
    char strBookId[MAXIDLENGTH + 1];
    int bookId = -1;
    
    do {
        printf("Enter the id for a book whose info you would like printed.\n");
        printf("Valid id's range from 0 to %d.\n", list.numBooks - 1);
        
        scanf(" %5s", strBookId);
        int matched = (int) strspn(strBookId, "0123456789");
        if (matched == strlen(strBookId)) {
            bookId = atoi(strBookId);
            if (bookId >= 0 && bookId <= list.numBooks - 1) {
                validSelection = 1;
            } else {
                printf("Please enter a valid ID.\n");
            }
        } else {
            printf("Please enter a number.\n");
        }
        FLUSH;
    } while (!validSelection);
    
    return bookId;
}

void PrintBookInfo(BOOK bookNode) {
    printf("Found book at ID #%d\n", bookNode.bookId);
    printf("Title: %s\n", bookNode.title);
    printf("Author: %s\n", bookNode.author);
    printf("Year published: %s\n", bookNode.published);
    putchar('\n');
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
 
 
 
 TEST - InitializeOrDeleteLinkedList()
 
 --- Test AppendLinkedList() ---
 
 1. Start program, initialize list by selecting 1
    On next menu print some books using print book info option
 - Try a book somewhere in the middle of the list. Make sure info is as expected
 
 2. Start program, initialize list by selecting 1
    On next menu print some books using print book info option
 - Try a book at the beginning of the list. Make sure info is as expected
 
 3. Start program, initialize list by selecting 1
    On next menu print some books using print book info option
 - Try a book at the end of the list. Make sure info is as expected
 
 4. Start program, initialize list by selecting 1
    Hit 2 to append more books to the end of the list
    On next menu print some books using print book info option
 - Try a book somewhere in the middle of the list. Make sure info is as expected
 
 5. Start program, initialize list by selecting 1
    On next menu print some books using print book info option
    Hit 2 to append more books to the end of the list
 - Try a book at the beginning of the list. Make sure info is as expected
 
 6. Start program, initialize list by selecting 1
    On next menu print some books using print book info option
    Hit 2 to append more books to the end of the list
 - Try a book at the end of the list. Make sure info is as expected
 
 *** Test OpenFileInReadMode() ***
 1. Use bad filepath or filename.
 - Should print error message, and then go back to menu without initializing list.
 
 2. Use valid filename and filepath. Add breakpoint to check file pointer, fp, was opened correctly.
 - fp should be pointing to expected file.
 
 *** Test GetBookFromFile() ***
 1. Add breakpoint after GetBookFromFile() call.
 - Check that the tempBook object was created correctly and all of the member variables are as expected.
 
 2. Add breakpoint after GetBookFromFile() call.
    Add a newline with some gibberish to the end of the read file.
 - Should read all of the books correctly. Once you reach the line with gibberish, should give error about not being able to read book, and then go back to menu.
 
 
 --- Test DeleteLinkedList() ---
 1. Run program. Hit 1 to initialize the list.
    Hit 1 the second time the menu pops up to delete the list.
 - Add breakpoint in menu and make sure that HEADER has pHead = NULL and numBooks = 0, meaning the list was deleted.
 
 2. Run program. Hit 2 to append more data to list.
    Hit 1 the next time the menu pops up to delete the list.
 - Add breakpoint in menu and make sure that HEADER has pHead = NULL and numBooks = 0, meaning the list was deleted.
 
 
 --- Test GetBookIdFromUser() ---
 
 1. Run program. Hit 1 to initialize the list.
    Hit 2 to append more books to end of list
    Hit 4 to get book info.
 - Enter a valid id somewhere in the middle and make sure the expected info is printed
 
 2. Run program. Hit 1 to initialize the list.
    Hit 2 to append more books to end of list
    Hit 4 to get book info.
 - Enter a valid id at the beginning of the list, '0', and make sure the expected info is printed
 
 3. Run program. Hit 1 to initialize the list.
    Hit 2 to append more books to end of list
    Hit 4 to get book info.
 - Enter the last valid id in the list and make sure the expected info is printed
 
 4. Run program. Hit 1 to initialize the list.
    Hit 2 to append more books to end of list
    Hit 4 to get book info.
 - Enter some letters, 'adwagwaf'.
 - Should print error message and ask you to try again.
 
 5. Run program. Hit 1 to initialize the list.
    Hit 2 to append more books to end of list
    Hit 4 to get book info.
 - Enter a id that's invalid, '99'.
 - Should print error message and ask you to try again.
 
 6. Run program. Hit 1 to initialize the list.
    Hit 2 to append more books to end of list
    Hit 4 to get book info.
 - Enter some gibberish followed by a valid id 'a5'.
 - Should print error message and ask you to try again.
 
 7. Run program. Hit 1 to initialize the list.
    Hit 2 to append more books to end of list
    Hit 4 to get book info.
 - Enter a id that's valid followed by invalid characters, '5adwadw'.
 - Should print error message and ask you to try again.
 
 */
