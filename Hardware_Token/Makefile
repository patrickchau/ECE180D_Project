# Author:   Joseph Miller

CC = gcc
CFLAGS = -Wall -Wextra
LDLIBS = -pthread -lwiringPi
OPS = -g -o 
OBJFILES = hardware_token.o hardware.o network.o signal_handler.o
TARGET = hardware_token

all: $(OBJFILES)
	$(CC) $(CFLAGS) $(OPS) $(TARGET) $(OBJFILES) $(LDLIBS)

clean:
	rm -f $(TARGET) $(OBJFILES)