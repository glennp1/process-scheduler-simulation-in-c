// COMP30023 Computer Systems
// 2021 Semester 1
//
// Priority Queue implementation for processes
// Implementation by Glenn Phillips (820624)
//
// Template by Matt Farrugia <matt.farrugia@unimelb.edu.au>
// modifications made by Tobias Edwards <tobias.edwards@unimelb.edu.au>
// COMP20007 Design of Algorithms, 2019 Semester 1, Assignment 1

// --- System Libraries ---
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

// --- Project Libraries ---
#include "priority_queue.h"
#include "process.h"

// --- Type Definitions ---
typedef struct node_s node_t;

// a list node points to the next node in the list, and to some data
struct node_s {
    node_t *next;
    process_t process;
    int priority;
};

// todo not sure if useful
typedef struct min_node_pair_s min_node_pair_t;

// a pair of two nodes
struct min_node_pair_s {
    node_t *min_prev;
    node_t *min_node;
};


// a priority_queue points to its first and last nodes, and stores its size
// i.e., (num. nodes)
struct priority_queue_s {
    node_t *head;
    node_t *tail;
    int size;
};

// --- Function Prototypes ---

// helper function to create a new node and return its address
node_t *new_pq_node();

// helper function to find the element with the lowest priority,
// then return this lowest element and the node prior to it as a min node pair
min_node_pair_t find_min_pq_node(priority_queue_t *queue);

// helper function to remove "this" node from the priority queue
void remove_pq_node(priority_queue_t *queue, node_t *prev, node_t *this);

// helper function to clear memory of a node (does not free the node's data)
void free_pq_node(node_t *node);

// --- Function Implementations ---

// create a new queue and return a pointer to it
priority_queue_t *new_priority_queue() {
    priority_queue_t *queue = malloc(sizeof *queue);
    assert(queue);

    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;

    return queue;
}

// destroy a queue and free its memory
void free_priority_queue(priority_queue_t *queue) {
    assert(queue != NULL);
    // free each node
    node_t *node = queue->head;
    node_t *next;
    while (node) {
        next = node->next;
        free_pq_node(node);
        node = next;
    }
    // free the queue struct itself
    free(queue);
}

// helper function to create a new node and return its address
node_t *new_pq_node() {
    node_t *node = malloc(sizeof *node);
    assert(node);

    return node;
}

// helper function to clear memory of a node
void free_pq_node(node_t *node) {
    free(node);
}

// insert an element into the queue
void priority_queue_insert(priority_queue_t *queue, process_t process, int priority) {
    assert(queue != NULL);

    // create and initialise a new queue node
    node_t *node = new_pq_node();
    node->process = process;
    node->priority = priority;
    node->next = queue->head; // next will be the old first node (may be null)

    // place it at the start of the queue
    queue->head = node;

    // if queue was empty, this new node is also the last node now
    if(queue->size == 0) {
        queue->tail = node;
    }

    // and we need to keep size updated!
    queue->size++;
}

// remove the element with the lowest priority and return the process
process_t priority_queue_remove_min(priority_queue_t *queue) {

    // find the min node pair associated with the lowest priority element
    min_node_pair_t min_node_pair = find_min_pq_node(queue);

    // save the process stored in the min node
    process_t process = min_node_pair.min_node->process;

    // safely remove the min node
    remove_pq_node(queue, min_node_pair.min_prev, min_node_pair.min_node);

    // done!
    return process;
}

// todo replace node pair with node update using pointers
// helper function to find the element with the lowest priority,
// then return this lowest element and the node prior to it as a min node pair
min_node_pair_t find_min_pq_node(priority_queue_t *queue) {
    assert(queue != NULL);
    assert(queue->size > 0);

    node_t *min_prev = NULL;
    node_t *min_node = queue->head;
    int min_priority = min_node->priority;

    node_t *prev = queue->head;
    node_t *node = queue->head->next;
    int priority;
    while (node != NULL) {
        priority = node->priority;

        // if we find a lower priority, replace the current minimums
        if (priority < min_priority) {
            min_prev = prev;
            min_node = node;
            min_priority = priority;
        }

        prev = node;
        node = node->next;
    }

    // return min_prev and min_node as a node pair
    min_node_pair_t min_node_pair = {
            min_prev,
            min_node
    };

    return (min_node_pair);
}

// helper function to remove "this" node from the priority queue
void remove_pq_node(priority_queue_t *queue, node_t *prev, node_t *this) {
    // If we're the head or the tail then we better update that we're removing this node

    if (this == queue->head) {
        queue->head = queue->head->next; // update the head
    } else {
        assert(prev != NULL); // if the previous node exists
        assert(prev->next == this); // if the previous node is directly before the minimum
        prev->next = this->next; // the previous node's next is the minimum's next
    }

    if (this == queue->tail) {
        queue->tail = prev; // update the tail
    }

    queue->size--; // update the queue size

    // and we're finished with the node holding this data
    free_pq_node(this);
}

// todo change this to process time remaining???
// update an elements priority in the queue by process
// returns whether or not this was succesful (i.e., the process was already
// in the queue)
//bool priority_queue_update(priority_queue_t *queue, process_t process, int new_priority) {
//    assert(queue != NULL);
//
//    node_t *node = queue->head;
//    while (node != NULL) {
//        if (node->process == process) {
//            node->priority = new_priority;
//            return true;
//        }
//
//        node = node->next;
//    }
//
//    return false;
//}

// returns whether the queue contains no elements (true) or some elements (false)
bool priority_queue_is_empty(priority_queue_t *queue) {
    assert(queue != NULL);
    return (queue->size==0);
}