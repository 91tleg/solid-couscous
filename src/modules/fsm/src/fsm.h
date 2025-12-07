#ifndef FSM_H
#define FSM_H

#include "button_event.h"
#include "fsm_states.h"

/**
 * @brief Processes an event in the state machine.
 *
 * Evaluates the event and updates the state machine.
 *
 * @param state Pointer to the state to be updated.
 * @param event The event to process.
 */
void fsm_process_event(fsm_state_e *state, button_event_e event);

#endif // FSM_H
