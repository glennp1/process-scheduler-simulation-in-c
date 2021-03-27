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

// --- Function Implementations ---
int main(int argc, char *argv[]) {

    // handle and store input arguments
    input_arguments_t input = handle_input(argc, argv);

    // todo temporary print
    printf("filename: %s\n", input.filename);
    printf("number of processors: %d\n", input.processors);
    printf("challenge: %s\n", input.challenge ? "true" : "false");

    // read in processes
    // todo save this in queue
    priority_queue_t *queuing_processes = queue_processes(input.filename);

    // todo temporary print
    // while the queue is not empty
    process_t *removed_process;


    unsigned int curr_time = 0;

    // todo remove processes matching time
    while(!priority_queue_is_empty(queuing_processes)) {

        printf("** %u **\n", curr_time);

        while (!priority_queue_is_empty(queuing_processes)) {
            removed_process = priority_queue_remove_min_if_equals(
                    queuing_processes, curr_time);

            if (removed_process != NULL) {
                printf("%u %u %u %c\n", removed_process->time_arrived,
                       removed_process->process_id, removed_process->execution_time,
                       removed_process->parallelisable ? 'p' : 'n');
                free_process(removed_process);
            }
            else {
                break;
            }
        }

        curr_time++;
    }



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

    //printf("%d", queuing_processes->head->process.time_arrived);

    // execute

    // tick = 0
    // while there are still processes to execute

        // for each in running list
            // check its current process has finished
            // if it has
                // (1) change its state to FINISHED, add it to the finished list
            // if it has not
                // (2) add it to the waiting list and change its state to WAITING
                // this ensures both current and new processes are considered together

        // while head of queue matches the tick
            // (3) dequeue the head, change its state to WAITING and add it to waiting list data structure
            // todo determine which data structure is appropriate for waiting (needs to be sorted)

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

    // END OF WHILE


    // ongoing output




    // print final results

    // free data
    free_input(input);

    return 0;
}