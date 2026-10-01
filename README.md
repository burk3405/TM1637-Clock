# TM1637-Clock
A standalone 12-hour digital clock built with an Arduino Nano, a TM1637 4-digit 7-segment display, and a Waveshare Pico-RTC-DS3231 module.

The DS3231 provides battery-backed timekeeping, so the clock does not require Wi-Fi or an Internet connection.

## Components
- [Arduino Nano](https://www.amazon.com/dp/B07G99NNXL)
- [TM1637 4-Digit 7-Segment Display](https://www.amazon.com/dp/B0BFQNFX6D)
- [Waveshare Pico-RTC-DS3231](https://www.amazon.com/dp/B0F82SVYZS)
- CR1220 battery for the RTC module
- 4 x Female-to-male Dupont jumper wires
- 4 x Female-to-female Dupont jumper wires
- USB data cable and Arduino IDE

## Wiring

### TM1637 Display → Arduino Nano

| TM1637 Pin | Arduino Nano Pin | Purpose |
|---|---|---|
| VCC | 5V | Display power |
| GND | GND | Ground |
| CLK | D2 | Clock/data timing |
| DIO | D3 | Data |

The TM1637 display uses two digital pins for communication.

### Waveshare Pico-RTC-DS3231 → Arduino Nano

Use the **female header on the Waveshare RTC module** for the male ends of the Dupont wires. The RTC's male header is intended for stacking with a Raspberry Pi Pico.

| Arduino Nano | RTC Module / Pico Header | Purpose |
|---|---|---|
| 3.3V | 3V3 / Pico pin 36 | RTC power |
| GND | GND / Pico ground pin | Ground |
| A4 | GP20 / Pico pin 26 | I²C SDA |
| A5 | GP21 / Pico pin 27 | I²C SCL |

### Complete Wiring Diagram

```text
                     Arduino Nano
                  ┌─────────────────┐
                  │                 │
       5V ────────┼─────────────────┼──── VCC → TM1637
       GND ───────┼─────────────────┼──── GND → TM1637
       D2 ────────┼─────────────────┼──── CLK → TM1637
       D3 ────────┼─────────────────┼──── DIO → TM1637
                  │                 │
       3.3V ──────┼──────┐          │
       GND ───────┼──────┼──────────┼──── GND → RTC
       A4 ────────┼──────┼──────────┼──── GP20 / SDA → RTC
       A5 ────────┼──────┼──────────┼──── GP21 / SCL → RTC
                  └──────┼──────────┘
                         │
                  ┌──────▼──────────┐
                  │ Waveshare       │
                  │ Pico-RTC-DS3231 │
                  │                 │
                  │ DS3231          │
                  │ CR1220 battery  │
                  └─────────────────┘
Why the RTC Is Needed

The Arduino Nano's millis() function can be used to count elapsed time, but it is not a real-time clock and does not retain the current time when power is removed.

The DS3231 is a dedicated real-time clock. It keeps track of seconds, minutes, hours, date, month, and year independently of the Arduino.

With the backup battery installed:

The Nano can be powered off.
The DS3231 continues keeping time from its battery.
When the Nano is powered back on, it reads the current time from the DS3231.
The TM1637 displays the current hour and minute.

No Wi-Fi or Internet connection is required.

Initial RTC Setup

The DS3231 must be given the correct date and time once when it is first used.

For example:

rtc.adjust(DateTime(2026, 10, 1, 17, 30, 0));

The values are:

year, month, day, hour, minute, second

The hour uses 24-hour time:

00 = 12:00 AM
12 = 12:00 PM
13 = 1:00 PM
17 = 5:00 PM

After setting the RTC, remove or comment out the rtc.adjust() line before using the final clock program.

Otherwise, every time the Nano starts, the RTC would be reset to the same initial time.

I²C Address

The DS3231 uses I²C.

For this project:

SDA = Arduino Nano A4
SCL = Arduino Nano A5
I²C address = 0x68

An I²C scanner should report:

I2C device found at address 0x68

This confirms that the Nano can communicate with the RTC.

Arduino Libraries

Install these libraries through:

Arduino IDE → Tools → Manage Libraries

TM1637

Install:

TM1637Display
RTC

Install:

RTClib by Adafruit

The project also uses Arduino's built-in:

#include <Wire.h>

library for I²C communication.

Final Program

The final clock program should use the DS3231 as the source of time rather than millis().

The display is configured as a 12-hour clock:

1:05
2:30
7:42
12:15

The colon remains continuously illuminated.

AM and PM are intentionally not displayed because the 4-digit display does not have a dedicated AM/PM indicator in this project.

Startup Behavior
Normal operation
DS3231
   ↓
Current date/time
   ↓
Arduino Nano
   ↓
12-hour conversion
   ↓
TM1637
   ↓
7:42
After power is removed
Arduino Nano OFF
        ↓
DS3231 continues running
        ↓
CR1220 battery powers RTC
        ↓
Nano powered back ON
        ↓
Nano reads current time
        ↓
TM1637 displays current time

The Nano does not need to be connected to a computer after the program has been uploaded.

Recommended Testing Order
1. Test the TM1637

Confirm that the display can show a test time such as:

7:42
2. Connect the RTC

Connect:

Nano 3.3V → RTC 3V3
Nano GND  → RTC GND
Nano A4   → RTC GP20 / SDA
Nano A5   → RTC GP21 / SCL
3. Run the I²C scanner

The expected result is:

I2C device found at address 0x68
4. Set the RTC time

Run the one-time RTC setup sketch with the correct date and time.

5. Upload the final clock program

The final program should read the time using:

DateTime now = rtc.now();

and send the hour/minute to the TM1637.

6. Test power loss

After confirming the clock works:

Disconnect USB power from the Nano.
Wait several minutes.
Power the Nano back on.
Confirm that the displayed time has advanced correctly.
Important Notes
RTC power: Use the Nano's 3.3V output for the Waveshare RTC module.
TM1637 power: Use the Nano's 5V output for the TM1637.
Ground: The Nano, TM1637, and RTC must share ground.
RTC connector: Use the RTC module's female header with the male ends of the Dupont wires.
RTC battery: Keep the CR1220 installed so the DS3231 can maintain time while the Nano is powered off.
Do not repeatedly run the RTC initialization sketch after the correct time has been set.
The project intentionally has no Wi-Fi or Internet dependency.
Pin Summary
Arduino Nano	Connected Device	Pin/Signal
5V	TM1637	VCC
GND	TM1637	GND
D2	TM1637	CLK
D3	TM1637	DIO
3.3V	Pico-RTC-DS3231	3V3
GND	Pico-RTC-DS3231	GND
A4	Pico-RTC-DS3231	GP20 / SDA
A5	Pico-RTC-DS3231	GP21 / SCL
Project Architecture
                 ┌───────────────────┐
                 │   CR1220 Battery  │
                 └─────────┬─────────┘
                           │
                 ┌─────────▼─────────┐
                 │ Waveshare         │
                 │ Pico-RTC-DS3231   │
                 │                   │
                 │ DS3231 RTC        │
                 └─────────┬─────────┘
                           │
                    I²C: A4 / A5
                           │
                 ┌─────────▼─────────┐
                 │   Arduino Nano    │
                 │                   │
                 │ Time conversion   │
                 │ 24h → 12h         │
                 └─────────┬─────────┘
                           │
                      D2 / D3
                           │
                 ┌─────────▼─────────┐
                 │      TM1637       │
                 │   4-Digit LED     │
                 │                   │
                 │       7:42        │
                 └───────────────────┘

"""
