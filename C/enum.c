#include <stdio.h>

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
    NINE,
    ONE_HANDRED = 100
};

enum number
{
    zero,
    one,
    two
};

int main(){
    enum NUMBER num;
    enum number NUM;
    printf("%d %d %d %d %d %d %d %d %d %d %d\n", ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, ONE_HANDRED);
    printf("%d %d %d\n",zero, one, two);
    num = TWO;
    printf("%d\n", TWO);
    num = ONE_HANDRED;
    printf("%d\n", ONE_HANDRED);
    return 0;
}