import struct
import random
import time
import serial
from datetime import datetime

ECU_ADDRESSES = {
    0x1404: "BATTERY_VOLTAGE",
    0x154B: "ENGINE_SPEED",
    0x140B: "ENGINE_SPEED",
    0x1405: "COOLANT_TEMP",
    0x1489: "IGNITION_ADVANCE",
    0x1400: "AIRFLOW_SENSOR",
    0x1414: "ENGINE_LOAD",
    0x1487: "THROTTLE",
    0x15F0: "INJECTOR_PULSE_WIDTH",
    0x158A: "IACVALVE",
    0x1530: "TIMING_CORRECTION",
    0x1403: "O2_AVERAGE",
    0x1488: "AF_CORRECTION",
    0x1516: "ATMOSPHERIC_PRESSURE",
    0x00BE: "MANIFOLD_PRESSURE",
    0x144D: "BOOST_SOLENOID",
    0x15A8: "INPUT_SWITCHES",
    0x15A9: "INOUT_SWITCHES",
    0x0047: "ACTIVE_TROUBLE_CODE_ONE",
    0x0048: "ACTIVE_TROUBLE_CODE_TWO",
    0x0049: "ACTIVE_TROUBLE_CODE_THREE",
    0x1604: "STORED_TROUBLE_CODE_ONE",
    0x1605: "STORED_TROUBLE_CODE_TWO",
    0x1606: "STORED_TROUBLE_CODE_THREE",
}

PORT = "/dev/cu.usbserial-1420"
BAUD_RATE = 9600

ecu_sim = serial.Serial(port=PORT, baudrate=BAUD_RATE, bytesize=8, parity='N', stopbits=1, timeout=1)

def log(message):
    print(f"[{datetime.now().strftime('%H:%M:%S')}] {message}")

def simulate_ecu():
    log(f"ECU Simulator running on {PORT} at {BAUD_RATE} baud")
    while True:
        if ecu_sim.in_waiting >= 4:  # Expecting 4-byte request (0x78 MSB LSB 0x00)
            request = ecu_sim.read(4)
            log(f"Received request: {request.hex()}")

            if len(request) == 4:
                header, msb, lsb, footer = struct.unpack("BBBB", request)

                if header == 0x12 and footer == 0:
                    log(f"Stop command recieved")

                elif header == 0x78 and footer == 0x00:  # Validate format
                    address = (msb << 8) | lsb  # Combine MSB and LSB
                    if address in ECU_ADDRESSES:
                        log(f"Valid address: {hex(address)} ({ECU_ADDRESSES[address]})")

                        response_value = random.randint(220, 255)
                        log(f"Generated response: {response_value}")

                        # Reply format: MSB, LSB, Data
                        response = struct.pack("BBB", msb, lsb, response_value)
                        ecu_sim.write(response)
                        log(f"Sent response: {response.hex()}")
                    else:
                        log(f"Read command recieved: {hex(address)}")
                elif msb == 0x46 and lsb == 0x48:
                    log(f"Get romid command recieved")

                    response = struct.pack("BBB", 0x71, 0x36, 0)
                    ecu_sim.write(response)
                    log(f"Sent romid: {response.hex()}")
                elif header == 0xAA and footer == 0x00:
                    address = (msb << 8) | lsb  # Combine MSB and LSB
                    log(f"Clear command recieved: {hex(address)} ({ECU_ADDRESSES[address]})")
                else:
                    log("Invalid request format")
        time.sleep(0.1)

if __name__ == "__main__":
    simulate_ecu()