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
 * @file      main.c
 * @brief     main source file
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
#include "driver_max31856_read_test.h"
#include "driver_max31856_interrupt_test.h"
#include "driver_max31856_basic.h"
#include "driver_max31856_shot.h"
#include "driver_max31856_interrupt.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "gpio.h"
#include "uart.h"
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
volatile uint16_t g_len;                   /**< uart buffer length */
static uint8_t gs_flag = 0;                /**< local flag */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq */

/**
 * @brief exti 0 irq
 * @note  none
 */
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
 * @brief     gpio exti callback
 * @param[in] pin gpio pin
 * @note      none
 */
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if (pin == GPIO_PIN_0)
    {
        if (g_gpio_irq != NULL)
        {
            g_gpio_irq();
        }
    }
}

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
            double deg;
            
            max31856_interface_debug_print("max31856: irq cold junction high fault.\n");
            gs_flag = 1;
            
            /* read cold junction temperature */
            res = max31856_interrupt_read_cold_junction_temperature(&deg);
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
            double deg;
            
            max31856_interface_debug_print("max31856: irq cold junction low fault.\n");
            gs_flag = 1;
            
            /* read cold junction temperature */
            res = max31856_interrupt_read_cold_junction_temperature(&deg);
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
            uint8_t fault;
            double deg;
            
            max31856_interface_debug_print("max31856: irq thermocouple temperature high fault.\n");
            gs_flag = 1;
            
            /* continuous read */
            res = max31856_interrupt_read(&deg, &fault);
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
            uint8_t fault;
            double deg;
            
            max31856_interface_debug_print("max31856: irq thermocouple temperature low fault.\n");
            gs_flag = 1;
            
            /* continuous read */
            res = max31856_interrupt_read(&deg, &fault);
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
 * @brief     max31856 full function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t max31856(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"cold-junction-low", required_argument, NULL, 1},
        {"cold-junction-high", required_argument, NULL, 2},
        {"temperature-low", required_argument, NULL, 3},
        {"temperature-high", required_argument, NULL, 4},
        {"type", required_argument, NULL, 5},
        {"timeout", required_argument, NULL, 6},
        {"times", required_argument, NULL, 7},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    float cold_junction_low = 20.0f;
    float cold_junction_high = 30.0f;
    float temperature_low = 25.0f;
    float temperature_high = 30.0f;
    uint32_t times = 3;
    uint32_t timeout = 5000;
    max31856_thermocouple_type_t thermocouple_type = MAX31856_THERMOCOUPLE_TYPE_K;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* cold junction low */
            case 1 :
            {
                /* set cold junction low */
                cold_junction_low = (float)atof(optarg);
                
                break;
            }
            
            /* cold junction high */
            case 2 :
            {
                /* set cold junction high */
                cold_junction_high = (float)atof(optarg);
                
                break;
            }
            
            /* temperature low */
            case 3 :
            {
                /* set temperature low */
                temperature_low = (float)atof(optarg);
                
                break;
            }
            
            /* temperature high */
            case 4 :
            {
                /* set temperature high */
                temperature_high = (float)atof(optarg);
                
                break;
            }
            
            /* type */
            case 5 :
            {
                if (strcmp(optarg, "B") == 0)
                {
                    thermocouple_type = MAX31856_THERMOCOUPLE_TYPE_B;
                }
                else if (strcmp(optarg, "E") == 0)
                {
                    thermocouple_type = MAX31856_THERMOCOUPLE_TYPE_E;
                }
                else if (strcmp(optarg, "J") == 0)
                {
                    thermocouple_type = MAX31856_THERMOCOUPLE_TYPE_J;
                }
                else if (strcmp(optarg, "K") == 0)
                {
                    thermocouple_type = MAX31856_THERMOCOUPLE_TYPE_K;
                }
                else if (strcmp(optarg, "N") == 0)
                {
                    thermocouple_type = MAX31856_THERMOCOUPLE_TYPE_N;
                }
                else if (strcmp(optarg, "R") == 0)
                {
                    thermocouple_type = MAX31856_THERMOCOUPLE_TYPE_R;
                }
                else if (strcmp(optarg, "S") == 0)
                {
                    thermocouple_type = MAX31856_THERMOCOUPLE_TYPE_S;
                }
                else if (strcmp(optarg, "T") == 0)
                {
                    thermocouple_type = MAX31856_THERMOCOUPLE_TYPE_T;
                }
                else if (strcmp(optarg, "GAIN8") == 0)
                {
                    thermocouple_type = MAX31856_THERMOCOUPLE_TYPE_VOLTAGE_GAIN_8;
                }
                else if (strcmp(optarg, "GAIN32") == 0)
                {
                    thermocouple_type = MAX31856_THERMOCOUPLE_TYPE_VOLTAGE_GAIN_32;
                }
                else
                {
                    return 5;
                }
                
                break;
            }
            
            /* timeout */
            case 6 :
            {
                /* set timeout */
                timeout = atol(optarg);
                
                break;
            } 
            
            /* running times */
            case 7 :
            {
                /* set the times */
                times = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        uint8_t res;
        
        /* run the register test */
        res = max31856_register_test();
        if (res != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_read", type) == 0)
    {
        uint8_t res;
        
        /* run the read test */
        res = max31856_read_test(thermocouple_type, times);
        if (res != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_int", type) == 0)
    {
        uint8_t res;
        
        /* run the interrupt test */
        g_gpio_irq = max31856_interrupt_test_irq_handler;
        (void)gpio_interrupt_init();
        res = max31856_interrupt_test(thermocouple_type,
                                      cold_junction_low,
                                      cold_junction_high,
                                      temperature_low,
                                      temperature_high,
                                      timeout);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
             g_gpio_irq = NULL;
            
            return 1;
        }
        
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* init */
        res = max31856_basic_init(thermocouple_type);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            double temp_deg;
            uint8_t fault;
            
            /* read data */
            res = max31856_basic_read(&temp_deg, &fault);
            if (res != 0)
            {
                (void)max31856_basic_deinit();
                if (fault != 0)
                {
                    if ((fault & MAX31856_FAULT_STATUS_CJRANGE) != 0)
                    {
                        max31856_interface_debug_print("max31856: cold junction out of range.\n");
                    }
                    else if ((fault & MAX31856_FAULT_STATUS_TCRANGE) != 0)
                    {
                        max31856_interface_debug_print("max31856: thermocouple out of range.\n");
                    }
                    else if ((fault & MAX31856_FAULT_STATUS_CJHIGH) != 0)
                    {
                        max31856_interface_debug_print("max31856: cold junction high fault.\n");
                    }
                    else if ((fault & MAX31856_FAULT_STATUS_CJLOW) != 0)
                    {
                        max31856_interface_debug_print("max31856: cold junction low fault.\n");
                    }
                    else if ((fault & MAX31856_FAULT_STATUS_TCHIGH) != 0)
                    {
                        max31856_interface_debug_print("max31856: thermocouple temperature high fault.\n");
                    }
                    else if ((fault & MAX31856_FAULT_STATUS_TCLOW) != 0)
                    {
                        max31856_interface_debug_print("max31856: thermocouple temperature low fault.\n");
                    }
                    else if ((fault & MAX31856_FAULT_STATUS_OVUV) != 0)
                    {
                        max31856_interface_debug_print("max31856: overvoltage or undervoltage input fault.\n");
                    }
                    else if ((fault & MAX31856_FAULT_STATUS_OPEN) != 0)
                    {
                        max31856_interface_debug_print("max31856: thermocouple open circuit fault.\n");
                    }
                    else
                    {
                        max31856_interface_debug_print("max31856: fault is unknown.\n");
                    }
                }
                
                return 1;
            }
            
            /* output */
            max31856_interface_debug_print("%d/%d %0.3fC.\n", i + 1, times, temp_deg);
            
            /* delay 1000ms */
            max31856_interface_delay_ms(1000);
        }
        
        /* deinit */
        (void)max31856_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_shot", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* init */
        res = max31856_shot_init(thermocouple_type);
        if (res != 0)
        {
            return 1;
        }
        
        /* loop */
        for (i = 0; i < times; i++)
        {
            double temp_deg;
            uint8_t fault;
            
            /* read data */
            res = max31856_shot_read(&temp_deg, &fault);
            if (res != 0)
            {
                (void)max31856_shot_deinit();
                if (fault != 0)
                {
                    if ((fault & MAX31856_FAULT_STATUS_CJRANGE) != 0)
                    {
                        max31856_interface_debug_print("max31856: cold junction out of range.\n");
                    }
                    else if ((fault & MAX31856_FAULT_STATUS_TCRANGE) != 0)
                    {
                        max31856_interface_debug_print("max31856: thermocouple out of range.\n");
                    }
                    else if ((fault & MAX31856_FAULT_STATUS_CJHIGH) != 0)
                    {
                        max31856_interface_debug_print("max31856: cold junction high fault.\n");
                    }
                    else if ((fault & MAX31856_FAULT_STATUS_CJLOW) != 0)
                    {
                        max31856_interface_debug_print("max31856: cold junction low fault.\n");
                    }
                    else if ((fault & MAX31856_FAULT_STATUS_TCHIGH) != 0)
                    {
                        max31856_interface_debug_print("max31856: thermocouple temperature high fault.\n");
                    }
                    else if ((fault & MAX31856_FAULT_STATUS_TCLOW) != 0)
                    {
                        max31856_interface_debug_print("max31856: thermocouple temperature low fault.\n");
                    }
                    else if ((fault & MAX31856_FAULT_STATUS_OVUV) != 0)
                    {
                        max31856_interface_debug_print("max31856: overvoltage or undervoltage input fault.\n");
                    }
                    else if ((fault & MAX31856_FAULT_STATUS_OPEN) != 0)
                    {
                        max31856_interface_debug_print("max31856: thermocouple open circuit fault.\n");
                    }
                    else
                    {
                        max31856_interface_debug_print("max31856: fault is unknown.\n");
                    }
                }
                
                return 1;
            }
            
            /* output */
            max31856_interface_debug_print("%d/%d %0.3fC.\n", i + 1, times, temp_deg);
            
            /* delay 1000ms */
            max31856_interface_delay_ms(1000);
        }
        
        /* deinit */
        (void)max31856_shot_deinit();
        
        return 0;
    }
    else if (strcmp("e_int", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        /* output */
        max31856_interface_debug_print("max31856: cold junction low fault threshold %0.3fC.\n", cold_junction_low);
        max31856_interface_debug_print("max31856: cold junction high fault threshold %0.3fC.\n", cold_junction_high);
        max31856_interface_debug_print("max31856: temperature low fault threshold %0.3fC.\n", temperature_low);
        max31856_interface_debug_print("max31856: temperature high fault threshold %0.3fC.\n", temperature_high);
        
        /* interrupt init */
        g_gpio_irq = max31856_interrupt_irq_handler;
        (void)gpio_interrupt_init();
        res = max31856_interrupt_init(thermocouple_type,
                                      cold_junction_low,
                                      cold_junction_high,
                                      temperature_low,
                                      temperature_high,
                                      a_callback);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
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
        
        /* deinit */
        max31856_interrupt_deinit();
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        max31856_interface_debug_print("Usage:\n");
        max31856_interface_debug_print("  max31856 (-i | --information)\n");
        max31856_interface_debug_print("  max31856 (-h | --help)\n");
        max31856_interface_debug_print("  max31856 (-p | --port)\n");
        max31856_interface_debug_print("  max31856 (-t reg | --test=reg)\n");
        max31856_interface_debug_print("  max31856 (-t read | --test=read) [--type=<B | E | J | K | N | R | S | T | GAIN8 | GAIN32>] [--times=<num>]\n");
        max31856_interface_debug_print("  max31856 (-t int | --test=int) [--type=<B | E | J | K | N | R | S | T | GAIN8 | GAIN32>]\n");
        max31856_interface_debug_print("           [--cold-junction-low=<deg>] [--cold-junction-high=<deg>]\n");
        max31856_interface_debug_print("           [--temperature-low=<deg>] [--temperature-high=<deg>]\n");
        max31856_interface_debug_print("           [--timeout=<ms>]\n");
        max31856_interface_debug_print("  max31856 (-e read | --example=read) [--type=<B | E | J | K | N | R | S | T | GAIN8 | GAIN32>] [--times=<num>]\n");
        max31856_interface_debug_print("  max31856 (-e shot | --example=shot) [--type=<B | E | J | K | N | R | S | T | GAIN8 | GAIN32>] [--times=<num>]\n");
        max31856_interface_debug_print("  max31856 (-e int | --example=int) [--type=<B | E | J | K | N | R | S | T | GAIN8 | GAIN32>]\n");
        max31856_interface_debug_print("           [--cold-junction-low=<deg>] [--cold-junction-high=<deg>]\n");
        max31856_interface_debug_print("           [--temperature-low=<deg>] [--temperature-high=<deg>]\n");
        max31856_interface_debug_print("           [--timeout=<ms>]\n");
        max31856_interface_debug_print("\n");
        max31856_interface_debug_print("Options:\n");
        max31856_interface_debug_print("      --cold-junction-low=<deg>      Set the cold junction low temperature.([default: 20])\n");
        max31856_interface_debug_print("      --cold-junction-high=<deg>     Set the cold junction high temperature.([default: 30])\n");
        max31856_interface_debug_print("  -e <read | shot | int>, --example=<read | shot | int>\n");
        max31856_interface_debug_print("                                     Run the driver example.\n");
        max31856_interface_debug_print("  -h, --help                         Show the help.\n");
        max31856_interface_debug_print("  -i, --information                  Show the chip information.\n");
        max31856_interface_debug_print("  -p, --port                         Display the pin connections of the current board.\n");
        max31856_interface_debug_print("  -t <reg | read | int>, --test=<reg | read | int>\n");
        max31856_interface_debug_print("                                     Run the driver test.\n");
        max31856_interface_debug_print("      --temperature-low=<deg>        Set the low temperature.([default: 25])\n");
        max31856_interface_debug_print("      --temperature-high=<deg>       Set the high temperature.([default: 30])\n");
        max31856_interface_debug_print("      --type=<B | E | J | K | N | R | S | T | GAIN8 | GAIN32>\n");
        max31856_interface_debug_print("                                     Set the thermocouple type.([default: K])\n");
        max31856_interface_debug_print("      --timeout=<ms>                 Set the timeout in ms.([default: 5000])\n");
        max31856_interface_debug_print("      --times=<num>                  Set the running times.([default: 3])\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        max31856_info_t info;
        
        /* print max31856 info */
        max31856_info(&info);
        max31856_interface_debug_print("max31856: chip is %s.\n", info.chip_name);
        max31856_interface_debug_print("max31856: manufacturer is %s.\n", info.manufacturer_name);
        max31856_interface_debug_print("max31856: interface is %s.\n", info.interface);
        max31856_interface_debug_print("max31856: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        max31856_interface_debug_print("max31856: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        max31856_interface_debug_print("max31856: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        max31856_interface_debug_print("max31856: max current is %0.2fmA.\n", info.max_current_ma);
        max31856_interface_debug_print("max31856: max temperature is %0.1fC.\n", info.temperature_max);
        max31856_interface_debug_print("max31856: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        max31856_interface_debug_print("max31856: SPI interface SCK connected to GPIOA PIN5.\n");
        max31856_interface_debug_print("max31856: SPI interface MISO connected to GPIOA PIN6.\n");
        max31856_interface_debug_print("max31856: SPI interface MOSI connected to GPIOA PIN7.\n");
        max31856_interface_debug_print("max31856: SPI interface CS connected to GPIOA PIN4.\n");
        max31856_interface_debug_print("max31856: INT connected to GPIOB PIN0.\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart init */
    uart_init(115200);
    
    /* shell init && register max31856 function */
    shell_init();
    shell_register("max31856", max31856);
    uart_print("max31856: welcome to libdriver max31856.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("max31856: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("max31856: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("max31856: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("max31856: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("max31856: param is invalid.\n");
            }
            else
            {
                uart_print("max31856: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
