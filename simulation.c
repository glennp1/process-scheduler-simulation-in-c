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
#define CHILDREN_DONE 0
#define ONE_TICK 1
#define SYNC_COST 1
#define DECIMAL_CONVERSION 100
#define NORMAL_ROUNDING_AMOUNT 0.5
#define ROUNDING_UP_AMOUNT 1

// --- Type Definitions ---

// --- Helper Function Prototypes ---

// performs one tick (second) of the specified simulation
void perform_simulation_tick(simulation_t *simulation);

// moves all processes that match the current tick to the current arrivals
void add_current_arrivals(simulation_t *simulation);

void generate_and_add_subprocesses(simulation_t *simulation, process_t *parent);

// attempts to allocate all of the current arrivals to cpus
void allocate_processes_to_cpu(simulation_t *simulation);

void update_all_cpus(simulation_t *simulation);

void tell_parent_child_finished(simulation_t *simulation, process_t *child);

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
    simulation->started_this_tick = new_priority_queue();
    simulation->finished_this_tick = new_priority_queue();
    simulation->finished = new_priority_queue();

    // generate processes based on the file and add them to the simulation
    generate_processes(input.filename, simulation);

    // for cpus
    simulation->all_cpus = new_priority_queue();
    simulation->available_cpus = new_priority_queue();
    simulation->unavailable_cpus = new_priority_queue();

    // generate the specified number of cpus and add them to the simulation
    generate_cpus(input.processors, simulation);

    return simulation;
}

// starts the specified simulation
void start_simulation(simulation_t *simulation) {

    // todo update simulation ending condition
    // todo should be when the all processes are finished
    // #processes - #finished == 0

    // todo update for parallelisable
    // while the the number of finished processes does not equal the total number
    while (simulation->finished->size != simulation->all_processes->size) {

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
    free_priority_queue(simulation->started_this_tick);
    free_priority_queue(simulation->finished_this_tick);
    free_priority_queue(simulation->finished);

    // free all of the individual cpus
    free_pq_data(simulation->all_cpus);

    // free all of the cpu priority queues
    free_priority_queue(simulation->all_cpus);
    free_priority_queue(simulation->available_cpus);
    free_priority_queue(simulation->unavailable_cpus);

    // todo free each other part of struct
    // input ????
}

// *** Helper Function Implementations ***

// performs one tick (second) of the specified simulation
void perform_simulation_tick(simulation_t *simulation) {

    add_current_arrivals(simulation);

    // (4) allocate processes to cpus based on whatever allocator is currently in use
    allocate_processes_to_cpu(simulation);

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

            // if the process is parallelisable
            if (current_arrival->parallelisable) {
                generate_and_add_subprocesses(simulation, current_arrival);

            }
            // Otherwise
            else {
                // add it to the current arrivals queue
                priority_queue_insert(simulation->current_arrivals,
                                      (data_t*) current_arrival,
                                      current_arrival->time_remaining);

                // increment the number of processes remaining
                simulation->proc_remaining++;
            }
        }
        else {
            break;
        }
    }
}

void generate_and_add_subprocesses(simulation_t *simulation, process_t *parent) {

    // calculate number of subprocesses
    unsigned int num_subprocesses;

    // num child is equal to the minimum of execution time and num cpus
    // this is the largest value of num subprocesses for which:
    // 1. (execution time / num subprocesses) >= 1
    //      equivalent to: execution time >= num subprocesses
    //      equivalent to: num subprocesses <= execution time
    //
    // 2. num subprocesses <= num cpus)

    // if the number of cpus is smaller/equal to, set this as the number of subprocesses
    if (simulation->all_cpus->size <= parent->execution_time) {
        num_subprocesses = simulation->all_cpus->size;
    }
    // otherwise if the execution time is smaller, set this as the number of subprocesses
    else {
        num_subprocesses = parent->execution_time;
    }

    // calculate the execution time for each of the subprocesses
    unsigned int execution_time;

    execution_time = (unsigned int) round_up(
            (double) parent->execution_time / (double) num_subprocesses)
                    + SYNC_COST;

    // create each of the child subprocesses
    process_t *child;
    for (int i = 0; i < num_subprocesses; i++) {

        // create child subprocess
        child = create_subprocess(parent, execution_time, i);

        // track the number of children not finished within the parent
        parent->num_children_not_finished++;

        // add the child subprocess to the current arrivals queue
        // IMPORTANT: note that the parent does not get added
        priority_queue_insert(simulation->current_arrivals,
                              (data_t*) child,
                              child->time_remaining);

        // we do not increment the number of processes remaining for a child process

        // also add the child subprocess to all processes
        priority_queue_insert(simulation->all_processes,
                              (data_t*) child,
                              child->time_remaining);
    }
}

