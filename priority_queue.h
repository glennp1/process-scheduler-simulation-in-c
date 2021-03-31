// COMP30023 Computer Systems
// 2021 Semester 1
//
// Priority Queue implementation for processes and cpus
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

// --- Constant Definitions ---

// --- Enum Definitions ---

// stores the primary queue data type

typedef enum data_type_state data_type_state_t;

enum data_type_state {
    PROCESS,
    CPU
};

typedef struct data_s data_t;

struct data_s {
    data_type_state_t data_type;
};

// --- Type Definitions ---
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

// destroy all of the data stored in a priority queue (not the queue or nodes themselves)
void free_pq_data(priority_queue_t *queue);

// insert an element into the queue
void priority_queue_insert(priority_queue_t *queue, data_t *data, unsigned int priority);

// remove the first element irrespective of priority
data_t *priority_queue_remove(priority_queue_t *queue);

// remove the element with the lowest priority and return it
data_t *priority_queue_remove_min(priority_queue_t *queue);

// tries to remove the element with the lowest priority, if the priority
// equals a value then return the element, otherwise return a null pointer
data_t *priority_queue_remove_min_if_equals(priority_queue_t *queue, unsigned int value);

// swaps the two priority queue pointers with one another
void swap_priority_queues(priority_queue_t *first, priority_queue_t *second);

// returns whether the queue contains no elements (true) or some elements (false)
bool priority_queue_is_empty(priority_queue_t *queue);

#endif //PRIORITY_QUEUE_H
