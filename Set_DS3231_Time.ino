#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

void setup()
{
    Serial.begin(9600);

    if (!rtc.begin())
    {
        Serial.println("Couldn't find RTC!");
        while (1);
    }

    Serial.println("RTC found.");

    // ==================================================
    // SET THE RTC TIME HERE
    //
    // DateTime(year, month, day, hour, minute, second)
    //
    // The hour uses 24-hour time.
    // ==================================================

    rtc.adjust(DateTime(2026, 10, 1, 15, 20, 0));

    Serial.println("RTC time has been set.");
}

void loop()
{
    DateTime now = rtc.now();

    Serial.print(now.year());
    Serial.print("/");
    Serial.print(now.month());
    Serial.print("/");
    Serial.print(now.day());

    Serial.print(" ");

    Serial.print(now.hour());
    Serial.print(":");

    if (now.minute() < 10)
        Serial.print("0");

    Serial.print(now.minute());

    Serial.print(":");

    if (now.second() < 10)
        Serial.print("0");

    Serial.println(now.second());

    delay(1000);
}
