#!/usr/bin/make
CC = gcc
CFLAGS = -Os
TARGET = unpack
OBJS = main.o
all:$(TARGET)
$(TARGET):$(OBJS)
	$(CC) $(OBJS) -o $(TARGET)
clean:
	rm -f $(TARGET) $(OBJS)
