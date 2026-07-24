[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MAX31856

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/max31856/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

MAX31856は、冷接点補償を行い、あらゆるタイプの熱電対からの信号をデジタル化します。出力データは摂氏でフォーマットされます。このコンバータは、0.0078125℃の精度で温度を分解し、+1800℃から-210℃までの読み取りが可能で（熱電対の種類によって異なります）、熱電対電圧測定精度は±0.15%です。熱電対入力は、最大±45Vの過電圧から保護されています。ルックアップテーブル（LUT）には、複数のタイプの熱電対（K、J、N、R、S、T、E、B）の直線性補正データが格納されています。50Hzと60Hzのライン周波数フィルタリングと熱電対故障検出機能が含まれています。SPI互換インターフェースにより、熱電対タイプの選択、変換および故障検出プロセスの設定が可能です。

LibDriver MAX31856は、LibDriver社が開発したMAX31856用のフル機能ドライバです。連続読み取り、単一読み取り、およびその他の追加機能を提供します。LibDriverはMISRA規格に準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver MAX31856のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver MAX31856用のプラットフォームに依存しないSPIバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver MAX31856ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver MAX31856プログラミング例が含まれています。

/ docディレクトリには、LibDriver MAX31856オフラインドキュメントが含まれています。

/ datasheetディレクトリには、MAX31856データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないSPIバステンプレートを参照して、指定したプラットフォームのSPIバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/max31856/index.html](https://www.libdriver.com/docs/max31856/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。