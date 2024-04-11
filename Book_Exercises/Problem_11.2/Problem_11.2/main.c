//
//  main.c
//  Problem_11.2
//
//  Created by Noah Turnquist on 4/9/24.
//

#include <stdio.h>

typedef struct {
    char* suit; //Clubs, Diamonds, Hearts, Spades
    int value; //1 (Ace) ... 10, 11 (Jack), 12 (Queen), 13 (King)
} CARD;

int main(void) {
    CARD deck[52];
    
    int counter = 0;
    for (int i = 1; i < 5; i++) {
        for (int j = 1; j < 14; j++) {
            CARD temp;
            temp.value = j;
            switch (i) {
                case 1:
                    temp.suit = "Clubs";
                    break;
                case 2:
                    temp.suit = "Diamonds";
                    break;
                case 3:
                    temp.suit = "Hearts";
                    break;
                case 4:
                    temp.suit = "Spades";
                    break;
            }
            deck[counter] = temp;
            counter++;
        }
    }
    
    return 0;
}
