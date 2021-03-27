//
// Created by Glenn Phillips on 23/3/21.
//

// --- System Libraries ---
#include <stdbool.h>
#include <stdio.h> // todo remove?
#include <math.h> // todo remove

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

    // todo testing ints
    // todo unsigned int is perfect for 2^32 - 1
    unsigned int test = 4294967295;
    printf("%u\n", test);

    // todo unsigned long is perfect for 2^64 - 1
    unsigned long int test_two = pow(2, 63) * 1.999999999999999;
    printf("%lu\n", test_two);


    // free simulation
    free_simulation(simulation);

    // free input data
    free_input(input);

    return 0;
}
