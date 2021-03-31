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

// removes the cpu from the specified queue with that is the emptiest and has
// the lowest id
cpu_t *remove_emptiest_and_lowest_id_cpu(priority_queue_t *cpus) {

    // *** get the emptiest cpus
    priority_queue_t *emptiest_cpus = new_priority_queue();
    unsigned int emptiest_cpu_total_time_remaining;

    // remove one of the emptiest from the cpus, save its total time remaining
    cpu_t *emptiest_cpu = (cpu_t*) priority_queue_remove_min(cpus);
    emptiest_cpu_total_time_remaining = emptiest_cpu->total_time_remaining;

    // add it to the emptiest cpus, priority is cpu id
    priority_queue_insert(emptiest_cpus,
                          (data_t*) emptiest_cpu,
                          emptiest_cpu->cpu_id);

    // add any other cpus that have a matching total time remaining to emptiest cpus
    while(!priority_queue_is_empty(cpus)) {
        emptiest_cpu = (cpu_t*) priority_queue_remove_min_if_equals(
                cpus, emptiest_cpu_total_time_remaining);

        if (emptiest_cpu != NULL) {
            // add it to the emptiest cpus, priority is cpu id
            priority_queue_insert(emptiest_cpus,
                                  (data_t*) emptiest_cpu,
                                  emptiest_cpu->cpu_id);
        }
        else {
            break;
        }
    }

    // *** get the lowest id cpu
    // get the lowest id cpu, from the emptiest cpus
    cpu_t *emptiest_and_lowest_id_cpu = (cpu_t *) priority_queue_remove_min(emptiest_cpus);

    // *** add back to cpus
    // now add all the emptiest cpus back to the cpus
    cpu_t *cpu_to_add_back;
    while(!priority_queue_is_empty(emptiest_cpus)) {

        // get each emptiest cpu
        cpu_to_add_back = (cpu_t *) priority_queue_remove(emptiest_cpus);

        // add it back to cpus, priority is total time remaining
        priority_queue_insert(cpus,
                              (data_t*) cpu_to_add_back,
                              cpu_to_add_back->total_time_remaining);
    }

    // *** free the queue used
    free_priority_queue(emptiest_cpus);

    return emptiest_and_lowest_id_cpu;

}

// destroy a cpu and free all of its associated memory
void free_cpu(cpu_t *cpu) {
    assert(cpu != NULL);

    free_priority_queue(cpu->waiting);

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

    cpu->total_time_remaining = 0;

    cpu->waiting = new_priority_queue();
    cpu->running = NULL;

    return cpu;
}