[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MAX31856

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/max31856/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

The MAX31856 performs cold-junction compensation and digitizes the signal from any type of thermocouple. The output data is formatted in degrees Celsius. This converter resolves temperatures to 0.0078125°C, allows readings as high as +1800°C and as low as -210°C (depending on thermocouple type), and exhibits thermocouple voltage measurement accuracy of ±0.15%. The thermocouple inputs are protected against overvoltage conditions up to ±45V. A lookup table (LUT) stores linearity correction data for several types of thermocouples (K, J, N, R, S, T, E, and B). Line frequency filtering of 50Hz and 60Hz is included, as is thermocouple fault detection. A SPI-compatible interface allows selection of thermocouple type and setup of the conversion and fault detection processes.

LibDriver MAX31856 is a full-featured driver for MAX31856, launched by LibDriver.It provides continuous reading, single reading and additional features. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver MAX31856 source files.

/interface includes LibDriver MAX31856 SPI platform independent template.

/test includes LibDriver MAX31856 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver MAX31856 sample code.

/doc includes LibDriver MAX31856 offline document.

/datasheet includes MAX31856 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface SPI platform independent template and finish your platform SPI driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_max31856_basic.h"

uint8_t res;
uint32_t i;
max31856_thermocouple_type_t thermocouple_type = MAX31856_THERMOCOUPLE_TYPE_K;

/* init */
res = max31856_basic_init(thermocouple_type);
if (res != 0)
{
    return 1;
}

...
    
/* loop */
for (i = 0; i < 3; i++)
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
    max31856_interface_debug_print("%d/%d %0.3fC.\n", i + 1, 3, temp_deg);

    /* delay 1000ms */
    max31856_interface_delay_ms(1000);
    
    ...
}

...
    
/* deinit */
(void)max31856_basic_deinit();

return 0;
```

#### example shot

```C
#include "driver_max31856_shot.h"

uint8_t res;
uint32_t i;
max31856_thermocouple_type_t thermocouple_type = MAX31856_THERMOCOUPLE_TYPE_K;

/* init */
res = max31856_shot_init(thermocouple_type);
if (res != 0)
{
    return 1;
}

...
    
/* loop */
for (i = 0; i < 3; i++)
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
    max31856_interface_debug_print("%d/%d %0.3fC.\n", i + 1, 3, temp_deg);

    /* delay 1000ms */
    max31856_interface_delay_ms(1000);
    
    ...
}

...
    
/* deinit */
(void)max31856_shot_deinit();

return 0;
```

#### example interrupt

```C
#include "driver_max31856_interrupt.h"

uint8_t res;
uint32_t i;
float cold_junction_low = 20.0f;
float cold_junction_high = 30.0f;
float temperature_low = 25.0f;
float temperature_high = 30.0f;
uint32_t timeout = 5000;
max31856_thermocouple_type_t thermocouple_type = MAX31856_THERMOCOUPLE_TYPE_K;
static uint8_t gs_flag = 0;
uint8_t (*g_gpio_irq)(void) = NULL;

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

/* output */
max31856_interface_debug_print("max31856: cold junction low fault threshold %0.3fC.\n", cold_junction_low);
max31856_interface_debug_print("max31856: cold junction high fault threshold %0.3fC.\n", cold_junction_high);
max31856_interface_debug_print("max31856: temperature low fault threshold %0.3fC.\n", temperature_low);
max31856_interface_debug_print("max31856: temperature high fault threshold %0.3fC.\n", temperature_high);

...
    
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

...
    
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
    
    ...
}

...
    
/* deinit */
max31856_interrupt_deinit();
(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/max31856/index.html](https://www.libdriver.com/docs/max31856/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.