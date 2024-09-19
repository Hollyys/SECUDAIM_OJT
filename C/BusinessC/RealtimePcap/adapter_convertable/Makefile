# DPDK IFLAGS
IFLAGS += -I../dpdk-21.11/config
IFLAGS += -I../dpdk-21.11/build
IFLAGS += -I../dpdk-21.11/lib/net
IFLAGS += -I../dpdk-21.11/lib/meter
IFLAGS += -I../dpdk-21.11/lib/ethdev
IFLAGS += -I../dpdk-21.11/lib/mempool
IFLAGS += -I../dpdk-21.11/lib/mbuf
IFLAGS += -I../dpdk-21.11/lib/ring
IFLAGS += -I../dpdk-21.11/lib/eal/include
IFLAGS += -I../dpdk-21.11/lib/eal/linux/include
IFLAGS += -I../dpdk-21.11/lib/eal/x86/include

#  Compiler
CC = gcc
# Compile Options
CFLAGS = -Wall -g $(IFLAGS) -mssse3
# External Library
LIBS = -lpcap
LIBS += -L../dpdk-21.11/build/lib -lrte_eal -lrte_mempool -lrte_ring -lrte_ethdev -lrte_mbuf -lrte_hash

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
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Rule for generate Object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cleaning Rule
clean:
	rm -f $(TARGET) $(OBJS)

# Rebuiling Rule
rebuild: clean all
