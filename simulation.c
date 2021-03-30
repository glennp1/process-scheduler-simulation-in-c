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

// moves all processes that match the current tick to the current arrivals
void add_current_arrivals(simulation_t *simulation, unsigned int curr_tick);

// attempts to allocate all of the current arrivals to cpus
void allocate_processes_to_cpu(simulation_t *simulation);

cpu_t *remove_emptiest_cpu(simulation_t *simulation);

process_t *remove_shortest_current_arrival(simulation_t *simulation);

void update_all_cpus(simulation_t *simulation, unsigned int curr_tick);

process_t *remove_shortest_waiting(cpu_t *cpu);

// returns if the shortest waiting process should be swapped with the running process
bool should_swap_running_process(process_t *shortest_waiting, process_t *running_process);

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
    simulation->shortest_current_arrivals = new_priority_queue();
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
    while(curr_tick < 150) {
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
    free_priority_queue(simulation->shortest_current_arrivals);
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

    add_current_arrivals(simulation, curr_tick);

    // (4) allocate processes to cpus based on whatever allocator is currently in use
    allocate_processes_to_cpu(simulation);

    // todo no available processes once here -- fix
    update_all_cpus(simulation, curr_tick);

    // todo ongoing output
    // (6.1) sort finished this tick, print (priority = id)

    // (6.2) sort added this tick, print (priority = id)
}

// moves all processes that match the current tick to the current arrivals
void add_current_arrivals(simulation_t *simulation, unsigned int curr_tick) {

    process_t *current_arrival;

    // move all the processes in future arrivals that match the current tick
    // to the current arrivals queue
    while (!priority_queue_is_empty(simulation->future_arrivals)) {

        // add the process with the lowest time arrived if it equals the current tick
        current_arrival = (process_t*) priority_queue_remove_min_if_equals(
                simulation->future_arrivals, curr_tick);

        // if a process was found
        if (current_arrival != NULL) {

            // add it to the current arrivals queue
            priority_queue_insert(simulation->current_arrivals,
                                  (data_t*) current_arrival,
                                  current_arrival->time_remaining);

            // todo remove
            printf("added: ");
            print_process(current_arrival);
        }
        else {
            break;
        }
    }
}

// attempts to allocate all of the current arrivals to cpus
void allocate_processes_to_cpu(simulation_t *simulation) {

    // continue while neither the current arrivals nor available cpus queues are empty
    while ((!priority_queue_is_empty(simulation->current_arrivals)) &&
            (!priority_queue_is_empty(simulation->available_cpus))) {

        cpu_t *emptiest_cpu = remove_emptiest_cpu(simulation);

        // todo remove
        printf("removed: ");
        print_cpu(emptiest_cpu);

        process_t *shortest_current_arrival = remove_shortest_current_arrival(simulation);

        // todo remove
        printf("removed: ");
        print_process(shortest_current_arrival);

        // add shortest current arrival to the waiting queue of the emptiest cpu
        // priority is time remaining
        priority_queue_insert(emptiest_cpu->waiting,
                              (data_t*) shortest_current_arrival,
                              shortest_current_arrival->time_remaining);
    }
}

cpu_t *remove_emptiest_cpu(simulation_t *simulation) {

    // remove one of the emptiest cpus
    cpu_t *emptiest_cpu;
    unsigned int emptiest_cpu_total_time_remaining;
    emptiest_cpu = (cpu_t*) priority_queue_remove_min(simulation->available_cpus);
    emptiest_cpu_total_time_remaining = emptiest_cpu->total_time_remaining;

    // add it to the emptiest cpus, priority is cpu id
    priority_queue_insert(simulation->emptiest_cpus,
                          (data_t*) emptiest_cpu,
                          emptiest_cpu->cpu_id);

    // add any other cpus that have a matching total time remaining to emptiest cpus
    while(!priority_queue_is_empty(simulation->available_cpus)) {
        emptiest_cpu = (cpu_t*) priority_queue_remove_min_if_equals(
                simulation->available_cpus, emptiest_cpu_total_time_remaining);

        if (emptiest_cpu != NULL) {
            // add it to the emptiest cpus, priority is cpu id
            priority_queue_insert(simulation->emptiest_cpus,
                                  (data_t*) emptiest_cpu,
                                  emptiest_cpu->cpu_id);
        }
        else {
            break;
        }
    }

    // update the emptiest cpu to be the emptiest cpu with the lowest cpu id
    emptiest_cpu = (cpu_t*) priority_queue_remove_min(simulation->emptiest_cpus);

    // now add all the emptiest cpus back to available cpus
    cpu_t *cpu_to_add_back;
    while(!priority_queue_is_empty(simulation->emptiest_cpus)) {

        // get each emptiest cpu
        cpu_to_add_back = (cpu_t*) priority_queue_remove_min(simulation->emptiest_cpus);

        // add it back to available cpus, priority is total time remaining
        priority_queue_insert(simulation->available_cpus,
                              (data_t*) cpu_to_add_back,
                              cpu_to_add_back->total_time_remaining);
    }

    return emptiest_cpu;
}

