CC = gcc
TARGET = app.out
OBJS = main.o kor.o usa.o

CFLAGS = -Wall
LDFLAGS = -lc

all : app.out

$(TARGET) : $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm -f $(OBJS) $(TARGET)
