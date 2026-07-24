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
 * @file      driver_max31856_interrupt_test.h
 * @brief     driver max31856 interrupt test header file
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

#ifndef DRIVER_MAX31856_INTERRUPT_TEST_H
#define DRIVER_MAX31856_INTERRUPT_TEST_H

#include "driver_max31856_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup max31856_test_driver
 * @{
 */

/**
 * @brief  interrupt test irq callback
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t max31856_interrupt_test_irq_handler(void);

/**
 * @brief     interrupt test
 * @param[in] type thermocouple type
 * @param[in] cold_junction_low_fault_threshold_deg cold junction low fault threshold
 * @param[in] cold_junction_high_fault_threshold_deg cold junction high fault threshold
 * @param[in] temperature_low_fault_threshold_deg temperature low fault threshold
 * @param[in] temperature_high_fault_threshold_deg temperature high fault threshold
 * @param[in] timeout timeout in ms
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t max31856_interrupt_test(max31856_thermocouple_type_t type,
                                float cold_junction_low_fault_threshold_deg,
                                float cold_junction_high_fault_threshold_deg,
                                float temperature_low_fault_threshold_deg,
                                float temperature_high_fault_threshold_deg,
                                uint32_t timeout);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
