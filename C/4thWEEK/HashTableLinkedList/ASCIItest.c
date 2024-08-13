#include <stdio.h>
#include <ctype.h> // isalpha() 함수 사용을 위해 포함

int sumAsciiValues(const char *str)
{
    int sum = 0;
    while (*str)
    {
        // if (isalpha(*str))
        if ((*str >= 'A' && *str <= 'Z') || (*str >= 'a' && *str <= 'z'))
        {
            sum += (int)(*str);
        }
        str++;
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
