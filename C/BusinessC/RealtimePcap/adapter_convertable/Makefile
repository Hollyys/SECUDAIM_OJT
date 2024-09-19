#  Compiler
CC = gcc
# Compile Options
CFLAGS = -Wall -g
# External Library
LIBS = $(pkg-config --cflags --libs libdpdk) -lpcap

# Target file name (executable file name)
TARGET = run
# Source file (*.c)
SRCS = parser.c parson.c adapter.c adapter_pcap.c adapter_dpdk.c signal.c daemon.c main.c
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
