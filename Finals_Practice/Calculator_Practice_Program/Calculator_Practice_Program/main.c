//
//  main.c
//  Calculator_Practice_Program
//
//  Created by Noah Turnquist on 5/8/24.
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define FLUSH while(getchar() != '\n')
#define DEBUG 1
#define MAXEQLENGTH 50

double Calculate(char* equation);
int NumOpNum(char* equation, double* left, char* op, double* right);
void ParseParanthesesFromEquation(char* equation, char* tokenString, int* startIdx, int* endIdx);
void CleanEquation(char* cleanedEquation, char* equation);
int VerifyEquation(char* equation);
void ReplaceChunkOfEquation(char* equation, double tempResult, int startIdx, int endIdx);
void GetEquationFromUser(char* equation);

int main(int argc, const char * argv[]) {
    
    char equation[MAXEQLENGTH + 1];
    while (!strchr(equation, 'q')) {
        GetEquationFromUser(equation);
        if (!strchr(equation, 'q')) {
            if (VerifyEquation(equation)) {
                printf("%s = %g\n", equation, Calculate(equation));
            } else {
                printf("Sorry. That is not a valid equation.\n");
            }
        }
    }
    
    return 0;
}

double Calculate(char* equation) {
    
    char cleanedEquation[strlen(equation) + 1];
    CleanEquation(cleanedEquation, equation);
    
    double result = 0.0;
    double left;
    char op;
    double right;
    char exprString[strlen(cleanedEquation) + 1];
    double tempResult;
    
    //Base case - Equation is a single number
    if (strspn(cleanedEquation, "0123456789.") == strlen(cleanedEquation)) {
        sscanf(cleanedEquation, "%lf", &result);
        return result;
    //Base case - Equation is simple number -> operator -> number
    } else if (NumOpNum(cleanedEquation, &left, &op, &right) == 3) {
        switch (op) {
            case '+':
                result = left + right;
                break;
            case '-':
                result = left - right;
                break;
            case '*':
                result = left * right;
                break;
            case '/':
                result = left / right;
                break;
        }
        return result;
    } else {
//        char newStr[strlen(equation) + 1];
//        
//        
//        //Check for parantheses
//        char tokenString[strlen(equation) + 1];
//        int startIdx;
//        int endIdx;
//        ParseParanthesesFromEquation(equation, tokenString, &startIdx, &endIdx);
//        
//        if (DEBUG) {
//            printf("Token: %s\n", tokenString);
//        }
//        
//        if (strlen(tokenString) > 0) {
//            double paraResult = Calculate(tokenString);
//            ReplaceChunkOfEquation(equation, paraResult, startIdx, endIdx);
//            
//            if(DEBUG) {
//                printf("Parantheses replaced equation: %s\n", equation);
//            }
//        }
        
        //Check for multiplication or division
        
        //Check for addition or subtraction
        sscanf(cleanedEquation, "%lf %c %lf", &left, &op, &right);
        sprintf(exprString, "%g%c%g", left, op, right);
        tempResult = Calculate(exprString);
        
        char* cleanedEqPtr = cleanedEquation;
        
        int leftChars = (int) strspn(cleanedEqPtr, "01234567890.");
        cleanedEqPtr += leftChars;
        cleanedEqPtr++;
        int rightChars = (int) strspn(cleanedEqPtr, "01234567890.");
        cleanedEqPtr += rightChars;
        
        char tempResultStr[strlen(cleanedEquation) + 1];
        sprintf(tempResultStr, "%g", tempResult);
        
        char* tempResultItr = tempResultStr;
        char* tempResultEnd = tempResultStr + strlen(tempResultStr);
        for (char* itr = cleanedEquation; itr < cleanedEqPtr; itr++, tempResultItr++) {
            if (tempResultItr < tempResultEnd) {
                *itr = *tempResultItr;
            } else {
                *itr = ' ';
            }
        }
        
        return Calculate(cleanedEquation);
    }
}

