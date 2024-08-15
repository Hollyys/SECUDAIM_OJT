## 240813 [Hash Table - csv readable]

1. csv파일을 읽어서 토큰 단위로 파싱
    - strtok() 함수 활용
        
        ```c
        char* strtok(char* str, char* delimiters);
        ```
        
        첫번째 파라미터는 파싱할 문자열이고 두번째 파라미터는 기준으로 문자열을 토큰화(분할) 할 것 인지 나타내는 문자이다.
        
    
    ```c
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, file))
    {
    	char *hash_key = strtok(buffer, ",");
    	char *data = strtok(NULL, ",");
    	printf("key: %s, data: %s", hash_key, data);
    	add(hash_key, data);
    }
    ```
    
    fgets()를 통해 csv파일 내부의 데이터를 한 줄씩 읽고, 이를 buffer에 저장한다. 최초의 strtok는 한 줄 전체에서 “,”를 기준으로 토큰화 하고 첫번째 토큰 즉, 키 값을 hash_key에 저장한다. 또 두번째 strtok는 토큰화를 진행하고 남은 토큰을 data에 저장한다.
    
2. 모든 데이터가 csv파일의 특정 레이블로 저장되는 issue
    
    최종 생성된 해시 테이블의 모든 키, 데이터가 csv파일 마지막 줄로 출력되었다.  csv파일을 한 줄 한 줄 파싱하여 해시테이블에 넣을 때 마다 테이블의 현황을 출력하도록 했는데 새로운 키와 데이터가 전달될 때 마다 모든 데이터가 새로운 데이터로 바뀌었다.
    
    - 수정 전 코드
        
        ```c
        struct node *createNode(char *hash_key, char *data)
        {
        	struct node *newNode;
        
        	newNode = (struct node *)malloc(sizeof(struct node));
        
        	newNode->hash_key = hash_key;
        	newNode->data = data;
        	newNode->next = NULL;
        
        	return newNode;
        }
        ```
        
    - 수정 후 코드
        
        ```c
        struct node *createNode(char *hash_key, char *data)
        {
        	struct node *newNode;
        
        	newNode = (struct node *)malloc(sizeof(struct node));
        
        	newNode->hash_key = strdup(hash_key);
        	newNode->data = strdup(data);
        	newNode->next = NULL;
        
        	return newNode;
        }
        ```
        
        기존의 코드는 새로운 노드의 키와 데이터에 직접 주어진 키, 데이터를 전달하였다. 이 경우 각 노드에 hash_key와 data의 포인트 값을 가져가게 되며 차후 각 노드들은 직접적으로 데이터의 값이 아닌 포인터를 통해 해당 위치에 저장된 값에 접근하게 된다. 이때 새로운 데이터를 입력받아 hash_key 및 data에 다른 데이터가 저장된다 하더라도 기존의 노드들은  포인터를 통해 바뀐 데이터가 저장되어있는 공간으로 접근한다. 따라서 새로운 데이터가 입력될 때 마다 모든 데이터들이 새롭게 입력된 값으로 바뀌어버리는 것이다. 이를 방지하기 위해 일정한 메모리 공간을 가리키는 것이 아닌 새로운 메모리 공간에 값을 복사하여 해당 영역의 포인터를 전달함으로서 새로운 값이 들어와도 덮어쓰이지 않는다.