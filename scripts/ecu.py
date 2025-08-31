import struct
import random
import time
import serial
from datetime import datetime

ECU_ADDRESSES = {
    0x1404: "BATTERY_VOLTAGE",
    0x154B: "VEHICLE_SPEED",
    0x140B: "ENGINE_SPEED",
    0x1405: "COOLANT_TEMP",
    0x1489: "IGNITION_ADVANCE",
    0x1400: "AIRFLOW_SENSOR",
    0x1414: "ENGINE_LOAD",
    0x1487: "THROTTLE",
    0x15F0: "INJECTOR_PULSE_WIDTH",
    0x158A: "IAC_VALVE",
    0x1530: "TIMING_CORRECTION",
    0x1403: "O2_AVERAGE",
    0x1488: "AF_CORRECTION",
    0x1516: "ATMOSPHERIC_PRESSURE",
    0x00BE: "MANIFOLD_PRESSURE",
    0x144D: "BOOST_SOLENOID",
    0x15A8: "INPUT_SWITCHES",
    0x15A9: "IO_SWITCHES",
    0x0047: "ACTIVE_TROUBLE_CODE_ONE",
    0x0048: "ACTIVE_TROUBLE_CODE_TWO",
    0x0049: "ACTIVE_TROUBLE_CODE_THREE",
    0x1604: "STORED_TROUBLE_CODE_ONE",
    0x1605: "STORED_TROUBLE_CODE_TWO",
    0x1606: "STORED_TROUBLE_CODE_THREE"
}

PORT = "/dev/cu.usbserial-1420"
BAUD_RATE = 1953

ecu_sim = serial.Serial(
    port=PORT,
    baudrate=BAUD_RATE,
    bytesize=8,
    parity='E',
    stopbits=1,
    timeout=1
)

sensor_values = {addr: random.randint(0, 255) for addr in ECU_ADDRESSES.keys()}

def log(message):
    print(f"[{datetime.now().strftime('%H:%M:%S')}] {message}")

def update_sensor_values():
    for addr in sensor_values:
        delta = random.randint(-1, 1)
        sensor_values[addr] = max(0, min(255, sensor_values[addr] + delta))

def pretty_print(data):
    print_hex = " ".join(f"{b:02X}" for b in data)
    print_ascii = "".join(chr(b) if 32 <= b <= 126 else "." for b in data)
    return f"ASCII: {print_ascii} | HEX: {print_hex}"

def simulate_ecu():
    log(f"ECU Simulator running on {PORT} at {BAUD_RATE} baud")

    streaming = False
    stream_addr = None
    romid_mode = False

    romid_bytes = (0x71, 0x36, 0x00)

    while True:
        update_sensor_values()

        while ecu_sim.in_waiting >= 4:
            peek = ecu_sim.read(1)
            while peek[0] not in (0x78, 0x12):
                log(f"Discarding stray byte: {peek[0]:02X}")
                peek = ecu_sim.read(1)
            rest = ecu_sim.read(3)
            request = peek + rest
            header, msb, lsb, footer = struct.unpack("BBBB", request)

            # Stop command
            if header == 0x12 and msb == 0x00 and lsb == 0x00 and footer == 0x00:
                log("Stop command received")
                streaming = False
                stream_addr = None
                romid_mode = False
                continue

            # ROMID command
            elif header == 0x78 and msb == 0xFF and lsb == 0xFE and footer == 0x00:
                log("ROMID command received")
                streaming = True
                romid_mode = True
                stream_addr = None
                continue

            # Read command
            elif header == 0x78 and footer == 0x00:
                addr = (msb << 8) | lsb
                log(f"Read request for {hex(addr)} ({ECU_ADDRESSES.get(addr, 'UNKNOWN')})")
                streaming = True
                romid_mode = False
                stream_addr = addr
                continue

            else:
                log(f"Unknown command: {pretty_print(request)}")

        if streaming:
            if romid_mode:
                # ROMID response format: FF FE junk b1 b2 b3
                packet = struct.pack("BBBBBB", 0xFF, 0xFE, 0x00, *romid_bytes)
                ecu_sim.write(packet)
                ecu_sim.flush()
                log(f"Sent ROMID response: {' '.join(f'{b:02X}' for b in packet)}")
            elif stream_addr is not None:
                # Read response format: MSB LSB DATA
                value = sensor_values.get(stream_addr, random.randint(0, 255))
                packet = struct.pack("BBB", (stream_addr >> 8) & 0xFF, stream_addr & 0xFF, value)
                ecu_sim.write(packet)
                ecu_sim.flush()
                log(f"Sent data {value} for {hex(stream_addr)} ({ECU_ADDRESSES.get(stream_addr)}): {' '.join(f'{b:02X}' for b in packet)}")

        time.sleep(0.05)


if __name__ == "__main__":
    simulate_ecu()