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
    unsigned int end_time; // the time the process ends, used to calculate statistics

    // (only for parallelisable processes)
    process_t *parent_process; // stores the parent process, indicating that this is a sub-process
    unsigned int sub_process_id; // [0, 2^32)
    unsigned int num_children_not_finished; // stores the number of children that haven't finished running yet
};

// --- Function Prototypes ---

// generate processes based on the file and add them to the simulation
void generate_processes(char* filename, simulation_t *simulation);

// creates the specified sub process and returns it, based on a parent process
process_t *create_subprocess(process_t *parent, unsigned int execution_time, unsigned int subprocess_id);

// removes the process with the shortest time remaining and the
// lowest id from the specified queue
process_t *remove_shortest_and_lowest_id_process(priority_queue_t *process_queue);

// destroy a process and free its memory
void free_process(process_t *process);

#endif //PROCESS_H
