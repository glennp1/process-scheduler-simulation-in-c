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
#define NO_TIME_LEFT 0 // when time is done


// --- Type Definitions ---

// --- Helper Function Prototypes ---

// performs one tick (second) of the specified simulation
void perform_simulation_tick(simulation_t *simulation, unsigned int curr_tick);

// updates all running cpu processes that have finished
void update_finished_processes(simulation_t *simulation);

// moves all processes that match the current tick to the current arrivals
void add_current_arrivals(simulation_t *simulation, unsigned int curr_tick);

// attempts to allocate all of the current arrivals to cpus
void allocate_processes_to_cpu(simulation_t *simulation);

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
    simulation->finished = new_priority_queue();

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

    // todo currently unused
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
    free_priority_queue(simulation->finished);

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

// *** Helper Function Implementations ***

// performs one tick (second) of the specified simulation
void perform_simulation_tick(simulation_t *simulation, unsigned int curr_tick) {


    printf("** tick: %u **\n", curr_tick);


    update_finished_processes(simulation);

    add_current_arrivals(simulation, curr_tick);




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

// updates all running cpu processes that have finished
void update_finished_processes(simulation_t *simulation) {

    // while the available cpus queue is not empty
    cpu_t *removed_cpu;
    while (!priority_queue_is_empty(simulation->available_cpus)) {

        // remove the cpu with the lowest time remaining
        removed_cpu = (cpu_t*) priority_queue_remove_min(simulation->available_cpus);

        // if a cpu was removed
        if (removed_cpu != NULL) {

            // If the cpu has a running process
            if (removed_cpu->running != NULL) {

                // if the running process of that cpu has finished (time remaining = 0)
                if (removed_cpu->running->time_remaining == NO_TIME_LEFT) {

                    // add it to the finished this tick queue
                    priority_queue_insert(simulation->finished_this_tick,
                                          (data_t*) removed_cpu,
                                          removed_cpu->total_time_remaining);

                    // add it to the finished queue
                    priority_queue_insert(simulation->finished,
                                          (data_t*) removed_cpu,
                                          removed_cpu->total_time_remaining);
                }
            }
            // todo temporary print
            print_cpu(removed_cpu);

            // once done add the cpu itself to unavailable
            priority_queue_insert(simulation->unavailable_cpus,
                                  (data_t*) removed_cpu,
                                  removed_cpu->total_time_remaining);
        }
        else {
            break;
        }
    }

    // make the unavailable cpus available again
    swap_priority_queues(simulation->available_cpus, simulation->unavailable_cpus);
}

// moves all processes that match the current tick to the current arrivals
void add_current_arrivals(simulation_t *simulation, unsigned int curr_tick) {

    process_t *removed_process;

    // move all the processes in future arrivals that match the current tick
    // to the current arrivals queue
    while (!priority_queue_is_empty(simulation->future_arrivals)) {

        // remove the process with the lowest time arrived if it equals the current tick
        removed_process = (process_t*) priority_queue_remove_min_if_equals(
                simulation->future_arrivals, curr_tick);

        // if a process was removed
        if (removed_process != NULL) {

            // add it to the current arrivals queue
            priority_queue_insert(simulation->current_arrivals,
                                  (data_t*) removed_process,
                                  removed_process->time_remaining);
        }
        else {
            break;
        }
    }
}

// attempts to allocate all of the current arrivals to cpus
void allocate_processes_to_cpu(simulation_t *simulation) {

    // continue until there are either no processes or no cpus left
    while (!priority_queue_is_empty(simulation->future_arrivals) ||
            !priority_queue_is_empty(simulation->available_cpus)) {



    }




    // get all the shortest processes in waiting queue
// add them to express queue???

// get all the longest processes in running
// add them to rejects queue??? (priority = ID)

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
