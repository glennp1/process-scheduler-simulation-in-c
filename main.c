//
// Created by Glenn Phillips on 23/3/21.
//

// --- System Libraries ---
#include <stdio.h>

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

    return 0;
}