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

int hashfunction(char *hash_key)
{
	return key % BUCKET_SIZE;
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
		printf("[ %d ] is deleted.\n", hash_key);
	}
	else
	{
		printf("[ %d ] is not exsist.\n", hash_key);
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
		printf("KEY: [ %d ], VALUE: [ %d ]\n", node->hash_key, node->value);
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
		printf("Bucket[%d] : (Count: %d), ", i, hashtable[i].count);
		while (iterator != NULL)
		{
			printf("(key : %d, val: %d) -> ", iterator->key, iterator->data);
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

	printf("\nHASH TABLE Bucket memory check\n");
	display();

	FILE *file = fopen("hash.csv", "r"); // CSV 파일 열기
	if (!file)
	{
		fprintf(stderr, "Could not open file\n");
		return 1;
	}

	char buffer[BUFFER_SIZE];
	while (fgets(buffer, BUFFER_SIZE, file))
	{
		char *hash_key = strtok(buffer, ",");
		char *data = strtok(buffer, ",");
	}

	fclose(file); // 파일 닫기

	for (int i = 0; i < 17; i++)
	{
		add(i, 10 * i);
	}

	free(hashtable);

	return 0;
}
