//
// Created by Glenn Phillips on 28/3/21.
//

#ifndef CPU_H
#define CPU_H

// --- System Libraries ---

// --- Project Libraries ---
#include "priority_queue.h"
#include "simulation.h"
#include "process.h"

// --- Constant Definitions ---

// --- Type Definitions ---
typedef struct priority_queue_s priority_queue_t; // forward declaration so it compiles

typedef struct simulation_s simulation_t; // forward declaration so it compiles

typedef struct process_s process_t; // forward declaration so it compiles

typedef struct cpu_s cpu_t;

struct cpu_s {
    data_t data; // so the process can be typecast as data

    // todo add other variables
    int cpu_id; // [0,1024)
    unsigned int total_time_remaining; // [0, 2^32)

    priority_queue_t *waiting;
    process_t *running;
};

// --- Function Prototypes ---

// generate the specified number of cpus and add them to the simulation
void generate_cpus(int num_cpus, simulation_t *simulation);

cpu_t *remove_emptiest_and_lowest_id_cpu(priority_queue_t *cpu_queue);

// destroy a cpu and free all of its associated memory
void free_cpu(cpu_t *cpu);

#endif //CPU_H
