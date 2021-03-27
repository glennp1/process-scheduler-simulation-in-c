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
    process_t *removed_process;

    printf("** tick: %u **\n", curr_tick);

//
//
//    // todo remove remaining processes
//    while(!priority_queue_is_empty(queuing_processes)) {
//
//        // remove a process and print it
//        removed_process = priority_queue_remove_min(queuing_processes);
//
//        printf("%u %u %u %c\n", removed_process->time_arrived,
//               removed_process->process_id, removed_process->execution_time,
//               removed_process->parallelisable ? 'p' : 'n');
//        free_process(removed_process);
//    }

    // for each in running list
    // check its current process has finished
    // if it has
    // (1) change its state to FINISHED, add it to the finished list
    // if it has not
    // (2) add it to the waiting list and change its state to WAITING
    // this ensures both current and new processes are considered together

    // while head of queue matches the tick
    // (3) dequeue the head, change its state to WAITING and add it to waiting list data structure

    while (!priority_queue_is_empty(queuing_processes)) {
        removed_process = priority_queue_remove_min_if_equals(
                queuing_processes, curr_tick);

        if (removed_process != NULL) {

            // todo do something...

            // todo temporary print
            printf("%u %u %u %c\n", removed_process->time_arrived,
                   removed_process->process_id, removed_process->execution_time,
                   removed_process->parallelisable ? 'p' : 'n');

            // todo
            // free the process - only if done
            free_process(removed_process);
        }
        else {
            break;
        }
    }



    // (4) sort the waiting list based on whatever allocator is currently in use

    // todo take best from wait list, compare it with the worst of the running processes
    // if one is better than other then swap ???
    // record / print this swap


    // for each processor in processors
    // (5) add the head of the waiting list to the running list, change state to RUNNING

    // (6) sort finished, print
    // ()

    // increment tick
    // for each processor
    // (7) decrease the time remaining on it's respective process

    // todo ongoing output


    // END OF WHILE


}
