//
// Created by Glenn Phillips on 25/3/21.
//

// --- Project Libraries ---
#include "input_handler.h"

// --- Function Implementations ---

// handles the input of the command line arguments,
// returns the input stored in a struct of type input_arguments_t
//
// NOTE: as we can assume there will be "no malformed input" as per the specification
// we only need to handle for the changing ORDER of arguments, rather than validate them as well
input_arguments_t handle_input(int num_arguments, char *arguments[]) {

    // initialise the default input
    input_arguments_t input;
    input.challenge = DEFAULT_CHALLENGE;

    // iterate through each of the input arguments that occur after "./allocate"
    for (int i = 1; i < num_arguments; i++) {

        // if the current argument matches the filename flag
        if (strcmp(arguments[i], FILENAME_FLAG) == 0) {
            // proceed to the following argument
            i++;
            // then store the string of this argument into the filename input
            // NOTE: dynamically assigning malloc ensures that strcpy works in ALL cases
            input.filename = malloc((strlen(arguments[i]) + 1) * sizeof(char));
            strcpy(input.filename, arguments[i]);
        }

        // if the current argument matches the processors flag
        else if (strcmp(arguments[i], PROCESSORS_FLAG) == 0) {
            // proceed to the following argument
            i++;
            // then store the numeric value of this argument within the processors input
            input.processors = atoi(arguments[i]);
        }

        // if the current argument matches the challenge flag
        else if (strcmp(arguments[i], CHALLENGE_FLAG) == 0) {
            // then set the challenge input to true
            input.challenge = true;
        }

        // if the current argument is something else
        else {
            // todo handle error
            // can print to stderr??? rather than stdout which is assessed
        }
    }
    return(input);
}

// frees the memory allocated for the input
void free_input(input_arguments_t input) {
    free(input.filename);
}