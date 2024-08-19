#  Compiler
CC = gcc
# Compile Options
CFLAGS = -Wall -g

# Target file name (executable file name)
TARGET = HashTableLinkedList

# Source file (*.c)
SRCS = main.c HashTableLinkedList.c
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

