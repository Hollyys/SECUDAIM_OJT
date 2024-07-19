#include <stdio.h>

 int main(){
    int l=0;
    int r=0;

    for(int i=0; i<9; i++){
        if(i <= 4 ){
            l=4-i;
            r=8-l;
            for(int j=0; j<9; j++){
                if(j==l || j==r){
                    printf("%c ", i+65);
                }
                else
                    printf("* ");
            }
            printf("\n");
        }
        else{
            l=i-4;
            r=8-l;
            for(int j=0; j<9; j++){
                if(j==l || j==r){
                    printf("%c ", i+65);
                }
                else
                    printf("* ");
            }
            printf("\n");
        }
    }
   return 0;
 }