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
 * @file      driver_max31856.h
 * @brief     driver max31856 header file
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

#ifndef DRIVER_MAX31856_H
#define DRIVER_MAX31856_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup max31856_driver max31856 driver function
 * @brief    max31856 driver modules
 * @{
 */

/**
 * @addtogroup max31856_base_driver
 * @{
 */

/**
 * @brief max31856 bool enumeration definition
 */
typedef enum
{
    MAX31856_BOOL_FALSE = 0x00,        /**< false */
    MAX31856_BOOL_TRUE  = 0x01,        /**< true */
} max31856_bool_t;

/**
 * @brief max31856 open circuit detection enumeration definition
 */
typedef enum
{
    MAX31856_OPEN_CIRCUIT_DETECTION_DISABLE = 0x00,        /**< disable */
    MAX31856_OPEN_CIRCUIT_DETECTION_MODE1   = 0x01,        /**< 13.3ms / 40ms */
    MAX31856_OPEN_CIRCUIT_DETECTION_MODE2   = 0x02,        /**< 33.4ms / 60ms */
    MAX31856_OPEN_CIRCUIT_DETECTION_MODE3   = 0x03,        /**< 113.4ms / 140ms */
} max31856_open_circuit_detection_t;

/**
 * @brief max31856 fault mode enumeration definition
 */
typedef enum
{
    MAX31856_FAULT_MODE_COMPARATOR = 0x00,        /**< comparator mode */
    MAX31856_FAULT_MODE_INTERRUPT  = 0x01,        /**< interrupt mode */
} max31856_fault_mode_t;

/**
 * @brief max31856 noise rejection filter enumeration definition
 */
typedef enum
{
    MAX31856_NOISE_REJECTION_FILTER_60HZ = 0x00,        /**< 60hz */
    MAX31856_NOISE_REJECTION_FILTER_50HZ = 0x01,        /**< 50hz */
} max31856_noise_rejection_filter_t;

/**
 * @brief max31856 sample average enumeration definition
 */
typedef enum
{
    MAX31856_SAMPLE_AVERAGE_1  = 0x00,        /**< 1 sample */
    MAX31856_SAMPLE_AVERAGE_2  = 0x01,        /**< 2 samples */
    MAX31856_SAMPLE_AVERAGE_4  = 0x02,        /**< 4 samples */
    MAX31856_SAMPLE_AVERAGE_8  = 0x03,        /**< 8 samples */
    MAX31856_SAMPLE_AVERAGE_16 = 0x04,        /**< 16 samples */
} max31856_sample_average_t;

/**
 * @brief max31856 thermocouple type enumeration definition
 */
typedef enum
{
    MAX31856_THERMOCOUPLE_TYPE_B               = 0x00,        /**< b type */
    MAX31856_THERMOCOUPLE_TYPE_E               = 0x01,        /**< e type */
    MAX31856_THERMOCOUPLE_TYPE_J               = 0x02,        /**< j type */
    MAX31856_THERMOCOUPLE_TYPE_K               = 0x03,        /**< k type */
    MAX31856_THERMOCOUPLE_TYPE_N               = 0x04,        /**< n type */
    MAX31856_THERMOCOUPLE_TYPE_R               = 0x05,        /**< r type */
    MAX31856_THERMOCOUPLE_TYPE_S               = 0x06,        /**< s type */
    MAX31856_THERMOCOUPLE_TYPE_T               = 0x07,        /**< t type */
    MAX31856_THERMOCOUPLE_TYPE_VOLTAGE_GAIN_8  = 0x08,        /**< 8 * 1.6 * 2^17 * vin */
    MAX31856_THERMOCOUPLE_TYPE_VOLTAGE_GAIN_32 = 0x0C,        /**< 32 * 1.6 * 2^17 * vin */
} max31856_thermocouple_type_t;

/**
 * @brief max31856 fault mask enumeration definition
 */
typedef enum
{
    MAX31856_FAULT_MASK_CJHIGH = 5,        /**< cold junction high fault threshold mask */
    MAX31856_FAULT_MASK_CJLOW  = 4,        /**< cold junction low fault threshold mask */
    MAX31856_FAULT_MASK_TCHIGH = 3,        /**< thermocouple temperature high fault threshold mask */
    MAX31856_FAULT_MASK_TCLOW  = 2,        /**< thermocouple temperature low fault threshold mask */
    MAX31856_FAULT_MASK_OVUV   = 1,        /**< over voltage or undervoltage input fault mask */
    MAX31856_FAULT_MASK_OPEN   = 0,        /**< thermocouple open circuit fault mask */
} max31856_fault_mask_t;

/**
 * @brief max31856 fault status enumeration definition
 */
typedef enum
{
    MAX31856_FAULT_STATUS_CJRANGE = (1 << 7),        /**< cold junction out of range */
    MAX31856_FAULT_STATUS_TCRANGE = (1 << 6),        /**< thermocouple out of range */
    MAX31856_FAULT_STATUS_CJHIGH  = (1 << 5),        /**< cold junction high fault */
    MAX31856_FAULT_STATUS_CJLOW   = (1 << 4),        /**< cold junction low fault */
    MAX31856_FAULT_STATUS_TCHIGH  = (1 << 3),        /**< thermocouple temperature high fault */
    MAX31856_FAULT_STATUS_TCLOW   = (1 << 2),        /**< thermocouple temperature low fault */
    MAX31856_FAULT_STATUS_OVUV    = (1 << 1),        /**< overvoltage or undervoltage input fault */
    MAX31856_FAULT_STATUS_OPEN    = (1 << 0),        /**< thermocouple open circuit fault */
} max31856_fault_status_t;

/**
 * @brief max31856 handle structure definition
 */
typedef struct max31856_handle_s
{
    uint8_t (*spi_init)(void);                                            /**< point to a spi_init function address */
    uint8_t (*spi_deinit)(void);                                          /**< point to a spi_deinit function address */
    uint8_t (*spi_read)(uint8_t reg, uint8_t *buf, uint16_t len);         /**< point to a spi_read function address */
    uint8_t (*spi_write)(uint8_t reg, uint8_t *buf, uint16_t len);        /**< point to a spi_write function address */
    void (*delay_ms)(uint32_t ms);                                        /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                      /**< point to a debug_print function address */
    void (*receive_callback)(uint8_t type);                               /**< point to a receive_callback function address */
    uint8_t inited;                                                       /**< inited flag */
} max31856_handle_t;

/**
 * @brief max31856 information structure definition
 */
typedef struct max31856_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} max31856_info_t;

