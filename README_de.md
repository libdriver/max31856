[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver MAX31856
[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/max31856/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der MAX31856 kompensiert Kaltstellen und digitalisiert das Signal von Thermoelementen aller Art. Die Ausgabedaten werden in Grad Celsius formatiert. Dieser Wandler erreicht eine Temperaturauflösung von 0,0078125 °C, ermöglicht Messwerte von +1800 °C bis -210 °C (abhängig vom Thermoelementtyp) und bietet eine Messgenauigkeit der Thermoelementspannung von ±0,15 %. Die Thermoelementeingänge sind gegen Überspannungen bis ±45 V geschützt. Eine Lookup-Tabelle (LUT) speichert Linearitätskorrekturdaten für verschiedene Thermoelementtypen (K, J, N, R, S, T, E und B). Eine Netzfrequenzfilterung von 50 Hz und 60 Hz sowie eine Thermoelement-Fehlererkennung sind integriert. Über eine SPI-kompatible Schnittstelle können der Thermoelementtyp ausgewählt und die Konvertierungs- und Fehlererkennungsprozesse konfiguriert werden.

LibDriver MAX31856 ist ein umfassender Treiber für den MAX31856, entwickelt von LibDriver. Er bietet Funktionen für kontinuierliches und einzelnes Auslesen sowie weitere Features. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic](#example-basic)
    - [example shot](#example-shot)
    - [example interrupt](#example-interrupt)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver MAX31856-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver MAX31856 SPI.

/test enthält den Testcode des LibDriver MAX31856-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver MAX31856-Beispielcode.

/doc enthält das LibDriver MAX31856-Offlinedokument.

/Datenblatt enthält MAX31856-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige SPI-Schnittstellenvorlage und stellen Sie Ihren Plattform-SPI-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/max31856/index.html](https://www.libdriver.com/docs/max31856/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.