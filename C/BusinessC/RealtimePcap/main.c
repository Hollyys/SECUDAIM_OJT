#include <stdio.h>
#include <time.h>

int main() {
    time_t current_time;
    time(&current_time);

    struct tm *local_time = localtime(&current_time);

    char file_name[100];
    strftime(file_name, sizeof(file_name), "%Y-%m-%d_%H:%M:%S.cap", local_time);

    printf("File name: %s\n", file_name);

    return 0;
}

