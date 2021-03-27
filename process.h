//
// Created by Glenn Phillips on 25/3/21.
//

#ifndef PROCESS_H
#define PROCESS_H

// --- System Libraries ---

// --- Project Libraries ---
#include "priority_queue.h"

// --- Constant Definitions ---


// --- Type Definitions ---
typedef struct priority_queue_s priority_queue_t; // forward declaration so it compiles

typedef struct process_s process_t;

// todo store each process
struct process_s {
    unsigned int time_arrived; // [0, 2^32)
    unsigned int process_id; // [0, 2^32)
    unsigned int execution_time; // [1, 2^32)
    bool parallelisable; // true if parallelisable, false if not
};

// --- Function Prototypes ---
// create a new process and return a pointer to it
process_t *new_process();

// destroy a process and free its memory
void free_process(process_t *process);

// loads processes from the specified file and returns them as a priority queue
// with priorities based on time arrived
priority_queue_t *queue_processes(char* filename);

#endif //PROCESS_H
