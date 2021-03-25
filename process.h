//
// Created by Glenn Phillips on 25/3/21.
//

#ifndef PROCESS_H
#define PROCESS_H

// --- System Libraries ---

// --- Project Libraries ---


// --- Constant Definitions ---


// --- Type Definitions ---
// todo store each process
typedef struct {
    unsigned int time_arrived; // [0, 2^32)
    unsigned int process_id; // [0, 2^32)
    unsigned int execution_time; // [1, 2^32)
    bool parallelisable; // n (false) or p (true)
} process_t;

// --- Function Prototypes ---


#endif //PROCESS_H
