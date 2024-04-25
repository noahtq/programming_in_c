//HW #15, Noah Turnquist

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FLUSH while(getchar() != '\n')
#define MAXTITLE 50
#define MAXAUTHOR 50
#define MAXYEAR 4
#define DEFAULTFILEPATH "/Users/noahturnquist/Documents/College/Spring_2024/Programming_in_C/Assignments/HW_15_Noah_Turnquist/HW_15_Noah_Turnquist/"
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
    int listInitialized;
} HEADER;

enum MENUOPTIONS {
    INITORDELETELIST = 1,
    APPEND,
    APPENDFROMBINARY,
    WRITETOBINARY,
    PRINTBOOK,
    PRINTALLBOOKS,
    SORTLIST,
    EXIT
};

enum FILEOPTIONS { OVERWRITE = 1, RENAME, ABORT };

int GetMenuOptionFromUser(HEADER list);
HEADER InitializeOrDeleteLinkedList(HEADER list);
HEADER DeleteLinkedList(HEADER list);
void PrintAllBooks(HEADER list);
HEADER AppendLinkedList(HEADER list, FILE* fp, int binaryFile);
HEADER AppendLinkedListFromDefaultFile(char* fileName, HEADER list);
HEADER AppendLinkedListFromUserBinaryFile(HEADER list);
FILE* OpenReadBinaryFile(void);
FILE* OpenFileInReadMode(char* fileName);
void AppendFileNameToFilePath(const char* fileName, const char* filepath, char* fullPath, int stringLength);
BOOK GetBookFromFile(FILE* fp, int bookId);
BOOK GetBookFromBinaryFile(FILE* fp, int bookId);
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
void LinkedListSelectionSort(HEADER* list);

