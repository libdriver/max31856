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
 * @file      driver_max31856.c
 * @brief     driver max31856 source file
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

#include "driver_max31856.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Maxim Integrated MAX31856"       /**< chip name */
#define MANUFACTURER_NAME         "Maxim Integrated"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        3.0f                              /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f                              /**< chip max supply voltage */
#define MAX_CURRENT               2.0f                              /**< chip max current */
#define TEMPERATURE_MIN           -55.0f                            /**< chip min operating temperature */
#define TEMPERATURE_MAX           125.0f                            /**< chip max operating temperature */
#define DRIVER_VERSION            1000                              /**< driver version */

/**
 * @brief chip register definition
 */
#define MAX31856_REG_CR0           0x00        /**< configuration 0 register */
#define MAX31856_REG_CR1           0x01        /**< configuration 1 register */
#define MAX31856_REG_MASK          0x02        /**< fault mask register */
#define MAX31856_REG_CJHF          0x03        /**< cold-junction high fault threshold register */
#define MAX31856_REG_CJLF          0x04        /**< cold-junction low fault threshold register */
#define MAX31856_REG_LTHFTH        0x05        /**< linearized temperature high fault threshold msb register */
#define MAX31856_REG_LTHFTL        0x06        /**< linearized temperature high fault threshold lsb register */
#define MAX31856_REG_LTLFTH        0x07        /**< linearized temperature low fault threshold msb register */
#define MAX31856_REG_LTLFTL        0x08        /**< linearized temperature low fault threshold lsb register */
#define MAX31856_REG_CJTO          0x09        /**< cold-junction temperature offset register */
#define MAX31856_REG_CJTH          0x0A        /**< cold-junction temperature msb register */
#define MAX31856_REG_CJTL          0x0B        /**< cold-junction temperature lsb register */
#define MAX31856_REG_LTCBH         0x0C        /**< linearized tc temperature byte 2 register */
#define MAX31856_REG_LTCBM         0x0D        /**< linearized tc temperature byte 1 register */
#define MAX31856_REG_LTCBL         0x0E        /**< linearized tc temperature byte 0 register */
#define MAX31856_REG_SR            0x0F        /**< fault status register */

/**
 * @brief      read bytes
 * @param[in]  *handle pointer to a max31856 handle structure
 * @param[in]  reg register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len length of data
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_max31856_read(max31856_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->spi_read(reg, buf, len) != 0)        /* read data */
    {
        return 1;                                    /* return error */
    }
    
    return 0;                                        /* success return 0 */
}

