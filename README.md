# BLE_MOUSE_low_level

## Overview

**BLE_MOUSE** is a Bluetooth Low Energy (BLE) mouse emulator designed to run on an ESP32 microcontroller. The program configures the ESP32 to act as a BLE HID (Human Interface Device) mouse, allowing it to connect to a host device (e.g., a computer or tablet) and simulate mouse movements and button presses. The primary purpose is to provide automated mouse cursor movement to prevent screen locking on the host device, with optional manual control via physical buttons for scrolling.

## Purpose

The main objectives of the BLE_MOUSE project are:
- **Coding example**: Provide an example of a BlueTooth mouse HID device, not using any external libraries
- **Prevent Screen Lock**: Automatically moves the mouse cursor up and down at regular intervals to keep the host device active, preventing automatic screen locking or sleep mode.
- **Manual Mouse Control**: Supports two physical pushbuttons to trigger mouse scrolling (up or down) when pressed, providing user-controlled interaction.
- **BLE HID Emulation**: Emulates a standard USB mouse with a simplified 5-byte input report, supporting left, right, and middle button clicks, as well as X/Y cursor movement.

## Features

- **BLE Connectivity**: Establishes a BLE connection as an HID mouse device named "Flip-O-Matic-C3".
- **Automatic Cursor Movement**: Periodically sends small cursor movements (up and down) to prevent screen locking, with configurable delays (5 seconds up, 50 seconds down).
- **Button-Based Control**: Two pushbuttons (connected to pins 38 and 39) allow manual triggering of mouse scroll up or down.
- **LED Indicator**: An LED (connected to pin 5) can be used for status indication (though not actively used in the current code).
- **Customizable HID Report**: Implements a simplified USB mouse report map with 3 buttons and X/Y movement, based on a 5-byte input report structure.
- **Battery Level Reporting**: Simulates a battery level (set to 7%) for compatibility with BLE HID standards.
- **Security**: Uses BLE security with bonding authentication for a secure connection to the host.

## Hardware Requirements

- **ESP32 Microcontroller**: The program is designed for an ESP32 board with BLE support.
- works with the classics ESP32-S2 or ESP32-S3 as well as the ESP32-C3 as the "NoLogo ESP32C3 Supermini" available here (for instance) https://aliexpress.com/item/1005008329584726.html
- **Pushbuttons**: Two buttons connected to GPIO pins 38 and 39 with internal pull-up resistors.
- **LED**: An optional LED connected to GPIO pin 5 for status indication.
- **Power Supply**: A suitable power source for the ESP32-C3 (e.g., USB or battery).

## Software Dependencies

- **Arduino Framework**: The program is written for the Arduino IDE with ESP32 board support.
- **ESP32 BLE Libraries**: Utilizes only librairies provided by the base ESP32 (espressif) software, no need to install extra libraries

## How It Works

1. **Initialization**:
   - Initializes the ESP32 as a BLE device named "Flip-O-Matic-C3".
   - Configures a BLE server with an HID service to emulate a mouse.
   - Sets up a custom HID report map for a simplified mouse with 3 buttons and X/Y movement.
   - Starts BLE advertising with the HID mouse appearance.
   - Initializes two pushbutton pins (38, 39) with pull-up resistors and an LED pin (5).

2. **BLE Connection Handling**:
   - Uses a custom `MyCallbacks` class to handle BLE connection and disconnection events.
   - Enables/disables notifications on the mouse input report characteristic when a host connects or disconnects.

3. **Main Operation**:
   - When connected to a host via BLE, the program automatically sends mouse movement commands:
     - Moves the cursor up (`Y = -1`) every 5 seconds.
     - Moves the cursor down (`Y = +1`) every 50 seconds.
   - If buttons are pressed:
     - Button on pin 38 triggers continuous scroll down (`Y = -96`).
     - Button on pin 39 triggers continuous scroll up (`Y = +1`).
   - Notifications are sent to the host via the BLE input report characteristic.

4. **Task Management**:
   - Runs the BLE server in a separate FreeRTOS task (`taskServer`) to handle BLE operations concurrently with the main loop.

## Usage

1. **Setup**:
   - Connect pushbuttons to GPIO pins 38 and 39, and an optional LED to pin 5.
   - Upload the code to an ESP32 board using the Arduino IDE with ESP32 board support.

2. **Operation**:
   - Power on the ESP32; it will start advertising as "Flip-O-Matic-C3".
   - Pair the device with a BLE-compatible host (e.g., a computer or tablet).
   - The device will automatically move the mouse cursor to prevent screen locking.
   - Press the buttons on pins 38 or 39 to manually trigger scrolling.

3. **Monitoring**:
   - Open the Serial Monitor (115200 baud) to view logs, such as connection status and mouse movement events.

## Limitations

- **Fixed Movement Pattern**: The automatic movement is limited to up/down cursor shifts with hardcoded delays.
- **Button Mode**: The code has a commented-out condition (`if (true)`) that, when used, enables a  button-based modes.
- **No Wheel Support**: The report map supports a mouse wheel, but the current code does not utilize it.
- **Battery Level**: The battery level is hardcoded to 7% and not dynamically updated.

## Future Improvements

- Add support for mouse wheel scrolling in the report map.


## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Built using the ESP32 Arduino core and BLE HID libraries.
- Inspired by USB HID mouse specifications and example projects for BLE mouse emulation.