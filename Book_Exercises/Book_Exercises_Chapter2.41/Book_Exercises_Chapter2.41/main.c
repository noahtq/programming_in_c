#include <stdio.h>
#define PI 3.14159
#define NAME "Ken"

int main(void) {
    int a;
    int b;
    int c;
    
    char reverse_a[50];
    char reverse_b[50];
    char reverse_c[50];
    
    printf("%s\n", NAME);
    printf("Please enter three numbers: ");
    scanf("%d %d %d", &a, &b, &c);
    printf("Your numbers forward:\n %d\n %d\n %d\n", a, b, c);
    
    sprintf(reverse_a, "%d", a);
    sprintf(reverse_b, "%d", b);
    sprintf(reverse_c, "%d", c);
    
    printf("Your numbers reversed:\n %c%c\n %c%c\n %c%c\n", reverse_a[1], reverse_a[0], reverse_b[1], reverse_b[0], reverse_c[1], reverse_c[0]);
    
    return 0;
}
