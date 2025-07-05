#include <Arduino.h>
#include <cstdint>

#include "clockface.h"

#ifndef OUTPUT
#define OUTPUT (1)
#endif

#define DATA (5)  // SER
#define LATCH (4) // RCLK
#define CLOCK (3) // SRCLK

#include <WiFi.h>
#include "time.h"

const char *ssid = "NETGEAR48";
const char *password = "wideunit443";

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -(5 * 60 * 60);
const int daylightOffset_sec = 0;

void WriteOut(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin, union UFaceBits state);

void printLocalTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    Serial.print("Day of week: ");
    Serial.println(&timeinfo, "%A");
    Serial.print("Month: ");
    Serial.println(&timeinfo, "%B");
    Serial.print("Day of Month: ");
    Serial.println(&timeinfo, "%d");
    Serial.print("Year: ");
    Serial.println(&timeinfo, "%Y");
    Serial.print("Hour: ");
    Serial.println(&timeinfo, "%H");
    Serial.print("Hour (12 hour format): ");
    Serial.println(&timeinfo, "%I");
    Serial.print("Minute: ");
    Serial.println(&timeinfo, "%M");
    Serial.print("Second: ");
    Serial.println(&timeinfo, "%S");

    Serial.println("Time variables");
    char timeHour[3];
    strftime(timeHour, 3, "%H", &timeinfo);
    Serial.println(timeHour);
    char timeWeekDay[10];
    strftime(timeWeekDay, 10, "%A", &timeinfo);
    Serial.println(timeWeekDay);
    Serial.println();
}

union UFaceBits circle_light_state = {0};

