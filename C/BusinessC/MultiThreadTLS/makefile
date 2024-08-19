CC = gcc
CFLAGS = -Wall -g
LIBS = -lpthread

TARGET = multiThreadTls

SRCS = main.c threads.c parson.c HashTableLinkedList.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAG) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(TARGET) $(OBJS)
