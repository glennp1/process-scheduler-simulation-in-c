//
// Created by Glenn Phillips on 25/3/21.
//

#ifndef PROCESS_READER_H
#define PROCESS_READER_H

// --- System Libraries ---
#include <stdlib.h>
#include <stdio.h> // todo maybe remove

// --- Project Libraries ---


// --- Constant Definitions ---


// --- Type Definitions ---
// todo store each process
typedef struct {
    unsigned int time_arrived; // [0, 2^32)
    unsigned int process_id; // [0, 2^32)
    unsigned int execution_time; // [1, 2^32)
    char parallelisable; // n (not parallelisable) or p (parallelisable)
} process_t;

// --- Function Prototypes ---
void read_in_processes(char* filename);

#endif //PROCESS_READER_H
