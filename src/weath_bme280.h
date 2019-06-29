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

#pragma once

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

// bme280_t
typedef struct
{
    float temp;
    float pres;
    float hum;
    bool valChanged;
} bme280_t;

class weath_bme280 : public Adafruit_BME280
{
private:
    bme280_t _bme280_val;
    bme280_t _bme280_prev;

public:
    weath_bme280();
    bool setup();
    bool readAll(bme280_t *bme280_val);
};
