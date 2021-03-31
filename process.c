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

// --- Constant Definitions ---
#define NO_CPU -1 // indicates that a process is not scheduled to any cpu
#define NOT_ENDED 0 // indicates that a process has not ended yet
#define NO_SUBPROCESS_ID 0 // indicates that there is no subprocess id
#define NO_CHILDREN 0 // indicates that there are no children subprocesses

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

// creates the specified sub process and returns it, based on a parent process
process_t *create_subprocess(process_t *parent, unsigned int execution_time, unsigned int subprocess_id) {

    process_t *subprocess = malloc(sizeof *subprocess);
    // check the subprocess was successfully created
    assert(subprocess);

    // Add each of the fields to it
    subprocess->data.data_type = PROCESS;
    subprocess->time_arrived = parent->time_arrived;
    subprocess->process_id = parent->process_id;
    subprocess->execution_time = execution_time;
    subprocess->time_remaining = execution_time;
    subprocess->parallelisable = true;
    subprocess->cpu_scheduled_on = NO_CPU;
    subprocess->end_time = NOT_ENDED;

    // important subprocess fields
    subprocess->parent_process = parent;
    subprocess->sub_process_id = subprocess_id;

    // not needed
    subprocess->num_children_not_finished = NO_CHILDREN;

    return subprocess;
}

// removes the process with the shortest time remaining and the
// lowest id from the specified queue
process_t *remove_shortest_and_lowest_id_process(priority_queue_t *processes) {

    // *** get the shortest processes
    priority_queue_t *shortest_processes = new_priority_queue();
    unsigned int shortest_process_time_remaining;

    // remove one of the shortest from the processes, save its time remaining
    process_t *shortest_process = (process_t*) priority_queue_remove_min(processes);
    shortest_process_time_remaining = shortest_process->time_remaining;

    // add it to the shortest processes, priority is process id
    priority_queue_insert(shortest_processes,
                          (data_t*) shortest_process,
                          shortest_process->process_id);

    // add any other processes that have a matching time remaining to shortest process
    while(!priority_queue_is_empty(processes)) {
        shortest_process = (process_t*) priority_queue_remove_min_if_equals(
                processes, shortest_process_time_remaining);

        if (shortest_process != NULL) {
            // add it to the shortest processes, priority is process id
            priority_queue_insert(shortest_processes,
                                  (data_t*) shortest_process,
                                  shortest_process->process_id);
        }
        else {
            break;
        }
    }

    // *** get the lowest id processes
    priority_queue_t *lowest_id_processes = new_priority_queue();
    unsigned int lowest_id_process_id;

    // remove one of the lowest ids from the shortest processes, save its id
    process_t *lowest_id_process = (process_t*) priority_queue_remove_min(shortest_processes);
    lowest_id_process_id = lowest_id_process->process_id;

    // add it to the lowest id processes, priority is sub process id
    priority_queue_insert(lowest_id_processes,
                          (data_t*) lowest_id_process,
                          lowest_id_process->sub_process_id);

    // add any other shortest processes that have a matching id to lowest id process
    while(!priority_queue_is_empty(shortest_processes)) {
        lowest_id_process = (process_t*) priority_queue_remove_min_if_equals(
                shortest_processes, lowest_id_process_id);

        if (lowest_id_process != NULL) {
            // add it to the lowest id processes, priority is sub process id
            priority_queue_insert(lowest_id_processes,
                                  (data_t*) lowest_id_process,
                                  lowest_id_process->sub_process_id);
        }
        else {
            break;
        }
    }

    // *** get the lowest sub id process
    // get the lowest sub id process, from the lowest id processes
    process_t *shortest_and_lowest_id_process = (process_t *) priority_queue_remove_min(lowest_id_processes);

    // *** add back to processes
    // now add all the shortest processes back to the processes
    process_t *process_to_add_back;
    while(!priority_queue_is_empty(shortest_processes)) {

        // get each shortest process
        process_to_add_back = (process_t*) priority_queue_remove(shortest_processes);

        // add it back to processes, priority is time remaining
        priority_queue_insert(processes,
                              (data_t*) process_to_add_back,
                              process_to_add_back->time_remaining);
    }

    // and now add all the lowest id processes back to the processes
    while(!priority_queue_is_empty(lowest_id_processes)) {

        // get each lowest id process
        process_to_add_back = (process_t*) priority_queue_remove(lowest_id_processes);

        // add it back to processes, priority is time remaining
        priority_queue_insert(processes,
                              (data_t*) process_to_add_back,
                              process_to_add_back->time_remaining);
    }

    // *** free the two queues used
    free_priority_queue(shortest_processes);
    free_priority_queue(lowest_id_processes);

    return shortest_and_lowest_id_process;
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
    process->cpu_scheduled_on = NO_CPU;
    process->end_time = NOT_ENDED;

    // Not used unless found to be parallelisable
    process->parent_process = NULL;
    process->sub_process_id = NO_SUBPROCESS_ID;
    process->num_children_not_finished = NO_CHILDREN;

    return process;
}
