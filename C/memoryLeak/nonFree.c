#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char* tmp;
	tmp = malloc(10);
	strcpy(tmp, "911 GT3");
	printf("%s\n", tmp);

	return 0;
}
