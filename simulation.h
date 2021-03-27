//
// Created by Glenn Phillips on 27/3/21.
//

#ifndef SIMULATION_H
#define SIMULATION_H

// --- System Libraries ---

// --- Project Libraries ---
#include "priority_queue.h"
#include "input_handler.h"
#include "cpu.h"

// --- Constant Definitions ---

// --- Type Definitions ---
typedef struct priority_queue_s priority_queue_t; // forward declaration so it compiles

typedef struct cpu_s cpu_t; // forward declaration so it compiles

typedef struct simulation_s simulation_t;

struct simulation_s {
    priority_queue_t *all_processes;
    priority_queue_t *future_arrivals;
    priority_queue_t *current_arrivals;
    priority_queue_t *shortest_arrivals;

    // todo change this later to cpu
    cpu_t *cpu;

    // for printing updates
    priority_queue_t *started_this_tick;
    priority_queue_t *finished_this_tick;
};

// --- Function Prototypes ---

// creates a new simulation based on the specified input
// then returns a pointer to the simulation
simulation_t *new_simulation(input_arguments_t input);

// starts the specified simulation
void start_simulation(simulation_t *simulation);

// performs one tick (second) of the specified simulation
void perform_simulation_tick(simulation_t *simulation, unsigned int curr_tick);

// destroy a simulation and free all of its associated memory
void free_simulation(simulation_t *simulation);

#endif //SIMULATION_H
