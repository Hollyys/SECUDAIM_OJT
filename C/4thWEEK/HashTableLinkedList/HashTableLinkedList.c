#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bucket *hashtable = NULL;
#define BUFFER_SIZE 1024
int BUCKET_SIZE = 31;

struct node
{
	char *hash_key;
	char *data;
	struct node *next;
};

struct bucket
{
	struct node *head;
	int count;
};

struct node *createNode(char *hash_key, char *data)
{
	struct node *newNode;

	newNode = (struct node *)malloc(sizeof(struct node));

	newNode->hash_key = hash_key;
	newNode->data = data;
	newNode->next = NULL;

	return newNode;
}

int sumAsciiValues(const char *str)
{
	int sum = 0;
	while (*str)
	{
		if ((*str >= 'A' && *str <= 'Z') || (*str >= 'a' && *str <= 'z'))
		{
			sum += (int)(*str);
		}
		str++;
	}
	return sum;
}

int hashfunction(char *hash_key)
{
	return sumAsciiValues(hash_key) % BUCKET_SIZE;
}

void add(char *hash_key, char *data)
{
	int hashindex = hashfunction(hash_key);

	struct node *newNode = createNode(hash_key, data);

	if (hashtable[hashindex].count == 0)
	{
		hashtable[hashindex].count = 1;
		hashtable[hashindex].head = newNode;
	}
	else
	{
		newNode->next = hashtable[hashindex].head;
		hashtable[hashindex].head = newNode;
		hashtable[hashindex].count++;
	}
}

void remove_key(char *hash_key)
{
	int hashindex = hashfunction(hash_key);
	int flag = 0;

	struct node *node;
	struct node *before;

	node = hashtable[hashindex].head;

	while (node != NULL)
	{
		if (strcmp(node->hash_key, hash_key) == 0)
		{
			if (node == hashtable[hashindex].head)
			{
				hashtable[hashindex].head = node->next;
			}
			else
			{
				before->next = node->next;
			}
			hashtable[hashindex].count--;
			free(node);
			flag = 1;
		}
		before = node;
		node = node->next;
	}
	printf("\n> ");
	if (flag == 1)
	{
		printf("[ %s ] is deleted.\n", hash_key);
	}
	else
	{
		printf("[ %s ] is not exsist.\n", hash_key);
	}
}

void search(char *hash_key)
{
	int hashindex = hashfunction(hash_key);
	struct node *node = hashtable[hashindex].head;
	int flag = 0;
	while (node != NULL)
	{
		if (strcmp(node->hash_key, hash_key) == 0)
		{
			flag = 1;
			break;
		}
		node = node->next;
	}
	printf("\n> ");
	if (flag == 1)
	{
		printf("KEY: [ %s ], VALUE: [ %s ]\n", node->hash_key, node->data);
	}
	else
	{
		printf("No such KEY exsist.\n");
	}
}

void display()
{
	struct node *iterator;
	printf("\n======= DISPLAY =========\n\n");
	for (int i = 0; i < BUCKET_SIZE; i++)
	{
		iterator = hashtable[i].head;
		printf("Bucket[%d] : ", i);
		while (iterator != NULL)
		{
			printf("(key : %s, data: %s) -> ", iterator->hash_key, iterator->data);
			iterator = iterator->next;
		}
		printf("\n");
	}
	printf("\n=========================\n");
}

int main()
{
	hashtable = (struct bucket *)malloc(BUCKET_SIZE * sizeof(struct bucket));
	memset(hashtable, 0, BUCKET_SIZE * sizeof(struct bucket));

	FILE *file = fopen("hash.csv", "r");
	if (!file)
	{
		fprintf(stderr, "Could not open file\n");
		return 1;
	}

	char buffer[BUFFER_SIZE];
	while (fgets(buffer, BUFFER_SIZE, file))
	{
		char *hash_key = strtok(buffer, ",");
		char *data = strtok(NULL, ",");
		printf("key: %s, data: %s", hash_key, data);
		add(hash_key, data);
	}

	fclose(file);
	free(hashtable);

	display();

	return 0;
}
