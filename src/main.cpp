/**
 * M 5 S T A C K    W E A T H E R    S T A T I O N
 *
 * Copyright (C) 2019  Nicolas Jeanmonod, ouilogique.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <Arduino.h>
#include <M5Stack.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <main.h>
#include <weath_bme280.h>

weath_bme280 bme;

/**
 *
 */
void print2Serial(bme280_t bme280_val)
{
    Serial.print("Temperature = ");
    Serial.print(bme280_val.temp, 1);
    Serial.println(" *C");

    Serial.print("Pressure = ");
    Serial.print(bme280_val.pres, 0);
    Serial.println(" hPa");

    Serial.print("Humidity = ");
    Serial.print(bme280_val.hum, 1);
    Serial.println(" %RH");

    Serial.println("\n##########\n");
}

/**
 *
 */
void drawDegreeGlyph(int32_t degx0, int32_t degy0, uint32_t font_color)
{
    M5.Lcd.fillCircle(degx0, degy0, 11, font_color);
    M5.Lcd.fillCircle(degx0, degy0, 7, BLACK);
}

/**
 *
 */
void printLine2OLED(float val, const char formatStr[], char str[], char strPrev[],
                    int32_t pox_x, int32_t pox_y,
                    uint8_t font_size, uint16_t font_color, uint16_t bkg_color)
{
    // Format string to display.
    sprintf(str, formatStr, val);

    // Set text size.
    M5.Lcd.setTextSize(font_size);

    // Rewriting old measurements with background color
    // to reduce the screen flicker.
    M5.Lcd.setTextColor(bkg_color);
    M5.Lcd.drawString(strPrev, pox_x, pox_y, GFXFF);

    // Print new measurement to screen.
    M5.Lcd.setTextColor(font_color);
    M5.Lcd.drawString(str, pox_x, pox_y, GFXFF);

    // Keep a copy of the measurement.
    strcpy(strPrev, str);
}

/**
 *
 */
void print2OLED(bme280_t bme280_val)
{
    const int8_t NB_LINES = 3;
    const int8_t NB_CHAR = 10;
    static char str[NB_LINES][NB_CHAR];
    static char strPrev[NB_LINES][NB_CHAR];

    // Temperature.
    printLine2OLED(bme280_val.temp, "%.1f *C", str[0], strPrev[0],
                   160, 50,
                   2, GREENYELLOW, BLACK);
    drawDegreeGlyph(210, 50 - 14, GREENYELLOW);

    // Humidity.
    printLine2OLED(bme280_val.hum, "%.1f %%RH", str[1], strPrev[1],
                   160, 125,
                   1, YELLOW, BLACK);

    // Pressure.
    printLine2OLED(bme280_val.pres, "%.0f hPa", str[2], strPrev[2],
                   160, 190,
                   1, YELLOW, BLACK);
}

/**
 *
 */
void setupM5()
{
    // Serial is not started with M5 lib
    // otherwise baud rate cannot be changed.
    M5.begin(true, false, false, true);
    M5.Lcd.setBrightness(128);
    M5.Lcd.fillScreen(BKG_COLOR);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setFreeFont(CURRENT_FONT);
}

/**
 *
 */
void setupSerial()
{
    Serial.begin(BAUD_RATE);
    Serial.print("\n\n##########################");
    Serial.print("\nCOMPILATION DATE AND TIME:\n");
    Serial.print(__DATE__);
    Serial.print("\n");
    Serial.print(__TIME__);
    Serial.print("\n##########################\n\n");
}

/**
 *
 */
void setup()
{
    setupSerial();
    setupM5();
    bme.setup();
}

/**
 *
 */
void loop()
{
    // Measure weather data.
    // Resume loop if values have not changed.
    bme280_t bme280_val;
    bool valChanged = bme.readAll(&bme280_val);
    if (!valChanged)
    {
        delay(1000);
        return;
    }

    // Print values to serial.
    print2Serial(bme280_val);

    // Print to OLED
    print2OLED(bme280_val);

 delay(1000);

// #define TIME_TO_SLEEP 1
//     esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * 1000000);
//     Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
//                    " Seconds");
//     esp_deep_sleep_start();
}
