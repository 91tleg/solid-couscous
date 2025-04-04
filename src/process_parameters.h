#ifndef PROCESS_PARAMTERS_H
#define PROCESS_PARAMTERS_H

#include <stdint.h>
#include "ssm1.h"
#include "state_machine.h"

/**
 * @brief Reads the battery voltage measured at the ECU.
 * @note Conversion factor: x * 0.08 V
 */
void read_battery_voltage(struct state_machine_data *data);

/**
 * @brief Reads the vehicle speed from the speed sensor.
 * @note Conversion factor: x / 1.6 mph
 */
void read_vehicle_speed(struct state_machine_data *data); 

/**
 * @brief Reads the engine RPM from the cam and crank angle sensors.
 * @note Conversion factor: x * 25 rev/min
 */
void read_engine_speed(struct state_machine_data *data);

/**
 * @brief Reads the engine coolant temperature.
 * @note Use lookup table for conversion to degrees Fahrenheit.
 */
void read_coolant_temp(struct state_machine_data *data);

/**
 * @brief Reads the airflow voltage from the mass airflow (MAF) sensor.
 * The MAF sensor measures the amount of air being used for the air/fuel mixture.
 * @note Conversion factor: x / 50 V
 *
 */
void read_airflow(struct state_machine_data *data);

/**
 * @brief Reads the throttle opening percentage.
 */
void read_throttle_percentage(struct state_machine_data *data);

/**
 * @brief Reads the throttle position sensor voltage.
 */
void read_throttle_signal(struct state_machine_data *data);

 /**
 * @brief Reads the intake manifold pressure.
 * @note Conversion formula: x / 0.128 - 1060
 */
void read_manifold_pressure(struct state_machine_data *data);

/**
 * @brief Reads the boost control duty cycle.
 * @note Conversion factor: x / 2.56
 */
void read_boost_control_duty_cycle(struct state_machine_data *data);

/**
 * @brief Reads the ignition timing in degrees before top dead center (BTDC).
 * @details This is one of the primary engine control parameters.
 * - Timing is adjusted when knock is detected.
 * - Also affected by crankshaft angular velocity fluctuations (engine jerking).
 * @note Conversion factor: x degrees
 */
void read_ignition_timing(struct state_machine_data *data);

/**
 * @brief Reads the engine load in terms of air mass per revolution.
 * @details Engine load represents the amount of air consumed per engine revolution.
 * - Calculated using MAF (mass/time) sensor and RPM (rev/time) sensor:
 *   (mass / time) / (rev / time) = mass / rev
 * @note Conversion factor: x air mass/rev
 */
void read_load(struct state_machine_data *data);

/**
 * @brief Reads the injector pulse width (IPW).
 * @details The IPW represents the duration an injector remains open per intake stroke.
 * - Directly related to engine load.
 * - More air intake requires increased fuel injection for a stoichiometric ratio.
 * - Additional corrections applied during conditions like cold starts.
 * @note Conversion factor: x * 0.128 ms
 */
void read_injector_pulse_width(struct state_machine_data *data);

/**
 * @brief Reads the idle air control valve (IACV) duty cycle.
 * @details Controls air bypassing the throttle for idle speed adjustments.
 * - Continuously modulated to regulate idle speed.
 * - Higher duty cycle = higher idle speed.
 * - Adjusts for electrical loads, A/C operation, deceleration fuel cut-off, and engine temperature compensation.
 * @note Conversion factor: x / 2 % duty cycle
 */
void read_iacv_duty_cycle(struct state_machine_data *data);

/**
 * @brief Reads the oxygen (O2) sensor signal voltage.
 * @details Indicates the air/fuel mixture:
 * - Lower values (~0V) indicate a lean mixture.
 * - Higher values (~1V) indicate a rich mixture.
 * @note Conversion factor: x / 100 V
 */
void read_o2_signal(struct state_machine_data *data);

/**
 * @brief Reads the timing correction adjustment.
 * @details Represents timing advance added to prevent knock.
 * - Adjusts dynamically when knock is detected by the knock sensor.
 * @note Conversion factor: x degrees
 */
void read_timing_correction(struct state_machine_data *data);

/**
 * @brief Reads the fuel trim adjustment.
 * @details Modifies fuel injection to maintain a stoichiometric air/fuel ratio based on O2 sensor feedback.
 * - Lean mixtures (0.1V - 0.5V O2 sensor) increase fuel trim.
 * - Rich mixtures (0.5V - 0.9V O2 sensor) decrease fuel trim.
 * @note Conversion formula: (x - 128) / 1.28
 */
void read_fuel_trim(struct state_machine_data *data);

/**
 * @brief Reads the atmospheric pressure.
 * @note Conversion formula: x * 1.25 + 500
 */
void read_atmosphere_pressure(struct state_machine_data *data);

/**
 * @brief Reads the input switches.
 */
void read_input_switches(struct state_machine_data *data);

/**
 * @brief Reads the I/O switches.
 */
void read_inout_switches(struct state_machine_data *data);

/**
 * @brief Reads an active or stored trouble codes from address.
 * @param addr The memory address of the trouble code.
 */


 /*
void read_trouble_code_one(uint16_t addr);
void read_trouble_code_two(uint16_t addr);
void read_trouble_code_three(uint16_t addr);
*/


/**
 * @brief Read the first trouble codes.
 */
void read_active_trouble_code_one(struct state_machine_data *data);
void read_stored_trouble_code_one(struct state_machine_data *data);

/**
 * @brief Read the second trouble codes.
 */
void read_active_trouble_code_two(struct state_machine_data *data);
void read_stored_trouble_code_two(struct state_machine_data *data);

/**
 * @brief Read the third trouble codes.
 */
void read_active_trouble_code_three(struct state_machine_data *data);
void read_stored_trouble_code_three(struct state_machine_data *data);

#endif