// attempts to allocate all of the current arrivals to cpus
void allocate_processes_to_cpu(simulation_t *simulation) {

    // continue while neither the current arrivals nor available cpus queues are empty
    while ((!priority_queue_is_empty(simulation->current_arrivals)) &&
            (!priority_queue_is_empty(simulation->available_cpus))) {

        cpu_t *emptiest_cpu = remove_emptiest_and_lowest_id_cpu(simulation->available_cpus);

        process_t *shortest_current_arrival = remove_shortest_and_lowest_id_process(simulation->current_arrivals);

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

                // add it to the finished queue
                priority_queue_insert(simulation->finished,
                                      (data_t*) cpu->running,
                                      cpu->running->time_remaining);

                // set the end time for the process
                cpu->running->end_time = simulation->curr_tick;

                // if the process was a subprocess
                if (cpu->running->parent_process != NULL) {
                    // inform the parent
                    tell_parent_child_finished(simulation, cpu->running);
                }
                // otherwise if it is a normal process
                else {
                    // add it to the finished this tick queue, so it is printed
                    priority_queue_insert(simulation->finished_this_tick,
                                          (data_t*) cpu->running,
                                          cpu->running->time_remaining);

                    // decrease the number of processes remaining
                    simulation->proc_remaining--;
                }

                // store that there is no running process
                cpu->running = NULL;
            }
        }

        // todo schedule running processes
        // if the waiting queue is not empty
        if (!priority_queue_is_empty(cpu->waiting)) {

            // remove the shortest waiting process
            process_t *shortest_waiting = remove_shortest_and_lowest_id_process(cpu->waiting);

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

void tell_parent_child_finished(simulation_t *simulation, process_t *child) {

    // decrease the number of children not finished
    child->parent_process->num_children_not_finished--;

    // check if the parent is done
    if (child->parent_process->num_children_not_finished == CHILDREN_DONE) {

        // add the parent to the finished queue
        priority_queue_insert(simulation->finished,
                              (data_t*) child->parent_process,
                              child->parent_process->time_remaining);

        // store the finish time of the parent as the finish time of the last child
        child->parent_process->end_time = child->end_time;

        // add it to the finished this tick queue, so it is printed
        priority_queue_insert(simulation->finished_this_tick,
                              (data_t*) child->parent_process,
                              child->parent_process->time_remaining);

        // decrease the number of processes remaining
        simulation->proc_remaining--;

    }


}


// returns if the shortest waiting process should be swapped with the running process
bool should_swap_running_process(process_t *shortest_waiting, process_t *running_process) {
    // we compare its time remaining with the shortest waiting
    if (shortest_waiting->time_remaining < running_process->time_remaining) {
        return true;
    }
        // in the case of time remaining ties
    else if (shortest_waiting->time_remaining == running_process->time_remaining) {

        // use process id to resolve
        if (shortest_waiting->process_id < running_process->process_id) {
            return true;
        }

        // in the case of process id ties, use sub process id to resolve
        else if (shortest_waiting->process_id == running_process->process_id &&
                shortest_waiting->sub_process_id < running_process->sub_process_id) {
            return true;
        }
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
    double num_processes = 0;
    bool first_iteration = true;

    // for each finished process
    process_t *process;
    while (!priority_queue_is_empty(simulation->finished)) {
        process = (process_t*) priority_queue_remove(simulation->finished);

        // We are only interested in normal/parent processes (those with no parent)
        if (process->parent_process == NULL) {

            // calculate the turnaround time and add it to the total
            process_turnaround_time = process->end_time - process->time_arrived;
            total_turnaround_time += process_turnaround_time;

            // calculate the time overhead rounded to two decimal places and add it to the total
            process_time_overhead = round_to_two_places(
                    process_turnaround_time / process->execution_time);
            total_time_overhead += process_time_overhead;

            // update the maximum time overhead
            if (first_iteration || process_time_overhead > max_time_overhead) {
                max_time_overhead = process_time_overhead;
            }
            first_iteration = false;

            // increment the total
            num_processes++;
        }

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
    printf("Turnaround time %g\n", average_turnaround_time);
    printf("Time overhead %g %g\n", max_time_overhead, average_time_overhead);
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
    // round it to the nearest int by typecasting
    unsigned int number_int = (unsigned int) number;

    // if the number stay the same then it is already a whole number
    if (number == (double) number_int) {
        return number;
    }

    // otherwise we round up the number and return it
    return (double) (number_int + ROUNDING_UP_AMOUNT);
}
