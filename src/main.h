
#pragma once

/**
 * OLED screen
 */
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

/**
 * BME280
 */
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

// bme280_val
typedef struct
{
    float temp;
    float pres;
    float alt;
    float hum;
} bme280_val;
