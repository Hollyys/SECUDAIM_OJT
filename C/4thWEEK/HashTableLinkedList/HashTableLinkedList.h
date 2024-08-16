#ifndef HASHTABLELINKEDLIST_H
#define HASHTABLELINKEDLIST_H

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

int sumAsciiValues(const char *str);
int hashfunction(char *hash_key);
void add(char *hash_key, char *data);
void remove_key(char *hash_key);
void search(char *hash_key);
void display();
struct node *createNode(char *hash_key, char *data);

#endif

