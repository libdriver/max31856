[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MAX31856

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/max31856/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

MAX31856은 냉접점 보상을 수행하고 모든 유형의 열전대 신호를 디지털화합니다. 출력 데이터는 섭씨(°C) 단위로 표시됩니다. 이 변환기는 0.0078125°C의 온도 분해능을 제공하며, 열전대 유형에 따라 최대 +1800°C에서 최저 -210°C까지 측정이 가능하고, 열전대 전압 측정 정확도는 ±0.15%입니다. 열전대 입력은 최대 ±45V의 과전압으로부터 보호됩니다. LUT(룩업 테이블)에는 여러 유형의 열전대(K, J, N, R, S, T, E, B)에 대한 선형성 보정 데이터가 저장되어 있습니다. 50Hz 및 60Hz의 라인 주파수 필터링과 열전대 오류 감지 기능이 포함되어 있습니다. SPI 호환 인터페이스를 통해 열전대 유형을 선택하고 변환 및 오류 감지 프로세스를 설정할 수 있습니다.

LibDriver MAX31856은 LibDriver에서 출시한 MAX31856용 완벽한 기능을 갖춘 드라이버입니다. 연속 읽기, 단일 읽기 및 추가 기능을 제공합니다. LibDriver는 MISRA 규정을 준수합니다.

### 콘텐츠

  - [설명](#설명)
  - [설치](#설치)
  - [사용](#사용)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [문서](#문서)
  - [기고](#기고)
  - [저작권](#저작권)
  - [문의하기](#문의하기)

### 설명

/src 디렉토리에는 LibDriver MAX31856의 소스 파일이 포함되어 있습니다.

/interface 디렉토리에는 LibDriver MAX31856용 플랫폼 독립적인 SPI버스 템플릿이 포함되어 있습니다.

/test 디렉토리에는 LibDriver MAX31856드라이버 테스트 프로그램이 포함되어 있어 칩의 필요한 기능을 간단히 테스트할 수 있습니다.

/example 디렉토리에는 LibDriver MAX31856프로그래밍 예제가 포함되어 있습니다.

/doc 디렉토리에는 LibDriver MAX31856오프라인 문서가 포함되어 있습니다.

/datasheet 디렉토리에는 MAX31856데이터시트가 있습니다.

/project 디렉토리에는 일반적으로 사용되는 Linux 및 마이크로컨트롤러 개발 보드의 프로젝트 샘플이 포함되어 있습니다. 모든 프로젝트는 디버깅 방법으로 셸 스크립트를 사용하며, 자세한 내용은 각 프로젝트의 README.md를 참조하십시오.

/misra 에는 LibDriver misra 코드 검색 결과가 포함됩니다.

### 설치

/interface 디렉토리에서 플랫폼 독립적인 SPI버스 템플릿을 참조하여 지정된 플랫폼에 대한 SPI버스 드라이버를 완성하십시오.

/src 디렉터리, 플랫폼용 인터페이스 드라이버 및 자체 드라이버를 프로젝트에 추가합니다. 기본 예제 드라이버를 사용하려면 /example 디렉터리를 프로젝트에 추가합니다.

### 사용

/example 디렉터리의 예제를 참조하여 자신만의 드라이버를 완성할 수 있습니다. 기본 프로그래밍 예제를 사용하려는 경우 사용 방법은 다음과 같습니다.

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

### 문서

온라인 문서: [https://www.libdriver.com/docs/max31856/index.html](https://www.libdriver.com/docs/max31856/index.html).

오프라인 문서: /doc/html/index.html.

### 기고

CONTRIBUTING.md 를 참조하십시오.

### 저작권

저작권 (c) 2015 - 지금 LibDriver 판권 소유

MIT 라이선스(MIT)

이 소프트웨어 및 관련 문서 파일("소프트웨어")의 사본을 얻은 모든 사람은 이에 따라 무제한 사용, 복제, 수정, 통합, 출판, 배포, 2차 라이선스를 포함하여 소프트웨어를 처분할 수 있는 권리가 부여됩니다. 소프트웨어의 사본에 대한 라이선스 및/또는 판매, 그리고 소프트웨어가 위와 같이 배포된 사람의 권리에 대한 2차 라이선스는 다음 조건에 따릅니다.

위의 저작권 표시 및 이 허가 표시는 이 소프트웨어의 모든 사본 또는 내용에 포함됩니다.

이 소프트웨어는 상품성, 특정 목적에의 적합성 및 비침해에 대한 보증을 포함하되 이에 국한되지 않는 어떠한 종류의 명시적 또는 묵시적 보증 없이 "있는 그대로" 제공됩니다. 어떤 경우에도 저자 또는 저작권 소유자는 계약, 불법 행위 또는 기타 방식에 관계없이 소프트웨어 및 기타 소프트웨어 사용으로 인해 발생하거나 이와 관련하여 발생하는 청구, 손해 또는 기타 책임에 대해 책임을 지지 않습니다.

### 문의하기

연락주세요lishifenging@outlook.com.