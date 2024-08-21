## 240819 [Multi Thread TLS]

1. 개발 문서
    - 요구사항
        - 멀티 스레드 구현
        - 각 스레드 마다  독립적으로 hash.csv를 읽어 해시테이블 생성
        - 각 스레드 상태를 주기적으로 출력
            - INT : 초기화 작업 중
            - RUNNING : 동작 중
            - DONE : 완료
            - SLEEP : 쉬는 중
        - 모든 스레드가 DONE or SLEEP 상태가 되면 각 스레드의 해시테이블 출력
    - 설계
        - 기존 해시테이블 연결리스트 소스를 기반으로  개발
        - 스레드에 관한 정보는  json parser 기반으로 작성
        - json 파싱 코드와 마찬가지로 main 함수에서 파싱하여 스레드를 읽고 각 스레드에 해시 테이블과 관련된 메소드를 준다. (필요없는 삭제와 같은 기능은 삭제)
        - 상태 출력
            - INT : 초기화 작업 중 →  해시 구조체 생성시 출력
            - RUNNING : 동작 중 → 해시테이블 생성시 출력
            - DONE : 완료 → 해시테이블 생성 완료시 출력(hash.csv를 끝까지 읽은 경우)
            - SLEEP : 쉬는 중
        - 해시테이블 출력 조건: count 전역변수를 주어 DONE이 될 경우 하나씩 증가하여 스레드 개수와 동일할 경우 display() 호출
    - 구조
    - 이슈
        - 240819: 각 스레드에서 각각 테이블을 생성해서 출력하는 것이 아닌, 하나의 테이블만 출력이 됨.
            
            Todo: display()를 특정 테이블의 포인터를 받아 출력하도록 변경하고 sigint handler에서 호출해볼 것. 또한 출력하는 부분에서 원자성을 고려하여 재설계 해볼것.
            
            ![image.png](C%2032a05fb848824170a1fc2ea9b315e180/image%202.png)
            
            Solution: 해시 테이블을 tls 변수로 선언하여 해결.
            
            ```c
            // threads.c
            
            // 전역 TLS 변수로 선언
            __thread struct bucket *hashtable = NULL
            ```
            
            240820: 멀티스레드 환경에서 signal사용
            
            기존의 싱글스레드 에서의 signal과 멀티스레드 환경에서의 signal이 사용법이 다름
            
            Solution:
            
            ```c
            // main.c
            // 간단한 시그널 세팅
            
            signal(SIGINT, signal_handler);
            ```
            
            ```c
            // threads.c
            // 추가적인 시그널 세팅이 필요한 경우
            
            // Signal 관련 설정을 위한 설정 저장
            void setup_signal_handler()
            {
            		// 특정 시그널을 처리하기 위한 정보 저장
                struct sigaction sa;
                
                // 시그널 발생 시 호출될 핸들러 함수
                sa.sa_handler = signal_handler;
                
                // sa_mask는 시그널을 처리하는 동안 블록 할 추가적인 시그널 정의
                // sigemptyset을 통해 비어있는 상태로 만듦
                // 즉 추가적으로 처리할 시그널은 없다는 의미
                sigemptyset(&sa.sa_mask);
                
                // sa_flags는 시그넗 처리시에 사용되는 옵션 플래그를 설정
                sa.sa_flags = 0;
                
                // 어떤 시그널에 대하여 처리를 할 것인지
                sigaction(SIGINT, &sa, NULL);
            }
            ```
            
            시그널이 발생했을 때 이를 처리하는 핸들러 설정 (이 경우 SIGINT)
            
            ```c
            volatile sig_atomic_t sigint_received = 0;
            ```
            
            **volatile sig_atomic_t 데이터 타입: 전역변수이자 flag변수로 사용해야 할 경우 사용. Atomic하게 레지스터를 읽고 쓸 수 있게 해줌.**
            
            (volatile 변수의 경우 컴파일러가 변수의 값을 레지스터가 아닌 메모리에서 직접 읽고 쓸 수 있도록 한다.)
            
            → 일반 변수로 변경 후 사용 시 모든 스레드에 signal이 전달되지 않는 듯 했음. 여러번 signal을 보내야 테이블이 출력되고 종료되는 느낌
            
        - 240820: 여러개의 스레드가 동시에 해시테이블을 출력하여 결과물이 섞여서 나옴.
            
            Soluton: Mutex를 통해 테이블을 출력하는 부분에 Lock을 걸어 해결
