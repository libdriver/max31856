[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MAX31856

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/max31856/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

MAX31856執行冷端補償，並將來自任何類型熱電偶的訊號數位化。 輸出數據以攝氏度為組織進行格式化。 該轉換器將溫度解析為0.0078125°C，允許讀數高達+1800°C，低至-210°C（取決於熱電偶類型），並顯示熱電偶電壓測量精度為±0.15%。 熱電偶輸入端可防止高達±45V的過電壓條件。 查找錶（LUT）存儲了幾種類型的熱電偶（K、J、N、R、S、T、E和B）的線性校正數據。 包括50Hz和60Hz的線路頻率濾波，以及熱電偶故障檢測。 SPI相容介面允許選擇熱電偶類型以及設定轉換和故障檢測過程。

LibDriver MAX31856是LibDriver推出的MAX31856的全功能驅動，該驅動提供熱電偶溫度連續模式讀取、熱電偶溫度單次模式讀取等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver MAX31856的源文件。

/interface目錄包含了LibDriver MAX31856與平台無關的SPI總線模板。

/test目錄包含了LibDriver MAX31856驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver MAX31856編程範例。

/doc目錄包含了LibDriver MAX31856離線文檔。

/datasheet目錄包含了MAX31856數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的SPI總線模板，完成指定平台的SPI總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

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

### 文檔

在線文檔: [https://www.libdriver.com/docs/max31856/index.html](https://www.libdriver.com/docs/max31856/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。