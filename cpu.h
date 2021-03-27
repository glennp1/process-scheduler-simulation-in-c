//
// Created by Glenn Phillips on 28/3/21.
//

#ifndef CPU_H
#define CPU_H

// --- System Libraries ---

// --- Project Libraries ---
#include "priority_queue.h"

// --- Constant Definitions ---

// --- Type Definitions ---
typedef struct priority_queue_s priority_queue_t; // forward declaration so it compiles

typedef struct cpu_s cpu_t;

struct cpu_s {

    // todo add other variables
    // todo investigate int size
    unsigned long long int total_time_remaining;

    priority_queue_t *waiting;
    priority_queue_t *shortest_waiting;
    priority_queue_t *running;
    priority_queue_t *finished;

};

// --- Function Prototypes ---

// creates a new cpu then returns a pointer to it
cpu_t *new_cpu();

// destroy a cpu and free all of its associated memory
void free_cpu(cpu_t *cpu);

#endif //CPU_H
