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

// --- Helper Function Prototypes ---

// creates a new cpu then returns a pointer to it
cpu_t *new_cpu(int cpu_id);

// --- Function Implementations ---

// generate the specified number of cpus and add them to the simulation
void generate_cpus(int num_cpus, simulation_t *simulation) {
    for (int i = 0; i < num_cpus; i++) {
        cpu_t *cpu = new_cpu(i);

        // add the cpu to the all cpus and available cpus
        // with the priority matching the total time remaining
        priority_queue_insert(simulation->all_cpus, (data_t*) cpu, cpu->total_time_remaining);
        priority_queue_insert(simulation->available_cpus, (data_t*) cpu, cpu->total_time_remaining);
    }
}


// destroy a cpu and free all of its associated memory
void free_cpu(cpu_t *cpu) {
    assert(cpu != NULL);

    free_priority_queue(cpu->waiting);
    free_priority_queue(cpu->shortest_waiting);
    free_priority_queue(cpu->finished);

    free(cpu);
}

// --- Helper Function Implementations ---

// creates a new cpu then returns a pointer to it
cpu_t *new_cpu(int cpu_id) {

    cpu_t *cpu = malloc(sizeof *cpu);
    // check the cpu was successfully created
    assert(cpu);

    cpu->data.data_type = CPU;

    cpu->cpu_id = cpu_id;

    // todo initialise total time remaining as 0 ???
    cpu->total_time_remaining = 0;

    cpu->waiting = new_priority_queue();
    cpu->shortest_waiting = new_priority_queue();
    cpu->running = NULL;
    cpu->finished = new_priority_queue();

    return cpu;
}