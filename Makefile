# COMP30023 Computer Systems
# 2021 Semester 1
#
# Assignment 1 Makefile
# By Glenn Phillips (820624)

CC=gcc
CFLAGS=-I -Wall

allocate: main.o hello.o
	$(CC) $(CFLAGS) -o allocate main.o hello.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c


hello.o: hello.c hello.h
	$(CC) $(CFLAGS) -c hello.c

.PHONY: clean

clean:
	rm -f allocate main.o hello.o


# Template from COMP20007

#CC     = gcc
#CFLAGS = -Wall
#ALL    = p1 p2 p3
#
#all: $(ALL)
#
#P1_FILES = p1.c
#p1: $(P1_FILES)
#$(CC) $(CFLAGS) -o p1 $(P1_FILES)
#
#P2_FILES = p2.c graph.c priorityqueue.c list.c
#p2: $(P2_FILES)
#$(CC) $(CFLAGS) -o p2 $(P2_FILES)
#
#P3_FILES = p3.c
#p3: $(P3_FILES)
#$(CC) $(CFLAGS) -o p3 $(P3_FILES)
#
#.PHONY: clean
#
#clean:
#rm -f $(ALL)
