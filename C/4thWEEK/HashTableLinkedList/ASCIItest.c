#include <stdio.h>
#include <ctype.h> // isalpha() 함수 사용을 위해 포함

int sumAsciiValues(const char *str)
{
    int sum = 0;
    while (*str)
    { // 문자열의 끝까지 반복
        if (isalpha(*str))
        {                       // 문자가 알파벳인지 확인
            sum += (int)(*str); // 아스키 값 누적
        }
        str++; // 다음 문자로 이동
    }
    return sum;
}

int main()
{
    const char *text = "l8LMINR";
    int sum = sumAsciiValues(text);
    printf("문자열 \"%s\"의 알파벳 아스키 코드 값의 합: %d\n", text, sum);
    return 0;
}
