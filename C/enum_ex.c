#include <stdio.h>
#include <string.h>

enum NUMBER
{
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE
};

int main(){
    enum NUMBER num;
    int input_int;
    char input_str[100];

    printf("Insert the integer: ");
    scanf("%s", &input_str);

    printf("%s\n", input_str);

    for(int i=0; i<strlen(input_str); i++){
        // char digit = input_str[i];
        // int digit;
        // sprintf(digit,"%c", input_str[i]);
        int digit = input_str[i] - '0';

        switch (digit)
        {
            case ZERO:
                printf("ZERO ");
                break;
            case ONE:
                /* code */
                printf("ONE ");
                break;
            case TWO:
                /* code */
                printf("TWO ");
                break;
            case THREE:
                /* code */
                printf("THREE ");
                break;
            case FOUR:
                /* code */
                printf("FOUR ");
                break;
            case FIVE:
                /* code */
                printf("FIVE ");
                break;
            case SIX:
                /* code */
                printf("SIX ");
                break;
            case SEVEN:
                /* code */
                printf("SEVEN ");
                break;
            case EIGHT:
                /* code */
                printf("EIGHT ");
                break;
            case NINE:
                /* code */
                printf("NINE ");
                break;
        }
    }
    printf("\n");

    return 0;
}