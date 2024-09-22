![header](https://capsule-render.vercel.app/api?type=waving&color=gradient&height=300&section=header&text=RnD%20%20OJT%20%20repo&fontSize=60&animation=twinkling)
# SECUDAIM R&D Center OJT repository

<br/><br/>

### Make file Templete
```c
#  Compiler
CC = gcc
# Compile Options
CFLAGS = -Wall -g
# External Library
LIBS = 


# Target file name (executable file name)
TARGET = run
# Source file (*.c)
SRCS =
# Object file (*.o)
OBJS = $(SRCS:.c=.o)

# Default Target
all: $(TARGET)

# Rule for generate executable files
$(TARGET): $(OBJS)
    $(CC) $(CFLAG) -o $(TARGET) $(OBJS) $(LIBS)

# Rule for generate Object files
%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

# Cleaning Rule
clean:
    rm -f $(TARGET) $(OBJS)

# Rebuiling Rule
rebuild: clean all

```

### 자주 사용하는 vim 명령어

#### 복붙

- 블럭 씌우기 : [ v ]
    - 라인 단위로 블럭 씌우기 : [ shift + v ]
- 복사: [ y ]
- 잘라내기: [ d ]
- 붙여넣기: [ p ]

#### 창 분할

- 수평 분할 : [ new ] or [ sp ]
- 수직 분할 : [ vs ]
- 분할된 화면에서 커서 위치 바꾸기: [ ww ]

#### 이동

- 줄의 처음 위치로 커서 이동 : [ - ]
- 맨 위로 커서 이동 : [ gg ]
- 맨 아래로 커서 이동: [ shift + g ]
- 현재 페이지 맨 위로 커서 이동: [ shift + h ]
- 현재 페이지 맨 아래로 커서 이동: [ shift + l ]
- 원하는 줄 번호로 커서 이동: [ :줄번호 ]
- ‘{’로 이동
    - forward : [ ]] ]
    - backward : [ [[ ]
- ‘}’로 이동
    - forward : [ ][ ]
    - backward : [ [] ]

#### 기타

- Undo : [u]
- Redo : [ ctrl + r ]
- 들여쓰기 : [ >> }
    - 한 문단 전체 들여쓰기 : [ >} ]
    - 여러 라인 들여쓰기 : [ v ] 를 통해 블럭을 지정한 다음 [ > ]
 

<br/><br/>
<br/><br/>
[<img align="left" alt="Hollyys | velog" width="48px" src="https://img.icons8.com/?size=100&id=F6H2fsqXKBwH&format=png&color=000000" />][website]

[website]: (https://www.notion.so/Sunghan-Shin-889cb5d661c746b68b3378ba76ccdae3?pvs=4)
<- Notion Link

### This is for Commit Test
