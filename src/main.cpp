/**
 * M5-stack-weather-station
 */

#include <Arduino.h>
#include <M5Stack.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <main.h>

Adafruit_BME280 bme;

/**
 *
 */
bool getTemperature(char temperatureStr[])
{
    float temp = bme.readTemperature();
    temp = round(10 * temp) / 10;
    static float tempPrev = -1E6;
    bool tempHasChanged = (temp != tempPrev);
    tempPrev = temp;
    sprintf(temperatureStr, "%.1f *C", temp);
    return tempHasChanged;
}

void readBME280()
{
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}

/**
 *
 */
void drawDegreeGlyph(int32_t degx0, int32_t degy0)
{
    M5.Lcd.fillCircle(degx0, degy0, 11, FONT_COLOR);
    M5.Lcd.fillCircle(degx0, degy0, 7, BLACK);
}

/**
 *
 */
void setup()
{
    Serial.begin(BAUD_RATE);
    M5.begin();
    M5.Lcd.setBrightness(128);
    M5.Lcd.fillScreen(BKG_COLOR);
    M5.Lcd.setTextSize(FONT_SIZE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setFreeFont(CURRENT_FONT);

    // default settings
    bool status = bme.begin();
    if (!status)
    {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1)
            ;
    }
}

/**
 *
 */
void loop()
{
    const int8_t NB_CHAR = 10;
    static char temperatureStr[NB_CHAR];
    static char temperatureStrPrev[NB_CHAR];

    // Measure temperature.
    // Return if temperature has not changed
    bool tempHasChanged = getTemperature(&temperatureStr[0]);
    if (!tempHasChanged)
    {
        delay(1000);
        return;
    }

    readBME280();

    // Print temperature to serial.
    Serial.println(temperatureStr);

    // Rewrite old measurement with background color.
    // This reduces the flickering.
    M5.Lcd.setTextColor(BKG_COLOR);
    M5.Lcd.drawString(temperatureStrPrev, 160, 95, GFXFF);

    // Print new measurement to screen.
    M5.Lcd.setTextColor(FONT_COLOR);
    M5.Lcd.drawString(temperatureStr, 160, 95, GFXFF);
    drawDegreeGlyph(210, 81);

    // Keep a copy of the measurement.
    strcpy(temperatureStrPrev, temperatureStr);

    delay(1000);
}
