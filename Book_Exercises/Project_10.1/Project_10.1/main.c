//Book Practice Project #38
//Page 592

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define ROMANSTRINGSIZE 11
#define ROMANCHARS "IVXLCDM"
#define FLUSH while(getchar() != '\n')

void GetRomanNumeralFromUser(char* romanNumerals);
int VerifyStringIsRomanNumeral(const char* str);
void StringToAllUpper(char* str, int strSize);
int RomanCharToNumber(char c);
int ConvertRomanToDecimal(char* str);
void OutputDecimalNumber(const char* str, const int n);

int main(void) {
    char romanNumerals[ROMANSTRINGSIZE];
    int decimalNumber;
    int keepGoing = 1;
    
    while (keepGoing) {
        GetRomanNumeralFromUser(romanNumerals);
        if (strlen(romanNumerals) <= 0) {
            break;
        }
        StringToAllUpper(romanNumerals, ROMANSTRINGSIZE);
        if (!VerifyStringIsRomanNumeral(romanNumerals)) {
            printf("Error: That string is not a roman numeral.\n");
            continue;
        }
        decimalNumber = ConvertRomanToDecimal(romanNumerals);
        OutputDecimalNumber(romanNumerals, decimalNumber);
        putchar('\n');
    }
    
    return 0;
}

void GetRomanNumeralFromUser(char* romanNumerals) {
    printf("Please enter a number in roman numerals (empty line to quit):\n");
    fgets(romanNumerals, ROMANSTRINGSIZE, stdin);
    if (strchr(romanNumerals, '\n') == NULL) {
        FLUSH;
    } else {
        romanNumerals[strlen(romanNumerals) - 1] = '\0';
    }
}

int VerifyStringIsRomanNumeral(const char* str) {
    int romanChars = (int) strspn(str, ROMANCHARS);
    return romanChars == strlen(str) ? 1 : 0;
}

void StringToAllUpper(char* str, int strSize) {
    for (int i = 0; i < strSize; i++) {
        if (islower(str[i])) {
            str[i] = toupper(str[i]);
        }
    }
}

int RomanCharToNumber(char c) {
    switch (c) {
        case 'I':
            return 1;
            break;
        case 'V':
            return 5;
            break;
        case 'X':
            return 10;
            break;
        case 'L':
            return 50;
            break;
        case 'C':
            return 100;
            break;
        case 'D':
            return 500;
            break;
        case 'M':
            return 1000;
            break;
        default:
            return 0;
    }
}

int ConvertRomanToDecimal(char* str) {
    int sum = 0;
    for (char* itr = str; itr < str + ROMANSTRINGSIZE && *itr != '\0'; itr++) {
        if (*(itr + 1) == '\0' || RomanCharToNumber(*itr) >= RomanCharToNumber(*(itr + 1))) {
            sum += RomanCharToNumber(*itr);
//            printf("Basic add\n");
        } else {
            sum += RomanCharToNumber(*(itr + 1)) - RomanCharToNumber(*itr);
//            printf("Double add\n");
            itr++;
        }
    }
    return sum;
}

void OutputDecimalNumber(const char* str, const int n) {
    printf("%s converted to decimal is %d\n", str, n);
}
