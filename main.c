//
// Created by Glenn Phillips on 23/3/21.
//

// --- System Libraries ---


// --- Project Libraries ---
#include "input_handler.h"

// todo implement
//#include "process.h"

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Function Prototypes ---
int main(int argc, char *argv[]);

// --- Function Implementations ---
int main(int argc, char *argv[]) {

    // initialise input
    input_arguments_t input;

    // handle input
    input = handle_input(argc, argv);

    // todo temporary output
    printf("filename: %s\n", input.filename);
    printf("number of processors: %d\n", input.processors);
    printf("own scheduler: %s\n", input.challenge ? "true" : "false");

    // ongoing output


    // print final results

    return 0;
}