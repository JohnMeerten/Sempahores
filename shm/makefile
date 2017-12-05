CC = gcc
CFLAGS = -O2 -Wall -Werror
LDFLAGS = -lrt -pthread

.PHONY: all clean

all: consumer producer

consumer : consumer.c
	$(CC) $(CFLAGS) consumer.c -g -o consumer $(LDFLAGS)

producer : producer.c
	$(CC) $(CFLAGS) producer.c -g -o producer $(LDFLAGS) 