process_t *remove_shortest_current_arrival(simulation_t *simulation) {


    // remove one of the shortest current arrivals
    process_t *shortest_current_arrival;
    unsigned int shortest_current_arrival_time_remaining;
    shortest_current_arrival = (process_t*) priority_queue_remove_min(simulation->current_arrivals);
    shortest_current_arrival_time_remaining = shortest_current_arrival->time_remaining;

    // add it to the shortest current arrivals, priority is process id
    priority_queue_insert(simulation->shortest_current_arrivals,
                          (data_t*) shortest_current_arrival,
                          shortest_current_arrival->process_id);

    // add any other processes that have a matching time remaining to shortest currents arrivals
    while(!priority_queue_is_empty(simulation->current_arrivals)) {
        shortest_current_arrival = (process_t*) priority_queue_remove_min_if_equals(
                simulation->current_arrivals, shortest_current_arrival_time_remaining);

        if (shortest_current_arrival != NULL) {
            // add it to the shortest current arrivals, priority is process id
            priority_queue_insert(simulation->shortest_current_arrivals,
                                  (data_t*) shortest_current_arrival,
                                  shortest_current_arrival->process_id);
        }
        else {
            break;
        }
    }

    // update the shortest current arrival to be the shortest current arrival with the lowest process id
    shortest_current_arrival = (process_t *) priority_queue_remove_min(simulation->shortest_current_arrivals);

    // now add all the shortest current arrivals back to current arrivals
    process_t *process_to_add_back;
    while(!priority_queue_is_empty(simulation->shortest_current_arrivals)) {

        // get each shortest current arrival
        process_to_add_back = (process_t*) priority_queue_remove_min(simulation->shortest_current_arrivals);

        // add it back to current arrivals, priority is time remaining
        priority_queue_insert(simulation->shortest_current_arrivals,
                              (data_t*) process_to_add_back,
                              process_to_add_back->time_remaining);
    }

    return shortest_current_arrival;
}

