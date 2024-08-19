#include <stdio.h>
#include <stdlib.h>
int main() {
  char* data = NULL;
  char* result = NULL;

  data = (char*)malloc(10);

  for(int i=0; i<10; i++)
  {
    result = data+i;
    data[i] += 'a'+i;
    printf("%c", data[i]); 
  }

  free(data);
  free(result);  
  return 0;
}