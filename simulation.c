//
// Created by Glenn Phillips on 27/3/21.
//

// --- System Libraries ---
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

// --- Project Libraries ---
#include "simulation.h"
#include "process.h"

// --- Constant Definitions ---
#define NO_TIME_LEFT 0 // when time is done
#define STARTING_TICK 0
#define STARTING_PROC_REMAINING 0
#define ONE_TICK 1
#define DECIMAL_CONVERSION 100
#define NORMAL_ROUNDING_AMOUNT 0.5
#define ROUNDING_UP_AMOUNT 1

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// performs one tick (second) of the specified simulation
void perform_simulation_tick(simulation_t *simulation);

// moves all processes that match the current tick to the current arrivals
void add_current_arrivals(simulation_t *simulation);

// attempts to allocate all of the current arrivals to cpus
void allocate_processes_to_cpu(simulation_t *simulation);

cpu_t *remove_emptiest_cpu(simulation_t *simulation);

process_t *remove_shortest_current_arrival(simulation_t *simulation);

void update_all_cpus(simulation_t *simulation);

process_t *remove_shortest_waiting(cpu_t *cpu);

// returns if the shortest waiting process should be swapped with the running process
bool should_swap_running_process(process_t *shortest_waiting, process_t *running_process);

void display_execution_transcript(simulation_t *simulation);

void increment_simulation_tick(simulation_t *simulation);

void display_performance_statistics(simulation_t *simulation);

// rounds the specified number to two decimal places
double round_to_two_places(double number);

// rounds up the specified number to the nearest whole number
double round_up(double number);

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
    simulation->curr_tick = STARTING_TICK;
    simulation->proc_remaining = STARTING_PROC_REMAINING;

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

    // the total number of processes in the simulation
    unsigned int total_processes = simulation->all_processes->size;

    // todo update simulation ending condition
    // todo should be when the all processes are finished
    // #processes - #finished == 0

    // todo update for parallelisable
    // while the the number of finished processes does not equal the total number
    while (simulation->finished->size != total_processes) {

        perform_simulation_tick(simulation);
        increment_simulation_tick(simulation);
    }

    // once done calculate and display the performance statistics
    display_performance_statistics(simulation);
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
void perform_simulation_tick(simulation_t *simulation) {

    add_current_arrivals(simulation);

    // (4) allocate processes to cpus based on whatever allocator is currently in use
    allocate_processes_to_cpu(simulation);

    // todo no available processes once here -- fix
    update_all_cpus(simulation);

    // todo ongoing output
    display_execution_transcript(simulation);
}

// moves all processes that match the current tick to the current arrivals
void add_current_arrivals(simulation_t *simulation) {

    process_t *current_arrival;

    // move all the processes in future arrivals that match the current tick
    // to the current arrivals queue
    while (!priority_queue_is_empty(simulation->future_arrivals)) {

        // todo fix this
        // store the process with the lowest time arrived if it equals the current tick
        current_arrival = (process_t*) priority_queue_remove_min_if_equals(
                simulation->future_arrivals, simulation->curr_tick);

        // if a process was found
        if (current_arrival != NULL) {

            // add it to the current arrivals queue
            priority_queue_insert(simulation->current_arrivals,
                                  (data_t*) current_arrival,
                                  current_arrival->time_remaining);

            // increment the number of processes remaining
            simulation->proc_remaining++;
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

        process_t *shortest_current_arrival = remove_shortest_current_arrival(simulation);

        // add shortest current arrival to the waiting queue of the emptiest cpu
        // priority is time remaining
        priority_queue_insert(emptiest_cpu->waiting,
                              (data_t*) shortest_current_arrival,
                              shortest_current_arrival->time_remaining);

        // increment the total time remaining on the cpu
        emptiest_cpu->total_time_remaining += shortest_current_arrival->time_remaining;

        // have the process record the cpu it is scheduled on
        shortest_current_arrival->cpu_scheduled_on = emptiest_cpu->cpu_id;

        // add the emptiest cpu back to the available cpus
        // priority is the total time remaining
        priority_queue_insert(simulation->available_cpus,
                              (data_t*) emptiest_cpu,
                              emptiest_cpu->total_time_remaining);
    }
}

// todo refactor? - make sub function "remove_smallest_by_id" ???
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

// todo refactor? - make sub function "remove_smallest_by_id" ???
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
        process_to_add_back = (process_t*) priority_queue_remove(simulation->shortest_current_arrivals);

        // add it back to current arrivals, priority is time remaining
        priority_queue_insert(simulation->current_arrivals,
                              (data_t*) process_to_add_back,
                              process_to_add_back->time_remaining);
    }

    return shortest_current_arrival;
}

