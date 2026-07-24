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
 * @file      driver_max31856_shot.c
 * @brief     driver max31856 shot source file
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

#include "driver_max31856_shot.h"

static max31856_handle_t gs_handle;        /**< max31856 handle */

/**
 * @brief     shot example init
 * @param[in] type thermocouple type
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t max31856_shot_init(max31856_thermocouple_type_t type)
{
    uint8_t res;
    int8_t offset;
    
    /* link interface function */
    DRIVER_MAX31856_LINK_INIT(&gs_handle, max31856_handle_t); 
    DRIVER_MAX31856_LINK_SPI_INIT(&gs_handle, max31856_interface_spi_init);
    DRIVER_MAX31856_LINK_SPI_DEINIT(&gs_handle, max31856_interface_spi_deinit);
    DRIVER_MAX31856_LINK_SPI_READ(&gs_handle, max31856_interface_spi_read);
    DRIVER_MAX31856_LINK_SPI_WRITE(&gs_handle, max31856_interface_spi_write);
    DRIVER_MAX31856_LINK_DELAY_MS(&gs_handle, max31856_interface_delay_ms);
    DRIVER_MAX31856_LINK_DEBUG_PRINT(&gs_handle, max31856_interface_debug_print);
    DRIVER_MAX31856_LINK_RECEIVE_CALLBACK(&gs_handle, max31856_interface_receive_callback);
    
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
    res = max31856_set_open_circuit_detection(&gs_handle, MAX31856_SHOT_DEFAULT_OPEN_CIRCUIT_DETECTION);
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
    res = max31856_set_fault_mode(&gs_handle, MAX31856_SHOT_DEFAULT_FAULT_MODE);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set fault mode failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default noise rejection filter */
    res = max31856_set_noise_rejection_filter(&gs_handle, MAX31856_SHOT_DEFAULT_NOISE_REJECTION_FILTER);
    if (res != 0)
    {
        max31856_interface_debug_print("max31856: set noise rejection filter failed.\n");
        (void)max31856_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default sample average */
    res = max31856_set_sample_average(&gs_handle, MAX31856_SHOT_DEFAULT_SAMPLE_AVERAGE);
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
    
    /* set default cold junction temperature offset */
    res = max31856_cold_junction_temperature_offset_convert_to_register(&gs_handle, MAX31856_SHOT_DEFAULT_COLD_JUNCTION_TEMPERATURE_OFFSET, &offset);
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
    
    return 0;
}

/**
 * @brief  shot example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t max31856_shot_deinit(void)
{
    /* close max31856 */
    if (max31856_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      shot example read the temperature
 * @param[out] *temp_deg pointer to a converted temperature buffer
 * @param[out] *fault pointer to a fault buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t max31856_shot_read(double *temp_deg, uint8_t *fault)
{
    uint8_t res;
    int32_t raw;
    
    /* single read */
    res = max31856_single_read(&gs_handle, &raw, temp_deg);
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
 * @brief      shot example read the cold junction temperature
 * @param[out] *temp_deg pointer to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t max31856_shot_read_cold_junction_temperature(double *temp_deg)
{
    int16_t raw;
    
    /* read cold junction temperature */
    if (max31856_read_cold_junction_temperature(&gs_handle, &raw, temp_deg) != 0)
    {
        return 1;
    }
    
    return 0;
}
