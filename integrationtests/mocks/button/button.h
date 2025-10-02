#ifndef MOCK_BUTTON_H
#define MOCK_BUTTON_H

#include "state_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

state_event_e read_state_event(void); 

#ifdef __cplusplus
}
#endif

#endif // MOCK_BUTTON_H