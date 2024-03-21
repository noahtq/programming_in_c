//Homework #4, Noah Turnquist

#include <stdio.h>
#include <ctype.h>

void Conditional(char ch);
void Switching(char ch);
int IsUpperAOrLowerA(char ch);

int main(void) {
    char ch;
    
    printf("Homework #4, Noah Turnquist\n");

    printf("Please enter a single character and then hit enter: ");
    if (scanf("%c", &ch) != 1) {
        printf("Error: invalid character entered.\n");
        return 1;
    }
    Conditional(ch);
    Switching(ch);
    
    return 0;
}

void Conditional(char ch) {
    printf("Running if function with %c.\n", ch);
    
    if (islower(ch) && ch != 'a') {
        ch = toupper(ch);
    } else if (isupper(ch) && ch != 'A') {
        ch = tolower(ch);
    } else if (isdigit(ch)) {
        ch += 64;
    }
    printf("If function: New value of character: %c\n", ch);
}

void Switching(char ch) {
    printf("Running switch function with %c.\n", ch);
    
    int ch_exp = isupper(ch) + isdigit(ch) * 2 + IsUpperAOrLowerA(ch) * 3;
    switch(ch_exp) {
        case 0:
            ch = toupper(ch);
            break;
        case 1:
            ch = tolower(ch);
            break;
        case 2:
            ch += 64;
            break;
    }
    printf("Switch function: New value of character: %c\n", ch);
}

int IsUpperAOrLowerA(char ch) {
    //Return 1 if lowercase a, 2 if uppercase A, 0 otherwise
    if (ch == 'a') {
        return 1;
    } else if (ch == 'A') {
        return 2;
    } else {
        return 0;
    }
}


//Homework #4, Noah Turnquist
//Please enter a single character and then hit enter: 5
//Running if function with 5.
//If function: New value of character: u
//Running switch function with 5.
//Switch function: New value of character: u
//Program ended with exit code: 0


//VERIFICATION PLAN

//Test 1 - Test with uppercase 'A'
//Input: 'A'
//Expected Output:
//If function prints 'A'
//Switch function prints 'A'

//Test 2 - Test with lowercase 'a'
//Input: 'a'
//Expected Output:
//If function prints 'a'
//Switch function prints 'a'

//Test 3 - Test with lowercase character other than 'a'
//Input: 'd'
//Expected Output:
//If function prints 'D'
//Switch function prints 'D'

//Test 4 - Test with uppercase character other than 'A'
//Input: 'D'
//Expected Output:
//If function prints 'd'
//Switch function prints 'd'

//Test 5 - Test with regular digit
//Input: 3
//Expected Output:
//If function prints 's'
//Switch function prints 's'
