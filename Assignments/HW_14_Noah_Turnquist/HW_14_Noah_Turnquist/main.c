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
#define INPUTFILENAME "HW14Data.txt"
#define MAXIDLENGTH 5
#define DEFAULTFILENAME "default"
#define MAXFILENAMESIZE 20

typedef struct {
    int bookId;
    char title[MAXTITLE + 1];
    char author[MAXAUTHOR + 1];
    char published[MAXYEAR + 1];
} BOOK;

typedef struct node_tag{
    BOOK data;
    struct node_tag* link;
} NODE;

typedef struct {
    NODE* pHead;
    NODE* pLast;
    int numBooks;
} HEADER;

enum MENUOPTIONS { INITORDELETELIST = 1, APPEND, WRITETOBINARY, PRINTBOOK, EXIT };
enum FILEOPTIONS { OVERWRITE = 1, RENAME, ABORT };

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
void WriteListToBinaryFile(HEADER list);
FILE* OpenWrBinaryFile(char* fullFileName, int fullFilePathSize, char* fileName);
void GetFileNameFromUser(char* fileName);
int CheckFileExists(const char* filepath);
int GetUserFileOption(const char* fileName);

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
            case WRITETOBINARY:
                WriteListToBinaryFile(list);
                break;
            case PRINTBOOK:
                PrintBookById(list);
                break;
            case EXIT:
                printf("Exiting program.\n");
                break;
        }
    } while(userChoice != EXIT);
    
    list = DeleteLinkedList(list);
    
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
    
    HEADER newHeader = { NULL, NULL, 0 };
    return newHeader;
}

