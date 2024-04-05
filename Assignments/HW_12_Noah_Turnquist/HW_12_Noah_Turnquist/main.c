// HW #12, Noah Turnquist

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#define STUDENTARRSIZE 50
#define MAXNAMESIZE 20
#define IDLENGTH 4
#define NUMOFQUIZZES 4
#define MAXOUTPUTSTRSIZE 35
#define DATAFILEPATH "/Users/noahturnquist/Documents/College/Spring_2024/Programming_in_C/Assignments/HW_12_Noah_Turnquist/HW_12_Noah_Turnquist/HW12Data.txt"

typedef struct {
    char name[MAXNAMESIZE + 1];
    char id[IDLENGTH + 1];
    int quizzes[NUMOFQUIZZES];
    int exam;
} STUDENT;

STUDENT* CreateStudentArray(void);
int ReadStudentsFromFile(STUDENT* studentArray, char* filePath);
void PrintStudentInfo(STUDENT* studentArray, int studentsRead, int* highestQuizScores, int* lowestQuizScores, int* averageQuizScores, int highestExamScore, int lowestExamScore, int averageExamScore);
void GetAllScoreInfo(STUDENT* studentArray, int studentsRead, int* highestQuizScores, int* lowestQuizScores, int* averageQuizScores, int* highestExamScore, int* lowestExamScore, int* averageExamScore);
void PrintInfoHeaders(void);
void PrintTitle(char* title);
void PrintStatisticsLine(char* label, int* quizArr, int examScore);

int main(void) {
    STUDENT* studentArray;
    int studentsRead;
    int highestQuizScores[NUMOFQUIZZES];
    int lowestQuizScores[NUMOFQUIZZES];
    int averageQuizScores[NUMOFQUIZZES];
    int highestExamScore;
    int lowestExamScore;
    int averageExamScore;
    
    studentArray = CreateStudentArray();
    if (studentArray == NULL) {
        return 1;
    }
    
    studentsRead = ReadStudentsFromFile(studentArray, DATAFILEPATH);
    if (studentsRead <= 0) {
        printf("No students in file.\n");
        return 1;
    }
    
    GetAllScoreInfo(studentArray, studentsRead, highestQuizScores, lowestQuizScores, averageQuizScores, &highestExamScore, &lowestExamScore, &averageExamScore);
    
    printf("HW #12, Noah Turnquist\n\n");
    PrintStudentInfo(studentArray, studentsRead, highestQuizScores, lowestQuizScores, averageQuizScores, highestExamScore, lowestExamScore, averageExamScore);
    printf("\n***Finished***\n");
    
    free(studentArray);
    studentArray = NULL;
    
    return 0;
}

STUDENT* CreateStudentArray(void) {
    /*
     Create an array on the heap of for STUDENTS of STUDENTARRSIZE size.
     Error check and return a pointer to this array.
     */
    
    STUDENT* studentArray = (STUDENT*) malloc(STUDENTARRSIZE * sizeof(STUDENT));
    if (studentArray == NULL) {
        printf("Could not allocate student array on heap.\n");
        return NULL;
    }
    return studentArray;
}

int ReadStudentsFromFile(STUDENT* studentArray, char* filePath) {
    /*
     Read in students contained in a file at filePath, save them
     in the array pointed to by studentArray.
     */
    
    int studentsRead = 0;
    FILE* studentFile;
    
    if ((studentFile = fopen(filePath, "r")) == NULL) {
        printf("Error opening student file.\n");
        return -1;
    }
    
    int keepReading = 1;
    STUDENT* itr = studentArray;
    STUDENT* end = studentArray + STUDENTARRSIZE;
    while (keepReading) {
        STUDENT tempStudent;
        char fullName[MAXNAMESIZE + 1], lastName[MAXNAMESIZE];
        
        if (fscanf(studentFile, " %s %s %s", fullName, lastName, tempStudent.id) == 3) {
            for (int i = 0; i < NUMOFQUIZZES; i++) {
                fscanf(studentFile, " %d", &(tempStudent.quizzes[i]));
            }
            fscanf(studentFile, " %d", &(tempStudent.exam));
            
            strncat(fullName, " ", 1); //Add space between first and last name
            int maxCopyLength = MAXNAMESIZE - (int) strlen(fullName);
            strncat(fullName, lastName, maxCopyLength);
            strncpy(tempStudent.name, fullName, MAXNAMESIZE);
            
            if (itr < end) {
                *itr = tempStudent;
                itr++;
                studentsRead++;
            }
            
        } else {
            keepReading = 0;
        }
    }
    
    fclose(studentFile);
    return studentsRead;
}

