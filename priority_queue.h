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

typedef struct priority_queue_s priority_queue_t;

// --- Function Prototypes ---
// create a new queue and return a pointer to it
priority_queue_t *new_priority_queue();

// destroy a queue and free its memory
void free_priority_queue(priority_queue_t *queue);

// insert an element into the queue
void priority_queue_insert(priority_queue_t *queue, process_t process, int priority);

// remove the element with the lowest priority and return the process
process_t priority_queue_remove_min(priority_queue_t *queue);

// remove the element with the lowest priority if the priority
// equals a value, then return the process
// process_t priority_queue_remove_min_if_equals(priority_queue_t *queue, int value);

// todo?
// remove the element with the lowest priority and return the process
//process_t priority_queue_remove_min_if_less_than(priority_queue_t *queue);

// remove all the elements with the lowest priority and return the processes in an array
//process_t[] priority_queue_remove_all_min(priority_queue_t *queue);



// todo change this to process time remaining???
// update an elements priority in the queue by process
// returns whether or not this was succesful (i.e., the process was already
// in the queue)
bool priority_queue_update(priority_queue_t *queue, process_t process, int new_priority);

// returns whether the queue contains no elements (true) or some elements (false)
bool priority_queue_is_empty(priority_queue_t *queue);

#endif //PRIORITY_QUEUE_H
