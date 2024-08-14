## 240723 **[Numpy]**

1. partioning 알고리즘에 관하여
    
    **주어진 배열에서 가장 작은 k개의 원소를 순서에 상관없이 배치**
    
    ```python
    import numpy as np
    
    data = np.array([5,3,1,2,4])
    partitioned_data = np.partition(data, 3)
    
    print(partitioned_data)
    ```
    
    ```python
    # 주피터 노트북에서 구동한 결과 값
    [2 1 3 4 5]
    ```
    
    - Chat GPT 답변
        
        파이썬의 NumPy 라이브러리에서 제공하는 `partition` 함수는 "Introselect" 알고리즘을 사용합니다. Introselect는 Introsort 알고리즘의 선택 부분(selection part)에 해당합니다. Introsort는 Quicksort, Heapsort, 그리고 Insertion Sort의 혼합 알고리즘으로, 평균적으로는 Quicksort의 속도와 최악의 경우 Heapsort의 속도를 보장합니다.
        
        Introselect는 Quicksort의 선택(selection) 변형입니다. 이 알고리즘은 다음과 같은 방식으로 작동합니다:
        
        1. **퀵 셀렉트 (Quickselect)**: Quicksort의 파티션 방법을 사용하여 원하는 순서 통계량 (예: k번째 작은 원소)을 찾습니다. Quicksort와 같이 피벗을 선택하고 배열을 두 부분으로 나누지만, 재귀적으로 탐색하는 쪽은 원하는 k번째 원소가 속한 쪽으로만 탐색합니다.
        2. **재귀 제한**: 만약 재귀 깊이가 특정 임계값을 초과하면, 이 알고리즘은 Heapsort로 전환합니다. 이는 Quicksort의 최악의 시간 복잡도를 피하기 위한 조치입니다.
        3. **최소 배열 크기**: 배열의 크기가 작아지면 Insertion Sort로 전환합니다. 이는 작은 배열에서의 정렬 속도를 향상시키기 위함입니다.
        
        결론적으로, NumPy의 `partition` 함수는 Quicksort 기반의 Quickselect 알고리즘을 사용하여 배열을 재배열하며, 필요한 경우 Heapsort와 Insertion Sort를 사용하여 최적의 성능을 보장합니다.
        
    - Chat GPT 알고리즘 흐름 설명
        
        기준 인덱스 3 (`k=3`)으로 배열 `data = np.array([5, 3, 1, 2, 4])`을 partitioning하는 과정을 단계별로 설명해 드리겠습니다.
        
        1. **초기 배열**: `[5, 3, 1, 2, 4]`
        2. **기준 인덱스 선택**: `k = 3` (0-based index)
        
        NumPy의 `partition` 함수는 기준 인덱스 `k`에 있는 값이 정확히 그 위치에 오도록 배열을 재배열합니다. 나머지 값들은 정렬되지 않은 상태로 작은 값들이 앞에, 큰 값들이 뒤에 배치됩니다.
        
        퀵셀렉트(Quickselect) 알고리즘을 사용하여 partitioning 과정을 설명하겠습니다.
        
        1. **초기 배열**: `[5, 3, 1, 2, 4]`
        2. **기준 인덱스 선택**: `k = 3`
        3. **퀵셀렉트 알고리즘 적용**:
            - 피벗 선택: 일반적으로 배열의 첫 번째 원소를 피벗으로 선택합니다.
            - 첫 번째 피벗: `5`
        4. **첫 번째 partitioning**:
            - 피벗보다 작은 값: `[3, 1, 2, 4]`
            - 피벗보다 큰 값: `[]`
            
            배열 상태: `[3, 1, 2, 4, 5]`
            
        
        이제 `k=3`보다 작은 인덱스 부분 `[3, 1, 2, 4]`에 대해 다시 partitioning을 진행합니다.
        
        1. **새로운 피벗 선택**: `3` (첫 번째 원소)
        2. **두 번째 partitioning**:
            - 피벗보다 작은 값: `[1, 2]`
            - 피벗보다 큰 값: `[4]`
            
            배열 상태: `[1, 2, 3, 4, 5]`
            
        3. **기준 인덱스 확인**:
            - 기준 인덱스 `k=3`에 있는 값은 `4`입니다. 배열이 나눠지는 기준은 `4`입니다.
            - `k=3`보다 작은 인덱스 부분: `[1, 2, 3]`
            - `k=3`보다 큰 인덱스 부분: `[5]`
        
        결과적으로, partitioning 후 배열은 `[1, 2, 3, 4, 5]`가 됩니다. `k=3`에 있는 값 `4`는 정확히 그 자리에 위치하게 됩니다.
        
        ```python
        data = np.array([5, 3, 1, 2, 4])
        partitioned_data = np.partition(data, 3)
        
        # 위 코드 결과값
        [2 3 1 4 5]
        ```
        
        <aside>
        ❓ 주피터 노트북과 결과값이 다르게 출력되었**음**
        
        </aside>
        
        **이에대한 GPT답변**
        
        차이가 나는 이유는 Quickselect 알고리즘이 피벗을 선택하고 partitioning을 수행할 때 배열 내에서 요소의 순서가 정해지지 않기 때문입니다. Quickselect는 정확히 기준 인덱스의 값을 찾고, 배열을 그 기준 값보다 작은 값과 큰 값으로 나누지만, 나누어진 부분들 내부의 순서는 보장하지 않습니다.
        
        다시 말해, `partition` 함수는 기준 인덱스 `k`에 있는 값이 정확히 그 위치에 오도록 하고, 그보다 작은 값들이 앞에, 큰 값들이 뒤에 오도록 배열하지만, 정렬된 순서는 아닙니다.
        
        <aside>
        💡 위에 GPT 답변에 따르면 파티션 알고리즘은 주어진 array의 길이에 따라 다른 알고리즘을 활용한다. 최초 gpt의 답변은 기본 알고리즘을 기준으로 설명하였고 실제 활용 시에는 배열의 길이에 따라 다른 알고리즘으로 동작할 가능성이 있다.
        
        </aside>
        

