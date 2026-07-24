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
 * @file      driver_max31856_register_test.c
 * @brief     driver max31856 register test source file
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
 
#include "driver_max31856_register_test.h"
#include <stdlib.h>

static max31856_handle_t gs_handle;        /**< max31856 handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t max31856_register_test(void)
{
    uint8_t res;
    uint8_t status;
    int8_t threshold_reg8;
    int8_t threshold_reg8_check;
    int16_t threshold_reg16;
    int16_t threshold_reg16_check;
    float deg;
    float deg_check;
    max31856_info_t info;
    max31856_bool_t enable;
    max31856_fault_mode_t mode;
    max31856_thermocouple_type_t type;
    max31856_sample_average_t average;
    max31856_noise_rejection_filter_t filter;
    max31856_open_circuit_detection_t detection;

    /* link interface function */
    DRIVER_MAX31856_LINK_INIT(&gs_handle, max31856_handle_t); 
    DRIVER_MAX31856_LINK_SPI_INIT(&gs_handle, max31856_interface_spi_init);
    DRIVER_MAX31856_LINK_SPI_DEINIT(&gs_handle, max31856_interface_spi_deinit);
    DRIVER_MAX31856_LINK_SPI_READ(&gs_handle, max31856_interface_spi_read);
    DRIVER_MAX31856_LINK_SPI_WRITE(&gs_handle, max31856_interface_spi_write);
    DRIVER_MAX31856_LINK_DELAY_MS(&gs_handle, max31856_interface_delay_ms);
    DRIVER_MAX31856_LINK_DEBUG_PRINT(&gs_handle, max31856_interface_debug_print);
    DRIVER_MAX31856_LINK_RECEIVE_CALLBACK(&gs_handle, max31856_interface_receive_callback);
    
    /* get information */
    res = max31856_info(&info);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get info failed.\n");
        
        return 1;
    }
    else
    {
        /* print chip info */
        max31856_interface_debug_print("max31856: chip is %s.\n", info.chip_name);
        max31856_interface_debug_print("max31856: manufacturer is %s.\n", info.manufacturer_name);
        max31856_interface_debug_print("max31856: interface is %s.\n", info.interface);
        max31856_interface_debug_print("max31856: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        max31856_interface_debug_print("max31856: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        max31856_interface_debug_print("max31856: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        max31856_interface_debug_print("max31856: max current is %0.2fmA.\n", info.max_current_ma);
        max31856_interface_debug_print("max31856: max temperature is %0.1fC.\n", info.temperature_max);
        max31856_interface_debug_print("max31856: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    max31856_interface_debug_print("max31856: start register test.\n");
    
    /* init the max31856 */
    res = max31856_init(&gs_handle);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: init failed.\n");
       
        return 1;
    }
    
    /* max31856_set_open_circuit_detection/max31856_get_open_circuit_detection test */
    max31856_interface_debug_print("max31856: max31856_set_open_circuit_detection/max31856_get_open_circuit_detection test.\n");
    
    /* disable open circuit detection */
    res = max31856_set_open_circuit_detection(&gs_handle, MAX31856_OPEN_CIRCUIT_DETECTION_DISABLE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set open circuit detection failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: disable open circuit detection.\n");
    res = max31856_get_open_circuit_detection(&gs_handle, &detection);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get open circuit detection failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check open circuit detection %s.\n", (detection == MAX31856_OPEN_CIRCUIT_DETECTION_DISABLE) ? "ok" : "error");
    
    /* set open circuit detection mode1 */
    res = max31856_set_open_circuit_detection(&gs_handle, MAX31856_OPEN_CIRCUIT_DETECTION_MODE1);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set open circuit detection failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set open circuit detection mode1.\n");
    res = max31856_get_open_circuit_detection(&gs_handle, &detection);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get open circuit detection failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check open circuit detection %s.\n", (detection == MAX31856_OPEN_CIRCUIT_DETECTION_MODE1) ? "ok" : "error");
    
    /* set open circuit detection mode2 */
    res = max31856_set_open_circuit_detection(&gs_handle, MAX31856_OPEN_CIRCUIT_DETECTION_MODE2);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set open circuit detection failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set open circuit detection mode2.\n");
    res = max31856_get_open_circuit_detection(&gs_handle, &detection);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get open circuit detection failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check open circuit detection %s.\n", (detection == MAX31856_OPEN_CIRCUIT_DETECTION_MODE2) ? "ok" : "error");
    
    /* set open circuit detection mode3 */
    res = max31856_set_open_circuit_detection(&gs_handle, MAX31856_OPEN_CIRCUIT_DETECTION_MODE3);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set open circuit detection failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set open circuit detection mode3.\n");
    res = max31856_get_open_circuit_detection(&gs_handle, &detection);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get open circuit detection failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check open circuit detection %s.\n", (detection == MAX31856_OPEN_CIRCUIT_DETECTION_MODE3) ? "ok" : "error");
    
    /* max31856_set_cold_junction_sensor/max31856_get_cold_junction_sensor test */
    max31856_interface_debug_print("max31856: max31856_set_cold_junction_sensor/max31856_get_cold_junction_sensor test.\n");
    
    /* enable cold junction sensor */
    res = max31856_set_cold_junction_sensor(&gs_handle, MAX31856_BOOL_TRUE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set cold junction sensor failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: enable cold junction sensor.\n");
    res = max31856_get_cold_junction_sensor(&gs_handle, &enable);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get cold junction sensor failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check cold junction sensor %s.\n", (enable == MAX31856_BOOL_TRUE) ? "ok" : "error");
    
    /* disable cold junction sensor */
    res = max31856_set_cold_junction_sensor(&gs_handle, MAX31856_BOOL_FALSE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set cold junction sensor failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: disable cold junction sensor.\n");
    res = max31856_get_cold_junction_sensor(&gs_handle, &enable);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get cold junction sensor failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check cold junction sensor %s.\n", (enable == MAX31856_BOOL_FALSE) ? "ok" : "error");
    
    /* max31856_set_fault_mode/max31856_get_fault_mode test */
    max31856_interface_debug_print("max31856: max31856_set_fault_mode/max31856_get_fault_mode test.\n");
    
    /* set fault comparator mode */
    res = max31856_set_fault_mode(&gs_handle, MAX31856_FAULT_MODE_COMPARATOR);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mode failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set fault comparator mode.\n");
    res = max31856_get_fault_mode(&gs_handle, &mode);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get fault mode failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check fault mode %s.\n", (mode == MAX31856_FAULT_MODE_COMPARATOR) ? "ok" : "error");
    
    /* set fault interrupt mode */
    res = max31856_set_fault_mode(&gs_handle, MAX31856_FAULT_MODE_INTERRUPT);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mode failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set fault interrupt mode.\n");
    res = max31856_get_fault_mode(&gs_handle, &mode);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get fault mode failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check fault mode %s.\n", (mode == MAX31856_FAULT_MODE_INTERRUPT) ? "ok" : "error");
    
    /* max31856_set_noise_rejection_filter/max31856_get_noise_rejection_filter test */
    max31856_interface_debug_print("max31856: max31856_set_noise_rejection_filter/max31856_get_noise_rejection_filter test.\n");
    
    /* set noise rejection filter 60hz */
    res = max31856_set_noise_rejection_filter(&gs_handle, MAX31856_NOISE_REJECTION_FILTER_60HZ);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set noise rejection filter failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set noise rejection filter 60hz.\n");
    res = max31856_get_noise_rejection_filter(&gs_handle, &filter);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get noise rejection filter failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check noise rejection filter %s.\n", (filter == MAX31856_NOISE_REJECTION_FILTER_60HZ) ? "ok" : "error");
    
    /* set noise rejection filter 50hz */
    res = max31856_set_noise_rejection_filter(&gs_handle, MAX31856_NOISE_REJECTION_FILTER_50HZ);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set noise rejection filter failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set noise rejection filter 50hz.\n");
    res = max31856_get_noise_rejection_filter(&gs_handle, &filter);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get noise rejection filter failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check noise rejection filter %s.\n", (filter == MAX31856_NOISE_REJECTION_FILTER_50HZ) ? "ok" : "error");
    
    /* max31856_set_sample_average/max31856_get_sample_average test */
    max31856_interface_debug_print("max31856: max31856_set_sample_average/max31856_get_sample_average test.\n");
    
    /* set sample average 1 */
    res = max31856_set_sample_average(&gs_handle, MAX31856_SAMPLE_AVERAGE_1);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set sample average failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set sample average 1.\n");
    res = max31856_get_sample_average(&gs_handle, &average);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get sample average failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check sample average %s.\n", (average == MAX31856_SAMPLE_AVERAGE_1) ? "ok" : "error");
    
    /* set sample average 2 */
    res = max31856_set_sample_average(&gs_handle, MAX31856_SAMPLE_AVERAGE_2);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set sample average failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set sample average 2.\n");
    res = max31856_get_sample_average(&gs_handle, &average);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get sample average failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check sample average %s.\n", (average == MAX31856_SAMPLE_AVERAGE_2) ? "ok" : "error");
    
    /* set sample average 4 */
    res = max31856_set_sample_average(&gs_handle, MAX31856_SAMPLE_AVERAGE_4);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set sample average failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set sample average 4.\n");
    res = max31856_get_sample_average(&gs_handle, &average);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get sample average failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check sample average %s.\n", (average == MAX31856_SAMPLE_AVERAGE_4) ? "ok" : "error");
    
    /* set sample average 8 */
    res = max31856_set_sample_average(&gs_handle, MAX31856_SAMPLE_AVERAGE_8);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set sample average failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set sample average 8.\n");
    res = max31856_get_sample_average(&gs_handle, &average);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get sample average failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check sample average %s.\n", (average == MAX31856_SAMPLE_AVERAGE_8) ? "ok" : "error");
    
    /* set sample average 16 */
    res = max31856_set_sample_average(&gs_handle, MAX31856_SAMPLE_AVERAGE_16);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set sample average failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set sample average 16.\n");
    res = max31856_get_sample_average(&gs_handle, &average);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get sample average failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check sample average %s.\n", (average == MAX31856_SAMPLE_AVERAGE_16) ? "ok" : "error");
    
    /* max31856_set_thermocouple_type/max31856_get_thermocouple_type test */
    max31856_interface_debug_print("max31856: max31856_set_thermocouple_type/max31856_get_thermocouple_type test.\n");
    
    /* set thermocouple type b */
    res = max31856_set_thermocouple_type(&gs_handle, MAX31856_THERMOCOUPLE_TYPE_B);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set thermocouple type b.\n");
    res = max31856_get_thermocouple_type(&gs_handle, &type);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check thermocouple type %s.\n", (type == MAX31856_THERMOCOUPLE_TYPE_B) ? "ok" : "error");
    
    /* set thermocouple type e */
    res = max31856_set_thermocouple_type(&gs_handle, MAX31856_THERMOCOUPLE_TYPE_E);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set thermocouple type e.\n");
    res = max31856_get_thermocouple_type(&gs_handle, &type);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check thermocouple type %s.\n", (type == MAX31856_THERMOCOUPLE_TYPE_E) ? "ok" : "error");
    
    /* set thermocouple type j */
    res = max31856_set_thermocouple_type(&gs_handle, MAX31856_THERMOCOUPLE_TYPE_J);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set thermocouple type j.\n");
    res = max31856_get_thermocouple_type(&gs_handle, &type);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check thermocouple type %s.\n", (type == MAX31856_THERMOCOUPLE_TYPE_J) ? "ok" : "error");
    
    /* set thermocouple type k */
    res = max31856_set_thermocouple_type(&gs_handle, MAX31856_THERMOCOUPLE_TYPE_K);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set thermocouple type k.\n");
    res = max31856_get_thermocouple_type(&gs_handle, &type);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check thermocouple type %s.\n", (type == MAX31856_THERMOCOUPLE_TYPE_K) ? "ok" : "error");
    
    /* set thermocouple type n */
    res = max31856_set_thermocouple_type(&gs_handle, MAX31856_THERMOCOUPLE_TYPE_N);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set thermocouple type n.\n");
    res = max31856_get_thermocouple_type(&gs_handle, &type);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check thermocouple type %s.\n", (type == MAX31856_THERMOCOUPLE_TYPE_N) ? "ok" : "error");
    
    /* set thermocouple type r */
    res = max31856_set_thermocouple_type(&gs_handle, MAX31856_THERMOCOUPLE_TYPE_R);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set thermocouple type r.\n");
    res = max31856_get_thermocouple_type(&gs_handle, &type);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check thermocouple type %s.\n", (type == MAX31856_THERMOCOUPLE_TYPE_R) ? "ok" : "error");
    
    /* set thermocouple type s */
    res = max31856_set_thermocouple_type(&gs_handle, MAX31856_THERMOCOUPLE_TYPE_S);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set thermocouple type s.\n");
    res = max31856_get_thermocouple_type(&gs_handle, &type);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check thermocouple type %s.\n", (type == MAX31856_THERMOCOUPLE_TYPE_S) ? "ok" : "error");
    
    /* set thermocouple type t */
    res = max31856_set_thermocouple_type(&gs_handle, MAX31856_THERMOCOUPLE_TYPE_T);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set thermocouple type t.\n");
    res = max31856_get_thermocouple_type(&gs_handle, &type);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check thermocouple type %s.\n", (type == MAX31856_THERMOCOUPLE_TYPE_T) ? "ok" : "error");
    
    /* set thermocouple voltage gain 8 */
    res = max31856_set_thermocouple_type(&gs_handle, MAX31856_THERMOCOUPLE_TYPE_VOLTAGE_GAIN_8);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set thermocouple voltage gain 8.\n");
    res = max31856_get_thermocouple_type(&gs_handle, &type);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check thermocouple type %s.\n", (type == MAX31856_THERMOCOUPLE_TYPE_VOLTAGE_GAIN_8) ? "ok" : "error");
    
    /* set thermocouple voltage gain 32 */
    res = max31856_set_thermocouple_type(&gs_handle, MAX31856_THERMOCOUPLE_TYPE_VOLTAGE_GAIN_32);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set thermocouple voltage gain 32.\n");
    res = max31856_get_thermocouple_type(&gs_handle, &type);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check thermocouple type %s.\n", (type == MAX31856_THERMOCOUPLE_TYPE_VOLTAGE_GAIN_32) ? "ok" : "error");
    
    /* max31856_set_fault_mask/max31856_get_fault_mask test */
    max31856_interface_debug_print("max31856: max31856_set_fault_mask/max31856_get_fault_mask test.\n");
    
    /* enable cold junction high fault threshold mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_CJHIGH, MAX31856_BOOL_TRUE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: enable cold junction high fault threshold mask.\n");
    res = max31856_get_fault_mask(&gs_handle, MAX31856_FAULT_MASK_CJHIGH, &enable);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check fault mask %s.\n", (enable == MAX31856_BOOL_TRUE) ? "ok" : "error");
    
    /* disable cold junction high fault threshold mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_CJHIGH, MAX31856_BOOL_FALSE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: disable cold junction high fault threshold mask.\n");
    res = max31856_get_fault_mask(&gs_handle, MAX31856_FAULT_MASK_CJHIGH, &enable);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check fault mask %s.\n", (enable == MAX31856_BOOL_FALSE) ? "ok" : "error");
    
    /* enable cold junction low fault threshold mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_CJLOW, MAX31856_BOOL_TRUE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: enable cold junction low fault threshold mask.\n");
    res = max31856_get_fault_mask(&gs_handle, MAX31856_FAULT_MASK_CJLOW, &enable);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check fault mask %s.\n", (enable == MAX31856_BOOL_TRUE) ? "ok" : "error");
    
    /* disable cold junction low fault threshold mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_CJLOW, MAX31856_BOOL_FALSE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: disable cold junction low fault threshold mask.\n");
    res = max31856_get_fault_mask(&gs_handle, MAX31856_FAULT_MASK_CJLOW, &enable);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check fault mask %s.\n", (enable == MAX31856_BOOL_FALSE) ? "ok" : "error");
    
    /* enable thermocouple temperature high fault threshold mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_TCHIGH, MAX31856_BOOL_TRUE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: enable thermocouple temperature high fault threshold mask.\n");
    res = max31856_get_fault_mask(&gs_handle, MAX31856_FAULT_MASK_TCHIGH, &enable);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check fault mask %s.\n", (enable == MAX31856_BOOL_TRUE) ? "ok" : "error");
    
    /* disable thermocouple temperature high fault threshold mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_TCHIGH, MAX31856_BOOL_FALSE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: disable thermocouple temperature high fault threshold mask.\n");
    res = max31856_get_fault_mask(&gs_handle, MAX31856_FAULT_MASK_TCHIGH, &enable);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check fault mask %s.\n", (enable == MAX31856_BOOL_FALSE) ? "ok" : "error");
    
    /* enable thermocouple temperature low fault threshold mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_TCLOW, MAX31856_BOOL_TRUE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: enable thermocouple temperature low fault threshold mask.\n");
    res = max31856_get_fault_mask(&gs_handle, MAX31856_FAULT_MASK_TCLOW, &enable);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check fault mask %s.\n", (enable == MAX31856_BOOL_TRUE) ? "ok" : "error");
    
    /* disable thermocouple temperature low fault threshold mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_TCLOW, MAX31856_BOOL_FALSE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: disable thermocouple temperature low fault threshold mask.\n");
    res = max31856_get_fault_mask(&gs_handle, MAX31856_FAULT_MASK_TCLOW, &enable);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check fault mask %s.\n", (enable == MAX31856_BOOL_FALSE) ? "ok" : "error");
    
    /* enable over voltage or undervoltage input fault mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_OVUV, MAX31856_BOOL_TRUE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: enable over voltage or undervoltage input fault mask.\n");
    res = max31856_get_fault_mask(&gs_handle, MAX31856_FAULT_MASK_OVUV, &enable);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check fault mask %s.\n", (enable == MAX31856_BOOL_TRUE) ? "ok" : "error");
    
    /* disable over voltage or undervoltage input fault mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_OVUV, MAX31856_BOOL_FALSE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: disable over voltage or undervoltage input fault mask.\n");
    res = max31856_get_fault_mask(&gs_handle, MAX31856_FAULT_MASK_OVUV, &enable);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check fault mask %s.\n", (enable == MAX31856_BOOL_FALSE) ? "ok" : "error");
    
    /* enable thermocouple open circuit fault mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_OPEN, MAX31856_BOOL_TRUE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: enable thermocouple open circuit fault mask.\n");
    res = max31856_get_fault_mask(&gs_handle, MAX31856_FAULT_MASK_OPEN, &enable);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check fault mask %s.\n", (enable == MAX31856_BOOL_TRUE) ? "ok" : "error");
    
    /* disable thermocouple open circuit fault mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_OPEN, MAX31856_BOOL_FALSE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: disable thermocouple open circuit fault mask.\n");
    res = max31856_get_fault_mask(&gs_handle, MAX31856_FAULT_MASK_OPEN, &enable);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check fault mask %s.\n", (enable == MAX31856_BOOL_FALSE) ? "ok" : "error");
    
    /* max31856_set_cold_junction_high_fault_threshold/max31856_get_cold_junction_high_fault_threshold test */
    max31856_interface_debug_print("max31856: max31856_set_cold_junction_high_fault_threshold/max31856_get_cold_junction_high_fault_threshold test.\n");
    
    threshold_reg8 = rand() % 128;
    res = max31856_set_cold_junction_high_fault_threshold(&gs_handle, threshold_reg8);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set cold junction high fault threshold failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set cold junction high fault threshold %d.\n", threshold_reg8);
    res = max31856_get_cold_junction_high_fault_threshold(&gs_handle, &threshold_reg8_check);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get cold junction high fault threshold failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check cold junction high fault threshold %s.\n", (threshold_reg8 == threshold_reg8_check) ? "ok" : "error");
    
    /* max31856_set_cold_junction_low_fault_threshold/max31856_get_cold_junction_low_fault_threshold test */
    max31856_interface_debug_print("max31856: max31856_set_cold_junction_low_fault_threshold/max31856_get_cold_junction_low_fault_threshold test.\n");
    
    threshold_reg8 = -(rand() % 128);
    res = max31856_set_cold_junction_low_fault_threshold(&gs_handle, threshold_reg8);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set cold junction low fault threshold failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set cold junction low fault threshold %d.\n", threshold_reg8);
    res = max31856_get_cold_junction_low_fault_threshold(&gs_handle, &threshold_reg8_check);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get cold junction low fault threshold failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check cold junction low fault threshold %s.\n", (threshold_reg8 == threshold_reg8_check) ? "ok" : "error");
    
    /* max31856_set_temperature_high_fault_threshold/max31856_get_temperature_high_fault_threshold test */
    max31856_interface_debug_print("max31856: max31856_set_temperature_high_fault_threshold/max31856_get_temperature_high_fault_threshold test.\n");
    
    threshold_reg16 = rand() % 32768;
    res = max31856_set_temperature_high_fault_threshold(&gs_handle, threshold_reg16);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set temperature high fault threshold failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set temperature high fault threshold %d.\n", threshold_reg16);
    res = max31856_get_temperature_high_fault_threshold(&gs_handle, &threshold_reg16_check);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get temperature high fault threshold failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check temperature high fault threshold %s.\n", (threshold_reg16 == threshold_reg16_check) ? "ok" : "error");
    
    /* max31856_set_temperature_low_fault_threshold/max31856_get_temperature_low_fault_threshold test */
    max31856_interface_debug_print("max31856: max31856_set_temperature_low_fault_threshold/max31856_get_temperature_low_fault_threshold test.\n");
    
    threshold_reg16 = -(rand() % 32768);
    res = max31856_set_temperature_low_fault_threshold(&gs_handle, threshold_reg16);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set temperature low fault threshold failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set temperature low fault threshold %d.\n", threshold_reg16);
    res = max31856_get_temperature_low_fault_threshold(&gs_handle, &threshold_reg16_check);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get temperature low fault threshold failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check temperature low fault threshold %s.\n", (threshold_reg16 == threshold_reg16_check) ? "ok" : "error");
    
    /* max31856_set_cold_junction_temperature_offset/max31856_get_cold_junction_temperature_offset test */
    max31856_interface_debug_print("max31856: max31856_set_cold_junction_temperature_offset/max31856_get_cold_junction_temperature_offset test.\n");
    
    threshold_reg8 = -(rand() % 128);
    res = max31856_set_cold_junction_temperature_offset(&gs_handle, threshold_reg8);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set cold junction temperature offset failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set cold junction temperature offset %d.\n", threshold_reg8);
    res = max31856_get_cold_junction_temperature_offset(&gs_handle, &threshold_reg8_check);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get cold junction temperature offset failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check cold junction temperature offset %s.\n", (threshold_reg8 == threshold_reg8_check) ? "ok" : "error");
    
    /* max31856_set_cold_junction_temperature/max31856_get_cold_junction_temperature test */
    max31856_interface_debug_print("max31856: max31856_set_cold_junction_temperature/max31856_get_cold_junction_temperature test.\n");
    
    threshold_reg16 = rand() % 32768;
    res = max31856_set_cold_junction_temperature(&gs_handle, threshold_reg16);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set cold junction temperature failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set cold junction temperature %d.\n", threshold_reg16);
    res = max31856_get_cold_junction_temperature(&gs_handle, &threshold_reg16_check);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get cold junction temperature failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check cold junction temperature %d.\n", threshold_reg16_check);
    
    /* max31856_clear_fault test */
    max31856_interface_debug_print("max31856: max31856_clear_fault test.\n");
    
    /* clear fault */
    res = max31856_clear_fault(&gs_handle);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: clear fault failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check clear fault %s.\n", (res == 0) ? "ok" : "error");
    
    /* max31856_get_fault_status test */
    max31856_interface_debug_print("max31856: max31856_get_fault_status test.\n");
    
    /* get fault status */
    res = max31856_get_fault_status(&gs_handle, &status);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get fault status failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: status is 0x%02X.\n", status);
    
    /* max31856_cold_junction_fault_threshold_convert_to_register/max31856_cold_junction_fault_threshold_convert_to_data test */
    max31856_interface_debug_print("max31856: max31856_cold_junction_fault_threshold_convert_to_register/max31856_cold_junction_fault_threshold_convert_to_data test.\n");
    
    deg = (float)(rand() % 100) / 1000.0f + 2.0f;
    res = max31856_cold_junction_fault_threshold_convert_to_register(&gs_handle, deg, &threshold_reg8);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: cold junction fault threshold convert to register failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set cold junction fault threshold %0.3fC.\n", deg);
    res = max31856_cold_junction_fault_threshold_convert_to_data(&gs_handle, threshold_reg8, &deg_check);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: cold junction fault threshold convert to data failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check cold junction fault threshold %0.3fC.\n", deg_check);
    
    /* max31856_temperature_fault_threshold_convert_to_register/max31856_temperature_fault_threshold_convert_to_data test */
    max31856_interface_debug_print("max31856: max31856_temperature_fault_threshold_convert_to_register/max31856_temperature_fault_threshold_convert_to_data test.\n");
    
    deg = (float)(rand() % 1000) / 20.0f;
    res = max31856_temperature_fault_threshold_convert_to_register(&gs_handle, deg, &threshold_reg16);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: temperature fault threshold convert to register failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set temperature fault threshold %0.3fC.\n", deg);
    res = max31856_temperature_fault_threshold_convert_to_data(&gs_handle, threshold_reg16, &deg_check);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: temperature fault threshold convert to data failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check temperature fault threshold %0.3fC.\n", deg_check);
    
    /* max31856_cold_junction_temperature_offset_convert_to_register/max31856_cold_junction_temperature_offset_convert_to_data test */
    max31856_interface_debug_print("max31856: max31856_cold_junction_temperature_offset_convert_to_register/max31856_cold_junction_temperature_offset_convert_to_data test.\n");
    
    deg = (float)(rand() % 1000) / 500.0f;
    res = max31856_cold_junction_temperature_offset_convert_to_register(&gs_handle, deg, &threshold_reg8);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: cold junction temperature offset convert to register failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set cold junction temperature offset %0.3fC.\n", deg);
    res = max31856_cold_junction_temperature_offset_convert_to_data(&gs_handle, threshold_reg8, &deg_check);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: cold junction temperature offset convert to data failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check cold junction temperature offset %0.3fC.\n", deg_check);
    
    /* max31856_cold_junction_temperature_convert_to_register/max31856_cold_junction_temperature_convert_to_data test */
    max31856_interface_debug_print("max31856: max31856_cold_junction_temperature_convert_to_register/max31856_cold_junction_temperature_convert_to_data test.\n");
    
    deg = (float)(rand() % 1000) / 50.0f;
    res = max31856_cold_junction_temperature_convert_to_register(&gs_handle, deg, &threshold_reg16);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: cold junction temperature convert to register failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: set cold junction temperature %0.3fC.\n", deg);
    res = max31856_cold_junction_temperature_convert_to_data(&gs_handle, threshold_reg16, &deg_check);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: cold junction temperature convert to data failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    max31856_interface_debug_print("max31856: check cold junction temperature %0.3fC.\n", deg_check);
    
    /* finish register */
    max31856_interface_debug_print("max31856: finish register test.\n");
    (void)max31856_deinit(&gs_handle);
    
    return 0;
}