/**
 * @}
 */

/**
 * @defgroup max31856_link_driver max31856 link driver function
 * @brief    max31856 link driver modules
 * @ingroup  max31856_driver
 * @{
 */

/**
 * @brief     initialize max31856_handle_t structure
 * @param[in] HANDLE pointer to a max31856 handle structure
 * @param[in] STRUCTURE max31856_handle_t
 * @note      none
 */
#define DRIVER_MAX31856_LINK_INIT(HANDLE, STRUCTURE)               memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link spi_init function
 * @param[in] HANDLE pointer to a max31856 handle structure
 * @param[in] FUC pointer to a spi_init function address
 * @note      none
 */
#define DRIVER_MAX31856_LINK_SPI_INIT(HANDLE, FUC)                (HANDLE)->spi_init = FUC

/**
 * @brief     link spi_deinit function
 * @param[in] HANDLE pointer to a max31856 handle structure
 * @param[in] FUC pointer to a spi_deinit function address
 * @note      none
 */
#define DRIVER_MAX31856_LINK_SPI_DEINIT(HANDLE, FUC)              (HANDLE)->spi_deinit = FUC

/**
 * @brief     link spi_read function
 * @param[in] HANDLE pointer to a max31856 handle structure
 * @param[in] FUC pointer to a spi_read function address
 * @note      none
 */
#define DRIVER_MAX31856_LINK_SPI_READ(HANDLE, FUC)                (HANDLE)->spi_read = FUC

/**
 * @brief     link spi_write function
 * @param[in] HANDLE pointer to a max31856 handle structure
 * @param[in] FUC pointer to a spi_write function address
 * @note      none
 */
