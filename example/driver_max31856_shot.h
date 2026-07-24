/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_max31856_shot.h
 * @brief     driver max31856 shot include file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2026-07-08
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2026/07/08  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_MAX31856_SHOT_H
#define DRIVER_MAX31856_SHOT_H

#include "driver_max31856_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup max31856_example_driver
 * @{
 */

/**
 * @brief max31856 shot example default definition
 */
#define MAX31856_SHOT_DEFAULT_OPEN_CIRCUIT_DETECTION                  MAX31856_OPEN_CIRCUIT_DETECTION_MODE3        /**< 113.4ms / 140ms */
#define MAX31856_SHOT_DEFAULT_FAULT_MODE                              MAX31856_FAULT_MODE_INTERRUPT                /**< interrupt mode */
#define MAX31856_SHOT_DEFAULT_NOISE_REJECTION_FILTER                  MAX31856_NOISE_REJECTION_FILTER_50HZ         /**< 50hz */
#define MAX31856_SHOT_DEFAULT_SAMPLE_AVERAGE                          MAX31856_SAMPLE_AVERAGE_16                   /**< 16 samples */
#define MAX31856_SHOT_DEFAULT_COLD_JUNCTION_TEMPERATURE_OFFSET        0.0f

/**
 * @brief     shot example init
 * @param[in] type thermocouple type
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t max31856_shot_init(max31856_thermocouple_type_t type);

/**
 * @brief  shot example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t max31856_shot_deinit(void);

/**
 * @brief      shot example read the temperature
 * @param[out] *temp_deg pointer to a converted temperature buffer
 * @param[out] *fault pointer to a fault buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t max31856_shot_read(double *temp_deg, uint8_t *fault);

/**
 * @brief      shot example read the cold junction temperature
 * @param[out] *temp_deg pointer to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t max31856_shot_read_cold_junction_temperature(double *temp_deg);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
