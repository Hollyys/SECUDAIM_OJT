## 240729 [Global Interpreter Lock]

**여러 개의 스레드가 파이썬 바이트 코드를 하나만 사용할 수 있도록 Lock을 거는 것. 하나의 스레드만 파이썬 인터프리터를 제어할 수 있다. : 실제로 멀티스레딩을 구현하더라도 병렬적으로 실행되는 것이 아닌 순차적으로 실행된다.**

1. 왜 필요한가? 
    - 레퍼런스 카운팅
        
        파이썬은 각 객체에 대한 레퍼런스 카운팅(참조 횟수)를 유지하는 방식으로 객체사용을 추적함 → 파이썬은 멀티 프로세싱이 흔치 않던 시절에 만들어진 만큼 레퍼런스 카운팅에 스레드 안전성이 없음
        
    - 레이스 컨디션
        
        다수의 프로세스, 스레드가 공유자원을 서로 사용하려고 경합하는 현상 → 동기화 문제 발생
        
        : 두개의 스레드가 동일한 객체(레퍼런스 카운트 = 0)에 동시에 접근 했다고 했을때 각자 0인 기존 레퍼런스 카운트 변수를 2로 변경하려고 시도한다. 하지만 사실 두 개의 스레드가 접근했으므로 2가 되어야 하지만 2가 아닌 다른 값으로 변경이 될 수 있다.
        
2. 싱글 스레드와 멀티 스레드의 비교 (작동확인)
    
    I/O Bound한 경우보다 CPU Bound한 경우 오히려 멀티스레드가 실행시간이 더 오래 걸린다.
    
3. 다른 언어에서는 어떤 뮤텍스를 사용하고 있는가?
    - C
        
        원시함수 제공: malloc(), free() …
        
    - swift
        
        Automatic Reference Counting: 컴파일 시점에 컴파일러가 release코드 삽입
        
    - java, JavaScript, Python
        
        Garbage Collection: 사용하지 않는 영역 자동탐지(추정): 프로그램 실행 중 동적으로 메모리 해제
        
