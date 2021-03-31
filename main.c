//
// Created by Glenn Phillips on 23/3/21.
//

// --- System Libraries ---

// --- Project Libraries ---
#include "input_handler.h"
#include "process.h"
#include "simulation.h"

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Function Prototypes ---

// --- Function Implementations ---
int main(int argc, char *argv[]) {

    // handle and store input arguments
    input_arguments_t input = handle_input(argc, argv);

    // create a new simulation
    simulation_t *simulation = new_simulation(input);
    start_simulation(simulation);

    // free simulation
    free_simulation(simulation);

    // free input data
    free_input(input);

    return 0;
}
