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
#include "simulation.h"

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

    // create a new simulation
    simulation_t *simulation = new_simulation(input);



    // todo merge this function with main or pass queues to function
    start_simulation(simulation);

    // todo print final results



    // todo free simulation
    free_simulation(simulation);

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
