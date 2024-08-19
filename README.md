![header](https://capsule-render.vercel.app/api?type=waving&color=gradient&height=300&section=header&text=RnD%20%20OJT%20%20repo&fontSize=60&animation=twinkling)
# SECUDAIM R&D Center OJT repository

<br/><br/>

### Make file Templete
```c
#  Compiler
CC = gcc
# Compile Options
CFLAGS = -Wall -g

# Target file name (executable file name)
TARGET =
# Source file (*.c)
SRCS =
# Object file (*.o)
OBJS = $(SRCS:.c=.o)

# Default Target
all: $(TARGET)

# Rule for generate executable files
$(TARGET): $(OBJS)
    $(CC) $(CFLAG) -o $(TARGET) $(OBJS)

# Rule for generate Object files
%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

# Cleaning Rule
clean:
    rm -f $(TARGET) $(OBJS)

# Rebuiling Rule
rebuild: clean all

```
<br/><br/>
<br/><br/>
[<img align="left" alt="Hollyys | velog" width="48px" src="https://img.icons8.com/?size=100&id=F6H2fsqXKBwH&format=png&color=000000" />][website]

[website]: [https://velog.io/@imysh578](https://www.notion.so/Sunghan-Shin-889cb5d661c746b68b3378ba76ccdae3?pvs=4)
<- Notion Link
