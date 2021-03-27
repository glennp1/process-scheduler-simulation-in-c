# COMP30023 Computer Systems
# 2021 Semester 1
#
# Project 1 Makefile
# Implementation by Glenn Phillips (820624)
#
# Template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
# COMP20007 Design of Algorithms, 2019 Semester 1, Assignment 1

# Variables
CC		= 	gcc
CFLAGS	= 	-I -Wall
EXE		= 	allocate
OBJ		= 	main.o input_handler.o process.o priority_queue.o \
			simulation.o cpu.o

# Handles "$ make" and "$ make all"
all: $(EXE)

# The main executable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ)

# Other dependencies
input_handler.o: input_handler.h input_handler.c
	$(CC) $(CFLAGS) -c input_handler.c
process.o: process.h process.c
	$(CC) $(CFLAGS) -c process.c
priority_queue.o: priority_queue.h priority_queue.c
	$(CC) $(CFLAGS) -c priority_queue.c
simulation.o: simulation.h simulation.c
	$(CC) $(CFLAGS) -c simulation.c
cpu.o: cpu.h cpu.c
	$(CC) $(CFLAGS) -c cpu.c

# "clean" and "all" don't actually create files called "clean" and "all"
# and are therefore "Phony Targets"
.PHONY: clean all

# Removes the object and executable files
clean:
	rm -f $(EXE) $(OBJ)