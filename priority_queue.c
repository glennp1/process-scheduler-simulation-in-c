// COMP30023 Computer Systems
// 2021 Semester 1
//
// Priority Queue implementation for processes and cpus
// Implementation by Glenn Phillips (820624)
//
// Template by Matt Farrugia <matt.farrugia@unimelb.edu.au>
// modifications made by Tobias Edwards <tobias.edwards@unimelb.edu.au>
// COMP20007 Design of Algorithms, 2019 Semester 1, Assignment 1

// --- System Libraries ---
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

// todo remove
#include <stdio.h>

// --- Project Libraries ---
#include "priority_queue.h"
#include "process.h"

// --- Constant Definitions ---




// --- Type Definitions ---
typedef struct node_s node_t;

// a list node points to the next node in the list, and to some data
struct node_s {
    node_t *next;
    data_t *data;
    unsigned int priority;
};

// todo not sure if useful
typedef struct min_node_pair_s min_node_pair_t;

// a pair of two nodes
struct min_node_pair_s {
    node_t *min_prev;
    node_t *min_node;
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

// helper function to clear the memory of a data item
void free_data_item(data_t *data);

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

// destroy the specified queue and free its memory
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

// destroy all of the data stored in a priority queue (not the queue or nodes themselves)
void free_pq_data(priority_queue_t *queue) {
    assert(queue != NULL);
    // free the data stored in each node
    node_t *node = queue->head;
    node_t *next;
    while (node) {
        next = node->next;

        free_data_item(node->data);

        node->data = NULL;
        node = next;
    }
}

// insert an element into the queue
void priority_queue_insert(priority_queue_t *queue, data_t *data, unsigned int priority) {
    assert(queue != NULL);

    // create and initialise a new queue node
    node_t *node = new_pq_node();
    node->data = data;
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

// remove the element with the lowest priority and return it
data_t *priority_queue_remove_min(priority_queue_t *queue) {

    // find the min node pair associated with the lowest priority element
    min_node_pair_t min_node_pair = find_min_pq_node(queue);

    // save the data stored in the min node
    data_t *data = min_node_pair.min_node->data;

    // safely remove the min node
    remove_pq_node(queue, min_node_pair.min_prev, min_node_pair.min_node);

    // return the data
    return data;
}

// tries to remove the element with the lowest priority, if the priority
// equals a value then return the element, otherwise return a null pointer
data_t *priority_queue_remove_min_if_equals(priority_queue_t *queue, unsigned int value) {

    // find the min node pair associated with the lowest priority element
    min_node_pair_t min_node_pair = find_min_pq_node(queue);

    // check if the min node's priority is equal to the designated value
    if (min_node_pair.min_node->priority == value) {
        // save the data stored in the min node
        data_t *data = min_node_pair.min_node->data;

        // safely remove the min node
        remove_pq_node(queue, min_node_pair.min_prev, min_node_pair.min_node);

        // return the data
        return data;
    }
    else {
        // return a null pointer
        return NULL;
    }
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

// swaps the two priority queue pointers with one another
void swap_priority_queues(priority_queue_t *first, priority_queue_t *second) {
    priority_queue_t temp = *first;
    *first = *second;
    *second = temp;
}


// returns whether the queue contains no elements (true) or some elements (false)
bool priority_queue_is_empty(priority_queue_t *queue) {
    assert(queue != NULL);
    return (queue->size==0);
}

// --- Helper Function Implementations ---

// helper function to create a new node and return its address
node_t *new_pq_node() {
    node_t *node = malloc(sizeof *node);
    assert(node);

    return node;
}

// helper function to find the element with the lowest priority,
// then return this lowest element and the node prior to it as a min node pair
min_node_pair_t find_min_pq_node(priority_queue_t *queue) {
    assert(queue != NULL);
    assert(queue->size > 0);

    node_t *min_prev = NULL;
    node_t *min_node = queue->head;
    unsigned int min_priority = min_node->priority;

    node_t *prev = queue->head;
    node_t *node = queue->head->next;
    unsigned int priority;
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

    return min_node_pair;
}

// helper function to remove "this" node from the priority queue
void remove_pq_node(priority_queue_t *queue, node_t *prev, node_t *this) {
    // If we're the head or the tail then we better update that we're removing this node

    if (this == queue->head) {
        queue->head = queue->head->next; // update the head
    }
    else {
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

// helper function to clear memory of a node
void free_pq_node(node_t *node) {
    free(node);
}

// todo maybe move this to simulation
//  just have the queue remove one item then have simulation free it
// helper function to clear the memory of a data item
void free_data_item(data_t *data) {
    assert(data != NULL);

    // Free the data in the case it is a CPU
    if (data->data_type == CPU) {
        cpu_t *cpu = (cpu_t*) data;
        free_cpu(cpu);
    }
    // Free the data in the case it is a PROCESS
    else {
        process_t *process = (process_t*) data;
        free_process(process);
    }
}

