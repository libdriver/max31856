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
 * @file      driver_max31856_interrupt_test.c
 * @brief     driver max31856 interrupt test source file
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
 
#include "driver_max31856_interrupt_test.h"

static max31856_handle_t gs_handle;        /**< max31856 handle */
static uint8_t gs_flag = 0;                /**< local flag */

/**
 * @brief     interface receive callback
 * @param[in] type callback type
 * @note      none
 */
static void a_callback(uint8_t type)
{
    switch (type)
    {
        case MAX31856_FAULT_STATUS_CJRANGE :
        {
            max31856_interface_debug_print("max31856: irq cold junction out of range.\n");
            gs_flag = 1;
            
            break;
        }
        case MAX31856_FAULT_STATUS_TCRANGE :
        {
            max31856_interface_debug_print("max31856: irq thermocouple out of range.\n");
            gs_flag = 1;
            
            break;
        }
        case MAX31856_FAULT_STATUS_CJHIGH :
        {
            uint8_t res;
            int16_t raw;
            double deg;
            
            max31856_interface_debug_print("max31856: irq cold junction high fault.\n");
            gs_flag = 1;
            
            /* read cold junction temperature */
            res = max31856_read_cold_junction_temperature(&gs_handle, &raw, &deg);
            if (res == 0)
            {
                /* output */
                max31856_interface_debug_print("max31856: cold junction temperature is %0.03fC.\n", deg);
            }
            
            
            break;
        }
        case MAX31856_FAULT_STATUS_CJLOW :
        {
            uint8_t res;
            int16_t raw;
            double deg;
            
            max31856_interface_debug_print("max31856: irq cold junction low fault.\n");
            gs_flag = 1;
            
            /* read cold junction temperature */
            res = max31856_read_cold_junction_temperature(&gs_handle, &raw, &deg);
            if (res == 0)
            {
                /* output */
                max31856_interface_debug_print("max31856: cold junction temperature is %0.03fC.\n", deg);
            }
            
            break;
        }
        case MAX31856_FAULT_STATUS_TCHIGH :
        {
            uint8_t res;
            int32_t raw;
            double deg;
            
            max31856_interface_debug_print("max31856: irq thermocouple temperature high fault.\n");
            gs_flag = 1;
            
            /* continuous read */
            res = max31856_continuous_read(&gs_handle, &raw, &deg);
            if (res == 0)
            {
                /* output */
                max31856_interface_debug_print("max31856: temperature is %0.03fC.\n", deg);
            }
            
            break;
        }
        case MAX31856_FAULT_STATUS_TCLOW :
        {
            uint8_t res;
            int32_t raw;
            double deg;
            
            max31856_interface_debug_print("max31856: irq thermocouple temperature low fault.\n");
            gs_flag = 1;
            
            /* continuous read */
            res = max31856_continuous_read(&gs_handle, &raw, &deg);
            if (res == 0)
            {
                /* output */
                max31856_interface_debug_print("max31856: temperature is %0.03fC.\n", deg);
            }
            
            break;
        }
        case MAX31856_FAULT_STATUS_OVUV :
        {
            max31856_interface_debug_print("max31856: irq overvoltage or undervoltage input fault.\n");
            gs_flag = 1;
            
            break;
        }
        case MAX31856_FAULT_STATUS_OPEN :
        {
            max31856_interface_debug_print("max31856: irq thermocouple open circuit fault.\n");
            gs_flag = 1;
            
            break;
        }
        default :
        {
            max31856_interface_debug_print("max31856: unknown code.\n");
            gs_flag = 1;
            
            break;
        }
    }
}

/**
 * @brief  interrupt test irq callback
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t max31856_interrupt_test_irq_handler(void)
{
    if (max31856_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

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
                                uint32_t timeout)
{
    uint8_t res;
    int8_t offset;
    int8_t reg8;
    int16_t reg16;
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
    DRIVER_MAX31856_LINK_RECEIVE_CALLBACK(&gs_handle, a_callback);
    
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
    
    /* start interrupt test */
    max31856_interface_debug_print("max31856: start interrupt test.\n");
    
    /* output */
    max31856_interface_debug_print("max31856: cold junction low fault threshold %0.3fC.\n", cold_junction_low_fault_threshold_deg);
    max31856_interface_debug_print("max31856: cold junction high fault threshold %0.3fC.\n", cold_junction_high_fault_threshold_deg);
    max31856_interface_debug_print("max31856: temperature low fault threshold %0.3fC.\n", temperature_low_fault_threshold_deg);
    max31856_interface_debug_print("max31856: temperature high fault threshold %0.3fC.\n", temperature_high_fault_threshold_deg);
    
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
    
    /* disable cold junction high fault threshold mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_CJHIGH, MAX31856_BOOL_FALSE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable cold junction low fault threshold mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_CJLOW, MAX31856_BOOL_FALSE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable thermocouple temperature high fault threshold mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_TCHIGH, MAX31856_BOOL_FALSE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable thermocouple temperature low fault threshold mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_TCLOW, MAX31856_BOOL_FALSE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable over voltage or undervoltage input fault mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_OVUV, MAX31856_BOOL_FALSE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mask failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable thermocouple open circuit fault mask */
    res = max31856_set_fault_mask(&gs_handle, MAX31856_FAULT_MASK_OPEN, MAX31856_BOOL_FALSE);
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
    
    /* convert */
    res = max31856_cold_junction_fault_threshold_convert_to_register(&gs_handle, cold_junction_low_fault_threshold_deg, &reg8);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: cold junction fault threshold convert to register failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set cold junction low fault threshold */
    res = max31856_set_cold_junction_low_fault_threshold(&gs_handle, reg8);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set cold junction low fault threshold failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* convert */
    res = max31856_cold_junction_fault_threshold_convert_to_register(&gs_handle, cold_junction_high_fault_threshold_deg, &reg8);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: cold junction fault threshold convert to register failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set cold junction high fault threshold */
    res = max31856_set_cold_junction_high_fault_threshold(&gs_handle, reg8);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set cold junction high fault threshold failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* convert */
    res = max31856_temperature_fault_threshold_convert_to_register(&gs_handle, temperature_low_fault_threshold_deg, &reg16);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: temperature fault threshold convert to register failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set temperature low fault threshold */
    res = max31856_set_temperature_low_fault_threshold(&gs_handle, reg16);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set temperature low fault threshold failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* convert */
    res = max31856_temperature_fault_threshold_convert_to_register(&gs_handle, temperature_high_fault_threshold_deg, &reg16);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: temperature fault threshold convert to register failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set temperature high fault threshold */
    res = max31856_set_temperature_high_fault_threshold(&gs_handle, reg16);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set temperature high fault threshold failed.\n");
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
    
    /* start continuous read */
    res = max31856_start_continuous_read(&gs_handle);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: start continuous read failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* init 0 */
    gs_flag = 0;
    
    /* wait for timeout */
    for (i = 0; i < timeout; i++)
    {
        /* delay 1ms */
        max31856_interface_delay_ms(1);
        
        if (gs_flag != 0)
        {
            max31856_interface_debug_print("max31856: find interrupt.\n");
            
            break;
        }
    }
    
    /* finish interrupt test */
    max31856_interface_debug_print("max31856: finish interrupt test.\n");
    (void)max31856_stop_continuous_read(&gs_handle);
    (void)max31856_deinit(&gs_handle);
    
    return 0;
}
