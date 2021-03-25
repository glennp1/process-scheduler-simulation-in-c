//
// Created by Glenn Phillips on 23/3/21.
//

// --- System Libraries ---
#include <stdio.h> // for input / output
#include <stdlib.h> // for atoi
#include <stdbool.h> // for booleans
#include <string.h> // for strcmp

// --- Project Libraries ---
#include "hello.h"

// --- Constant Definitions ---
#define STARTING "The program is starting..."

// --- Type Definitions ---

// --- Function Prototypes ---
int main(int argc, char *argv[]);

// --- Function Implementations ---
int main(int argc, char *argv[]) {

    printf("%s\n", STARTING);

    say_hello();

    // todo move this to somewhere better
    // store the filename here
    char* filename = "blank";

    // store the processors here
    int num_processors = -1;

    // store the scheduler here
    bool own_scheduler = false;

    // parse in command line arguments
    // todo good enough for now
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) {

            // parse in processes.txt
            i++;
            // todo add guard???
            filename = argv[i];
        }
        else if (strcmp(argv[i], "-p") == 0) {
            // parse in number of processors
            i++;
            // todo add guard???
            num_processors = atoi(argv[i]);
        }
        else if (strcmp(argv[i], "-c") == 0) {
            // toggle own scheduler has been invoked
            own_scheduler = true;
        }
        else {
            // todo handle error
            // can print to stderr??? rather than stdout which is assessed
        }

    }

    printf("filename: %s\n", filename);
    printf("number of processors: %d\n", num_processors);
    printf("own scheduler: %d\n", own_scheduler);

    // ongoing output


    // print final results

    return 0;
}