void update_all_cpus(simulation_t *simulation, unsigned int curr_tick) {

    // while the available cpus queue is not empty
    cpu_t *cpu;
    process_t *running_process;

    while (!priority_queue_is_empty(simulation->available_cpus)) {

        // remove the first cpu from the available cpus and store it's running process
        cpu = (cpu_t*) priority_queue_remove(simulation->available_cpus);
        running_process = cpu->running;

        // todo remove
        printf("handling cpu: ");
        print_cpu(cpu);

        // todo update finished processes
        // if the cpu has an active running process
        if (running_process != NULL) {

            // and if the running process of that cpu has finished (time remaining = 0)
            if (running_process->time_remaining == NO_TIME_LEFT) {

                // add it to the finished this tick queue
                priority_queue_insert(simulation->finished_this_tick,
                                      (data_t*) running_process,
                                      running_process->time_remaining);

                // add it to the finished queue
                priority_queue_insert(simulation->finished,
                                      (data_t*) running_process,
                                      running_process->time_remaining);

                // store that there is no running process
                // todo maybe change this????
                running_process = NULL;

                // todo remove
                printf("tick: %u process finished: ", curr_tick);
                process_t  *process = (process_t*) priority_queue_remove(simulation->finished_this_tick);
                print_process(process);
            }
        }

        // todo schedule running processes
        // todo not sure if you just take first in waiting queue or the shortest waiting (locally)
        // todo DO THIS^^^
        process_t *shortest_waiting = remove_shortest_waiting(cpu);

        // now if the cpu now has no running process
        if (running_process == NULL) {

            // we can add the first process waiting in the queue to the cpu
            cpu->running = shortest_waiting;

            // add register that it has been added
            // add it to the added this tick queue
            priority_queue_insert(simulation->finished_this_tick,
                                  (data_t*) running_process,
                                  running_process->time_remaining);
        }
        // otherwise if the cpu has a running process already
        else if (running_process->time_remaining) {

            // if the running process should be swapped
            if (should_swap_running_process(shortest_waiting, running_process)) {
                // place the running process back to the waiting queue
                priority_queue_insert(cpu->waiting,
                                      (data_t *) running_process,
                                      running_process->time_remaining);

                // make the shortest waiting process the running process
                running_process = shortest_waiting;
            }
            // otherwise
            else {
                // place the shortest waiting process back in the waiting queue
                priority_queue_insert(cpu->waiting,
                                      (data_t*) shortest_waiting,
                                      shortest_waiting->time_remaining);
            }
        }

        // todo increment????
        // now if the cpu has a running process
        if (running_process != NULL) {

            // we can decrease the time remaining on the running process by 1
            running_process->time_remaining--;
        }

        // once done add the cpu itself to unavailable
        priority_queue_insert(simulation->unavailable_cpus,
                              (data_t*) cpu,
                              cpu->total_time_remaining);
    }

    // make the unavailable cpus available again
    swap_priority_queues(simulation->available_cpus, simulation->unavailable_cpus);
}


process_t *remove_shortest_waiting(cpu_t *cpu) {

    // remove one of the shortest waiting processes
    process_t *shortest_waiting;
    unsigned int shortest_waiting_time_remaining;
    shortest_waiting = (process_t*) priority_queue_remove(cpu->waiting);
    shortest_waiting_time_remaining = shortest_waiting->time_remaining;

    // add it to the shortest waiting, priority is process id
    priority_queue_insert(cpu->shortest_waiting,
                          (data_t*) shortest_waiting,
                          shortest_waiting->process_id);

    // add any other processes that have a matching time remaining to shortest waiting
    while(!priority_queue_is_empty(cpu->waiting)) {
        shortest_waiting = (process_t*) priority_queue_remove_min_if_equals(
                cpu->waiting, shortest_waiting_time_remaining);

        if (shortest_waiting != NULL) {
            // add it to the shortest waiting, priority is process id
            priority_queue_insert(cpu->shortest_waiting,
                                  (data_t*) shortest_waiting,
                                  shortest_waiting->process_id);
        }
        else {
            break;
        }
    }

    // update the shortest waiting to be the shortest waiting with the lowest process id
    shortest_waiting = (process_t *) priority_queue_remove_min(cpu->shortest_waiting);

    // now add all the shortest waiting back to waiting
    process_t *process_to_add_back;
    while(!priority_queue_is_empty(cpu->shortest_waiting)) {

        // get each shortest waiting
        process_to_add_back = (process_t*) priority_queue_remove_min(cpu->shortest_waiting);

        // add it back to waiting, priority is time remaining
        priority_queue_insert(cpu->shortest_waiting,
                              (data_t*) process_to_add_back,
                              process_to_add_back->time_remaining);
    }

    return shortest_waiting;
}

// returns if the shortest waiting process should be swapped with the running process
bool should_swap_running_process(process_t *shortest_waiting, process_t *running_process) {
    // we compare its time remaining with the shortest waiting
    if (shortest_waiting->time_remaining < running_process->time_remaining) {
        return true;
    }
        // in the case of ties use process id to resolve
    else if (shortest_waiting->time_remaining == running_process->time_remaining &&
             shortest_waiting->process_id < running_process->process_id) {
        return true;
    }
    // Otherwise it is false
    return false;
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
    printf("%u %u %u %c", process->time_arrived,
           process->process_id, process->execution_time,
           process->parallelisable ? 'p' : 'n');
    printf(" (%u)\n", process->time_remaining);
}
