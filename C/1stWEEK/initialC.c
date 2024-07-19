#include <stdio.h>

 int main(){
   FILE* file = fopen("1.txt", "w");
   char string[] = "hello, world";

   puts("Hello World");
   fputs("Hello World\n", file);
   printf("Hello World\n");
   fprintf(file, "Hello World\n");

   fclose(file);

   return 0;
 }