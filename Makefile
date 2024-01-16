CC = gcc
CFLAGS = -Wall -g -I/opt/homebrew/include
LIBS = -lm -pthread -lzmq -L/opt/homebrew/lib
TARGETS = $(patsubst %.c,%,$(wildcard *.c))

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean:
	rm -f $(TARGETS)