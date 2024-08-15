## 240731 [Hash Table - By Singly Linked List]

1. **Singly Linkedlist로 구현한 Hash Function에서 Bucket의 count가 자동적으로 초기값을 0으로 잡는 것 같다.**
    - Code
        
        ```c
        #include <stdio.h>
        #include <stdlib.h>
        
        struct bucket* hashtable = NULL;
        int BUCKET_SIZE = 10; 
        
        struct node{
                int key;
                int value;
                struct node* next;
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
                        newNode->next = hashtable[hashindex].head;
                        hashtable[hashindex].head = newNode;
                        hashtable[hashindex].count++;
                }
        }
        
        void remove_key(int key){
                int hashindex = hashfunction(key);
                int flag = 0;
        
                struct node* node;
                struct node* before;
        
                node = hashtable[hashindex].head;
        
                while(node != NULL){
                        if(node->key == key){
                                if(node == hashtable[hashindex].head){
                                        hashtable[hashindex].head = node->next;
                                }
                                else{
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
        
                printf("\nHASH TABLE Bucket memory check\n");
                display();
        
                for(int i=0; i<17; i++)
                {
                        add(i, 10*i);
                }
        
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
        ```
        
    
    **Solution**
    
    현재 상태의 코드는 malloc만으로 메모리 할당을 한 상태이다. 이때 count변수는 임의의 메모리 공간에 할당되어있으나 초기값이 0이었던 점은 우연히 0으로 초기화된 공간에 해당 변수가 할당되어 있었기 때문이었다. 이는 올바른 방법이 아니며 calloc 또는 malloc + memset의 조합으로 메모리 할당을 해야 제대로 된 메모리 활용 방식이다.
    
    ```c
    #include <string.h> // memset사용하기 위함
    
    // malloc + memset 조합
    hashtable = (struct bucket*)malloc(BUCKET_SIZE*sizeof(struct bucket));
    memset(hashtable, 0, BUCKET_SIZE*sizeof(struct bucket))
    ```
    
    ```c
    // calloc 조합
    hashtable = (struct bucket*)calloc(BUCKET_SIZE, sizeof(struct bucket));
    ```
    
    <aside>
    💡 malloc과 calloc의 형태는 같지만 전자는 할당된 공간의 값을 바꾸지 않고 후자는 할당된 공간의 값을 바꾼다.
    
    </aside>
    

## 240801 [Hash Table - By Doubly Linked List]

1. **Segmentation fault (core dumped) 발생**
    - Code
        
        ```c
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
        							if(node->next != NULL){
        								node->next->previous = NULL;
        							}
        								hashtable[hashindex].head = node->next;
        						}
        						else{
        							if(node->previous != NULL){
        								node->previous->next = node->next;
        							}
        							if(node->next != NULL){
        								node->next->previous = node->previous;
        							}
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
        		display();
        
        		remove_key(1);
        		display();
        
        		remove_key(10);
        		display();
        
        		remove_key(7);
        		display();
        
        		remove_key(30);
        		display();
        
        		add(32, 320);
        		add(62, 620);
        		display();
        
        		search(5);
        
        		display();
        
        		free(hashtable);
        
        		return 0;
        }
        ```
        
    - **Segmentation Fault?**
        
        Segmentation Fault는 C나 C++(메모리 관리가 프로그래머에게 주어지는 경우)에서 허용되지 않는 메모리 영역에 접근하려고 할 경우 발생하는 오류이다.
        
        - NULL인 경우
        - 할당받지 않은 메모리에 접근할 경우
    - 이 경우 해당 오류가 Head인 노드를 제거하려고 시도하는 경우 발생
    
    **Solution**
    
    remove_key()는 hashtable에서 사용자가 원하는 노드를 삭제하는 기능을 하는 함수이다. 이때 linked list의 가장 앞에 위치한 헤드 노드는 previous가 NULL을 가리킨다. 반대로 끝단에 위치한 노드의 경우 next가 NULL을 가리킨다. 하지만 기존의 코드는 조건 없이 앞뒤 노드의 포인터를 변경하려 하므로 previous 또는 next가 NULL인 경우 잘못된 메모리 접근이므로 segmentation fault 오류를 발생시킨다.
    
    - remove_key() : 수정 전
        
        ```c
        void remove_key(int key){
            int hashIndex = hashFunction(key);
            int flag = 0;
            struct node* node;
        
            node = hashTable[hashIndex].head;
        
            while (node != NULL)
            {
                if (node->key == key){
                    if (node == hashTable[hashIndex].head){
                        node->next->previous = NULL;
                        hashTable[hashIndex].head = node->next;
                    }
                    else{
                        node->previous->next = node->next;
                        node->next->previous = node->previous;
                    }
                    hashTable[hashIndex].count--;
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
        ```
        
    - remove_key() : 수정 후
        
        ```c
        void remove_key(int key){
        		int hashindex = hashfunction(key);
        		int flag = 0;
        
        		struct node* node;
        
        		node = hashtable[hashindex].head;
        
        		while(node != NULL){
        				if(node->key == key){
        						if(node == hashtable[hashindex].head){
        							if(node->next != NULL){
        								node->next->previous = NULL;
        							}
        								hashtable[hashindex].head = node->next;
        						}
        						else{
        							if(node->previous != NULL){
        							// Head가 아닌 노드이므로 previous가 NULL일 일이 없어서
        							//생략해도 되는 부분이긴하다.
        								node->previous->next = node->next;
        							}
        							if(node->next != NULL){
        								node->next->previous = node->previous;
        							}
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
        ```