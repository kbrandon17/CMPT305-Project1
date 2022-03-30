## This is a simple Makefile

# Define what compiler to use and the flags.
CC=cc
CXX=CC
CCFLAGS= -g -std=c99 -Wall -Werror


all: project1

# Compile all .c files into .o files
# % matches all (like * in a command)
# $< is the source file (.c file)
%.o : %.c %.h
	$(CC) -c $(CCFLAGS) $<


project1: QueueNode.o PriorityQueue.o EvalQueue.o EventQueue.o Simulation.o project1.o
	$(CC) -o project1 QueueNode.o PriorityQueue.o EvalQueue.o EventQueue.o Simulation.o project1.o -lm


clean:
	rm -f *.o project1