#define DRIVER_MAX31856_LINK_SPI_WRITE(HANDLE, FUC)               (HANDLE)->spi_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to a max31856 handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_MAX31856_LINK_DELAY_MS(HANDLE, FUC)                (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to a max31856 handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_MAX31856_LINK_DEBUG_PRINT(HANDLE, FUC)             (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE pointer to a max31856 handle structure
 * @param[in] FUC pointer to a receive_callback function address
 * @note      none
 */
#define DRIVER_MAX31856_LINK_RECEIVE_CALLBACK(HANDLE, FUC)        (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup max31856_base_driver max31856 base driver function
 * @brief    max31856 base driver modules
 * @ingroup  max31856_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info pointer to a max31856 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t max31856_info(max31856_info_t *info);

/**
 * @brief     irq handler
 * @param[in] *handle pointer to a max31856 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31856_irq_handler(max31856_handle_t *handle);

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to a max31856 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t max31856_init(max31856_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle pointer to a max31856 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 * @note      none
 */
uint8_t max31856_deinit(max31856_handle_t *handle);

/**
 * @brief      read data from the chip once
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[out] *raw pointer to a raw adc buffer
 * @param[out] *deg pointer to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 single read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 read timeout
 *             - 5 open circuit fault
 *             - 6 overvoltage or undervoltage input fault
 *             - 7 thermocouple out of range
 *             - 8 cold junction out of range
 * @note       none
 */
uint8_t max31856_single_read(max31856_handle_t *handle, int32_t *raw, double *deg);

/**
 * @brief      read data from the chip continuously
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[out] *raw pointer to a raw adc buffer
 * @param[out] *deg pointer to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 continuous read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 open circuit fault
 *             - 5 overvoltage or undervoltage input fault
 *             - 6 thermocouple out of range
 *             - 7 cold junction out of range
 * @note       this function can be used only after run max31856_start_continuous_read
 *             and can be stopped by max31856_stop_continuous_read
 */
uint8_t max31856_continuous_read(max31856_handle_t *handle, int32_t *raw, double *deg);