void PrintStudentInfo(STUDENT* studentArray, int studentsRead, int* highestQuizScores, int* lowestQuizScores, int* averageQuizScores, int highestExamScore, int lowestExamScore, int averageExamScore) {
    /*
     Outputting student info and statistics
     Broken down into subfunctions to accommodate changes to max name length,
     length of ID, number of quizzes.
     */
    
    PrintTitle("DATA");
    PrintInfoHeaders();
    
    char stuOutputStr[MAXOUTPUTSTRSIZE + 1];
    sprintf(stuOutputStr, "#%%2d: %%-%ds %%%ds ", MAXNAMESIZE, IDLENGTH);
    for (STUDENT* itr = studentArray; itr < studentArray + studentsRead; itr++) {
        int current = (int) (itr - studentArray);
        printf(stuOutputStr, current, itr->name, itr->id);
        for (int i = 0; i < NUMOFQUIZZES; i++) {
            printf("%6d ", itr->quizzes[i]);
        }
        printf("%6d\n", itr->exam);
    }
    putchar('\n');
    
    PrintTitle("STATISTICS");
    
    PrintStatisticsLine("Highest Scores", highestQuizScores, highestExamScore);
    PrintStatisticsLine("Lowest  Scores", lowestQuizScores, lowestExamScore);
    PrintStatisticsLine("Average Scores", averageQuizScores, averageExamScore);
}

void GetAllScoreInfo(STUDENT* studentArray, int studentsRead, int* highestQuizScores, int* lowestQuizScores, int* averageQuizScores, int* highestExamScore, int* lowestExamScore, int* averageExamScore) {
    /*
     Get all of the quiz and exam info from the array of students
     Save the quizes to a 2D array, save the exams to an array.
     Find the highest, lowest, and average for each quiz and exam.
     Return everything using pass by reference.
     */
    
    int allQuizScores[NUMOFQUIZZES][studentsRead];
    int allExamScores[studentsRead];
    
    for (STUDENT* itr = studentArray; itr < studentArray + studentsRead; itr++) {
        int current = (int) (itr - studentArray);
        for (int i = 0; i < NUMOFQUIZZES; i++) {
            allQuizScores[i][current] = itr->quizzes[i];
        }
        allExamScores[current] = itr->exam;
    }

    for (int i = 0; i < NUMOFQUIZZES; i++) {
        int sum = 0;
        int max = allQuizScores[i][0];
        int min = allQuizScores[i][0];
        
        for (int j = 0; j < studentsRead; j++) {
            sum += allQuizScores[i][j];
            if (max < allQuizScores[i][j]) {
                max = allQuizScores[i][j];
            }
            if (min > allQuizScores[i][j] && allQuizScores[i][j] > 0) {
                min = allQuizScores[i][j];
            }
        }
        
        int average = (int) (sum / studentsRead);
        
        highestQuizScores[i] = max;
        lowestQuizScores[i] = min;
        averageQuizScores[i] = average;
    }
    
    int examSum = 0;
    int examMax = allExamScores[0];
    int examMin = allExamScores[0];
    for (int i = 0; i < studentsRead; i++) {
        examSum += allExamScores[i];
        if (examMax < allExamScores[i]) {
            examMax = allExamScores[i];
        }
        if (examMin > allExamScores[i] && allExamScores[i] > 0) {
            examMin = allExamScores[i];
        }
    }
    
    int examAverage = (int) (examSum / studentsRead);
    
    *highestExamScore = examMax;
    *lowestExamScore = examMin;
    *averageExamScore = examAverage;
}

