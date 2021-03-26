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
    char parallelisable; // n (not parallelisable) or p (parallelisable)
};

// --- Function Prototypes ---
priority_queue_t *queue_processes(char* filename);

#endif //PROCESS_H
