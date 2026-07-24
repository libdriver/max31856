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
 * @file      driver_max31856_interrupt.c
 * @brief     driver max31856 interrupt source file
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

#include "driver_max31856_interrupt.h"

static max31856_handle_t gs_handle;        /**< max31856 handle */

/**
 * @brief  interrupt irq callback
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t max31856_interrupt_irq_handler(void)
{
    if (max31856_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     interrupt example init
 * @param[in] type thermocouple type
 * @param[in] cold_junction_low_fault_threshold_deg cold junction low fault threshold
 * @param[in] cold_junction_high_fault_threshold_deg cold junction high fault threshold
 * @param[in] temperature_low_fault_threshold_deg temperature low fault threshold
 * @param[in] temperature_high_fault_threshold_deg temperature high fault threshold
 * @param[in] *callback pointer to a callback address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t max31856_interrupt_init(max31856_thermocouple_type_t type,
                                float cold_junction_low_fault_threshold_deg,
                                float cold_junction_high_fault_threshold_deg,
                                float temperature_low_fault_threshold_deg,
                                float temperature_high_fault_threshold_deg,
                                void (*callback)(uint8_t type))
{
    uint8_t res;
    int8_t reg8;
    int16_t reg16;
    int8_t offset;
    
    /* link interface function */
    DRIVER_MAX31856_LINK_INIT(&gs_handle, max31856_handle_t); 
    DRIVER_MAX31856_LINK_SPI_INIT(&gs_handle, max31856_interface_spi_init);
    DRIVER_MAX31856_LINK_SPI_DEINIT(&gs_handle, max31856_interface_spi_deinit);
    DRIVER_MAX31856_LINK_SPI_READ(&gs_handle, max31856_interface_spi_read);
    DRIVER_MAX31856_LINK_SPI_WRITE(&gs_handle, max31856_interface_spi_write);
    DRIVER_MAX31856_LINK_DELAY_MS(&gs_handle, max31856_interface_delay_ms);
    DRIVER_MAX31856_LINK_DEBUG_PRINT(&gs_handle, max31856_interface_debug_print);
    DRIVER_MAX31856_LINK_RECEIVE_CALLBACK(&gs_handle, callback);
    
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
    
    /* set default open circuit detection */
    res = max31856_set_open_circuit_detection(&gs_handle, MAX31856_INTERRUPT_DEFAULT_OPEN_CIRCUIT_DETECTION);
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
    
    /* set default fault mode */
    res = max31856_set_fault_mode(&gs_handle, MAX31856_INTERRUPT_DEFAULT_FAULT_MODE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mode failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default noise rejection filter */
    res = max31856_set_noise_rejection_filter(&gs_handle, MAX31856_INTERRUPT_DEFAULT_NOISE_REJECTION_FILTER);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set noise rejection filter failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default sample average */
    res = max31856_set_sample_average(&gs_handle, MAX31856_INTERRUPT_DEFAULT_SAMPLE_AVERAGE);
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
    
    /* set default cold junction temperature offset */
    res = max31856_cold_junction_temperature_offset_convert_to_register(&gs_handle, MAX31856_INTERRUPT_DEFAULT_COLD_JUNCTION_TEMPERATURE_OFFSET, &offset);
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
    
    return 0;
}

/**
 * @brief  interrupt example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t max31856_interrupt_deinit(void)
{
    /* stop continuous read */
    if (max31856_stop_continuous_read(&gs_handle) != 0)
    {
        return 1;
    }
    
    /* close max31856 */
    if (max31856_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      interrupt example read the temperature
 * @param[out] *temp_deg pointer to a converted temperature buffer
 * @param[out] *fault pointer to a fault buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t max31856_interrupt_read(double *temp_deg, uint8_t *fault)
{
    uint8_t res;
    int32_t raw;
    
    /* continuous read */
    res = max31856_continuous_read(&gs_handle, &raw, temp_deg);
    if (res != 0)
    {
        return 1;
    }
    
    /* get fault status */
    res = max31856_get_fault_status(&gs_handle, fault);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      interrupt example read the cold junction temperature
 * @param[out] *temp_deg pointer to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t max31856_interrupt_read_cold_junction_temperature(double *temp_deg)
{
    int16_t raw;
    
    /* read cold junction temperature */
    if (max31856_read_cold_junction_temperature(&gs_handle, &raw, temp_deg) != 0)
    {
        return 1;
    }
    
    return 0;
}
