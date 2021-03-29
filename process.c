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

// --- Helper Function Prototypes ---

// create a new process and return a pointer to it
process_t *new_process(unsigned int time_arrived, unsigned int process_id,
                       unsigned int execution_time, char parallelisable);

// --- Function Implementations ---

// generate processes based on the file and add them to the simulation
void generate_processes(char* filename, simulation_t *simulation) {

    // for items being read in
    unsigned int time_arrived;
    unsigned int process_id;
    unsigned int execution_time;
    char parallelisable;

    // to keep track of the current process being read in from the file
    process_t *process;

    // read from the specified file
    FILE *file = fopen(filename, "r");

    // while the scanner continues to read the processes in from the file
    while (fscanf(file, "%u %u %u %c", &time_arrived, &process_id,
                  &execution_time, &parallelisable) == 4) {

        process = new_process(time_arrived, process_id, execution_time, parallelisable);

        // add the process to the all processes and future arrivals
        // with the priority matching the time arrived
        priority_queue_insert(simulation->all_processes, (data_t*) process, process->time_arrived);
        priority_queue_insert(simulation->future_arrivals, (data_t*) process, process->time_arrived);
    }


    // once done close the file
    fclose(file);
}

// destroy a process and free its memory
void free_process(process_t *process) {
    // check the process is not a null pointer
    assert(process != NULL);

    // free the process itself
    free(process);
}

// --- Helper Function Implementations ---

// create a new process and return a pointer to it
process_t *new_process(unsigned int time_arrived, unsigned int process_id,
                       unsigned int execution_time, char parallelisable) {

    process_t *process = malloc(sizeof *process);
    // check the process was successfully created
    assert(process);

    // Add each of the fields to it
    process->data.data_type = PROCESS;
    process->time_arrived = time_arrived;
    process->process_id = process_id;
    process->execution_time = execution_time;
    process->time_remaining = execution_time;
    process->parallelisable = (parallelisable == 'p') ? true : false;

    return process;
}
