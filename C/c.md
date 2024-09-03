## 240830 [Signal & Thread Safe Coding]

1. Signal Safety
    
    [https://man7.org/linux/man-pages/man7/signal-safety.7.html](https://man7.org/linux/man-pages/man7/signal-safety.7.html)
    
    - async-signal-safe: 시그널 핸들러의 실행 과정에서 interrupt 되더라도 side effect가 없어야 한다.  시그널 핸들러는 스레드, 프로세스의 진행 상황과는 상관없이 비동기적으로 실행되기 때문에 파일, 전역변수, 버퍼 등에 접근하여 상태를 변화시킬 수 있기 때문이다.
    - 사용하려는 함수가 async-signal-safe한지 확인하고 사용해야함
    - 가능한 원시적인 함수
2. Linux Kernel이 signal을 처리하는 과정
    1. 시그널 발생
    2. 시그널 전달 : 커널은 프로세스로 발생한 시그널을 전달한다. 시그널은 비동기 적으로 프로세스의 상태와 상관없이 전달된다.
    3. 시그널 처리 : 세가지 방법으로 처리할 수 있다.
        1. 기본 동작수행 : 시그널마다 정해진 기본 동작을 수행한다.
        2. 시그널 무시 : 단, sigkill. sigstop은 무시할 수 없음
        3. 사용자 정의 핸들러 실행 : 프로세스는 특정 시그널에 대해 핸들러를 설정하여 시그널 발생시 실행되도록 한다.
3. 시그널 처리 완료 : 시그널 핸들러가 실행된 후, 프로젝트는 원래 수행하던 작업으로 복귀한다.
