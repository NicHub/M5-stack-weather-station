/**
 * M5-stack-temperature
 */

#include <Arduino.h>
#include <M5Stack.h>
#define GFXFF 1

#define CF_OL24 &Orbitron_Light_24
#define CF_OL32 &Orbitron_Light_32
#define CF_RT24 &Roboto_Thin_24
#define CF_S24 &Satisfy_24
#define CF_Y32 &Yellowtail_32
#define GF_FSB24 &FreeSansBold24pt7b
#define GF_FSB18 &FreeSansBold18pt7b
#define GF_FSB12 &FreeSansBold12pt7b
#define CURRENT_FONT GF_FSB18
#define FONT_SIZE 2
#define FONT_COLOR GREENYELLOW
#define BKG_COLOR BLACK

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

/**
 *
 */
#ifdef __cplusplus
extern "C"
{
#endif
    uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

/**
 *
 */
bool getTemperature(char temperatureStr[])
{
    // float temp = (temprature_sens_read() - 32) / 1.8;
    float temp = bme.readTemperature();
    temp  = round(10 * temp) / 10;
#if false
    static float rand = 0.1;
    rand += 0.1;
    temp += rand;
#endif
    static float tempPrev = -1E6;
    bool tempHasChanged = (temp != tempPrev);
    tempPrev = temp;
    sprintf(temperatureStr, "%.1f _C", temp);
    return tempHasChanged;
}

void readBME80()
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
void setup()
{
    Serial.begin(BAUD_RATE);
    M5.begin();
    M5.Lcd.fillScreen(BKG_COLOR);
    M5.Lcd.setTextSize(FONT_SIZE);
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.setFreeFont(CURRENT_FONT);

    bool status;

    // default settings
    status = bme.begin();
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

    readBME80();

    // Print temperature to serial.
    Serial.println(temperatureStr);

    // Rewrite old measurement with background color.
    // This reduces the flickering.
    M5.Lcd.setTextColor(BKG_COLOR);
    M5.Lcd.drawString(temperatureStrPrev, 160, 95, GFXFF);

    // Print new measurement to screen.
    M5.Lcd.setTextColor(FONT_COLOR);
    M5.Lcd.drawString(temperatureStr, 160, 95, GFXFF);

    // Keep a copy of the measurement.
    strcpy(temperatureStrPrev, temperatureStr);

    delay(1000);
}
