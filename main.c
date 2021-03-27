//
// Created by Glenn Phillips on 23/3/21.
//

// --- System Libraries ---
#include <stdbool.h>
#include <stdio.h> // todo remove?

// --- Project Libraries ---
#include "input_handler.h"
#include "process.h"
#include "priority_queue.h"

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Function Prototypes ---

// helper function that starts the simulation
void start_simulation(priority_queue_t *queuing_processes);

// helper function that performs one tick (second) of the simulation
void perform_one_simulation_tick(priority_queue_t *queuing_processes, unsigned int curr_tick);

// --- Function Implementations ---
int main(int argc, char *argv[]) {

    // handle and store input arguments
    input_arguments_t input = handle_input(argc, argv);

    // todo temporary print
    printf("filename: %s\n", input.filename);
    printf("number of processors: %d\n", input.processors);
    printf("challenge: %s\n", input.challenge ? "true" : "false");

    // read in processes
    priority_queue_t *queuing_processes = queue_processes(input.filename);

    // todo create struct that contains all queues??
    // todo create struct that encapsulates entire simulation???

    // todo merge this function with main or pass queues to function
    start_simulation(queuing_processes);

    // todo print final results

    // free input data
    free_input(input);

    // todo remove remaining processes
//    process_t *removed_process;
//    while(!priority_queue_is_empty(queuing_processes)) {
//
//        // remove a process and free it
//        removed_process = priority_queue_remove_min(queuing_processes);
//        free_process(removed_process);
//    }

    return 0;
}

// helper function that starts the simulation
void start_simulation(priority_queue_t *queuing_processes) {

    unsigned int curr_tick = 0;
    unsigned int total_processes = queuing_processes->size;

    // todo update simulation ending condition
    // todo should be when the all processes are finished
    // #processes - #finished == 0

    // while the queue is not empty
    while(!priority_queue_is_empty(queuing_processes)) {
        perform_one_simulation_tick(queuing_processes, curr_tick);
        curr_tick++;
    }
}

// helper function that performs one tick (second) of the simulation
void perform_one_simulation_tick(priority_queue_t *queuing_processes, unsigned int curr_tick) {
    process_t *queued_process;

    printf("** tick: %u **\n", curr_tick);

    // todo could potentially use queue of queues?
    // (1)
    // for each CPU
        // if the running process has finished (time remaining = 0)
            // remove it from running
            // add it to the finished queue
            // add it to the finished this tick queue

    // todo find a better name than queuing processes
    // while the future? processes queue is not empty
    // (3) move all processes that match the current tick to the waiting queue

    while (!priority_queue_is_empty(queuing_processes)) {
        queued_process = priority_queue_remove_min_if_equals(
                queuing_processes, curr_tick);

        if (queued_process != NULL) {

            // todo add the process to the waiting queue
            // todo priority = time remaining

            // todo temporary print
            printf("%u %u %u %c\n", queued_process->time_arrived,
                   queued_process->process_id, queued_process->execution_time,
                   queued_process->parallelisable ? 'p' : 'n');

            // todo
            // free the process - only if done
            free_process(queued_process);
        }
        else {
            break;
        }
    }

    // get all the shortest processes in waiting queue
        // add them to express queue???

    // get all the longest processes in running
        // add them to rejects queue??? (priority = ID)


    // (4) sort the waiting list based on whatever allocator is currently in use

    // todo take best from wait list, compare it with the worst of the running processes
    // if one is better than other then swap ???
    // record / print this swap


    // for each processor in processors
    // (5) add the head of the waiting list to the running list, change state to RUNNING

    // todo ongoing output
    // (6.1) sort finished this tick, print (priority = id)

    // (6.2) sort added this tick, print (priority = id)

    // increment tick
    // for each processor
        // (7) decrease the time remaining on it's respective process


}
