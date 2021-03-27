//
// Created by Glenn Phillips on 25/3/21.
//

// --- System Libraries ---
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

// --- Project Libraries ---
#include "process.h"

// --- Function Implementations ---
// create a new process and return a pointer to it
process_t *new_process() {
    process_t *process = malloc(sizeof *process);
    // check the process was successfully created
    assert(process);

    return process;
}

// destroy a process and free its memory
void free_process(process_t *process) {
    // check the process is not a null pointer
    assert(process != NULL);

    // free the process itself
    free(process);
}

// loads processes from the specified file and stores them in
// the specified simulation
void load_processes(char* filename, simulation_t *simulation) {

    // create a new pointer to a new priority queue
    priority_queue_t *queuing_processes = new_priority_queue();

    // for items being read in
    unsigned int time_arrived;
    unsigned int process_id;
    unsigned int execution_time;
    unsigned int time_remaining;
    char parallelisable;

    // to keep track of the current process being read in from the file
    process_t *curr_process = new_process();

    // read from the specified file
    FILE *file = fopen(filename, "r");

    // while the scanner continues to read the processes in from the file
    while (fscanf(file, "%u %u %u %c", &time_arrived, &process_id,
                  &execution_time, &parallelisable) == 4) {

        // create a new process and add each of the fields to it
        curr_process = new_process();
        curr_process->time_arrived = time_arrived;
        curr_process->process_id = process_id;
        curr_process->execution_time = execution_time;
        curr_process->time_remaining = execution_time;
        curr_process->parallelisable = (parallelisable == 'p') ? true : false;

        // add the process to the all processes and future arrivals
        // with the priority matching the time arrived
        priority_queue_insert(simulation->all_processes, curr_process, curr_process->time_arrived);
        priority_queue_insert(simulation->future_arrivals, curr_process, curr_process->time_arrived);
    }

    // once done close the file
    fclose(file);
}
