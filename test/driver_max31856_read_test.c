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
 * @file      driver_max31856_read_test.c
 * @brief     driver max31856 read test source file
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
 
#include "driver_max31856_read_test.h"

static max31856_handle_t gs_handle;        /**< max31856 handle */

/**
 * @brief     read test
 * @param[in] type thermocouple type
 * @param[in] times test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t max31856_read_test(max31856_thermocouple_type_t type, uint32_t times)
{
    uint8_t res;
    int8_t offset;
    uint32_t i;
    max31856_info_t info;
    
    /* link interface function */
    DRIVER_MAX31856_LINK_INIT(&gs_handle, max31856_handle_t); 
    DRIVER_MAX31856_LINK_SPI_INIT(&gs_handle, max31856_interface_spi_init);
    DRIVER_MAX31856_LINK_SPI_DEINIT(&gs_handle, max31856_interface_spi_deinit);
    DRIVER_MAX31856_LINK_SPI_READ(&gs_handle, max31856_interface_spi_read);
    DRIVER_MAX31856_LINK_SPI_WRITE(&gs_handle, max31856_interface_spi_write);
    DRIVER_MAX31856_LINK_DELAY_MS(&gs_handle, max31856_interface_delay_ms);
    DRIVER_MAX31856_LINK_DEBUG_PRINT(&gs_handle, max31856_interface_debug_print);
    DRIVER_MAX31856_LINK_RECEIVE_CALLBACK(&gs_handle, max31856_interface_receive_callback);
    
    /* max31856 info */
    res = max31856_info(&info);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
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
    
    /* start read test */
    max31856_interface_debug_print("max31856: start read test.\n");
    
    /* max31856 init */
    res = max31856_init(&gs_handle);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: init failed.\n");
       
        return 1;
    }
    
    /* set thermocouple type */
    res = max31856_set_thermocouple_type(&gs_handle, type);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set thermocouple type failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set open circuit detection mode3 */
    res = max31856_set_open_circuit_detection(&gs_handle, MAX31856_OPEN_CIRCUIT_DETECTION_MODE3);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set open circuit detection failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable cold junction sensor */
    res = max31856_set_cold_junction_sensor(&gs_handle, MAX31856_BOOL_TRUE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set cold junction sensor failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set fault interrupt mode */
    res = max31856_set_fault_mode(&gs_handle, MAX31856_FAULT_MODE_INTERRUPT);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mode failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set noise rejection filter 50hz */
    res = max31856_set_noise_rejection_filter(&gs_handle, MAX31856_NOISE_REJECTION_FILTER_50HZ);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set noise rejection filter failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set sample average 16 */
    res = max31856_set_sample_average(&gs_handle, MAX31856_SAMPLE_AVERAGE_16);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set sample average failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable cold junction high fault threshold mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_CJHIGH, MAX31856_BOOL_TRUE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable cold junction low fault threshold mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_CJLOW, MAX31856_BOOL_TRUE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable thermocouple temperature high fault threshold mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_TCHIGH, MAX31856_BOOL_TRUE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable thermocouple temperature low fault threshold mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_TCLOW, MAX31856_BOOL_TRUE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable over voltage or undervoltage input fault mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_OVUV, MAX31856_BOOL_TRUE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enable thermocouple open circuit fault mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_OPEN, MAX31856_BOOL_TRUE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 0.0 offset */
    res = max31856_cold_junction_temperature_offset_convert_to_register(&gs_handle, 0.0f, &offset);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: cold junction temperature offset convert to register failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set cold junction temperature offset */
    res = max31856_set_cold_junction_temperature_offset(&gs_handle, offset);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set cold junction temperature offset failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear fault */
    res = max31856_clear_fault(&gs_handle);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: clear fault failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* cold junction temperature read test */
    max31856_interface_debug_print("max31856: cold junction temperature read test.\n");
    
    for (i = 0; i < times; i++)
    {
        int16_t raw;
        double deg;
        
        /* delay 1000ms */
        max31856_interface_delay_ms(1000);
        
        /* read temperature */
        res = max31856_read_cold_junction_temperature(&gs_handle, &raw, &deg);
        if (res != 0)
        {
            max31856_interface_debug_print("max31856: read cold junction temperature failed.\n");
            (void)max31856_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        max31856_interface_debug_print("max31856: cold junction temperature is %0.03fC.\n", deg);
    }
    
    /* single read test */
    max31856_interface_debug_print("max31856: single read test.\n");
    
    for (i = 0; i < times; i++)
    {
        int32_t raw;
        double deg;
        
        /* delay 1000ms */
        max31856_interface_delay_ms(1000);
        
        /* single read */
        res = max31856_single_read(&gs_handle, &raw, &deg);
        if (res != 0)
        {
            max31856_interface_debug_print("max31856: single read failed.\n");
            (void)max31856_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        max31856_interface_debug_print("max31856: temperature is %0.03fC.\n", deg);
    }
    
    /* continuous read test */
    max31856_interface_debug_print("max31856: continuous read test.\n");
    
    /* start continuous read */
    res = max31856_start_continuous_read(&gs_handle);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: start continuous read failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    for (i = 0; i < times; i++)
    {
        int32_t raw;
        double deg;
        
        /* delay 1000ms */
        max31856_interface_delay_ms(1000);
        
        /* continuous read */
        res = max31856_continuous_read(&gs_handle, &raw, &deg);
        if (res != 0)
        {
            max31856_interface_debug_print("max31856: continuous read failed.\n");
            (void)max31856_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        max31856_interface_debug_print("max31856: temperature is %0.03fC.\n", deg);
    }
    
    /* stop continuous read */
    res = max31856_stop_continuous_read(&gs_handle);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: stop continuous read failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish read test */
    max31856_interface_debug_print("max31856: finish read test.\n");  
    (void)max31856_deinit(&gs_handle);
    
    return 0;
}
