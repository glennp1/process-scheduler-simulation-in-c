//
// Created by Glenn Phillips on 25/3/21.
//

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

// --- System Libraries ---
#include <stdbool.h> // for booleans

// --- Project Libraries ---


// --- Constant Definitions ---
#define DEFAULT_CHALLENGE false
#define FILENAME_FLAG "-f"
#define PROCESSORS_FLAG "-p"
#define CHALLENGE_FLAG "-c"


// --- Type Definitions ---
typedef struct {
    char* filename; // the file describing the processes
    int processors; // the number of processors [1, 1024]
    bool challenge; // true if the scheduler from the challenge activity is invoked
} input_arguments_t;

// --- Function Prototypes ---
// handles the input of the command line arguments,
// returns the input stored in a struct of type input_arguments_t
input_arguments_t handle_input(int num_arguments, char *arguments[]);

// frees the memory allocated for the input
void free_input(input_arguments_t input);

#endif //INPUT_HANDLER_H
