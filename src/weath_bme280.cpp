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

#include <weath_bme280.h>

weath_bme280::weath_bme280() {}

float rounding(float num, float dec)
{
    float res;
    if (dec == 0)
        res = round(num);
    else
        res = round(num / dec) * dec;
    return res;
}

bool weath_bme280::setup()
{
    // default settings
    bool status = begin();
    return status;
}

bool weath_bme280::readAll(bme280_t *bme280_val)
{
    _bme280_val.temp = rounding(readTemperature(), 0.1);
    _bme280_val.pres = rounding(readPressure() / 100.0F, 0.0);
    _bme280_val.hum = rounding(readHumidity(), 0.1);

    bool valChanged =
        _bme280_val.temp != _bme280_prev.temp ||
        _bme280_val.pres != _bme280_prev.pres ||
        _bme280_val.hum != _bme280_prev.hum;
    _bme280_val.valChanged = valChanged;

    *bme280_val = _bme280_val;
    _bme280_prev = _bme280_val;
    return valChanged;
}
