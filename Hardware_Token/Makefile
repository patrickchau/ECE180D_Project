# Author:   Joseph Miller

CFLAGS = -Wall -Wextra
LDLIBS = -pthread -lwiringPi
OPS = -g -o

default: 7_seg

7_seg: 7_seg.c
	gcc $(CFLAGS) $(OPS) 7_seg 7_seg.c $(LDLIBS)

clean:
	rm -f 7_seg
