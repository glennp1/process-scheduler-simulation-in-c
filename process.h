//
// Created by Glenn Phillips on 25/3/21.
//

#ifndef PROCESS_H
#define PROCESS_H

// --- System Libraries ---

// --- Project Libraries ---
#include "priority_queue.h"
#include "simulation.h"

// --- Constant Definitions ---
#define NO_CPU -1; // indicates that a process is not scheduled to any cpu

// --- Type Definitions ---
typedef struct simulation_s simulation_t; // forward declaration so it compiles

typedef struct process_s process_t;

struct process_s {
    data_t data; // so the process can be typecast as data
    unsigned int time_arrived; // [0, 2^32)
    unsigned int process_id; // [0, 2^32)
    unsigned int execution_time; // [1, 2^32)
    unsigned int time_remaining; // [0, 2^32)
    bool parallelisable; // true if parallelisable, false if not
    int cpu_scheduled_on; // -1 if none, [0,1024) otherwise
};

// --- Function Prototypes ---

// generate processes based on the file and add them to the simulation
void generate_processes(char* filename, simulation_t *simulation);

// destroy a process and free its memory
void free_process(process_t *process);

#endif //PROCESS_H