HEADER AppendLinkedList(char* fileName, HEADER list) {
    FILE* fp;
    HEADER newHeader;
    BOOK tempBook;
    int bookId = list.numBooks;
    NODE* pHead = list.pHead;
    NODE* pPrev = list.pLast;
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
                    if (pHead == NULL) {
                        pHead = pNew;
                    }
                    pNew->link = NULL;
                    if (pPrev != NULL) {
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
        newHeader.pLast = pPrev;
    }
    
    fclose(fp);
    
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
    BOOK tempBook = { -1, "", "", "" };
    char title[MAXTITLE + 1];
    char author[MAXAUTHOR + 1];
    char published[MAXYEAR + 1];
    
    if (!FileToStringWithoutNewline(fp, title, MAXTITLE + 1)) {
        printf("Error: couldn't read title.\n");
        return tempBook;
    }
    if (!FileToStringWithoutNewline(fp, author, MAXAUTHOR + 1)) {
        printf("Error: couldn't read author.\n");
        return tempBook;
    }
    if (!FileToStringWithoutNewline(fp, published, MAXYEAR + 1)) {
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
    
    if (fgets(str, maxChars, fp)) {
        //If string size is reached before end of line in file.
        //Flush the rest of that line.
        if (strchr(str, '\n') == NULL) {
            while (getc(fp) != '\n');
        }
        
        //Remove all newlines and carriage returns from string
        while ((newline = strchr(str, '\n')) != NULL) {
            *newline = '\0';
        }
        
        while ((carriageReturn = strchr(str, '\r')) != NULL) {
            *carriageReturn = '\0';
        }
        
        //Ensure last character in string is null character
        str[maxChars - 1] = '\0';
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

void WriteListToBinaryFile(HEADER list) {
    FILE* fp;
    int fileLength = strlen(DEFAULTFILEPATH) + MAXFILENAMESIZE;
    char fileName[MAXFILENAMESIZE + 1];
    char fullFilePath[fileLength + 1];
    NODE* pWalker = list.pHead;
    
    if((fp = OpenWrBinaryFile(fullFilePath, fileLength, fileName)) == NULL) {
        printf("Error opening file for writing.\n");
    } else {
        printf("Successfully opened file.\n");
        
        while (pWalker != NULL) {
            if (fwrite(&(pWalker->data), sizeof(BOOK), 1, fp) != 1) {
                printf("Error couldn't write node.\n");
            } else {
                printf("Successfully wrote book %s to file %s\n", pWalker->data.title, fileName);
            }
            pWalker = pWalker->link;
        }
    }
    
    fclose(fp);
    putchar('\n');
}

FILE* OpenWrBinaryFile(char* fullFileName, int fullFilePathSize, char* fileName) {
    /*
     Prompt the user for the name of a file. Also give option to use default filename.
     Append filename to filepath. If file doesn't exist or
     file should be overwritten, open in write binary mode and return pointer to that file.
     If file does exist, have user select an option for how to proceed.
     If user chooses to abort, return NULL pointer.
     */
    
    int fileOption = -1;
    int fileExists;
    FILE* fPt;
    
    do {
        GetFileNameFromUser(fileName);
        AppendFileNameToFilePath(fileName, DEFAULTFILEPATH, fullFileName, fullFilePathSize);
        if ((fileExists = CheckFileExists(fullFileName)) == 1) {
            fileOption = GetUserFileOption(fileName);
        }
    } while(fileExists && fileOption == RENAME);
    
    if (fileExists == 0 || fileOption == OVERWRITE) {
        fPt = fopen(fullFileName, "wb");
    } else {
        fPt = NULL;
    }
    return fPt;
}

void GetFileNameFromUser(char* fileName) {
    /*
     Read in a filename from the user or use default filename if no name
     is provided.
     */
    
    printf("The default file name is: %s\n", DEFAULTFILENAME);
    printf("Enter a file name up to %d chars: ", MAXFILENAMESIZE);
    fgets(fileName, MAXFILENAMESIZE, stdin);
    if (fileName[0] == '\n') {
        strncpy(fileName, DEFAULTFILENAME, MAXFILENAMESIZE);
    }
    else if (!strchr(fileName, '\n')) {
        FLUSH;
    } else {
        *strchr(fileName, '\n') = '\0';
    }
}

int CheckFileExists(const char* filepath) {
    /*
     Check if file at filepath exists. If does exist return 1, otherwise
     return 0.
     */
    
    FILE* fCheck;
    int fileExists;
    if ((fCheck = fopen(filepath, "r")) != NULL) {
        fileExists = 1;
    } else {
        fileExists = 0;
    }
    fclose(fCheck);
    return fileExists;
}

int GetUserFileOption(const char* fileName) {
    /*
     Prompt the user to select one of the given file options: Overwrite, rename, or abort.
     Ensure user enters appropriate option and return their option.
     */
    
    int fileOption;
    int validEntry = 0;
    
    do {
        printf("File %s exists. Do you want to:\n", fileName);
        printf("\tOverwrite the file (%d),\n", OVERWRITE);
        printf("\tChange the filename (%d),\n", RENAME);
        printf("\tOr abort (%d)\n", ABORT);
        printf("Enter a number: ");
        if (scanf(" %d", &fileOption) == 1) {
            if (fileOption >= OVERWRITE && fileOption <= ABORT) {
                validEntry = 1;
            } else {
                printf("Please enter a valid number from the list of options.\n");
            }
        } else {
            printf("Please enter a number.\n");
        }
        FLUSH;
    } while (validEntry == 0);
    
    return fileOption;
}


//TODO: Update this once ready to submit assignment
//HW #14, Noah Turnquist
//Please select one of the options by entering the cooresponding number.
//Initialize the list: (1)
//Exit the program: (5)
//1
//
//Initialized list.
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from file to the list: (2)
//Write the list out to a binary file: (3)
//Get info on a book: (4)
//Exit the program: (5)
//4
//
//Enter the id for a book whose info you would like printed.
//Valid id's range from 0 to 14.5
//
//5
//
//Found book at ID #5
//Title: A Soldier's Duty
//Author: Johnson, Jean
//Year published: 2011
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from file to the list: (2)
//Write the list out to a binary file: (3)
//Get info on a book: (4)
//Exit the program: (5)
//4
//
//Enter the id for a book whose info you would like printed.
//Valid id's range from 0 to 14.
//0
//
//Found book at ID #0
//Title: On Basilisk Station
//Author: Weber, David
//Year published: 1993
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from file to the list: (2)
//Write the list out to a binary file: (3)
//Get info on a book: (4)
//Exit the program: (5)
//4
//
//Enter the id for a book whose info you would like printed.
//Valid id's range from 0 to 14.
//14
//
//Found book at ID #14
//Title: C Primer Plus
//Author: Prata, Stephen
//Year published: 2014
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from file to the list: (2)
//Write the list out to a binary file: (3)
//Get info on a book: (4)
//Exit the program: (5)
//2
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from file to the list: (2)
//Write the list out to a binary file: (3)
//Get info on a book: (4)
//Exit the program: (5)
//4
//
//Enter the id for a book whose info you would like printed.
//Valid id's range from 0 to 29.
//0
//
//Found book at ID #0
//Title: On Basilisk Station
//Author: Weber, David
//Year published: 1993
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from file to the list: (2)
//Write the list out to a binary file: (3)
//Get info on a book: (4)
//Exit the program: (5)
//4
//
//Enter the id for a book whose info you would like printed.
//Valid id's range from 0 to 29.
//15
//
//Found book at ID #15
//Title: On Basilisk Station
//Author: Weber, David
//Year published: 1993
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from file to the list: (2)
//Write the list out to a binary file: (3)
//Get info on a book: (4)
//Exit the program: (5)
//4
//
//Enter the id for a book whose info you would like printed.
//Valid id's range from 0 to 29.
//29
//
//Found book at ID #29
//Title: C Primer Plus
//Author: Prata, Stephen
//Year published: 2014
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from file to the list: (2)
//Write the list out to a binary file: (3)
//Get info on a book: (4)
//Exit the program: (5)
//4
//
//Enter the id for a book whose info you would like printed.
//Valid id's range from 0 to 29.2
//27
//
//Please enter a valid ID.
//Enter the id for a book whose info you would like printed.
//Valid id's range from 0 to 29.
//27
//
//Found book at ID #27
//Title: I, Robot
//Author: Asimov, Issac
//Year published: 1950
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from file to the list: (2)
//Write the list out to a binary file: (3)
//Get info on a book: (4)
//Exit the program: (5)
//3
//
//The default file name is: default
//Enter a file name up to 20 chars:
//File default exists. Do you want to:
//    Overwrite the file (1),
//    Change the filename (2),
//    Or abort (3)
//Enter a number: 1
//Successfully opened file.
//Successfully wrote book On Basilisk Station to file default
//Successfully wrote book The Sum of All Fears to file default
//Successfully wrote book Battle Born to file default
//Successfully wrote book Between Planets to file default
//Successfully wrote book Stranger in a Strange Land to file default
//Successfully wrote book A Soldier's Duty to file default
//Successfully wrote book Swords Against Wizardry to file default
//Successfully wrote book The Mote in God's Eye to file default
//Successfully wrote book Uncharted Stars to file default
//Successfully wrote book Raising Steam to file default
//Successfully wrote book Ender's Game to file default
//Successfully wrote book Foundation and Empire to file default
//Successfully wrote book I, Robot to file default
//Successfully wrote book The Hitchiker's guide to the Universe to file default
//Successfully wrote book C Primer Plus to file default
//Successfully wrote book On Basilisk Station to file default
//Successfully wrote book The Sum of All Fears to file default
//Successfully wrote book Battle Born to file default
//Successfully wrote book Between Planets to file default
//Successfully wrote book Stranger in a Strange Land to file default
//Successfully wrote book A Soldier's Duty to file default
//Successfully wrote book Swords Against Wizardry to file default
//Successfully wrote book The Mote in God's Eye to file default
//Successfully wrote book Uncharted Stars to file default
//Successfully wrote book Raising Steam to file default
//Successfully wrote book Ender's Game to file default
//Successfully wrote book Foundation and Empire to file default
//Successfully wrote book I, Robot to file default
//Successfully wrote book The Hitchiker's guide to the Universe to file default
//Successfully wrote book C Primer Plus to file default
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from file to the list: (2)
//Write the list out to a binary file: (3)
//Get info on a book: (4)
//Exit the program: (5)
//1
//
//Deleted list.
//Please select one of the options by entering the cooresponding number.
//Initialize the list: (1)
//Exit the program: (5)
//1
//
//Initialized list.
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from file to the list: (2)
//Write the list out to a binary file: (3)
//Get info on a book: (4)
//Exit the program: (5)
//2
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from file to the list: (2)
//Write the list out to a binary file: (3)
//Get info on a book: (4)
//Exit the program: (5)
//5
//
//Exiting program.
//Program ended with exit code: 0


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
 
 
 --- Test WriteListToBinaryFile() ---
 * All user input behavior doesn't need to be tested since those functions were stolen
 from HW13 and were fully tested in that assignment. *
 
 1. Run program. Hit 1 to initialize list.
    Hit 3 to output list. Use any filename desired.
 - Should see 15 books being written out, starting with 'On Basilisk Station' and ending with 'C Primer Plus'
 - Look at file and make sure contents look roughly correct.
 
 2. Run program. Hit 1 to initialize list.
    Hit 2 to append more data to end of list.
    Hit 3 to output list. Use any filename desired.
 - Should see 30 books being written out, starting with 'On Basilisk Station' and ending with 'C Primer Plus'
 - Should be two identical lists one after another.
 - Look at file and make sure contents look roughly correct.
 
 */
