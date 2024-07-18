#include <stdio.h>
#include <string.h>

int main(){
    char input[40];
    int lower_case = 0;
    int upper_case = 0;
    int integer = 0;

    printf("Insert the string: ");
    scanf("%[^\n]s", &input);
    // scanf("%s")는 공백문자를 읽어들이지 않지만
    // scanf("%[^\n]s")로 입력을 받으면 공백문자도 입력받을 수 있다.
    int len = strlen(input);

    for(int i=0; i<len; i++){
        if(input[i] >= 'A' && input[i] <= 'Z'){
            upper_case += 1;
        }
        else if(input[i] >= 'a' && input[i] <= 'z'){
            lower_case += 1;
        }
        else if(input[i] >= '0' && input[i] <= '9'){
            integer += 1;
        }
    }

    printf("Upper case: %d\n", upper_case);
    printf("Lower case: %d\n", lower_case);
    printf("Integer: %d\n", integer);

    return 0;
}