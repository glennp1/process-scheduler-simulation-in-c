//
// Created by Glenn Phillips on 28/3/21.
//


// --- System Libraries ---
#include <stdlib.h>
#include <assert.h>

// --- Project Libraries ---
#include "cpu.h"

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Function Prototypes ---

// --- Function Implementations ---

// creates a new cpu then returns a pointer to it
cpu_t *new_cpu() {

    cpu_t *cpu = malloc(sizeof *cpu);
    // check the cpu was successfully created
    assert(cpu);

    // todo figure out how to implement this
//    cpu->total_time_remaining

    cpu->waiting = new_priority_queue();
    cpu->shortest_waiting = new_priority_queue();
    cpu->running = new_priority_queue();
    cpu->finished = new_priority_queue();

    return cpu;
}

// destroy a cpu and free all of its associated memory
void free_cpu(cpu_t *cpu) {
    free_priority_queue(cpu->waiting);
    free_priority_queue(cpu->shortest_waiting);
    free_priority_queue(cpu->running);
    free_priority_queue(cpu->finished);
}