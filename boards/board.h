#ifndef BOARD_H
#define BOARD_H

#if defined(BOARD_ESP32DEV)
#include "esp32dev/pins.h"
#include "esp32dev/config.h"

#elif defined(ECU_SIM)
#include "ecu_sim/pins.h"
#include "ecu_sim/config.h"

#elif defined(UNIT_TEST)
#include "test/pins.h"
#include "test/config.h"

#else
#error "Board not defined"
#endif

#endif // BOARD_H