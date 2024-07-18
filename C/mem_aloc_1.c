#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    srand(time(NULL));
    int rand_num = rand()%50 + 1;
    int *history = (int)malloc(sizeof(int)*5);
    int cnt = 0;
    int flag = 0;

    for(;;){
        // Code
        int answer;
        printf("Insert your answer (-1 for quit): ");
        scanf("%d", &answer);
        if(answer == rand_num){
            flag = 1;
            printf("\n", answer);
            break;
        }

        if(answer == -1){
            break;
        }

        if (answer < rand_num){
            printf("%d  UP\n", answer);
        }
        else if (answer > rand_num){
            printf("%d  DOWN\n", answer);
        }

        if (cnt >= 6){
            realloc(history, sizeof(int)*(cnt+1));
        }
        history[cnt] = answer;
        cnt += 1;
       
    }

     printf("History: ");
    for(int i=0; i<cnt; i++){
        printf("%d ", history[i]);
    }
    printf("\n");

    if(flag == 1){
       printf("Found\n");
    }
    else{
        printf("Not found\n");
    }

    free(history);
    return 0;
}