int main(void) {
    int userChoice;
    HEADER list = { NULL, NULL, 0, 0 };
    
    printf("HW #15, Noah Turnquist\n");
    
    do {
        userChoice = GetMenuOptionFromUser(list);
        switch (userChoice) {
            case INITORDELETELIST:
                list = InitializeOrDeleteLinkedList(list);
                break;
            case APPEND:
                list = AppendLinkedListFromDefaultFile(INPUTFILENAME, list);
                break;
            case APPENDFROMBINARY:
                list = AppendLinkedListFromUserBinaryFile(list);
                break;
            case WRITETOBINARY:
                WriteListToBinaryFile(list);
                break;
            case PRINTBOOK:
                PrintBookById(list);
                break;
            case PRINTALLBOOKS:
                PrintAllBooks(list);
                break;
            case SORTLIST:
                LinkedListSelectionSort(&list);
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
    /*
     Prompt user to select a menu option.
     Only show valid menu options and check to make sure
     users enters a valid option.
     Return the users selection.
     */
    
    int validSelection = 0;
    char chUserChoice;
    int userChoice = -1;
    
    do {
        printf("Please select one of the options by entering the cooresponding number.\n");
        if (list.listInitialized == 0) {
            printf("Initialize the list: (%d)\n", INITORDELETELIST);
        } else {
            printf("Delete the list: (%d)\n", INITORDELETELIST);
            printf("Append data from default file to the list: (%d)\n", APPEND);
            printf("Append data from binary file to the list: (%d)\n", APPENDFROMBINARY);
            if (list.pHead) {
                printf("Write the list out to a binary file: (%d)\n", WRITETOBINARY);
                printf("Get info on a book: (%d)\n", PRINTBOOK);
                printf("Print all books in list: (%d)\n", PRINTALLBOOKS);
                printf("Sort the list using selection sort: (%d)\n", SORTLIST);
            }
        }
        printf("Exit the program: (%d)\n", EXIT);
        chUserChoice = getchar();
        if (isdigit(chUserChoice)) {
            userChoice = chUserChoice - '0';
            if (list.listInitialized == 0 && (userChoice == INITORDELETELIST || userChoice == EXIT)) {
                validSelection = 1;
            } else if (list.listInitialized == 1 && list.pHead == NULL && ((userChoice >= INITORDELETELIST && userChoice <= APPENDFROMBINARY) || userChoice == EXIT)) {
                validSelection = 1;
            } else if (list.pHead && list.listInitialized && (userChoice >= INITORDELETELIST && userChoice <= EXIT)) {
                validSelection = 1;
            } else {
                printf("Please pick from one of the menu options.\n");
            }
        } else {
            printf("Please enter a number.\n");
        }
        FLUSH;
    } while(!validSelection);
    
    return userChoice;
}

HEADER InitializeOrDeleteLinkedList(HEADER list) {
    /*
     If linked list is uninitialzied, initialize it
     otherwise delete all of the list elements and set
     pointers to NULL
     */
    
    HEADER newHeader = { NULL, NULL, 0, 0 };
    
    if (!list.listInitialized) {
        newHeader.listInitialized = 1;
        printf("Initialized list. Ready for appending.\n\n");
    } else {
        newHeader = DeleteLinkedList(list);
        printf("Deleted list.\n\n");
    }
    
    return newHeader;
}

void PrintAllBooks(HEADER list) {
    /*
     Go through linked list and print data
     of each book.
     */
    
    NODE* pWalker = list.pHead;
    
    printf("%-5s %-50s %-50s %-4s\n", "ID", "Title", "Author", "Year");
    printf("%-5s %-50s %-50s %-4s\n", "--", "-----", "------", "----");
    while(pWalker != NULL) {
        printf("%-5d %-50s %-50s (%4s)\n",
               pWalker->data.bookId,
               pWalker->data.title,
               pWalker->data.author,
               pWalker->data.published);
        pWalker = pWalker->link;
    }
    putchar('\n');
}

HEADER DeleteLinkedList(HEADER list) {
    /*
     Go through linked list and deallocate memory
     for nodes. Set HEADER pointers to null.
     */
    
    NODE* pCur = list.pHead;
    NODE* pNext;
    
    while(pCur != NULL) {
        pNext = pCur->link;
        free(pCur);
        pCur = pNext;
    }
    
    HEADER newHeader = { NULL, NULL, 0, 0 };
    return newHeader;
}

HEADER AppendLinkedList(HEADER list, FILE* fp, int binaryFile) {
    /*
     Grab all books in format (Title, Author, Year Published)
     from file in file pointer. Append to the end of the linked list "list".
     binaryFile indicates if reading from a binary file or not.
     */
    
    HEADER newHeader;
    BOOK tempBook;
    int bookId = list.numBooks;
    NODE* pHead = list.pHead;
    NODE* pPrev = list.pLast;
    NODE* pNew;
    while (!feof(fp)) {
        if (binaryFile) {
            tempBook = GetBookFromBinaryFile(fp, bookId);
        } else {
            tempBook = GetBookFromFile(fp, bookId);
        }
        if (tempBook.bookId != -1) {
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
        
        if (!binaryFile) {
            //Don't try to keep reading if there's extra newlines at the end of a line or the file
            DestroyNewlinesAndCarriageReturns(fp);
        }
    }
    
    newHeader.pHead = pHead;
    newHeader.numBooks = bookId;
    newHeader.pLast = pPrev;
    newHeader.listInitialized = list.listInitialized;
    
    printf("Appended %d books to linked list.\n\n", newHeader.numBooks - list.numBooks);
    
    return newHeader;
}

HEADER AppendLinkedListFromDefaultFile(char* fileName, HEADER list) {
    /*
     Grab all books in format (Title, Author, Year Published)
     from file at fileName. Append to the end of the linked list "list".
     */
    
    FILE* fp;
    HEADER newHeader;
    
    if ((fp = OpenFileInReadMode(fileName)) == NULL) {
        printf("Error. Couldn't open file.\n");
        newHeader.pHead = NULL;
        newHeader.numBooks = 0;
    } else {
        newHeader = AppendLinkedList(list, fp, 0);
    }
    
    fclose(fp);
    
    return newHeader;
}

HEADER AppendLinkedListFromUserBinaryFile(HEADER list) {
    FILE* fp;
    HEADER newHeader;
    
    fp = OpenReadBinaryFile();
    if (fp == NULL) {
        newHeader = list;
        printf("Couldn't open file.\n");
        return newHeader;
    }
    putchar('\n');
    newHeader = AppendLinkedList(list, fp, 1);
    
    return newHeader;
}

FILE* OpenReadBinaryFile(void) {
    /*
     Attempt to open a file in read binary mode and return a pointer to that
     file stream.
     User is prompted for filename and given several options if file already
     exists. There is also an option to use a default filename.
     */
    
    FILE* fp = NULL;
    char fileName[MAXFILENAMESIZE];
    int fullFilePathSize = strlen(DEFAULTFILEPATH) + MAXFILENAMESIZE;
    char fullFileName[fullFilePathSize + 1];
    
    do {
        GetFileNameFromUser(fileName);
        if (fileName[0] != 'q') {
            AppendFileNameToFilePath(fileName, DEFAULTFILEPATH, fullFileName, fullFilePathSize);
            if ((fp = fopen(fullFileName, "rb")) == NULL) {
                printf("Couldn't find file. Try again.\n");
            }
        } 
    } while(fp == NULL && fileName[0] != 'q');
    
    return fp;
}

FILE* OpenFileInReadMode(char* fileName) {
    /*
     Open the file in DEFAULTFILEPATH with the name fileName in read mode.
     Return a pointer to the file.
     */
    
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
     Add together a filename and filepath.
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
    /*
     Create a book object from the info in the file fp.
     Check for errors and return the book object.
     */
    
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

BOOK GetBookFromBinaryFile(FILE* fp, int bookId) {
    BOOK tempBook;
    
    if (fread(&tempBook, sizeof(BOOK), 1, fp) != 1) {
        //In case ID was overwritten by fRead. Make sure
        //book ID is -1 so that we can handle error in return function.
        tempBook.bookId = -1;
        return tempBook;
    }
    tempBook.bookId = bookId;
    return tempBook;
}

int FileToStringWithoutNewline(FILE* fp, char* str, int maxChars) {
    /*
     Read in a line characters from the text file fp,
     store the input in str.
     Return 1 if successful and 0 if unsuccessful.
     */
    
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
    /*
     When reading files from different operating systems,
     we might have extra newlines '\n' or carriage returns '\r'.
     This code discards those to make sure we can read everything as expected.
     */
    
    char ch;
    while((ch = getc(fp)) == '\n' || ch == '\r');
    ungetc(ch, fp);
}

void PrintBookById(HEADER list) {
    /*
     Prompt a user for a book ID. Print out the
     information for that book or an error message
     if the book can't be found.
     */
    
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
    /*
     Prompt the user for the ID of a book they would
     like to view the information for. Make sure
     user selects a valid ID and return the ID from
     function.
     */
    
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
    /*
     Print the id, title, author, and year published from
     a BOOK object.
     */
    
    printf("Found book at ID #%d\n", bookNode.bookId);
    printf("Title: %s\n", bookNode.title);
    printf("Author: %s\n", bookNode.author);
    printf("Year published: %s\n", bookNode.published);
    putchar('\n');
}

void WriteListToBinaryFile(HEADER list) {
    /*
     Prompt the user for a file name. File will be opened in write binary mode
     in the DEFAULTFILEPATH folder. Print out all of the book information
     contained in the list's nodes.
     */
    
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
    int fileExists = 0;
    FILE* fPt;
    
    do {
        GetFileNameFromUser(fileName);
        if (fileName[0] != 'q') {
            AppendFileNameToFilePath(fileName, DEFAULTFILEPATH, fullFileName, fullFilePathSize);
            if ((fileExists = CheckFileExists(fullFileName)) == 1) {
                fileOption = GetUserFileOption(fileName);
            }
        }
    } while(fileExists && fileOption == RENAME && fileName[0] != 'q');
    
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
    printf("Enter 'q' to abort.\n");
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

void LinkedListSelectionSort(HEADER* list) {
    /*
     Sorting linked list using selection sort.
     Loop through list one more time and set
     all book IDs to be correct.
     Have to do second pass through list since
     selection sort is unstable so the book IDs
     become out of order during the sorting.
     */
    
    NODE* pCur = list->pHead;
    while(pCur != NULL) {
        NODE* pMin = pCur;
        NODE* pWalker = pCur->link;
        while (pWalker != NULL) {
            if (strncmp(pMin->data.author, pWalker->data.author, MAXAUTHOR) > 0) { //TODO: Change to case insensitive string compare
                pMin = pWalker;
            }
            pWalker = pWalker->link;
        }
        
        //Swap all node connections i.e. swap places of nodes in linked list
        BOOK tempBook = pCur->data;
        pCur->data = pMin->data;
        pMin->data = tempBook;
        
        pCur = pCur->link;
    }
    
    int count = 0;
    NODE* pWalker = list->pHead;
    while(pWalker != NULL) {
        pWalker->data.bookId = count;
        
        count++;
        pWalker = pWalker->link;
    }
    
    printf("Sorted list using selection sort.\n\n");
}


//HW #15, Noah Turnquist
//Please select one of the options by entering the cooresponding number.
//Initialize the list: (1)
//Exit the program: (8)
//1
//
//Initialized list. Ready for appending.
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from default file to the list: (2)
//Append data from binary file to the list: (3)
//Exit the program: (8)
//2
//
//Appended 15 books to linked list.
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from default file to the list: (2)
//Append data from binary file to the list: (3)
//Write the list out to a binary file: (4)
//Get info on a book: (5)
//Print all books in list: (6)
//Sort the list using selection sort: (7)
//Exit the program: (8)
//2
//
//Appended 15 books to linked list.
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from default file to the list: (2)
//Append data from binary file to the list: (3)
//Write the list out to a binary file: (4)
//Get info on a book: (5)
//Print all books in list: (6)
//Sort the list using selection sort: (7)
//Exit the program: (8)
//6
//
//ID    Title                                              Author                                             Year
//--    -----                                              ------                                             ----
//0     On Basilisk Station                                Weber, David                                       (1993)
//1     The Sum of All Fears                               Clancy, Tom                                        (1992)
//2     Battle Born                                        Brown, Dale                                        (1999)
//3     Between Planets                                    Heinlein, Robert A.                                (1953)
//4     Stranger in a Strange Land                         Heinlein, Robert A.                                (1959)
//5     A Soldier's Duty                                   Johnson, Jean                                      (2011)
//6     Swords Against Wizardry                            Leiber, Fritz                                      (1968)
//7     The Mote in God's Eye                              Niven, Larry                                       (1992)
//8     Uncharted Stars                                    Norton, Andre                                      (1968)
//9     Raising Steam                                      Pratchett, Terry                                   (2013)
//10    Ender's Game                                       Card, Orson Scott                                  (1977)
//11    Foundation and Empire                              Asimov, Issac                                      (1951)
//12    I, Robot                                           Asimov, Issac                                      (1950)
//13    The Hitchiker's guide to the Universe              Adams, Douglas                                     (1979)
//14    C Primer Plus                                      Prata, Stephen                                     (2014)
//15    On Basilisk Station                                Weber, David                                       (1993)
//16    The Sum of All Fears                               Clancy, Tom                                        (1992)
//17    Battle Born                                        Brown, Dale                                        (1999)
//18    Between Planets                                    Heinlein, Robert A.                                (1953)
//19    Stranger in a Strange Land                         Heinlein, Robert A.                                (1959)
//20    A Soldier's Duty                                   Johnson, Jean                                      (2011)
//21    Swords Against Wizardry                            Leiber, Fritz                                      (1968)
//22    The Mote in God's Eye                              Niven, Larry                                       (1992)
//23    Uncharted Stars                                    Norton, Andre                                      (1968)
//24    Raising Steam                                      Pratchett, Terry                                   (2013)
//25    Ender's Game                                       Card, Orson Scott                                  (1977)
//26    Foundation and Empire                              Asimov, Issac                                      (1951)
//27    I, Robot                                           Asimov, Issac                                      (1950)
//28    The Hitchiker's guide to the Universe              Adams, Douglas                                     (1979)
//29    C Primer Plus                                      Prata, Stephen                                     (2014)
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from default file to the list: (2)
//Append data from binary file to the list: (3)
//Write the list out to a binary file: (4)
//Get info on a book: (5)
//Print all books in list: (6)
//Sort the list using selection sort: (7)
//Exit the program: (8)
//7
//
//Sorted list using selection sort.
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from default file to the list: (2)
//Append data from binary file to the list: (3)
//Write the list out to a binary file: (4)
//Get info on a book: (5)
//Print all books in list: (6)
//Sort the list using selection sort: (7)
//Exit the program: (8)
//6
//
//ID    Title                                              Author                                             Year
//--    -----                                              ------                                             ----
//0     The Hitchiker's guide to the Universe              Adams, Douglas                                     (1979)
//1     The Hitchiker's guide to the Universe              Adams, Douglas                                     (1979)
//2     Foundation and Empire                              Asimov, Issac                                      (1951)
//3     I, Robot                                           Asimov, Issac                                      (1950)
//4     Foundation and Empire                              Asimov, Issac                                      (1951)
//5     I, Robot                                           Asimov, Issac                                      (1950)
//6     Battle Born                                        Brown, Dale                                        (1999)
//7     Battle Born                                        Brown, Dale                                        (1999)
//8     Ender's Game                                       Card, Orson Scott                                  (1977)
//9     Ender's Game                                       Card, Orson Scott                                  (1977)
//10    The Sum of All Fears                               Clancy, Tom                                        (1992)
//11    The Sum of All Fears                               Clancy, Tom                                        (1992)
//12    Between Planets                                    Heinlein, Robert A.                                (1953)
//13    Between Planets                                    Heinlein, Robert A.                                (1953)
//14    Stranger in a Strange Land                         Heinlein, Robert A.                                (1959)
//15    Stranger in a Strange Land                         Heinlein, Robert A.                                (1959)
//16    A Soldier's Duty                                   Johnson, Jean                                      (2011)
//17    A Soldier's Duty                                   Johnson, Jean                                      (2011)
//18    Swords Against Wizardry                            Leiber, Fritz                                      (1968)
//19    Swords Against Wizardry                            Leiber, Fritz                                      (1968)
//20    The Mote in God's Eye                              Niven, Larry                                       (1992)
//21    The Mote in God's Eye                              Niven, Larry                                       (1992)
//22    Uncharted Stars                                    Norton, Andre                                      (1968)
//23    Uncharted Stars                                    Norton, Andre                                      (1968)
//24    C Primer Plus                                      Prata, Stephen                                     (2014)
//25    C Primer Plus                                      Prata, Stephen                                     (2014)
//26    Raising Steam                                      Pratchett, Terry                                   (2013)
//27    Raising Steam                                      Pratchett, Terry                                   (2013)
//28    On Basilisk Station                                Weber, David                                       (1993)
//29    On Basilisk Station                                Weber, David                                       (1993)
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from default file to the list: (2)
//Append data from binary file to the list: (3)
//Write the list out to a binary file: (4)
//Get info on a book: (5)
//Print all books in list: (6)
//Sort the list using selection sort: (7)
//Exit the program: (8)
//4
//
//The default file name is: default
//Enter 'q' to abort.
//Enter a file name up to 20 chars:
//Successfully opened file.
//Successfully wrote book The Hitchiker's guide to the Universe to file default
//Successfully wrote book The Hitchiker's guide to the Universe to file default
//Successfully wrote book Foundation and Empire to file default
//Successfully wrote book I, Robot to file default
//Successfully wrote book Foundation and Empire to file default
//Successfully wrote book I, Robot to file default
//Successfully wrote book Battle Born to file default
//Successfully wrote book Battle Born to file default
//Successfully wrote book Ender's Game to file default
//Successfully wrote book Ender's Game to file default
//Successfully wrote book The Sum of All Fears to file default
//Successfully wrote book The Sum of All Fears to file default
//Successfully wrote book Between Planets to file default
//Successfully wrote book Between Planets to file default
//Successfully wrote book Stranger in a Strange Land to file default
//Successfully wrote book Stranger in a Strange Land to file default
//Successfully wrote book A Soldier's Duty to file default
//Successfully wrote book A Soldier's Duty to file default
//Successfully wrote book Swords Against Wizardry to file default
//Successfully wrote book Swords Against Wizardry to file default
//Successfully wrote book The Mote in God's Eye to file default
//Successfully wrote book The Mote in God's Eye to file default
//Successfully wrote book Uncharted Stars to file default
//Successfully wrote book Uncharted Stars to file default
//Successfully wrote book C Primer Plus to file default
//Successfully wrote book C Primer Plus to file default
//Successfully wrote book Raising Steam to file default
//Successfully wrote book Raising Steam to file default
//Successfully wrote book On Basilisk Station to file default
//Successfully wrote book On Basilisk Station to file default
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from default file to the list: (2)
//Append data from binary file to the list: (3)
//Write the list out to a binary file: (4)
//Get info on a book: (5)
//Print all books in list: (6)
//Sort the list using selection sort: (7)
//Exit the program: (8)
//3
//
//The default file name is: default
//Enter 'q' to abort.
//Enter a file name up to 20 chars:
//
//Appended 30 books to linked list.
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from default file to the list: (2)
//Append data from binary file to the list: (3)
//Write the list out to a binary file: (4)
//Get info on a book: (5)
//Print all books in list: (6)
//Sort the list using selection sort: (7)
//Exit the program: (8)
//5
//
//Enter the id for a book whose info you would like printed.
//Valid id's range from 0 to 59.
//48
//
//Found book at ID #48
//Title: Swords Against Wizardry
//Author: Leiber, Fritz
//Year published: 1968
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from default file to the list: (2)
//Append data from binary file to the list: (3)
//Write the list out to a binary file: (4)
//Get info on a book: (5)
//Print all books in list: (6)
//Sort the list using selection sort: (7)
//Exit the program: (8)
//7
//
//Sorted list using selection sort.
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from default file to the list: (2)
//Append data from binary file to the list: (3)
//Write the list out to a binary file: (4)
//Get info on a book: (5)
//Print all books in list: (6)
//Sort the list using selection sort: (7)
//Exit the program: (8)
//6
//
//ID    Title                                              Author                                             Year
//--    -----                                              ------                                             ----
//0     The Hitchiker's guide to the Universe              Adams, Douglas                                     (1979)
//1     The Hitchiker's guide to the Universe              Adams, Douglas                                     (1979)
//2     The Hitchiker's guide to the Universe              Adams, Douglas                                     (1979)
//3     The Hitchiker's guide to the Universe              Adams, Douglas                                     (1979)
//4     Foundation and Empire                              Asimov, Issac                                      (1951)
//5     I, Robot                                           Asimov, Issac                                      (1950)
//6     Foundation and Empire                              Asimov, Issac                                      (1951)
//7     I, Robot                                           Asimov, Issac                                      (1950)
//8     Foundation and Empire                              Asimov, Issac                                      (1951)
//9     I, Robot                                           Asimov, Issac                                      (1950)
//10    Foundation and Empire                              Asimov, Issac                                      (1951)
//11    I, Robot                                           Asimov, Issac                                      (1950)
//12    Battle Born                                        Brown, Dale                                        (1999)
//13    Battle Born                                        Brown, Dale                                        (1999)
//14    Battle Born                                        Brown, Dale                                        (1999)
//15    Battle Born                                        Brown, Dale                                        (1999)
//16    Ender's Game                                       Card, Orson Scott                                  (1977)
//17    Ender's Game                                       Card, Orson Scott                                  (1977)
//18    Ender's Game                                       Card, Orson Scott                                  (1977)
//19    Ender's Game                                       Card, Orson Scott                                  (1977)
//20    The Sum of All Fears                               Clancy, Tom                                        (1992)
//21    The Sum of All Fears                               Clancy, Tom                                        (1992)
//22    The Sum of All Fears                               Clancy, Tom                                        (1992)
//23    The Sum of All Fears                               Clancy, Tom                                        (1992)
//24    Between Planets                                    Heinlein, Robert A.                                (1953)
//25    Between Planets                                    Heinlein, Robert A.                                (1953)
//26    Stranger in a Strange Land                         Heinlein, Robert A.                                (1959)
//27    Stranger in a Strange Land                         Heinlein, Robert A.                                (1959)
//28    Between Planets                                    Heinlein, Robert A.                                (1953)
//29    Between Planets                                    Heinlein, Robert A.                                (1953)
//30    Stranger in a Strange Land                         Heinlein, Robert A.                                (1959)
//31    Stranger in a Strange Land                         Heinlein, Robert A.                                (1959)
//32    A Soldier's Duty                                   Johnson, Jean                                      (2011)
//33    A Soldier's Duty                                   Johnson, Jean                                      (2011)
//34    A Soldier's Duty                                   Johnson, Jean                                      (2011)
//35    A Soldier's Duty                                   Johnson, Jean                                      (2011)
//36    Swords Against Wizardry                            Leiber, Fritz                                      (1968)
//37    Swords Against Wizardry                            Leiber, Fritz                                      (1968)
//38    Swords Against Wizardry                            Leiber, Fritz                                      (1968)
//39    Swords Against Wizardry                            Leiber, Fritz                                      (1968)
//40    The Mote in God's Eye                              Niven, Larry                                       (1992)
//41    The Mote in God's Eye                              Niven, Larry                                       (1992)
//42    The Mote in God's Eye                              Niven, Larry                                       (1992)
//43    The Mote in God's Eye                              Niven, Larry                                       (1992)
//44    Uncharted Stars                                    Norton, Andre                                      (1968)
//45    Uncharted Stars                                    Norton, Andre                                      (1968)
//46    Uncharted Stars                                    Norton, Andre                                      (1968)
//47    Uncharted Stars                                    Norton, Andre                                      (1968)
//48    C Primer Plus                                      Prata, Stephen                                     (2014)
//49    C Primer Plus                                      Prata, Stephen                                     (2014)
//50    C Primer Plus                                      Prata, Stephen                                     (2014)
//51    C Primer Plus                                      Prata, Stephen                                     (2014)
//52    Raising Steam                                      Pratchett, Terry                                   (2013)
//53    Raising Steam                                      Pratchett, Terry                                   (2013)
//54    Raising Steam                                      Pratchett, Terry                                   (2013)
//55    Raising Steam                                      Pratchett, Terry                                   (2013)
//56    On Basilisk Station                                Weber, David                                       (1993)
//57    On Basilisk Station                                Weber, David                                       (1993)
//58    On Basilisk Station                                Weber, David                                       (1993)
//59    On Basilisk Station                                Weber, David                                       (1993)
//
//Please select one of the options by entering the cooresponding number.
//Delete the list: (1)
//Append data from default file to the list: (2)
//Append data from binary file to the list: (3)
//Write the list out to a binary file: (4)
//Get info on a book: (5)
//Print all books in list: (6)
//Sort the list using selection sort: (7)
//Exit the program: (8)
//8
//
//Exiting program.
//Program ended with exit code: 0