void update_all_cpus(simulation_t *simulation) {

    // while the available cpus queue is not empty
    cpu_t *cpu;

    while (!priority_queue_is_empty(simulation->available_cpus)) {

        // remove the first cpu from the available cpus and store it's running process
        cpu = (cpu_t*) priority_queue_remove(simulation->available_cpus);

        // todo check for finished processes
        // if the cpu has an active running process
        if (cpu->running != NULL) {

            // and if the running process of that cpu has finished (time remaining = 0)
            if (cpu->running->time_remaining == NO_TIME_LEFT) {

                // add it to the finished this tick queue
                priority_queue_insert(simulation->finished_this_tick,
                                      (data_t*) cpu->running,
                                      cpu->running->time_remaining);

                // add it to the finished queue
                priority_queue_insert(simulation->finished,
                                      (data_t*) cpu->running,
                                      cpu->running->time_remaining);

                // decrease the number of processes remaining
                simulation->proc_remaining--;

                // set the end time for the process
                cpu->running->end_time = simulation->curr_tick;

                // store that there is no running process
                // todo maybe change this????
                cpu->running = NULL;
            }
        }

        // todo schedule running processes
        // if the waiting queue is not empty
        if (!priority_queue_is_empty(cpu->waiting)) {

            // remove the shortest waiting process
            process_t *shortest_waiting = remove_shortest_waiting(cpu);

            // now if the cpu now has no running process
            if (cpu->running == NULL) {

                // we can add the shortest waiting process in the queue to the cpu
                cpu->running = shortest_waiting;

                // add it to started this tick
                priority_queue_insert(simulation->started_this_tick,
                                      (data_t*) cpu->running,
                                      cpu->running->time_remaining);
            }
                // otherwise if the cpu has a running process already
            else {

                // if the running process should be swapped
                if (should_swap_running_process(shortest_waiting, cpu->running)) {
                    // add the running process back to the waiting queue
                    priority_queue_insert(cpu->waiting,
                                          (data_t *) cpu->running,
                                          cpu->running->time_remaining);

//                    // todo remove
//                    printf("%u,PAUSED,pid=%u,remaining_time=%u\n",
//                           simulation->curr_tick, cpu->running->process_id, cpu->running->time_remaining);

                    // make the shortest waiting process the running process
                    cpu->running = shortest_waiting;

                    // add the shortest waiting process to started this tick
                    priority_queue_insert(simulation->started_this_tick,
                                          (data_t*) cpu->running,
                                          cpu->running->time_remaining);
                }
                    // otherwise
                else {
                    // place the shortest waiting process back in the waiting queue
                    priority_queue_insert(cpu->waiting,
                                          (data_t*) shortest_waiting,
                                          shortest_waiting->time_remaining);
                }
            }
        }

        // once done add the cpu itself to unavailable
        priority_queue_insert(simulation->unavailable_cpus,
                              (data_t*) cpu,
                              cpu->total_time_remaining);
    }

    // make the unavailable cpus available again
    swap_priority_queues(simulation->available_cpus, simulation->unavailable_cpus);
}