void PrintInfoHeaders(void) {
    /*
     Print the headers for the student information
     Headers will update automatically depending on MAXNAMESIZE, NUMOFQUIZES,
     and IDLENGTH.
     */
    
    printf("Set#     ");
    printf("Name");
    for (int i = 0; i < MAXNAMESIZE - 7; i++) {
        putchar(' ');
    }
    printf(" ID  ");
    
    for (int i = 0; i < NUMOFQUIZZES; i++) {
        printf("Quiz %d ", i + 1);
    }
    
    printf(" Exam \n");
    
    printf("---- ");
    for (int i = 0; i < MAXNAMESIZE; i++) {
        putchar('-');
    }
    putchar(' ');
    
    for (int i = 0; i < IDLENGTH; i++) {
        putchar('-');
    }
    putchar(' ');
    
    for (int i = 0; i < NUMOFQUIZZES; i++) {
        printf("------ ");
    }
    printf("------\n");
}

void PrintTitle(char* title) {
    /*
     Print a title in the format: ==== title ====
     Title length varies by the MAXNAMESIZE, IDLENGTH, NUMOFQUIZZES
     */
    
    int totalStrSize = 13 + MAXNAMESIZE + IDLENGTH + 7 * NUMOFQUIZZES;
    int titleLen = (int) strlen(title);
    
    for (int i = 0; i < (int) ((totalStrSize - titleLen) / 2); i++) {
        putchar('=');
    }
    printf(" %s ", title);
    for (int i = 0; i < (int) ((totalStrSize - titleLen) / 2); i++) {
        putchar('=');
    }
    putchar('\n');
}

void PrintStatisticsLine(char* label, int* quizArr, int examScore) {
    /*
    Print label, the given value for each quiz, and the examScore value
    Formatted to match rest of output.
     */
    
    char outputStr[MAXOUTPUTSTRSIZE];
    int minWidth = 3 + MAXNAMESIZE + IDLENGTH;
    sprintf(outputStr, "    %%-%ds", minWidth);
    printf(outputStr, label);
    for (int i = 0; i < NUMOFQUIZZES; i++) {
        printf("%6d ", quizArr[i]);
    }
    printf("%6d\n", examScore);
}


//HW #12, Noah Turnquist
//
//============================== DATA ==============================
//Set#     Name              ID  Quiz 1 Quiz 2 Quiz 3 Quiz 4  Exam
//---- -------------------- ---- ------ ------ ------ ------ ------
//# 0: Julie Adams          1234     52      7    100     78     34
//# 1: Harry Smith          2134     90     36     90     77     30
//# 2: Tuan Nguyen          3124    100     45     20     90     70
//# 3: Jorge Gonzales       4532     11     17     81     32     77
//# 4: Amanda Trapp         5678     20     12     45     78     34
//# 5: Lou Breitenfeldt     6134     34     80     55     78     45
//# 6: Sarah Black          7874     60    100     56     78     78
//# 7: Bryan Devaux         8026     70     10     66     78     56
//# 8: Ling Wong            9893     34      9     77     78     20
//# 9: Bud Johnson          1947     45     40     88     78     55
//#10: Joe Giles            2877     55     50     99     78     80
//#11: Jim Nelson           3189     82     80    100     78     77
//#12: Paula Hung           4602     89     50     91     78     60
//#13: Ted Turner           5405     11     11      0     78     10
//#14: Evelyn Gilley        6999      0     98     89     78     20
//
//=========================== STATISTICS ===========================
//    Highest Scores                100    100    100     90     80
//    Lowest  Scores                 11      7     20     32     10
//    Average Scores                 50     43     70     75     49
//
//***Finished***
//Program ended with exit code: 0
