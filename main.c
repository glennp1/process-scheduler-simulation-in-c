//
// Created by Glenn Phillips on 23/3/21.
//

// --- System Libraries ---


// --- Project Libraries ---
#include "input_handler.h"
#include "process.h"

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Function Prototypes ---

// --- Function Implementations ---
int main(int argc, char *argv[]) {

    // handle and store input arguments
    input_arguments_t input = handle_input(argc, argv);

    // todo temporary output
    printf("filename: %s\n", input.filename);
    printf("number of processors: %d\n", input.processors);
    printf("own scheduler: %s\n", input.challenge ? "true" : "false");

    // read in processes
    // todo save this in queue
    read_in_processes(input.filename);

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