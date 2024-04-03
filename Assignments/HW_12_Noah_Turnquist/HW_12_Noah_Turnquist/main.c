// HW #12, Noah Turnquist

//TODO: Format output better
//TODO: Make it so header string is not hardcoded for number of quizzes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENTARRSIZE 50
#define MAXNAMESIZE 20
#define IDLENGTH 4
#define NUMOFQUIZZES 4
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
    
    printf("HW #12, Noah Turnquist\n");
    PrintStudentInfo(studentArray, studentsRead, highestQuizScores, lowestQuizScores, averageQuizScores, highestExamScore, lowestExamScore, averageExamScore);
    printf("\n***Finished***\n");
    
    free(studentArray);
    studentArray = NULL;
    
    return 0;
}

STUDENT* CreateStudentArray(void) {
    STUDENT* studentArray = (STUDENT*) malloc(STUDENTARRSIZE * sizeof(STUDENT));
    if (studentArray == NULL) {
        printf("Could not allocate student array on heap.\n");
        return NULL;
    }
    return studentArray;
}

int ReadStudentsFromFile(STUDENT* studentArray, char* filePath) {
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
    printf("================================= DATA ================================\n");
    printf("Set# Name ID Quiz 1 Quiz 2 Quiz 3 Quiz 4 Exam\n"); //TODO: Fix hardcoding number of quizzes
    printf("---- -------------------- ---- ------ ------ ------ ------ ------\n");
    
    for (STUDENT* itr = studentArray; itr < studentArray + studentsRead; itr++) {
        int current = (int) (itr - studentArray);
        printf("# %d: %s %s ", current, itr->name, itr->id);
        for (int i = 0; i < NUMOFQUIZZES; i++) {
            printf("%d ", itr->quizzes[i]);
        }
        printf("%d\n", itr->exam);
    }
    
    //TODO: Make this code simpler, maybe make function so code isn't repeated as much? Maybe make custom output strings?
    //TODO: Get rid of magic constants 10
//    int outputStrLength = 10 * NUMOFQUIZZES + 10 + 1;
//    char highestOutputStr[outputStrLength];
//    char lowestOutputStr[outputStrLength];
//    char averageOutputStr[outputStrLength];
//    
//    for (int i = 0; i < NUMOFQUIZZES; i++) {
//        strncat(highestOutputStr)
//    }
    
    printf("\n============================ STATISTICS ==============================\n");
    printf("Highest Scores: ");
    for (int i = 0; i < NUMOFQUIZZES; i++) {
        printf("%d ", highestQuizScores[i]);
    }
    printf("%d\n", highestExamScore);
    
    printf("Lowest Scores: ");
    for (int i = 0; i < NUMOFQUIZZES; i++) {
        printf("%d ", lowestQuizScores[i]);
    }
    printf("%d\n", lowestExamScore);
    
    printf("Average Scores: ");
    for (int i = 0; i < NUMOFQUIZZES; i++) {
        printf("%d ", averageQuizScores[i]);
    }
    printf("%d\n", averageExamScore);
}

void GetAllScoreInfo(STUDENT* studentArray, int studentsRead, int* highestQuizScores, int* lowestQuizScores, int* averageQuizScores, int* highestExamScore, int* lowestExamScore, int* averageExamScore) {
    int allQuizScores[NUMOFQUIZZES][studentsRead];
    int allExamScores[studentsRead];
    
    for (STUDENT* itr = studentArray; itr < studentArray + studentsRead; itr++) {
        int current = (int) (itr - studentArray);
        for (int i = 0; i < NUMOFQUIZZES; i++) {
            allQuizScores[i][current] = itr->quizzes[i];
        }
        allExamScores[current] = itr->exam;
    }
    
    //TODO: split finding highest, lowest, and average into seperate functions???
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