void setup()
{
    pinMode(DATA, OUTPUT);
    pinMode(LATCH, OUTPUT);
    pinMode(CLOCK, OUTPUT);

    union UFaceBits state = {0};
    WriteOut(DATA, CLOCK, LATCH, state);

    Serial.begin(9600);
    Serial.println("Hello world");

    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");

    // Init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    printLocalTime();

    // disconnect WiFi as it's no longer needed
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

uint8_t data = 128;

void WriteOut(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin, union UFaceBits state)
{
    uint8_t i;

    digitalWrite(latchPin, LOW);
    delayMicroseconds(100);

    for (unsigned int b = 0; b < sizeof(struct FaceBits); b++)
    {
        for (unsigned int i = 0; i < 8; i++)
        {
            int bit = (state.bytes[b] >> i) & 1; // this is right, my lables from 1.1 - 5.8 bits are backwards, chip order is correct
            // int bit = (state.bits[b] & (1 << (7 - i)));

            printf("[%3d]:%1d\t{%d.%d}\n", b * 8 + i, bit, b + 1, i + 1);

            digitalWrite(dataPin, bit);

            delayMicroseconds(100);
            digitalWrite(clockPin, HIGH);
            delayMicroseconds(100);
            digitalWrite(clockPin, LOW);
            delayMicroseconds(100);
        }
    }

    // for (i = 0; i < 8; i++) // foreach bit
    // {
    //     digitalWrite(dataPin, (val & (1 << (7 - i))));
    // }

    digitalWrite(latchPin, HIGH);
    delayMicroseconds(100);
}

// entry point for blinking sec and stuff
#define SetSec(FACE_FIELD, CASE_NO) FACE_FIELD = 1

void UpdateStateTime(UFaceBits *clock, struct tm timeinfo)
{

    memset(clock, 0, sizeof(UFaceBits));
    int hr_1to12 = timeinfo.tm_hour > 12 ? timeinfo.tm_hour - 12 : timeinfo.tm_hour;

    int hr_byte = hr_1to12 / 8; // byte index

    printf("hr is %d; byte %d\n", hr_1to12, hr_byte);

    // clang-format off
    
    
    switch (timeinfo.tm_hour)
    { 
    case 0: clock->face._12 = 1; break;
    case 1: clock->face._1 = 1; break;
    case 2: clock->face._2 = 1; break;
    case 3: clock->face._3 = 1; break;
    case 4: clock->face._4 = 1; break;
    case 5: clock->face._5 = 1; break;
    case 6: clock->face._6 = 1; break;
    case 7: clock->face._7 = 1; break;
    case 8: clock->face._8 = 1; break;
    case 9: clock->face._9 = 1; break;
    case 10: clock->face._10 = 1; break;
    case 11: clock->face._11 = 1; break;
    case 12: clock->face._12 = 1; break;
    case 13: clock->face._1 = 1; break;
    case 14: clock->face._2 = 1; break;
    case 15: clock->face._3 = 1; break;
    case 16: clock->face._4 = 1; break;
    case 17: clock->face._5 = 1; break;
    case 18: clock->face._6 = 1; break;
    case 19: clock->face._7 = 1; break;
    case 20: clock->face._8 = 1; break;
    case 21: clock->face._9 = 1; break;
    case 22: clock->face._10 = 1; break;
    case 23: clock->face._11 = 1; break;
    default:
        break;
    }


    switch (timeinfo.tm_min-1)
    { 
    case -1: clock->face._11_4 = 1; break; 
    case  0: clock->face._12_0 = 1; break;
    case  1: clock->face._12_1 = 1; break; 
    case  2: clock->face._12_2 = 1; break;
    case  3: clock->face._12_3 = 1; break;
    case  4: clock->face._12_4 = 1; break;
    case  5: clock->face._1_0 = 1; break;
    case  6: clock->face._1_1 = 1; break;
    case  7: clock->face._1_2 = 1; break;
    case  8: clock->face._1_3 = 1; break;
    case  9: clock->face._1_4 = 1; break;
    case 10: clock->face._2_0 = 1; break;
    case 11: clock->face._2_1 = 1; break;
    case 12: clock->face._2_2 = 1; break;
    case 13: clock->face._2_3 = 1; break;
    case 14: clock->face._2_4 = 1; break;
    case 15: clock->face._3_0 = 1; break;
    case 16: clock->face._3_1 = 1; break;
    case 17: clock->face._3_2 = 1; break;
    case 18: clock->face._3_3 = 1; break;
    case 19: clock->face._3_4 = 1; break;
    case 20: clock->face._4_0 = 1; break;
    case 21: clock->face._4_1 = 1; break;
    case 22: clock->face._4_2 = 1; break;
    case 23: clock->face._4_3 = 1; break;
    case 24: clock->face._4_4 = 1; break;
    case 25: clock->face._5_0 = 1; break;
    case 26: clock->face._5_1 = 1; break;
    case 27: clock->face._5_2 = 1; break;
    case 28: clock->face._5_3 = 1; break;
    case 29: clock->face._5_4 = 1; break;
    case 30: clock->face._6_0 = 1; break;
    case 31: clock->face._6_1 = 1; break;
    case 32: clock->face._6_2 = 1; break;
    case 33: clock->face._6_3 = 1; break;
    case 34: clock->face._6_4 = 1; break;
    case 35: clock->face._7_0 = 1; break;
    case 36: clock->face._7_1 = 1; break;
    case 37: clock->face._7_2 = 1; break;
    case 38: clock->face._7_3 = 1; break;
    case 39: clock->face._7_4 = 1; break;
    case 40: clock->face._8_0 = 1; break;
    case 41: clock->face._8_1 = 1; break;
    case 42: clock->face._8_2 = 1; break;
    case 43: clock->face._8_3 = 1; break;
    case 44: clock->face._8_4 = 1; break;
    case 45: clock->face._9_0 = 1; break;
    case 46: clock->face._9_1 = 1; break;
    case 47: clock->face._9_2 = 1; break;
    case 48: clock->face._9_3 = 1; break;
    case 49: clock->face._9_4 = 1; break;
    case 50: clock->face._10_0 = 1; break;
    case 51: clock->face._10_1 = 1; break;
    case 52: clock->face._10_2 = 1; break;
    case 53: clock->face._10_3 = 1; break;
    case 54: clock->face._10_4 = 1; break;
    case 55: clock->face._11_0 = 1; break;
    case 56: clock->face._11_1 = 1; break;
    case 57: clock->face._11_2 = 1; break;
    case 58: clock->face._11_3 = 1; break;
    case 59: clock->face._11_4 = 1; break;
    default:
        break;
    }


    
    switch (timeinfo.tm_sec-1)
    { 
    case-1: SetSec(clock->face._11_4, timeinfo.tm_min); break;
    case 0: SetSec(clock->face._12_0, timeinfo.tm_min); break;
    case 1: SetSec(clock->face._12_1, timeinfo.tm_min); break; 
    case 2: SetSec(clock->face._12_2, timeinfo.tm_min); break;
    case 3: SetSec(clock->face._12_3, timeinfo.tm_min); break;
    case 4: SetSec(clock->face._12_4, timeinfo.tm_min); break;
    case 5: SetSec(clock->face._1_0, timeinfo.tm_min); break;
    case 6: SetSec(clock->face._1_1, timeinfo.tm_min); break;
    case 7: SetSec(clock->face._1_2, timeinfo.tm_min); break;
    case 8: SetSec(clock->face._1_3, timeinfo.tm_min); break;
    case 9: SetSec(clock->face._1_4, timeinfo.tm_min); break;
    case 10: SetSec(clock->face._2_0, timeinfo.tm_min); break;
    case 11: SetSec(clock->face._2_1, timeinfo.tm_min); break;
    case 12: SetSec(clock->face._2_2, timeinfo.tm_min); break;
    case 13: SetSec(clock->face._2_3, timeinfo.tm_min); break;
    case 14: SetSec(clock->face._2_4, timeinfo.tm_min); break;
    case 15: SetSec(clock->face._3_0, timeinfo.tm_min); break;
    case 16: SetSec(clock->face._3_1, timeinfo.tm_min); break;
    case 17: SetSec(clock->face._3_2, timeinfo.tm_min); break;
    case 18: SetSec(clock->face._3_3, timeinfo.tm_min); break;
    case 19: SetSec(clock->face._3_4, timeinfo.tm_min); break;
    case 20: SetSec(clock->face._4_0, timeinfo.tm_min); break;
    case 21: SetSec(clock->face._4_1, timeinfo.tm_min); break;
    case 22: SetSec(clock->face._4_2, timeinfo.tm_min); break;
    case 23: SetSec(clock->face._4_3, timeinfo.tm_min); break;
    case 24: SetSec(clock->face._4_4, timeinfo.tm_min); break;
    case 25: SetSec(clock->face._5_0, timeinfo.tm_min); break;
    case 26: SetSec(clock->face._5_1, timeinfo.tm_min); break;
    case 27: SetSec(clock->face._5_2, timeinfo.tm_min); break;
    case 28: SetSec(clock->face._5_3, timeinfo.tm_min); break;
    case 29: SetSec(clock->face._5_4, timeinfo.tm_min); break;
    case 30: SetSec(clock->face._6_0, timeinfo.tm_min); break;
    case 31: SetSec(clock->face._6_1, timeinfo.tm_min); break;
    case 32: SetSec(clock->face._6_2, timeinfo.tm_min); break;
    case 33: SetSec(clock->face._6_3, timeinfo.tm_min); break;
    case 34: SetSec(clock->face._6_4, timeinfo.tm_min); break;
    case 35: SetSec(clock->face._7_0, timeinfo.tm_min); break;
    case 36: SetSec(clock->face._7_1, timeinfo.tm_min); break;
    case 37: SetSec(clock->face._7_2, timeinfo.tm_min); break;
    case 38: SetSec(clock->face._7_3, timeinfo.tm_min); break;
    case 39: SetSec(clock->face._7_4, timeinfo.tm_min); break;
    case 40: SetSec(clock->face._8_0, timeinfo.tm_min); break;
    case 41: SetSec(clock->face._8_1, timeinfo.tm_min); break;
    case 42: SetSec(clock->face._8_2, timeinfo.tm_min); break;
    case 43: SetSec(clock->face._8_3, timeinfo.tm_min); break;
    case 44: SetSec(clock->face._8_4, timeinfo.tm_min); break;
    case 45: SetSec(clock->face._9_0, timeinfo.tm_min); break;
    case 46: SetSec(clock->face._9_1, timeinfo.tm_min); break;
    case 47: SetSec(clock->face._9_2, timeinfo.tm_min); break;
    case 48: SetSec(clock->face._9_3, timeinfo.tm_min); break;
    case 49: SetSec(clock->face._9_4, timeinfo.tm_min); break;
    case 50: SetSec(clock->face._10_0, timeinfo.tm_min); break;
    case 51: SetSec(clock->face._10_1, timeinfo.tm_min); break;
    case 52: SetSec(clock->face._10_2, timeinfo.tm_min); break;
    case 53: SetSec(clock->face._10_3, timeinfo.tm_min); break;
    case 54: SetSec(clock->face._10_4, timeinfo.tm_min); break;
    case 55: SetSec(clock->face._11_0, timeinfo.tm_min); break;
    case 56: SetSec(clock->face._11_1, timeinfo.tm_min); break;
    case 57: SetSec(clock->face._11_2, timeinfo.tm_min); break;
    case 58: SetSec(clock->face._11_3, timeinfo.tm_min); break;
    case 59: SetSec(clock->face._11_4, timeinfo.tm_min); break;
    default:
        break;
    }

    // clang-format on
}

union UFaceBits state = {0};
int perotic_timer = 100;
struct tm timeinfo = {0};

void cli_test()
{
    static int g_min = 0;
    static int g_sec = 0;
    static int g_hr = 0;

    if (Serial.available())
    {
        int cmd = Serial.read();
        if (cmd == -1)
            return;
        memset(&state, 0, sizeof(state));
        if (cmd == 'q')
        {
            g_sec += 1;
            printf("sec: %d\n", g_sec);
            timeinfo.tm_sec = g_sec;
            UpdateStateTime(&state, timeinfo);
            WriteOut(DATA, CLOCK, LATCH, state);
        }
        else if (cmd == 'a')
        {
            g_sec -= 1;
            printf("sec: %d\n", g_sec);
            timeinfo.tm_sec = g_sec;
            UpdateStateTime(&state, timeinfo);
            WriteOut(DATA, CLOCK, LATCH, state);
        }
        else if (cmd == 'w')
        {
            g_min += 1;
            printf("min: %d\n", g_min);
            timeinfo.tm_min = g_min;
            UpdateStateTime(&state, timeinfo);
            WriteOut(DATA, CLOCK, LATCH, state);
        }
        else if (cmd == 's')
        {
            g_min -= 1;
            printf("min: %d\n", g_min);
            timeinfo.tm_min = g_min;
            UpdateStateTime(&state, timeinfo);
            WriteOut(DATA, CLOCK, LATCH, state);
        }
        else if (cmd == 'e')
        {
            g_hr += 1;
            printf("hr: %d\n", g_hr);
            timeinfo.tm_hour = g_hr;
            UpdateStateTime(&state, timeinfo);
            WriteOut(DATA, CLOCK, LATCH, state);
        }
        else if (cmd == 'd')
        {
            g_hr -= 1;
            printf("hr: %d\n", g_hr);
            timeinfo.tm_hour = g_hr;
            UpdateStateTime(&state, timeinfo);
            WriteOut(DATA, CLOCK, LATCH, state);
        }
        else
        {
            WriteOut(DATA, CLOCK, LATCH, state);
        }
    }
}

void loop()
{
    // uncomment to allow for qa:sec, ws:min, ed:hr keyboard controls
    // cli_test();
    // return;

    if (0 > (perotic_timer -= 1))
    {

        struct tm timeinfo;
        if (getLocalTime(&timeinfo))
        {
            UpdateStateTime(&state, timeinfo);
            WriteOut(DATA, CLOCK, LATCH, state);
        }

        perotic_timer = 50;
    }

    delay(1);
}