/**
 * @brief     start the chip reading
 * @param[in] *handle pointer to a max31856 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 start continuous read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31856_start_continuous_read(max31856_handle_t *handle);

/**
 * @brief     stop the chip reading
 * @param[in] *handle pointer to a max31856 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 stop continuous read failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31856_stop_continuous_read(max31856_handle_t *handle);

/**
 * @brief      read cold junction temperature
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[out] *raw pointer to a raw adc buffer
 * @param[out] *deg pointer to a converted temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 read cold junction temperature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_read_cold_junction_temperature(max31856_handle_t *handle, int16_t *raw, double *deg);

/**
 * @brief     set open circuit detection
 * @param[in] *handle pointer to a max31856 handle structure
 * @param[in] detection open circuit detection
 * @return    status code
 *            - 0 success
 *            - 1 set open circuit detection failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31856_set_open_circuit_detection(max31856_handle_t *handle, max31856_open_circuit_detection_t detection);

/**
 * @brief      get open circuit detection
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[out] *detection pointer to an open circuit detection buffer
 * @return     status code
 *             - 0 success
 *             - 1 get open circuit detection failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_get_open_circuit_detection(max31856_handle_t *handle, max31856_open_circuit_detection_t *detection);

/**
 * @brief     enable or disable cold junction sensor
 * @param[in] *handle pointer to a max31856 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set cold junction sensor failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31856_set_cold_junction_sensor(max31856_handle_t *handle, max31856_bool_t enable);

/**
 * @brief      get cold junction sensor status
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get cold junction sensor failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_get_cold_junction_sensor(max31856_handle_t *handle, max31856_bool_t *enable);

/**
 * @brief     set fault mode
 * @param[in] *handle pointer to a max31856 handle structure
 * @param[in] mode fault mode
 * @return    status code
 *            - 0 success
 *            - 1 set fault mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31856_set_fault_mode(max31856_handle_t *handle, max31856_fault_mode_t mode);

/**
 * @brief      get fault mode
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[out] *mode pointer to a fault mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fault mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_get_fault_mode(max31856_handle_t *handle, max31856_fault_mode_t *mode);

/**
 * @brief     clear fault
 * @param[in] *handle pointer to a max31856 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 clear fault failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31856_clear_fault(max31856_handle_t *handle);

/**
 * @brief     set noise rejection filter
 * @param[in] *handle pointer to a max31856 handle structure
 * @param[in] filter noise rejection filter
 * @return    status code
 *            - 0 success
 *            - 1 set noise rejection filter failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31856_set_noise_rejection_filter(max31856_handle_t *handle, max31856_noise_rejection_filter_t filter);

/**
 * @brief      get noise rejection filter
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[out] *filter pointer to a noise rejection filter buffer
 * @return     status code
 *             - 0 success
 *             - 1 get noise rejection filter failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_get_noise_rejection_filter(max31856_handle_t *handle, max31856_noise_rejection_filter_t *filter);

/**
 * @brief     set sample average
 * @param[in] *handle pointer to a max31856 handle structure
 * @param[in] average sample average
 * @return    status code
 *            - 0 success
 *            - 1 set sample average failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31856_set_sample_average(max31856_handle_t *handle, max31856_sample_average_t average);

/**
 * @brief      get sample average
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[out] *average pointer to a sample average buffer
 * @return     status code
 *             - 0 success
 *             - 1 get sample average failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_get_sample_average(max31856_handle_t *handle, max31856_sample_average_t *average);

/**
 * @brief     set thermocouple type
 * @param[in] *handle pointer to a max31856 handle structure
 * @param[in] type thermocouple type
 * @return    status code
 *            - 0 success
 *            - 1 set thermocouple type failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31856_set_thermocouple_type(max31856_handle_t *handle, max31856_thermocouple_type_t type);

/**
 * @brief      get thermocouple type
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[out] *type pointer to a thermocouple type buffer
 * @return     status code
 *             - 0 success
 *             - 1 get thermocouple type failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_get_thermocouple_type(max31856_handle_t *handle, max31856_thermocouple_type_t *type);

/**
 * @brief     set fault mask
 * @param[in] *handle pointer to a max31856 handle structure
 * @param[in] mask fault mask
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set fault mask failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31856_set_fault_mask(max31856_handle_t *handle, max31856_fault_mask_t mask, max31856_bool_t enable);

/**
 * @brief      get fault mask
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[in]  mask fault mask
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fault mask failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_get_fault_mask(max31856_handle_t *handle, max31856_fault_mask_t mask, max31856_bool_t *enable);

/**
 * @brief     set cold junction high fault threshold
 * @param[in] *handle pointer to a max31856 handle structure
 * @param[in] threshold fault threshold
 * @return    status code
 *            - 0 success
 *            - 1 set cold junction high fault threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31856_set_cold_junction_high_fault_threshold(max31856_handle_t *handle, int8_t threshold);

/**
 * @brief      get cold junction high fault threshold
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[out] *threshold pointer to a fault threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get cold junction high fault threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_get_cold_junction_high_fault_threshold(max31856_handle_t *handle, int8_t *threshold);

/**
 * @brief     set cold junction low fault threshold
 * @param[in] *handle pointer to a max31856 handle structure
 * @param[in] threshold fault threshold
 * @return    status code
 *            - 0 success
 *            - 1 set cold junction low fault threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31856_set_cold_junction_low_fault_threshold(max31856_handle_t *handle, int8_t threshold);

/**
 * @brief      get cold junction low fault threshold
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[out] *threshold pointer to a fault threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get cold junction low fault threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_get_cold_junction_low_fault_threshold(max31856_handle_t *handle, int8_t *threshold);

/**
 * @brief     set temperature high fault threshold
 * @param[in] *handle pointer to a max31856 handle structure
 * @param[in] threshold fault threshold
 * @return    status code
 *            - 0 success
 *            - 1 set temperature high fault threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31856_set_temperature_high_fault_threshold(max31856_handle_t *handle, int16_t threshold);

/**
 * @brief      get temperature high fault threshold
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[out] *threshold pointer to a fault threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature high fault threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_get_temperature_high_fault_threshold(max31856_handle_t *handle, int16_t *threshold);

/**
 * @brief     set temperature low fault threshold
 * @param[in] *handle pointer to a max31856 handle structure
 * @param[in] threshold fault threshold
 * @return    status code
 *            - 0 success
 *            - 1 set temperature low fault threshold failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31856_set_temperature_low_fault_threshold(max31856_handle_t *handle, int16_t threshold);

/**
 * @brief      get temperature low fault threshold
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[out] *threshold pointer to a fault threshold buffer
 * @return     status code
 *             - 0 success
 *             - 1 get temperature low fault threshold failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_get_temperature_low_fault_threshold(max31856_handle_t *handle, int16_t *threshold);

/**
 * @brief     set cold junction temperature offset
 * @param[in] *handle pointer to a max31856 handle structure
 * @param[in] offset temperature offset
 * @return    status code
 *            - 0 success
 *            - 1 set cold junction temperature offset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31856_set_cold_junction_temperature_offset(max31856_handle_t *handle, int8_t offset);

/**
 * @brief      get cold junction temperature offset
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[out] *offset pointer to a temperature offset buffer
 * @return     status code
 *             - 0 success
 *             - 1 get cold junction temperature offset failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_get_cold_junction_temperature_offset(max31856_handle_t *handle, int8_t *offset);

/**
 * @brief     set cold junction temperature
 * @param[in] *handle pointer to a max31856 handle structure
 * @param[in] temperature input temperature
 * @return    status code
 *            - 0 success
 *            - 1 set cold junction temperature failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31856_set_cold_junction_temperature(max31856_handle_t *handle, int16_t temperature);

/**
 * @brief      get cold junction temperature
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[out] *temperature pointer to a temperature buffer
 * @return     status code
 *             - 0 success
 *             - 1 get cold junction temperature failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_get_cold_junction_temperature(max31856_handle_t *handle, int16_t *temperature);

/**
 * @brief      get fault status
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[out] *status pointer to a fault status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fault status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_get_fault_status(max31856_handle_t *handle, uint8_t *status);

/**
 * @brief      convert the cold junction fault threshold to the register raw data
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[in]  deg degrees celsius
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_cold_junction_fault_threshold_convert_to_register(max31856_handle_t *handle, float deg, int8_t *reg);

/**
 * @brief      convert the register raw data to the cold junction fault threshold
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[in]  reg register raw data
 * @param[out] *deg pointer to a degrees celsius buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_cold_junction_fault_threshold_convert_to_data(max31856_handle_t *handle, int8_t reg, float *deg);

/**
 * @brief      convert the temperature fault threshold to the register raw data
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[in]  deg degrees celsius
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_temperature_fault_threshold_convert_to_register(max31856_handle_t *handle, float deg, int16_t *reg);

/**
 * @brief      convert the register raw data to the temperature fault threshold
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[in]  reg register raw data
 * @param[out] *deg pointer to a degrees celsius buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_temperature_fault_threshold_convert_to_data(max31856_handle_t *handle, int16_t reg, float *deg);

/**
 * @brief      convert the cold junction temperature offset to the register raw data
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[in]  deg degrees celsius
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_cold_junction_temperature_offset_convert_to_register(max31856_handle_t *handle, float deg, int8_t *reg);

/**
 * @brief      convert the register raw data to the cold junction temperature offset
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[in]  reg register raw data
 * @param[out] *deg pointer to a degrees celsius buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_cold_junction_temperature_offset_convert_to_data(max31856_handle_t *handle, int8_t reg, float *deg);

/**
 * @brief      convert the cold junction temperature to the register raw data
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[in]  deg degrees celsius
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_cold_junction_temperature_convert_to_register(max31856_handle_t *handle, float deg, int16_t *reg);

/**
 * @brief      convert the register raw data to the cold junction temperature
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[in]  reg register raw data
 * @param[out] *deg pointer to a degrees celsius buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_cold_junction_temperature_convert_to_data(max31856_handle_t *handle, int16_t reg, float *deg);

/**
 * @}
 */

/**
 * @defgroup max31856_extern_driver max31856 extern driver function
 * @brief    max31856 extern driver modules
 * @ingroup  max31856_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle pointer to a max31856 handle structure
 * @param[in] reg register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t max31856_set_reg(max31856_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[in]  reg register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t max31856_get_reg(max31856_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
