# COMP30023 Computer Systems
# 2021 Semester 1
#
# Assignment 1 Makefile
# Implementation by Glenn Phillips (820624)
#
# Template by Tobias Edwards
# COMP20007 Design of Algorithms, 2019 Semester 1, Assignment 1

# Variables
CC		= gcc
CFLAGS	= -I -Wall
EXE		= allocate
OBJ		= main.o input_handler.o process_reader.o

# Handles "$ make" and "$ make all"
all: $(EXE)

# The main executable
$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ)

# Other dependencies
input_handler.o: input_handler.h input_handler.c
	$(CC) $(CFLAGS) -c input_handler.c
process_reader.o: process_reader.h process_reader.c
	$(CC) $(CFLAGS) -c process_reader.c

# "clean" and "all" don't actually create files called "clean" and "all"
# and are therefore "Phony Targets"
.PHONY: clean all

# Removes the object and executable files
clean:
	rm -f $(EXE) $(OBJ)