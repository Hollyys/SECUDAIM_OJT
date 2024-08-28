#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashTableLinkedList.h"
#include "csvParse.h"

int main()
{
	init_table();
	run();
	display();
	free_table();

	return 0;
}
