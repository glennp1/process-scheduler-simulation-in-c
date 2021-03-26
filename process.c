//
// Created by Glenn Phillips on 25/3/21.
//

// --- System Libraries ---
#include <stdio.h>

// --- Project Libraries ---
#include "process.h"

// --- Function Implementations ---

// loads processes from the specified file and returns them as a priority queue
priority_queue_t *queue_processes(char* filename) {

    // create a new pointer to a new priority queue
    priority_queue_t *queuing_processes = new_priority_queue();

    // to keep track of the current process being read in from the file
    process_t curr_process;

    // to increment the priority each time,
    // effectively creating a normal queue
    int priority_count = 0;

    // read from the specified file
    FILE *file = fopen(filename, "r");

    // while the scanner continues to read the processes in from the file
    while (fscanf(file, "%d %d %d %c", &curr_process.time_arrived,
            &curr_process.process_id, &curr_process.execution_time, &curr_process.parallelisable) == 4) {

        // add the process to the queuing process with the priority increasing each time
        priority_queue_insert(queuing_processes, curr_process, priority_count++);
    }

    // once done close the file
    fclose(file);

    return queuing_processes;
}
