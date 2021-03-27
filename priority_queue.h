// COMP30023 Computer Systems
// 2021 Semester 1
//
// Priority Queue implementation for processes
// Implementation by Glenn Phillips (820624)
//
// Template by Matt Farrugia <matt.farrugia@unimelb.edu.au>
// modifications made by Tobias Edwards <tobias.edwards@unimelb.edu.au>
// COMP20007 Design of Algorithms, 2019 Semester 1, Assignment 1

#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

// --- System Libraries ---
#include <stdbool.h>

// --- Project Libraries ---
#include "process.h"

// --- Constant Definitions ---

// --- Type Definitions ---
typedef struct process_s process_t; // forward declaration so it compiles

typedef struct node_s node_t; // forward declaration so it compiles

typedef struct priority_queue_s priority_queue_t;

// a priority_queue points to its first and last nodes, and stores its size
// i.e., (num. nodes)
struct priority_queue_s {
    node_t *head;
    node_t *tail;
    unsigned int size;
};

// --- Function Prototypes ---

// todo implement additional functions with same name but different signature
//  i.e. for queues of CPUs rather than processes

// create a new queue and return a pointer to it
priority_queue_t *new_priority_queue();

// destroy the specified queue and free its memory
void free_priority_queue(priority_queue_t *queue);

// destroy all of the processes stored in a priority queue (not the queue or nodes themselves)
void free_pq_processes(priority_queue_t *queue);

// insert an element into the queue
void priority_queue_insert(priority_queue_t *queue, process_t *process, unsigned int priority);

// remove the element with the lowest priority and return the process
process_t *priority_queue_remove_min(priority_queue_t *queue);

// tries to remove the element with the lowest priority, if the priority
// equals a value then return the process, otherwise return a null pointer
process_t *priority_queue_remove_min_if_equals(priority_queue_t *queue, unsigned int value);

// todo?
// remove the element with the lowest priority and return the process
//process_t priority_queue_remove_min_if_less_than(priority_queue_t *queue);

// remove all the elements with the lowest priority and return the processes in an array
//process_t[] priority_queue_remove_all_min(priority_queue_t *queue);



// todo change this to process time remaining???
// update an elements priority in the queue by process
// returns whether or not this was succesful (i.e., the process was already
// in the queue)
// bool priority_queue_update(priority_queue_t *queue, process_t process, int new_priority);

// returns whether the queue contains no elements (true) or some elements (false)
bool priority_queue_is_empty(priority_queue_t *queue);

#endif //PRIORITY_QUEUE_H
