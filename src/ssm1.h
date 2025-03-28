/*
 * http://www.4bc.org/vanagon/SSM_params.html
 * http://www.alcyone.org.uk/ssm/ecureverse.html
 * https://github.com/P1kachu/ssm1-gc8
 */

 #ifndef SSM1_H
 #define SSM1_H

 #include <stdint.h>
 #include "parameters.h"
 #include "uart.h"
 
 /**
  * @brief Stops the ECU from sending data.
  * 
  * The ROM ID is a 6-digit hexadecimal number that identifies the ECU's firmware version, model year, car type, and market.
  * It can be obtained by sending the "Get RomID" command (00 46 48 49) while the ECU is transmitting.
  * Alternatively, the ROM ID can be read directly from the ROM at address 8C3D in some vehicles (e.g., Alcylone).
  */
 void stop_read(void);
 
 /**
  * @brief Retrieves the ROM ID from the ECU.
  * 
  * The ROM ID is a 6-digit hexadecimal number that identifies the ECU's firmware version, model year, car type, and market.
  * It can be obtained by sending the "Get RomID" command (00 46 48 49) while the ECU is transmitting.
  * Alternatively, the ROM ID can be read directly from the ROM at address 8C3D in some vehicles (e.g., Alcylone).
  * 
  * ROM ID Structure:
  * - Digit 1: Unknown, always '7' (Possibly denotes Subaru).
  * - Digit 2: Model year (0 = 1990, 2 = 1992, 4 = 1994, etc.). This may be earlier than the actual registration year.
  * - Digits 3-4: Car type and engine code:
  *   - 25 = SVX / EG33
  *   - 31 = Legacy / EJ20
  *   - 32 = Legacy / EJ22
  *   - 36 = Legacy / EJ20T
  *   - 3F = Impreza / EJ15
  *   - 40 = Impreza / EJ20T
  * - Digit 5: Domestic market:
  *   - 1 = Japan
  *   - 2 = USA
  *   - 3 = Europe
  *   - A = UK
  * - Digit 6: ECU firmware revision (relative to the ROM ID).
  * 
  * @param buffer Pointer to a buffer where the ROM ID will be stored.
  */
 void get_romid(uint8_t *buffer);
 
 /**
  * @brief Sends a command to read an ECU memory address.
  * 
  * The ECU transmits data continuously without flow control, meaning the receiving program must read
  * the data as quickly as it is sent. Failing to do so can cause the serial port input buffer to overflow, 
  * leading to data loss and desynchronization.
  * 
  * To ensure data integrity, it is advisable to perform a sanity check on the most significant byte (MSB) 
  * and least significant byte (LSB) before processing the actual data byte.
  * 
  * @param addr The 16-bit ECU memory address.
  */
 uint8_t read_data_from_address(uint16_t addr);
 
 /**
  * @brief Sends a command to clear an ECU memory address.
  * 
  * Command Format:
  * - `AA MSB LSB 00` → Clears the address to zero.
  * - `AA MSB LSB FF` → Clears the address using an inverted value (if required).
  * 
  * **Important:**  
  * - Writing data to the ECU requires an active data reception.  
  * - To write `FF` to address `1234`, first issue an ECU read command (`78 12 34 00`), then send the write command (`AA 12 34 FF`).  
  * 
  * @param addr The 16-bit memory address in the ECU to be cleared.
  */
 void send_clear_command(uint16_t addr);
 
 #endif // SSM1_H