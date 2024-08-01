#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct bucket* hashtable = NULL;
int BUCKET_SIZE = 10;

struct node{
		int key;
		int value;
		struct node* next;
		struct node* previous;
};

struct bucket{
		struct node* head;
		int count;
};

struct node* createNode(int key, int value){
		struct node* newNode;

		newNode = (struct node*)malloc(sizeof(struct node));

		newNode->key = key;
		newNode->value = value;
		newNode->next = NULL;
		newNode->previous = NULL;

		return newNode;
}

int hashfunction(int key){
		return key%BUCKET_SIZE;
}

void add(int key, int value){
		int hashindex = hashfunction(key);

		struct node* newNode = createNode(key, value);

		if(hashtable[hashindex].count == 0){
				hashtable[hashindex].count = 1;
				hashtable[hashindex].head = newNode;
		}
		else{
				hashtable[hashindex].head->previous = newNode;
				newNode->next = hashtable[hashindex].head;
				hashtable[hashindex].head = newNode;
				hashtable[hashindex].count++;
		}
}

void remove_key(int key){
		int hashindex = hashfunction(key);
		int flag = 0;

		struct node* node;

		node = hashtable[hashindex].head;

		while(node != NULL){
				if(node->key == key){
						if(node == hashtable[hashindex].head){
								node->next->previous = NULL;
								hashtable[hashindex].head = node->next;
						}
						else{
								node->previous->next = node->next;
								node->next->previous = node->previous;
						}
						hashtable[hashindex].count--;
						free(node);
						flag = 1;
				}
				node = node->next;
		}
		printf("\n> ");
		if(flag == 1){
				printf("[ %d ] is deleted.\n", key);
		}
		else{
				printf("[ %d ] is not exsist.\n", key);
		}
}

void search(int key){
		int hashindex = hashfunction(key);
		struct node* node = hashtable[hashindex].head;
		int flag = 0;
		while(node != NULL)
		{
				if(node->key ==  key){
						flag = 1;
						break;
				}
				node = node->next;
		}
		printf("\n> ");
		if(flag == 1){
				printf("KEY: [ %d ], VALUE: [ %d ]\n", node->key, node->value);
		}
		else{
				printf("No such KEY exsist.\n");
		}
}

void display(){
		struct node* iterator;
		printf("\n======= DISPLAY =========\n\n");
		for(int i = 0; i < BUCKET_SIZE; i++){
				iterator = hashtable[i].head;
				printf("Bucket[%d] : (Count: %d), ", i, hashtable[i].count);
				while(iterator != NULL)
				{
						printf("(key : %d, val: %d) -> ", iterator->key, iterator->value);
						iterator = iterator->next;
				}
				printf("\n");
		}
		printf("\n=========================\n");
}

int main(){
		hashtable = (struct bucket*)malloc(BUCKET_SIZE*sizeof(struct bucket));
		memset(hashtable, 0, BUCKET_SIZE*sizeof(struct bucket));
		
		printf("\nHASH TABLE Bucket memory check\n");

		display();

		for(int i=0; i<17; i++)
		{
				add(i, 10*i);
		}
		display();

		remove_key(14);
		remove_key(7);
		remove_key(30);

		add(32, 320);
		add(62, 620);

		search(5);

		display();

		free(hashtable);

		return 0;
}