/**
 * @brief     write bytes
 * @param[in] *handle pointer to a max31856 handle structure
 * @param[in] reg register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len length of data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_max31856_write(max31856_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->spi_write(0x80 | reg, buf, len) != 0)        /* write data */
    {
        return 1;                                            /* return error */
    }
    
    return 0;                                                /* success return 0 */
}

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
uint8_t max31856_init(max31856_handle_t *handle)
{
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->debug_print == NULL)                                         /* check debug_print */
    {
        return 3;                                                            /* return error */
    }
    if (handle->spi_init == NULL)                                            /* check spi_init */
    {
        handle->debug_print("max31856: spi_init is null.\n");                /* spi_init is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->spi_deinit == NULL)                                          /* check spi_deinit */
    {
        handle->debug_print("max31856: spi_deinit is null.\n");              /* spi_deinit is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->spi_read == NULL)                                            /* check spi_read */
    {
        handle->debug_print("max31856: spi_read is null.\n");                /* spi_read is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->spi_write == NULL)                                           /* check spi_write */
    {
        handle->debug_print("max31856: spi_write is null.\n");               /* spi_write is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->delay_ms == NULL)                                            /* check delay_ms */
    {
        handle->debug_print("max31856: delay_ms is null.\n");                /* delay_ms is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->receive_callback == NULL)                                    /* check receive_callback */
    {
        handle->debug_print("max31856: receive_callback is null.\n");        /* receive_callback is null */
       
        return 3;                                                            /* return error */
    }
    
    if (handle->spi_init() != 0)                                             /* spi init */
    {
        handle->debug_print("max31856: spi init failed.\n");                 /* spi init failed */
        
        return 1;                                                            /* return error */
    }
    
    handle->inited = 1;                                                      /* flag finish initialization */
    
    return 0;                                                                /* success return 0 */
}

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
uint8_t max31856_deinit(max31856_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CR0, &prev, 1);         /* read cr0 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: read cr0 failed.\n");           /* read cr0 failed */
       
        return 4;                                                      /* return error */
    }
    prev &= ~(1 << 7);                                                 /* clear settings */
    res = a_max31856_write(handle, MAX31856_REG_CR0, &prev, 1);        /* write cr0 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: write cr0 failed.\n");          /* write cr0 failed */
       
        return 4;                                                      /* return error */
    }
    
    res = handle->spi_deinit();                                        /* spi deinit */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: spi deinit failed.\n");         /* spi deinit failed */
       
        return 1;                                                      /* return error */
    }
    handle->inited = 0;                                                /* flag close */
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t max31856_single_read(max31856_handle_t *handle, int32_t *raw, double *deg)
{
    uint8_t res;
    uint8_t prev;
    uint8_t buf[3];
    uint16_t timeout;
    uint16_t i;
    uint32_t temp_raw;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CR0, &prev, 1);                              /* read cr0 */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("max31856: read cr0 failed.\n");                                /* read cr0 failed */
       
        return 1;                                                                           /* return error */
    }
    prev &= ~(1 << 7);                                                                      /* clear settings */
    prev |= 1 << 6;                                                                         /* set bool */
    res = a_max31856_write(handle, MAX31856_REG_CR0, &prev, 1);                             /* write cr0 */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("max31856: write cr0 failed.\n");                               /* write cr0 failed */
       
        return 1;                                                                           /* return error */
    }
    
    timeout = 200;                                                                          /* 2s timeout */
    for (i = 0; i < timeout; i++)                                                           /* loop */
    {
        res = a_max31856_read(handle, MAX31856_REG_CR0, &prev, 1);                          /* read cr0 */
        if (res != 0)                                                                       /* check result */
        {
            handle->debug_print("max31856: read cr0 failed.\n");                            /* read cr0 failed */
           
            return 1;                                                                       /* return error */
        }
        if (((prev >> 6) & 0x01) == 0)                                                      /* check bit */
        {
            break;                                                                          /* break */
        }
        handle->delay_ms(10);                                                               /* delay 10ms */
    }
    if (i >= timeout)                                                                       /* check timeout */
    {
        handle->debug_print("max31856: read timeout.\n");                                   /* read timeout */
       
        return 4;                                                                           /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_SR, &prev, 1);                               /* read fault status */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("max31856: read fault status failed.\n");                       /* read fault status failed */
       
        return 1;                                                                           /* return error */
    }
    if (((prev >> 0) & 0x01) != 0)                                                          /* check open circuit fault */
    {
        handle->debug_print("max31856: open circuit fault.\n");                             /* open circuit fault */
       
        return 5;                                                                           /* return error */
    }
    if (((prev >> 1) & 0x01) != 0)                                                          /* check overvoltage or undervoltage input fault */
    {
        handle->debug_print("max31856: overvoltage or undervoltage input fault.\n");        /* overvoltage or undervoltage input fault */
       
        return 6;                                                                           /* return error */
    }
    if (((prev >> 6) & 0x01) != 0)                                                          /* check thermocouple out of range */
    {
        handle->debug_print("max31856: thermocouple out of range.\n");                      /* thermocouple out of range */
       
        return 7;                                                                           /* return error */
    }
    if (((prev >> 7) & 0x01) != 0)                                                          /* check cold junction out of range */
    {
        handle->debug_print("max31856: cold junction out of range.\n");                     /* cold junction out of range */
       
        return 8;                                                                           /* return error */
    }
    res = a_max31856_read(handle, MAX31856_REG_LTCBH, buf, 3);                              /* read linearized tc temperature */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("max31856: read linearized tc temperature failed.\n");          /* read linearized tc temperature failed */
       
        return 1;                                                                           /* return error */
    }
    temp_raw = ((uint32_t)buf[0] << 16 | (uint32_t)buf[1] << 8 | 
                (uint32_t)buf[2] << 0);                                                     /* make the raw data */
    temp_raw = temp_raw >> 5;                                                               /* right shift */
    if ((temp_raw & 0x00040000U) != 0)                                                      /* check sign bit */
    {
        temp_raw |= 0xFFF80000U;                                                            /* set negative */
    }
    *raw = (int32_t)temp_raw;                                                               /* save the raw data */
    *deg = (double)(*raw) * 0.0078125;                                                      /* convert temperature */
    
    return 0;                                                                               /* success return 0 */
}

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
uint8_t max31856_continuous_read(max31856_handle_t *handle, int32_t *raw, double *deg)
{
    uint8_t res;
    uint8_t prev;
    uint8_t buf[3];
    uint32_t temp_raw;
    
    if (handle == NULL)                                                                     /* check handle */
    {
        return 2;                                                                           /* return error */
    }
    if (handle->inited != 1)                                                                /* check handle initialization */
    {
        return 3;                                                                           /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_SR, &prev, 1);                               /* read fault status */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("max31856: read fault status failed.\n");                       /* read fault status failed */
       
        return 1;                                                                           /* return error */
    }
    if (((prev >> 0) & 0x01) != 0)                                                          /* check open circuit fault */
    {
        handle->debug_print("max31856: open circuit fault.\n");                             /* open circuit fault */
       
        return 4;                                                                           /* return error */
    }
    if (((prev >> 1) & 0x01) != 0)                                                          /* check overvoltage or undervoltage input fault */
    {
        handle->debug_print("max31856: overvoltage or undervoltage input fault.\n");        /* overvoltage or undervoltage input fault */
       
        return 5;                                                                           /* return error */
    }
    if (((prev >> 6) & 0x01) != 0)                                                          /* check thermocouple out of range */
    {
        handle->debug_print("max31856: thermocouple out of range.\n");                      /* thermocouple out of range */
       
        return 6;                                                                           /* return error */
    }
    if (((prev >> 7) & 0x01) != 0)                                                          /* check cold junction out of range */
    {
        handle->debug_print("max31856: cold junction out of range.\n");                     /* cold junction out of range */
       
        return 7;                                                                           /* return error */
    }
    res = a_max31856_read(handle, MAX31856_REG_LTCBH, buf, 3);                              /* read linearized tc temperature */
    if (res != 0)                                                                           /* check result */
    {
        handle->debug_print("max31856: read linearized tc temperature failed.\n");          /* read linearized tc temperature failed */
       
        return 1;                                                                           /* return error */
    }
    temp_raw = ((uint32_t)buf[0] << 16 | (uint32_t)buf[1] << 8 | 
                (uint32_t)buf[2] << 0);                                                     /* make the raw data */
    temp_raw = temp_raw >> 5;                                                               /* right shift */
    if ((temp_raw & 0x00040000U) != 0)                                                      /* check sign bit */
    {
        temp_raw |= 0xFFF80000U;                                                            /* set negative */
    }
    *raw = (int32_t)temp_raw;                                                               /* save the raw data */
    *deg = (double)(*raw) * 0.0078125;                                                      /* convert temperature */
    
    return 0;                                                                               /* success return 0 */
}

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
uint8_t max31856_start_continuous_read(max31856_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CR0, &prev, 1);         /* read cr0 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: read cr0 failed.\n");           /* read cr0 failed */
       
        return 1;                                                      /* return error */
    }
    prev &= ~(1 << 7);                                                 /* clear settings */
    prev |= 1 << 7;                                                    /* set bool */
    prev &= ~(1 << 6);                                                 /* clear settings */
    res = a_max31856_write(handle, MAX31856_REG_CR0, &prev, 1);        /* write cr0 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: write cr0 failed.\n");          /* write cr0 failed */
       
        return 1;                                                      /* return error */
    }
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t max31856_stop_continuous_read(max31856_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CR0, &prev, 1);         /* read cr0 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: read cr0 failed.\n");           /* read cr0 failed */
       
        return 1;                                                      /* return error */
    }
    prev &= ~(1 << 7);                                                 /* clear settings */
    prev &= ~(1 << 6);                                                 /* clear settings */
    res = a_max31856_write(handle, MAX31856_REG_CR0, &prev, 1);        /* write cr0 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: write cr0 failed.\n");          /* write cr0 failed */
       
        return 1;                                                      /* return error */
    }
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t max31856_read_cold_junction_temperature(max31856_handle_t *handle, int16_t *raw, double *deg)
{
    uint8_t res;
    uint8_t buf[2];
    uint16_t temp_raw;
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CJTH, buf, 2);                             /* read cold junction temperature */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("max31856: read cold junction temperature failed.\n");        /* read cold junction temperature failed */
       
        return 1;                                                                         /* return error */
    }
    temp_raw = ((uint16_t)buf[0] << 8) | (uint16_t)buf[1];                                /* set temperature */
    temp_raw = temp_raw >> 2;                                                             /* right shift */
    if ((temp_raw & 0x2000U) != 0)                                                        /* check sign bit */
    {
        temp_raw |= 0xC000U;                                                              /* set negative */
    }
    *raw = (int16_t)temp_raw;                                                             /* save the raw data */
    *deg = (double)(*raw) * 0.015625;                                                     /* convert temperature */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t max31856_set_open_circuit_detection(max31856_handle_t *handle, max31856_open_circuit_detection_t detection)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CR0, &prev, 1);         /* read cr0 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: read cr0 failed.\n");           /* read cr0 failed */
       
        return 1;                                                      /* return error */
    }
    prev &= ~(3 << 4);                                                 /* clear settings */
    prev |= detection << 4;                                            /* set detection */
    res = a_max31856_write(handle, MAX31856_REG_CR0, &prev, 1);        /* write cr0 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: write cr0 failed.\n");          /* write cr0 failed */
       
        return 1;                                                      /* return error */
    }
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t max31856_get_open_circuit_detection(max31856_handle_t *handle, max31856_open_circuit_detection_t *detection)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CR0, &prev, 1);                   /* read cr0 */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("max31856: read cr0 failed.\n");                     /* read cr0 failed */
       
        return 1;                                                                /* return error */
    }
    *detection = (max31856_open_circuit_detection_t)((prev >> 4) & 0x03);        /* set detection */
    
    return 0;                                                                    /* success return 0 */
}

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
uint8_t max31856_set_cold_junction_sensor(max31856_handle_t *handle, max31856_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CR0, &prev, 1);         /* read cr0 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: read cr0 failed.\n");           /* read cr0 failed */
       
        return 1;                                                      /* return error */
    }
    prev &= ~(1 << 3);                                                 /* clear settings */
    prev |= !enable << 3;                                              /* set bool */
    res = a_max31856_write(handle, MAX31856_REG_CR0, &prev, 1);        /* write cr0 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: write cr0 failed.\n");          /* write cr0 failed */
       
        return 1;                                                      /* return error */
    }
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t max31856_get_cold_junction_sensor(max31856_handle_t *handle, max31856_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CR0, &prev, 1);         /* read cr0 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: read cr0 failed.\n");           /* read cr0 failed */
       
        return 1;                                                      /* return error */
    }
    *enable = (max31856_bool_t)(!((prev >> 3) & 0x01));                /* set bool */
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t max31856_set_fault_mode(max31856_handle_t *handle, max31856_fault_mode_t mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CR0, &prev, 1);         /* read cr0 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: read cr0 failed.\n");           /* read cr0 failed */
       
        return 1;                                                      /* return error */
    }
    prev &= ~(1 << 2);                                                 /* clear settings */
    prev |= mode << 2;                                                 /* set mode */
    res = a_max31856_write(handle, MAX31856_REG_CR0, &prev, 1);        /* write cr0 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: write cr0 failed.\n");          /* write cr0 failed */
       
        return 1;                                                      /* return error */
    }
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t max31856_get_fault_mode(max31856_handle_t *handle, max31856_fault_mode_t *mode)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CR0, &prev, 1);         /* read cr0 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: read cr0 failed.\n");           /* read cr0 failed */
       
        return 1;                                                      /* return error */
    }
   *mode = (max31856_fault_mode_t)((prev >> 2) & 0x01);                /* set mode */
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t max31856_clear_fault(max31856_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CR0, &prev, 1);         /* read cr0 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: read cr0 failed.\n");           /* read cr0 failed */
       
        return 1;                                                      /* return error */
    }
    prev &= ~(1 << 1);                                                 /* clear settings */
    prev |= 1 << 1;                                                    /* set bool */
    res = a_max31856_write(handle, MAX31856_REG_CR0, &prev, 1);        /* write cr0 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: write cr0 failed.\n");          /* write cr0 failed */
       
        return 1;                                                      /* return error */
    }
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t max31856_set_noise_rejection_filter(max31856_handle_t *handle, max31856_noise_rejection_filter_t filter)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CR0, &prev, 1);         /* read cr0 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: read cr0 failed.\n");           /* read cr0 failed */
       
        return 1;                                                      /* return error */
    }
    prev &= ~(1 << 0);                                                 /* clear settings */
    prev |= filter << 0;                                               /* set filter */
    res = a_max31856_write(handle, MAX31856_REG_CR0, &prev, 1);        /* write cr0 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: write cr0 failed.\n");          /* write cr0 failed */
       
        return 1;                                                      /* return error */
    }
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t max31856_get_noise_rejection_filter(max31856_handle_t *handle, max31856_noise_rejection_filter_t *filter)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CR0, &prev, 1);                /* read cr0 */
    if (res != 0)                                                             /* check result */
    {
        handle->debug_print("max31856: read cr0 failed.\n");                  /* read cr0 failed */
       
        return 1;                                                             /* return error */
    }
    *filter = (max31856_noise_rejection_filter_t)((prev >> 0) & 0x01);        /* set noise rejection filter */
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t max31856_set_sample_average(max31856_handle_t *handle, max31856_sample_average_t average)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CR1, &prev, 1);         /* read cr1 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: read cr1 failed.\n");           /* read cr1 failed */
       
        return 1;                                                      /* return error */
    }
    prev &= ~(7 << 4);                                                 /* clear settings */
    prev |= average << 4;                                              /* set average */
    res = a_max31856_write(handle, MAX31856_REG_CR1, &prev, 1);        /* write cr1 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: write cr1 failed.\n");          /* write cr1 failed */
       
        return 1;                                                      /* return error */
    }
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t max31856_get_sample_average(max31856_handle_t *handle, max31856_sample_average_t *average)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CR1, &prev, 1);         /* read cr1 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: read cr1 failed.\n");           /* read cr1 failed */
       
        return 1;                                                      /* return error */
    }
    *average = (max31856_sample_average_t)((prev >> 4) & 0x07);        /* set average */
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t max31856_set_thermocouple_type(max31856_handle_t *handle, max31856_thermocouple_type_t type)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CR1, &prev, 1);         /* read cr1 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: read cr1 failed.\n");           /* read cr1 failed */
       
        return 1;                                                      /* return error */
    }
    prev &= ~(0xF << 0);                                               /* clear settings */
    prev |= type << 0;                                                 /* set type */
    res = a_max31856_write(handle, MAX31856_REG_CR1, &prev, 1);        /* write cr1 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: write cr1 failed.\n");          /* write cr1 failed */
       
        return 1;                                                      /* return error */
    }
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t max31856_get_thermocouple_type(max31856_handle_t *handle, max31856_thermocouple_type_t *type)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                /* check handle */
    {
        return 2;                                                      /* return error */
    }
    if (handle->inited != 1)                                           /* check handle initialization */
    {
        return 3;                                                      /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CR1, &prev, 1);         /* read cr1 */
    if (res != 0)                                                      /* check result */
    {
        handle->debug_print("max31856: read cr1 failed.\n");           /* read cr1 failed */
       
        return 1;                                                      /* return error */
    }
    *type = (max31856_thermocouple_type_t)((prev >> 0) & 0x0F);        /* set thermocouple type */
    
    return 0;                                                          /* success return 0 */
}

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
uint8_t max31856_set_fault_mask(max31856_handle_t *handle, max31856_fault_mask_t mask, max31856_bool_t enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_MASK, &prev, 1);         /* read mask */
    if (res != 0)                                                       /* check result */
    {
        handle->debug_print("max31856: read mask failed.\n");           /* read mask failed */
       
        return 1;                                                       /* return error */
    }
    prev &= ~(1 << mask);                                               /* clear settings */
    prev |= enable << mask;                                             /* set bool */
    res = a_max31856_write(handle, MAX31856_REG_MASK, &prev, 1);        /* write mask */
    if (res != 0)                                                       /* check result */
    {
        handle->debug_print("max31856: write mask failed.\n");          /* write mask failed */
       
        return 1;                                                       /* return error */
    }
    
    return 0;                                                           /* success return 0 */
}

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
uint8_t max31856_get_fault_mask(max31856_handle_t *handle, max31856_fault_mask_t mask, max31856_bool_t *enable)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_MASK, &prev, 1);         /* read mask */
    if (res != 0)                                                       /* check result */
    {
        handle->debug_print("max31856: read mask failed.\n");           /* read mask failed */
       
        return 1;                                                       /* return error */
    }
    *enable = (max31856_bool_t)((prev >> mask) & 0x01);                 /* set bool */
    
    return 0;                                                           /* success return 0 */
}

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
uint8_t max31856_set_cold_junction_high_fault_threshold(max31856_handle_t *handle, int8_t threshold)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                            /* check handle */
    {
        return 2;                                                                                  /* return error */
    }
    if (handle->inited != 1)                                                                       /* check handle initialization */
    {
        return 3;                                                                                  /* return error */
    }
    
    prev = (uint8_t)(threshold);                                                                   /* set threshold */
    res = a_max31856_write(handle, MAX31856_REG_CJHF, &prev, 1);                                   /* write cold junction high fault threshold */
    if (res != 0)                                                                                  /* check result */
    {
        handle->debug_print("max31856: write cold junction high fault threshold failed.\n");       /* write cold junction high fault threshold failed */
       
        return 1;                                                                                  /* return error */
    }
    
    return 0;                                                                                      /* success return 0 */
}

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
uint8_t max31856_get_cold_junction_high_fault_threshold(max31856_handle_t *handle, int8_t *threshold)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CJHF, &prev, 1);                                   /* read cold junction high fault threshold */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("max31856: read cold junction high fault threshold failed.\n");       /* read cold junction high fault threshold failed */
       
        return 1;                                                                                 /* return error */
    }
    *threshold = (int8_t)(prev);                                                                  /* set threshold */
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t max31856_set_cold_junction_low_fault_threshold(max31856_handle_t *handle, int8_t threshold)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    prev = (uint8_t)(threshold);                                                                  /* set threshold */
    res = a_max31856_write(handle, MAX31856_REG_CJLF, &prev, 1);                                  /* write cold junction low fault threshold */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("max31856: write cold junction low fault threshold failed.\n");       /* write cold junction low fault threshold failed */
       
        return 1;                                                                                 /* return error */
    }
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t max31856_get_cold_junction_low_fault_threshold(max31856_handle_t *handle, int8_t *threshold)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CJLF, &prev, 1);                                  /* read cold junction low fault threshold */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("max31856: read cold junction low fault threshold failed.\n");       /* read cold junction low fault threshold failed */
       
        return 1;                                                                                /* return error */
    }
    *threshold = (int8_t)(prev);                                                                 /* set threshold */
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t max31856_set_temperature_high_fault_threshold(max31856_handle_t *handle, int16_t threshold)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    buf[0] = (threshold >> 8) & 0xFF;                                                             /* set msb */
    buf[1] = (threshold >> 0) & 0xFF;                                                             /* set lsb */
    res = a_max31856_write(handle, MAX31856_REG_LTHFTH, buf, 2);                                  /* write temperature high fault threshold */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("max31856: write temperature high fault threshold failed.\n");        /* write temperature high fault threshold failed */
       
        return 1;                                                                                 /* return error */
    }
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t max31856_get_temperature_high_fault_threshold(max31856_handle_t *handle, int16_t *threshold)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_LTHFTH, buf, 2);                                  /* read temperature high fault threshold */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("max31856: read temperature high fault threshold failed.\n");        /* read temperature high fault threshold failed */
       
        return 1;                                                                                /* return error */
    }
    *threshold = (int16_t)((uint16_t)buf[0] << 8 | buf[1]);                                      /* set threshold */
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t max31856_set_temperature_low_fault_threshold(max31856_handle_t *handle, int16_t threshold)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    buf[0] = (threshold >> 8) & 0xFF;                                                            /* set msb */
    buf[1] = (threshold >> 0) & 0xFF;                                                            /* set lsb */
    res = a_max31856_write(handle, MAX31856_REG_LTLFTH, buf, 2);                                 /* write temperature low fault threshold */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("max31856: write temperature low fault threshold failed.\n");        /* write temperature low fault threshold failed */
       
        return 1;                                                                                /* return error */
    }
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t max31856_get_temperature_low_fault_threshold(max31856_handle_t *handle, int16_t *threshold)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                         /* check handle */
    {
        return 2;                                                                               /* return error */
    }
    if (handle->inited != 1)                                                                    /* check handle initialization */
    {
        return 3;                                                                               /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_LTLFTH, buf, 2);                                 /* read temperature low fault threshold */
    if (res != 0)                                                                               /* check result */
    {
        handle->debug_print("max31856: read temperature low fault threshold failed.\n");        /* read temperature low fault threshold failed */
       
        return 1;                                                                               /* return error */
    }
    *threshold = (int16_t)((uint16_t)buf[0] << 8 | buf[1]);                                     /* set threshold */
    
    return 0;                                                                                   /* success return 0 */
}

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
uint8_t max31856_set_cold_junction_temperature_offset(max31856_handle_t *handle, int8_t offset)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    
    prev = (uint8_t)(offset);                                                                     /* set offset */
    res = a_max31856_write(handle, MAX31856_REG_CJTO, &prev, 1);                                  /* write cold junction temperature offset */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("max31856: write cold junction temperature offset failed.\n");        /* write cold junction temperature offset failed */
       
        return 1;                                                                                 /* return error */
    }
    
    return 0;                                                                                     /* success return 0 */
}

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
uint8_t max31856_get_cold_junction_temperature_offset(max31856_handle_t *handle, int8_t *offset)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CJTO, &prev, 1);                                  /* read cold junction temperature offset */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("max31856: read cold junction temperature offset failed.\n");        /* read cold junction temperature offset failed */
       
        return 1;                                                                                /* return error */
    }
    *offset = (int8_t)(prev);                                                                    /* set offset */
    
    return 0;                                                                                    /* success return 0 */
}

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
uint8_t max31856_set_cold_junction_temperature(max31856_handle_t *handle, int16_t temperature)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    buf[0] = (temperature >> 8) & 0xFF;                                                    /* set msb */
    buf[1] = (temperature >> 0) & 0xFF;                                                    /* set lsb */
    res = a_max31856_write(handle, MAX31856_REG_CJTH, buf, 2);                             /* write cold junction temperature */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("max31856: write cold junction temperature failed.\n");        /* write cold junction temperature failed */
       
        return 1;                                                                          /* return error */
    }
    
    return 0;                                                                              /* success return 0 */
}

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
uint8_t max31856_get_cold_junction_temperature(max31856_handle_t *handle, int16_t *temperature)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                                   /* check handle */
    {
        return 2;                                                                         /* return error */
    }
    if (handle->inited != 1)                                                              /* check handle initialization */
    {
        return 3;                                                                         /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_CJTH, buf, 2);                             /* read cold junction temperature */
    if (res != 0)                                                                         /* check result */
    {
        handle->debug_print("max31856: read cold junction temperature failed.\n");        /* read cold junction temperature failed */
       
        return 1;                                                                         /* return error */
    }
    *temperature = (int16_t)((uint16_t)buf[0] << 8 | buf[1]);                             /* set temperature */
    
    return 0;                                                                             /* success return 0 */
}

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
uint8_t max31856_get_fault_status(max31856_handle_t *handle, uint8_t *status)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_SR, &prev, 1);                /* read fault status */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("max31856: read fault status failed.\n");        /* read fault status failed */
       
        return 1;                                                            /* return error */
    }
    *status = prev;                                                          /* set status */
    
    return 0;                                                                /* success return 0 */
}

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
uint8_t max31856_cold_junction_fault_threshold_convert_to_register(max31856_handle_t *handle, float deg, int8_t *reg)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *reg = (int8_t)(deg);           /* convert real data to register data */
    
    return 0;                       /* success return 0 */
}

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
uint8_t max31856_cold_junction_fault_threshold_convert_to_data(max31856_handle_t *handle, int8_t reg, float *deg)
{
    if (handle == NULL)             /* check handle */
    {
        return 2;                   /* return error */
    }
    if (handle->inited != 1)        /* check handle initialization */
    {
        return 3;                   /* return error */
    }
    
    *deg = (float)(reg);            /* convert raw data to real data */
    
    return 0;                       /* success return 0 */
}

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
uint8_t max31856_temperature_fault_threshold_convert_to_register(max31856_handle_t *handle, float deg, int16_t *reg)
{
    if (handle == NULL)                     /* check handle */
    {
        return 2;                           /* return error */
    }
    if (handle->inited != 1)                /* check handle initialization */
    {
        return 3;                           /* return error */
    }
    
    *reg = (int16_t)(deg / 0.0625f);        /* convert real data to register data */
    
    return 0;                               /* success return 0 */
}

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
uint8_t max31856_temperature_fault_threshold_convert_to_data(max31856_handle_t *handle, int16_t reg, float *deg)
{
    if (handle == NULL)                   /* check handle */
    {
        return 2;                         /* return error */
    }
    if (handle->inited != 1)              /* check handle initialization */
    {
        return 3;                         /* return error */
    }
    
    *deg = (float)(reg) * 0.0625f;        /* convert raw data to real data */
    
    return 0;                             /* success return 0 */
}

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
uint8_t max31856_cold_junction_temperature_offset_convert_to_register(max31856_handle_t *handle, float deg, int8_t *reg)
{
    if (handle == NULL)                    /* check handle */
    {
        return 2;                          /* return error */
    }
    if (handle->inited != 1)               /* check handle initialization */
    {
        return 3;                          /* return error */
    }
    
    *reg = (int8_t)(deg / 0.0625f);        /* convert real data to register data */
    
    return 0;                              /* success return 0 */
}

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
uint8_t max31856_cold_junction_temperature_offset_convert_to_data(max31856_handle_t *handle, int8_t reg, float *deg)
{
    if (handle == NULL)                   /* check handle */
    {
        return 2;                         /* return error */
    }
    if (handle->inited != 1)              /* check handle initialization */
    {
        return 3;                         /* return error */
    }
    
    *deg = (float)(reg) * 0.0625f;        /* convert raw data to real data */
    
    return 0;                             /* success return 0 */
}

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
uint8_t max31856_cold_junction_temperature_convert_to_register(max31856_handle_t *handle, float deg, int16_t *reg)
{
    if (handle == NULL)                         /* check handle */
    {
        return 2;                               /* return error */
    }
    if (handle->inited != 1)                    /* check handle initialization */
    {
        return 3;                               /* return error */
    }
    
    *reg = (int16_t)(deg / 0.00390625f);        /* convert real data to register data */
    
    return 0;                                   /* success return 0 */
}

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
uint8_t max31856_cold_junction_temperature_convert_to_data(max31856_handle_t *handle, int16_t reg, float *deg)
{
    if (handle == NULL)                       /* check handle */
    {
        return 2;                             /* return error */
    }
    if (handle->inited != 1)                  /* check handle initialization */
    {
        return 3;                             /* return error */
    }
    
    *deg = (float)(reg) * 0.00390625f;        /* convert raw data to real data */
    
    return 0;                                 /* success return 0 */
}

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
uint8_t max31856_irq_handler(max31856_handle_t *handle)
{
    uint8_t res;
    uint8_t prev;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    res = a_max31856_read(handle, MAX31856_REG_SR, &prev, 1);                /* read fault status */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("max31856: read fault status failed.\n");        /* read fault status failed */
       
        return 1;                                                            /* return error */
    }
    if (((prev >> 7) & 0x01) != 0)                                           /* check cold junction out of range */
    {
        if (handle->receive_callback != NULL)                                /* not null */
        {
            handle->receive_callback(MAX31856_FAULT_STATUS_CJRANGE);         /* run the callback */
        }
    }
    if (((prev >> 6) & 0x01) != 0)                                           /* check thermocouple out of range */
    {
        if (handle->receive_callback != NULL)                                /* not null */
        {
            handle->receive_callback(MAX31856_FAULT_STATUS_TCRANGE);         /* run the callback */
        }
    }
    if (((prev >> 5) & 0x01) != 0)                                           /* check cold junction high fault */
    {
        if (handle->receive_callback != NULL)                                /* not null */
        {
            handle->receive_callback(MAX31856_FAULT_STATUS_CJHIGH);          /* run the callback */
        }
    }
    if (((prev >> 4) & 0x01) != 0)                                           /* check cold junction low fault */
    {
        if (handle->receive_callback != NULL)                                /* not null */
        {
            handle->receive_callback(MAX31856_FAULT_STATUS_CJLOW);           /* run the callback */
        }
    }
    if (((prev >> 3) & 0x01) != 0)                                           /* check thermocouple temperature high fault */
    {
        if (handle->receive_callback != NULL)                                /* not null */
        {
            handle->receive_callback(MAX31856_FAULT_STATUS_TCHIGH);          /* run the callback */
        }
    }
    if (((prev >> 2) & 0x01) != 0)                                           /* check thermocouple temperature low fault */
    {
        if (handle->receive_callback != NULL)                                /* not null */
        {
            handle->receive_callback(MAX31856_FAULT_STATUS_TCLOW);           /* run the callback */
        }
    }
    if (((prev >> 1) & 0x01) != 0)                                           /* check overvoltage or undervoltage input fault */
    {
        if (handle->receive_callback != NULL)                                /* not null */
        {
            handle->receive_callback(MAX31856_FAULT_STATUS_OVUV);            /* run the callback */
        }
    }
    if (((prev >> 0) & 0x01) != 0)                                           /* check thermocouple open circuit fault */
    {
        if (handle->receive_callback != NULL)                                /* not null */
        {
            handle->receive_callback(MAX31856_FAULT_STATUS_OPEN);            /* run the callback */
        }
    }
    
    return 0;                                                                /* success return 0 */
}

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
uint8_t max31856_set_reg(max31856_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    uint8_t res;
    
    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }
    
    res = a_max31856_write(handle, reg, buf, len);                    /* write data */
    if (res != 0)                                                     /* check result */
    {
        handle->debug_print("max31856: write data failed.\n");        /* write data failed */
       
        return 1;                                                     /* return error */
    }
    
    return 0;                                                         /* success return 0 */
}

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
uint8_t max31856_get_reg(max31856_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    uint8_t res;
    
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->inited != 1)                                         /* check handle initialization */
    {
        return 3;                                                    /* return error */
    }
    
    res = a_max31856_read(handle, reg, buf, len);                    /* read data */
    if (res != 0)                                                    /* check result */
    {
        handle->debug_print("max31856: read data failed.\n");        /* read data failed */
       
        return 1;                                                    /* return error */
    }
    
    return 0;                                                        /* success return 0 */
}

/**
 * @brief      get chip's information
 * @param[out] *info pointer to a max31856 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t max31856_info(max31856_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(max31856_info_t));                       /* initialize max31856 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "SPI", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
