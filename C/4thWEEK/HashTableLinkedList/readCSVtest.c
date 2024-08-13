#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main() {
    FILE *file = fopen("hash.csv", "r"); // CSV 파일 열기
    if (!file) {
        fprintf(stderr, "Could not open file\n");
        return 1;
    }

    char buffer[BUFFER_SIZE]; // 버퍼 선언
    while (fgets(buffer, BUFFER_SIZE, file)) { // 파일에서 줄을 읽기
        char *token = strtok(buffer, ","); // 첫 번째 토큰 가져오기
        while (token) {
            printf("%s ", token); // 토큰 출력
            token = strtok(NULL, ","); // 다음 토큰 가져오기
        }
        printf("\n");
    }

    fclose(file); // 파일 닫기
    return 0;
}
