#include <stdio.h>

 int main(){
    for(int i=2; i<=9; i++){
        for(int j=1; j<=9; j++){
        printf("%d X %d = %d\n", i, j, i*j);
    }
    }
    return 0;
 }