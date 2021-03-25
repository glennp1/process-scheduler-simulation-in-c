//
// Created by Glenn Phillips on 25/3/21.
//

// --- Project Libraries ---
#include "process_reader.h"

// --- Function Implementations ---

void read_in_processes(char* filename) {

    process_t curr_process;

    FILE *file = fopen(filename, "r");

    while (fscanf(file, "%d %d %d %c", &process.time_arrived,
            &process.process_id, &process.execution_time, &process.parallelisable) == 4) {

        // todo add processing
        printf("%d %d %d %c\n", process.time_arrived,
               process.process_id, process.execution_time, process.parallelisable);
    }


    fclose(file);

    return;
}
