//
// Created by Glenn Phillips on 25/3/21.
//

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

// --- System Libraries ---
#include <stdio.h> // for input / output // todo maybe remove?
#include <stdlib.h> // for atoi
#include <stdbool.h> // for booleans
#include <string.h> // for strcmp

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
input_arguments_t handle_input(int num_arguments, char *arguments[]);

void free_input(input_arguments_t input);

#endif //INPUT_HANDLER_H
