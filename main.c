//
// Created by Glenn Phillips on 23/3/21.
//

// --- System Libraries ---


// --- Project Libraries ---
#include "input_handler.h"
#include "process_reader.h"

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Function Prototypes ---

// --- Function Implementations ---
int main(int argc, char *argv[]) {

    // initialise input
    input_arguments_t input = handle_input(argc, argv);

    //
    processes_list


    // todo temporary output
    printf("filename: %s\n", input.filename);
    printf("number of processors: %d\n", input.processors);
    printf("own scheduler: %s\n", input.challenge ? "true" : "false");

    read_in_processes(input.filename);

    // ongoing output




    // print final results

    // free data
    free_input(input);

    return 0;
}