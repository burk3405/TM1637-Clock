#include <Wire.h>
#include <RTClib.h>
#include <TM1637Display.h>

// ==================================================
// TM1637
// ==================================================

#define CLK 3
#define DIO 2

TM1637Display display(CLK, DIO);

// ==================================================
// DS3231
// ==================================================

RTC_DS3231 rtc;

// ==================================================
// SETUP
// ==================================================

void setup()
{
    // TM1637 brightness
    display.setBrightness(7);

    // Start I2C
    Wire.begin();

    // Start RTC
    if (!rtc.begin())
    {
        // Show an error if the RTC cannot be found
        display.showNumberDec(8888);

        while (1)
        {
            delay(100);
        }
    }
}

// ==================================================
// LOOP
// ==================================================

void loop()
{
    // Get the current time from the DS3231
    DateTime now = rtc.now();

    // Get the current hour
    int hour24 = now.hour();

    // Convert 24-hour time to 12-hour time
    int hour12 = hour24 % 12;

    // Convert 0 to 12
    if (hour12 == 0)
    {
        hour12 = 12;
    }

    // Get the minute
    int minute = now.minute();

    // Combine hour and minute
    //
    // 7:42  -> 742
    // 12:05 -> 1205
    //
    int displayValue = hour12 * 100 + minute;

    // Display the time
    // 0x40 = colon ON
    display.showNumberDecEx(
        displayValue,
        0x40,
        false
    );

    // Update once per second
    delay(1000);
}