int NumOpNum(char* equation, double* left, char* op, double* right) {
    char throwaway;
    return sscanf(equation, " %lf %c %lf %c", left, op, right, &throwaway);
}

//TODO: Add error checking for user not closing their paranthesis
//TODO: Add error checking for user starting with a closing paranthesis instead of an open one
void ParseParanthesesFromEquation(char* equation, char* tokenString, int* startIdx, int* endIdx) {
    int insidePara = 0;
    int tokenStringIdx = 0;
    int extraPara = 0;
    
    for (int i = 0; i < strlen(equation); i++) {
        if (insidePara && equation[i] == '(') {
            extraPara++;
        }
        if (extraPara > 0 && equation[i] == ')') {
            extraPara--;
        } else if (equation[i] == ')') {
            insidePara = 0;
            *endIdx = i;
        }
        if (insidePara) {
            tokenString[tokenStringIdx] = equation[i];
            tokenStringIdx++;
        }
        if (equation[i] == '(' && extraPara == 0) {
            insidePara = 1;
            *startIdx = i;
        }
    }
}

void CleanEquation(char* cleanedEquation, char* equation) {
    //Remove all whitespace from equation
    int cleanedEquationIdx = 0;
    
    for (char* itr = equation; itr < equation + strlen(equation); itr++) {
        if (*itr != ' ') {
            cleanedEquation[cleanedEquationIdx] = *itr;
            cleanedEquationIdx++;
        }
    }
    //Make sure cleaned String has dilimeter
    cleanedEquation[cleanedEquationIdx] = '\0';
}

int VerifyEquation(char* equation) {
    char cleanedEquation[strlen(equation) + 1];
    int valid = 1;
    
    CleanEquation(cleanedEquation, equation);
    
    char* itr = cleanedEquation;
    char* end = cleanedEquation + strlen(cleanedEquation);
    double num;
    char op;
    int process = 0;
    while (itr < end && valid) {
        if (process == 0 || process == 2) {
            if(sscanf(cleanedEquation, "%lf", &num) == 1) {
                int numChars = (int) strspn(itr, "01234567890.");
                itr += numChars;
                if (process == 0) {
                    process++;
                } else if (process == 2) {
                    process = 0;
                }
            } else {
                valid = 0;
            }
        } else if (process == 1) {
            if (sscanf(cleanedEquation, "%c", &op) == 1) {
                itr++;
                process++;
            } else {
                valid = 0;
            }
        }
    }
    if (process != 0) {
        valid = 0;
    }
    return valid;
}

void ReplaceChunkOfEquation(char* equation, double tempResult, int startIdx, int endIdx) {
    char resultStr[strlen(equation) + 1];
    sprintf(resultStr, "%g", tempResult);
    
    for (int eqIdx = startIdx, paraIdx = 0; eqIdx <= endIdx; eqIdx++, paraIdx++) {
        if (resultStr[paraIdx] != '\0') {
            equation[eqIdx] = resultStr[paraIdx];
        } else {
            equation[eqIdx] = ' ';
        }
    }
}

void GetEquationFromUser(char* equation) {
    int validInput = 0;
    
    while (!validInput) {
        printf("Enter 'q' to quit.\n");
        printf("Please enter a simple equation:\n");
        fgets(equation, MAXEQLENGTH, stdin);
        if (!strchr(equation, '\n')) {
            printf("Error equation too long. Must be less than %d characters.\n", MAXEQLENGTH);
            FLUSH;
        } else {
            for (int i = 0; i < strlen(equation); i++) {
                if (equation[i] == '\n') {
                    equation[i] = '\0';
                }
            }
            if (strspn(equation, "0123456789.+-/*() q") != strlen(equation)) {
                printf("Error: invalid character in equation. Try again.\n");
            } else {
                validInput = 1;
            }
        }
    }
}


