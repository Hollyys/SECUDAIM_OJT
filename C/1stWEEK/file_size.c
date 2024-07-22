#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <errno.h>

int main(){
    int size;
    extern int errno;
    FILE *fp = fopen("test.txt", "r");
    if(fp == NULL){
        perror("ERROR");
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    if(size == -1){
        perror("ERROR");
    }

    printf("size: %d\n", size);

    fclose(fp);

    return 0;
}