4. No-GIL
    - 2023년 8월, GIL을 선택사항으로 두자는 제안을 승인함
        
        GIL을 아예 제거하게 된다면 싱글 스레드 프로그램(다수의 파이썬 레거시 프로그램)의 성능 저하 초래
        
        따라서 선택적으로 GIL을 제거할 수 있는 방향을 선택
        
        <aside>
        💡 스레드 안전성을 확보하면서 싱글 스레드에 영향을 주지 않기 위한 기법들
        
        - **편향 참조 카운팅**. 싱글 스레드에 의해서만 액세스되는 객체에 대한 카운트는 멀티 스레드에 의해 액세스되는 객체의 카운트와는 다르게(더 빠르게) 처리된다. 대부분의 객체는 단일 스레드에 의해서만 액세스되므로 싱글 스레드 프로그램에 대한 영향은 최소화된다.
        - **불멸화**. None과 같은 객체는 할당을 해제할 필요가 없으므로 이러한 객체의 참조 카운트도 추적할 필요가 없다.
        - **스레드 안전 메모리 할당**. C파이썬 객체에 대한 새로운 메모리 할당 시스템에서는 가비지 수집기에서 더 쉽게 객체를 추적하고 스레드에 안전한 방식으로 메모리를 할당할 수 있다.
        - **지연된 참조 카운팅**. 모듈의 최상위 함수와 같은 일부 객체에 대한 참조 카운트는 안전한 지연이 가능하다. 이렇게 하면 시간과 리소스가 모두 절약된다.
        - **수정된 가비지 수집기**. C파이썬 가비지 수집기는 둘 이상의 객체가 상호 참조를 갖는 순환 객체 참조를 정리한다. no-GIL 빌드에서는 객체 추적을 위한 “세대” 시스템이 제거되는 등 [가비지 수집기의 많은 부분이 변경](https://peps.python.org/pep-0703/#garbage-collection-cycle-collection)된다.
        </aside>
        
    - 어째서 파이썬만 문제가 되고 있는가?
        
        파이썬은 근본적으로 간단하고 알아보기 쉬운 프로그래밍 언어 컨셉으로 만들어짐. 따라서 다른 언어에 비해 멀티 스레딩과 같은 퍼포먼스 적 측면에 취약한 면이 있음. 파이썬도 가비지 컬렉션을 사용하고는 있지만 각종 고급 매커니즘을 사용하는 타 프로그래밍 언어의 그것과 달리 레퍼런스 카운팅에 의존하고 있다. 따라서 실질적인 CPU Bound 프로세스의 경우 스레드 관리는 GIL위주로 하고 있다.
        
5. Python Garbage Collection, Reference Count관련 글
    
    [https://devloper-dreaming.tistory.com/50](https://devloper-dreaming.tistory.com/50)
    

## 240730 [Polars]

1. **ERR: Missing required CPU features**

   ![Untitled 1](https://github.com/user-attachments/assets/131342f5-4832-4d63-be4b-58f67d32f28c)
    
    ```bash
    pip install polars-lts-cpu
    ```
    
3. `ComputeError: could not parse `NA` as dtype `i64` at column 'MasVnrArea' (column number 27)`
    
    ```python
    pl_df = pl.read_csv('train.csv', null_values='NA')
    # NA라는 문자열을 결측치로 처리
    ```
    
4. `ModuleNotFoundError: pa.Table requires 'pyarrow' module to be installed`
5. 퍼포먼스 체크
    
    ![Untitled](%E2%93%9F%E2%93%A8%E2%93%A3%E2%93%97%E2%93%9E%E2%93%9D%2061a237b23fae43cab76f127edf4ede1a/Untitled%201.png)
    
    Polars 진짜 빠르다
    
6. Polars로 읽은 CSV파일을 PyGWalker로 시각화 하기
    
    ```python
    pl_walker = pyg.walk( pl_df.to_pandas())
    pl_walker
    ```
    
    Polars DataFrame을 Pandas로 변환 후 출력해야한다.
    

## 240801 [Multi Processing]

1. Multi Threading VS Multi Processing
    - Multi Processing
        
        여러 개의 프로세스를 생성하여 프로세스가 독립적으로 작업을 처리
        
        : GIL의 영향을 받지 않고, 독립적인 메모리 공간을 가지기 때문에 자원관리에 용이하여 시스템이 안정적이다. 하지만 비교적 많은 메모리 공간을 사용하며, 프로세스 간 context switchging이 느리다.
        
        ```python
        import multiprocessing
        
        def calculate_square(numbers):
            for number in numbers:
                square = number * number
                print(f"The square of {number} is {square}")
        
        def main():
            numbers = [1, 2, 3, 4, 5]
            
            process1 = multiprocessing.Process(target=calculate_square, args=(numbers,))
            process1.start()
            
            process1.join()
        
        if __name__ == "__main__":
            main()
        ```
        
    - Multi Threading
        
        프로세서 내에서 쓰레드를 생성하여 작업을 처리
        
        : 쓰레드간의 context switching이 간단하고 빠르며 적은 메모리 공간을 사용하지만 GIL의 영향을 받는다.
        
        ```python
        import threading
        
        def calculate_square(numbers):
            for number in numbers:
                square = number * number
                print(f"The square of {number} is {square}")
        
        def main():
            numbers = [1, 2, 3, 4, 5]
            
            thread1 = threading.Thread(target=calculate_square, args=(numbers,))
            thread1.start()
            
            thread1.join()
        
        if __name__ == "__main__":
            main()
        ```
        
    - Multi Thread와 Multi Process의 가장 큰 차이점은 메모리 공유여부이다.
        
        프로세스들은 각자 다른 메모리 영역에서 동작하지만 쓰레드는 같은 메모리공간 내에서 작동한다.
        
