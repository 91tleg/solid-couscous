#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "state_defs.h"

/**
 * @brief Initializes the state machine data struct.
 *
 * @param data Pointer to the state machine data struct to initialize.
 */
void state_machine_init(struct state_machine_data *data);

/**
 * @brief Handles entering a new state in the state machine.
 *
 * @param data Pointer to the state machine data struct.
 * @param from The previous state.
 * @param to The new state to enter.
 */
void state_enter(struct state_machine_data *data, state_e from, state_e to);

/**
 * @brief Processes an event in the state machine.
 *
 * Evaluates the event and updates the state machine.
 *
 * @param data Pointer to the state machine data struct.
 * @param event The event to process.
 */
void process_event(struct state_machine_data *data, state_event_e event);

#endif // STATE_MACHINE_H