## 240724 **[Pandas]**

1. pandas append관련 issue
    
    ```python
    car_dict_list = [{'model':'911 GT3', 'maker':'porsche','hp':500},
                     {'model':'m2', 'maker':'bmw','hp':400},
                     {'model':'GR86', 'maker':'toyota','hp':230},
                     {'model':'range rover', 'maker':'land rover','hp':560},
                     {'model':'g80', 'maker':'genesis','hp':380}]
    df = pd.DataFrame(car_dict_list, columns = ['model', 'maker', 'hp'])
    # 기존의 DataFrame
    
    df2 = pd.DataFrame([
        ['sf90', 'ferrari', 1000]
    ], columns = ['model', 'maker', 'hp'])
    # 추가할 DataFrame
    
    df.append(df2, ignore_index = True)
    # append 시도
    ```
    
    DataFrame을 생성하여 기존의 DataFrame에 append하고자 하였으나
    
    ```python
    ---------------------------------------------------------------------------
    AttributeError                            Traceback (most recent call last)
    /tmp/ipykernel_9313/1169785372.py in ?()
    ----> 1 df.append(df2, ignore_index = True)
    
    ~/jupy/jup_notebook/lib/python3.8/site-packages/pandas/core/generic.py in ?(self, name)
       5985             and name not in self._accessors
       5986             and self._info_axis._can_hold_identifiers_and_holds_name(name)
       5987         ):
       5988             return self[name]
    -> 5989         return object.__getattribute__(self, name)
    
    AttributeError: 'DataFrame' object has no attribute 'append'
    ```
    
    위와 같은 오류 발생
    
    <aside>
    ⚠️ Pandas v2.0.0 이상의 버전에서 DataFrame.append 메소드가 사용되지 않아, concat을 사용하여 두 DataFrame을 합치는 것이 올바른 접근 방식이다.
    
    </aside>
    
    ```python
    # df.append(df2, ignore_index = True)
    df = pd.concat([df, df2], ignore_index = True)
    ```
    
2. apply()
    
    ```python
    def introduce(row):
        return 'I was born in ' + str(row.year) + " and my age is " + str(row.age)
    
    df['introduce'] = df.apply(introduce, axis=1)
    # axis = 0 : Index
    # axis = 1 : Columns
    # 함수를 적용할 축
    ```
    

## 240725 **[Practice]**

1. merge
    
    ```python
    df1 = pd.DataFrame({'ID': [1, 2, 3], 'Name': ['Alice', 'Bob', 'Charlie']})
    df2 = pd.DataFrame({'ID': [1, 2, 4], 'Age': [25, 30, 22]})
    # 주어진 두개의 데이터프레임을 공통열을 기준으로 병합하려는 Case
    
    merged_df = pd.merge(df1, df2, on='ID')
    # ID를 기준으로 merge하도록 지정했으며 Default는 Inner Join
    ```
    
    - Inner Join
        
        공통 열의 값이 두 테이터 프레임에 모두 있는 경우만 병합
        
        ```python
        merged_df = pd.merge(df1, df2, on='ID', how='inner')
        ```
        
        ```python
        # 실행결과
        
        ID	Name	Age
        0	1	Alice	25
        1	2	Bob	30
        ```
        
    
    - Left Join
        
        첫 번째 데이터 프레임의 모든 행을 유지하고, 두 번째 데이터 프레임에서 공통 열의 값이 일치하는 경우만 병합
        
        ```python
        merged_df = pd.merge(df1, df2, on='ID', how='left')
        ```
        
        ```python
        # 실행결과
        
        	ID	Name	Age
        0	1	Alice	25.0
        1	2	Bob	30.0
        2	3	Charlie	NaN
        ```
        
    - Right Join
        
        두 번째 데이터 프레임의 모든 행을 유지하고, 첫 번째 데이터 프레임에서 공통 열의 값이 일치하는 경우만 병합
        
        ```python
        merged_df = pd.merge(df1, df2, on='ID', how='right')
        ```
        
        ```python
        # 실행결과
        
        	ID	Name	Age
        0	1	Alice	25
        1	2	Bob	30
        2	4	NaN	22
        ```
        
    - Outer Join
        
        두 데이터 프레임의 모든 행을 유지하고, 공통 열의 값이 일치하는 경우 병합
        
        ```python
        merged_df = pd.merge(df1, df2, on='ID', how='outer')
        ```
        
        ```python
        # 실행결과
        
        	ID	Name	Age
        0	1	Alice	25.0
        1	2	Bob	30.0
        2	3	Charlie	NaN
        3	4	NaN	22.0
        ```
       