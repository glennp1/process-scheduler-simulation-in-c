//
// Created by Glenn Phillips on 27/3/21.
//

// --- System Libraries ---
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

// --- Project Libraries ---
#include "simulation.h"
#include "process.h"

// --- Constant Definitions ---

// --- Type Definitions ---

// --- Function Prototypes ---

// --- Function Implementations ---

// creates a new simulation based on the specified input
// then returns a pointer to the simulation
simulation_t *new_simulation(input_arguments_t input) {

    simulation_t *simulation = malloc(sizeof *simulation);
    // check the process was successfully created
    assert(simulation);

    simulation->all_processes = new_priority_queue();
    simulation->future_arrivals = new_priority_queue();
    simulation->current_arrivals = new_priority_queue();
    simulation->shortest_arrivals = new_priority_queue();

    // todo change this later to cpu
    simulation->waiting = new_priority_queue();
    simulation->shortest_waiting = new_priority_queue();
    simulation->running = new_priority_queue();
    simulation->finished = new_priority_queue();

    // for printing updates
    simulation->started_this_tick = new_priority_queue();
    simulation->finished_this_tick = new_priority_queue();

    // load processes from the file into the simulation
    load_processes(input.filename, simulation);

    return simulation;
}

// starts the specified simulation
void start_simulation(simulation_t *simulation) {
    unsigned int curr_tick = 0;
    unsigned int total_processes = simulation->all_processes->size;

    // todo update simulation ending condition
    // todo should be when the all processes are finished
    // #processes - #finished == 0

    // while the queue is not empty
    while(!priority_queue_is_empty(simulation->future_arrivals)) {
        perform_simulation_tick(simulation, curr_tick);
        curr_tick++;
    }
}

// performs one tick (second) of the specified simulation
void perform_simulation_tick(simulation_t *simulation, unsigned int curr_tick) {
    process_t *removed_process;

    printf("** tick: %u **\n", curr_tick);

    // todo could potentially use queue of queues?
    // (1)
    // for each CPU
    // if the running process has finished (time remaining = 0)
    // remove it from running
    // add it to the finished queue
    // add it to the finished this tick queue

    // todo find a better name than queuing processes
    // while the future? processes queue is not empty
    // (3) move all processes that match the current tick to the waiting queue

    while (!priority_queue_is_empty(simulation->future_arrivals)) {
        removed_process = priority_queue_remove_min_if_equals(
                simulation->future_arrivals, curr_tick);

        if (removed_process != NULL) {

            // todo add the process to the waiting queue
            // todo priority = time remaining

            // todo temporary print
            printf("%u %u %u %c\n", removed_process->time_arrived,
                   removed_process->process_id, removed_process->execution_time,
                   removed_process->parallelisable ? 'p' : 'n');
        }
        else {
            break;
        }
    }

    // get all the shortest processes in waiting queue
    // add them to express queue???

    // get all the longest processes in running
    // add them to rejects queue??? (priority = ID)


    // (4) sort the waiting list based on whatever allocator is currently in use

    // todo take best from wait list, compare it with the worst of the running processes
    // if one is better than other then swap ???
    // record / print this swap


    // for each processor in processors
    // (5) add the head of the waiting list to the running list, change state to RUNNING

    // todo ongoing output
    // (6.1) sort finished this tick, print (priority = id)

    // (6.2) sort added this tick, print (priority = id)

    // increment tick
    // for each processor
    // (7) decrease the time remaining on it's respective process
}

// destroy a simulation and free all of its associated memory
void free_simulation(simulation_t *simulation) {
    assert(simulation != NULL);

    // free all of the individual processes
    free_pq_processes(simulation->all_processes);

    // free all of the priority queues themselves
    free_priority_queue(simulation->all_processes);
    free_priority_queue(simulation->future_arrivals);
    free_priority_queue(simulation->current_arrivals);
    free_priority_queue(simulation->shortest_arrivals);

    // todo change this later to cpu
    free_priority_queue(simulation->waiting);
    free_priority_queue(simulation->shortest_waiting);
    free_priority_queue(simulation->running);
    free_priority_queue(simulation->finished);

    // for printing updates
    free_priority_queue(simulation->started_this_tick);
    free_priority_queue(simulation->finished_this_tick);

    // todo free each other part of struct
    // input ????
}
