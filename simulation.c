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

// --- Helper Function Prototypes ---

// prints the specified cpu
void print_cpu(cpu_t *cpu);

// prints the specified process
void print_process(process_t *process);

// --- Function Implementations ---

// creates a new simulation based on the specified input
// then returns a pointer to the simulation
simulation_t *new_simulation(input_arguments_t input) {

    simulation_t *simulation = malloc(sizeof *simulation);
    // check the simulation was successfully created
    assert(simulation);

    // simulation parameters
    // todo

    // simulation statistics
    // todo

    // for processes
    simulation->all_processes = new_priority_queue();
    simulation->future_arrivals = new_priority_queue();
    simulation->current_arrivals = new_priority_queue();
    simulation->shortest_arrivals = new_priority_queue();
    simulation->started_this_tick = new_priority_queue();
    simulation->finished_this_tick = new_priority_queue();

    // generate processes based on the file and add them to the simulation
    generate_processes(input.filename, simulation);

    // for cpus
    simulation->all_cpus = new_priority_queue();
    simulation->available_cpus = new_priority_queue();
    simulation->unavailable_cpus = new_priority_queue();
    simulation->emptiest_cpus = new_priority_queue();

    // generate the specified number of cpus and add them to the simulation
    generate_cpus(input.processors, simulation);

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


    printf("** tick: %u **\n", curr_tick);

    // todo could potentially use queue of queues?
    // (1)
    // for each CPU

    cpu_t *removed_cpu;
    while (!priority_queue_is_empty(simulation->available_cpus)) {
        removed_cpu = (cpu_t*) priority_queue_remove_min(simulation->available_cpus);
        if (removed_cpu != NULL) {

            // todo do something
            print_cpu(removed_cpu);

            // if the running process has finished (time remaining = 0)
            // remove it from running
            // add it to the finished queue
            // add it to the finished this tick queue

            priority_queue_insert(simulation->unavailable_cpus,
                                  (data_t*) removed_cpu,
                                  removed_cpu->total_time_remaining);

            printf("hi\n");
        }
        else {
            break;
        }
    }

    // todo switch unavailable_cpus and available_cpus




    // (3) move all processes that match the current tick to the waiting queue


    process_t *removed_process;

    // move all the processes in future arrivals that match the current tick
    // to the current arrivals queue
    while (!priority_queue_is_empty(simulation->future_arrivals)) {
        removed_process = (process_t*) priority_queue_remove_min_if_equals(
                simulation->future_arrivals, curr_tick);

        if (removed_process != NULL) {

            print_process(removed_process);


            priority_queue_insert(simulation->current_arrivals,
                                  (data_t*) removed_process,
                                  removed_process->time_remaining);

            // todo add the process to the waiting queue
            // todo priority = time remaining


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

    // free all of the simulation parameters
    // todo

    // free all of the simulation statistics
    // todo

    // free all of the individual processes
    free_pq_data(simulation->all_processes);

    // free all of the process priority queues
    free_priority_queue(simulation->all_processes);
    free_priority_queue(simulation->future_arrivals);
    free_priority_queue(simulation->current_arrivals);
    free_priority_queue(simulation->shortest_arrivals);
    free_priority_queue(simulation->started_this_tick);
    free_priority_queue(simulation->finished_this_tick);

    // free all of the individual cpus
    free_pq_data(simulation->all_cpus);

    // free all of the cpu priority queues
    free_priority_queue(simulation->all_cpus);
    free_priority_queue(simulation->available_cpus);
    free_priority_queue(simulation->unavailable_cpus);
    free_priority_queue(simulation->emptiest_cpus);

    // todo free each other part of struct
    // input ????
}

// prints the specified cpu
void print_cpu(cpu_t *cpu) {
    printf("cpu id: %d\n",cpu->cpu_id);
    if (cpu->running != NULL) {
        printf("running process: %u", cpu->running->process_id);
    }
}

// prints the specified process
void print_process(process_t *process) {
    printf("%u %u %u %c\n", process->time_arrived,
           process->process_id, process->execution_time,
           process->parallelisable ? 'p' : 'n');
}
