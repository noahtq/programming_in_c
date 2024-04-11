//
//  main.c
//  Problem_11.1
//
//  Created by Noah Turnquist on 4/9/24.
//

#include <stdio.h>

typedef struct {
    int hours;
    int minutes;
    int seconds;
} TIME;

TIME elapsedTime(TIME start, TIME now);

int main(void) {
    TIME yesterday = { 5, 32, 15 };
    TIME now = { 34, 05, 22 };
    TIME elapsed = elapsedTime(yesterday, now);
    printf("Hours: %d, minutes: %d, seconds: %d\n", elapsed.hours, elapsed.minutes, elapsed.seconds);
    
    return 0;
}

TIME elapsedTime(TIME start, TIME now) {
    int totalElapsedSeconds = (now.hours - start.hours) * 3600 + (now.minutes - start.minutes) * 60 + (now.seconds - start.seconds);
    TIME newtime;
    newtime.hours = totalElapsedSeconds / 3600;
    newtime.minutes = (totalElapsedSeconds % 3600) / 60;
    newtime.seconds = ((totalElapsedSeconds % 3600) % 60);
    return newtime;
}