// todo refactor? - make sub function "remove_smallest_by_id" ???
process_t *remove_shortest_waiting(cpu_t *cpu) {

    // remove one of the shortest waiting processes
    process_t *shortest_waiting;
    unsigned int shortest_waiting_time_remaining;
    shortest_waiting = (process_t*) priority_queue_remove_min(cpu->waiting);
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

void display_execution_transcript(simulation_t *simulation) {
    process_t *process_to_display;

    // display finished this tick
    while (!priority_queue_is_empty(simulation->finished_this_tick)) {
        process_to_display = (process_t*) priority_queue_remove_min(simulation->finished_this_tick);

        printf("%u,FINISHED,pid=%u,proc_remaining=%u\n",
               simulation->curr_tick, process_to_display->process_id, simulation->proc_remaining);
    }

    // display started this tick
    while (!priority_queue_is_empty(simulation->started_this_tick)) {
        process_to_display = (process_t*) priority_queue_remove_min(simulation->started_this_tick);

        printf("%u,RUNNING,pid=%u,remaining_time=%u,cpu=%d\n",
               simulation->curr_tick, process_to_display->process_id, process_to_display->time_remaining,
               process_to_display->cpu_scheduled_on);
    }
}

void increment_simulation_tick(simulation_t *simulation) {

    // while the available cpus queue is not empty
    cpu_t *cpu;

    while (!priority_queue_is_empty(simulation->available_cpus)) {

        // remove the first cpu from the available cpus and store it's running process
        cpu = (cpu_t*) priority_queue_remove(simulation->available_cpus);

        // if there is a running process
        if (cpu->running != NULL) {

            // decrease the time remaining on it
            cpu->running->time_remaining--;

            // decrease the total time remaining on the cpu
            cpu->total_time_remaining--;
        }

        // once done add the cpu itself to unavailable
        priority_queue_insert(simulation->unavailable_cpus,
                              (data_t*) cpu,
                              cpu->total_time_remaining);
    }

    // make the unavailable cpus available again
    swap_priority_queues(simulation->available_cpus, simulation->unavailable_cpus);

    // increment the simulation tick
    simulation->curr_tick++;

}

void display_performance_statistics(simulation_t *simulation) {

    // calculate the performance statistics
    double average_turnaround_time;
    double total_turnaround_time = 0;
    double max_time_overhead = 0;
    double average_time_overhead;
    double total_time_overhead = 0;
    double process_turnaround_time;
    double process_time_overhead;
    double num_processes = simulation->all_processes->size;
    bool first_iteration = true;

    // for each finished process
    process_t *process;
    while (!priority_queue_is_empty(simulation->finished)) {
        process = (process_t*) priority_queue_remove(simulation->finished);

        // todo remove
//        printf("for process = %u, ", process->process_id);

        // calculate the turnaround time and add it to the total
        process_turnaround_time = process->end_time - process->time_arrived;
        total_turnaround_time += process_turnaround_time;

        // todo remove
//        printf("turnaround time = %lf, ", process_turnaround_time);

        // calculate the time overhead rounded to two decimal places and add it to the total
        process_time_overhead = round_to_two_places(
                process_turnaround_time / process->execution_time);
        total_time_overhead += process_time_overhead;

        // todo remove
//        printf("time overhead = %lf\n", process_time_overhead);

        // update the maximum time overhead
        if (first_iteration || process_time_overhead > max_time_overhead) {
            max_time_overhead = process_time_overhead;
        }
        first_iteration = false;
    }

    // calculate the average turnaround time, rounded up to the nearest integer
    average_turnaround_time = round_up(
            total_turnaround_time / num_processes);
    // round the max time overhead to two decimal places
    max_time_overhead = round_to_two_places(max_time_overhead);
    // calculate the average time overhead, rounding to two decimal places
    average_time_overhead = round_to_two_places(
            total_time_overhead / num_processes);

    // to calculate the makespan, subtract one from the current tick
    // because the simulation ends the tick after the last process finishes
    unsigned int makespan = simulation->curr_tick - ONE_TICK;

    // display the performance statistics
    printf("Turnaround time %lf\n", average_turnaround_time);
    printf("Time overhead %lf %lf\n", max_time_overhead, average_time_overhead);
    printf("Makespan %u\n", makespan);
}

// rounds the specified number to two decimal places
double round_to_two_places(double number) {

    // multiple the number by 100 and round it to the nearest int by typecasting
    unsigned int number_multiplied_by_conversion = (unsigned int) (number * DECIMAL_CONVERSION + NORMAL_ROUNDING_AMOUNT);

    // convert the number back to a double and return it
    double number_divided_by_conversion = (double) number_multiplied_by_conversion / DECIMAL_CONVERSION;
    return number_divided_by_conversion;
}

// rounds up the specified number to the nearest whole number
double round_up(double number) {
    // round it up to the nearest int by typecasting
    unsigned int number_rounded_up = (unsigned int) (number + ROUNDING_UP_AMOUNT);

    // convert back to a double and return
    return (double) number_rounded_up;
}

// todo remove
// prints the specified cpu
void print_cpu(cpu_t *cpu) {
    printf("cpu id: %d\n",cpu->cpu_id);
    if (cpu->running != NULL) {
        printf("running process: %u", cpu->running->process_id);
    }
}

// todo remove
// prints the specified process
void print_process(process_t *process) {
    printf("%u %u %u %c", process->time_arrived,
           process->process_id, process->execution_time,
           process->parallelisable ? 'p' : 'n');
    printf(" (%u)\n", process->time_remaining);
}
