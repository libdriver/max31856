### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

SPI Pin: SCK/MISO/CS  PA5/PA6/PA4.

GPIO Pin: INT PB0.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. MAX31856

#### 3.1 Command Instruction

1. Show max31856 chip and driver information.

   ```shell
   max31856 (-i | --information)
   ```

2. Show max31856 help.

   ```shell
   max31856 (-h | --help)
   ```

3. Show max31856 pin connections of the current board.

   ```shell
   max31856 (-p | --port)
   ```

4. Run max31856 register test. 

   ```shell
   max31856 (-t reg | --test=reg)
   ```

5. Run max31856 read test, num is the test times. 

   ```shell
   max31856 (-t read | --test=read) [--type=<B | E | J | K | N | R | S | T | GAIN8 | GAIN32>] [--times=<num>]
   ```
   
6. Run max31856 interrupt test, deg is the temperature in C and ms is the timeout in ms. 

   ```shell
   max31856 (-t int | --test=int) [--type=<B | E | J | K | N | R | S | T | GAIN8 | GAIN32>] [--cold-junction-low=<deg>] [--cold-junction-high=<deg>] [--temperature-low=<deg>] [--temperature-high=<deg>] [--timeout=<ms>]
   ```
   
7. Run max31856 read function, num is the read times. 

   ```shell
   max31856 (-e read | --example=read) [--type=<B | E | J | K | N | R | S | T | GAIN8 | GAIN32>] [--times=<num>]
   ```
   
8. Run max31856 shot function, num is the read times. 

   ```shell
   max31856 (-e shot | --example=shot) [--type=<B | E | J | K | N | R | S | T | GAIN8 | GAIN32>] [--times=<num>]
   ```
   
9. Run max31856 interrupt function, deg is the temperature in C and ms is the timeout in ms. 

   ```shell
   max31856 (-e int | --example=int) [--type=<B | E | J | K | N | R | S | T | GAIN8 | GAIN32>] [--cold-junction-low=<deg>] [--cold-junction-high=<deg>] [--temperature-low=<deg>] [--temperature-high=<deg>] [--timeout=<ms>]
   ```

#### 3.2 Command Example

```shell
max31856 -i

max31856: chip is Maxim Integrated MAX31856.
max31856: manufacturer is Maxim Integrated.
max31856: interface is SPI.
max31856: driver version is 1.0.
max31856: min supply voltage is 3.0V.
max31856: max supply voltage is 3.6V.
max31856: max current is 2.00mA.
max31856: max temperature is 125.0C.
max31856: min temperature is -55.0C.
```

```shell
max31856 -p

max31856: SPI interface SCK connected to GPIOA PIN5.
max31856: SPI interface MISO connected to GPIOA PIN6.
max31856: SPI interface MOSI connected to GPIOA PIN7.
max31856: SPI interface CS connected to GPIOA PIN4.
max31856: INT connected to GPIOB PIN0.
```

```shell
max31856 -t reg

max31856: chip is Maxim Integrated MAX31856.
max31856: manufacturer is Maxim Integrated.
max31856: interface is SPI.
max31856: driver version is 1.0.
max31856: min supply voltage is 3.0V.
max31856: max supply voltage is 3.6V.
max31856: max current is 2.00mA.
max31856: max temperature is 125.0C.
max31856: min temperature is -55.0C.
max31856: start register test.
max31856: max31856_set_open_circuit_detection/max31856_get_open_circuit_detection test.
max31856: disable open circuit detection.
max31856: check open circuit detection ok.
max31856: set open circuit detection mode1.
max31856: check open circuit detection ok.
max31856: set open circuit detection mode2.
max31856: check open circuit detection ok.
max31856: set open circuit detection mode3.
max31856: check open circuit detection ok.
max31856: max31856_set_cold_junction_sensor/max31856_get_cold_junction_sensor test.
max31856: enable cold junction sensor.
max31856: check cold junction sensor ok.
max31856: disable cold junction sensor.
max31856: check cold junction sensor ok.
max31856: max31856_set_fault_mode/max31856_get_fault_mode test.
max31856: set fault comparator mode.
max31856: check fault mode ok.
max31856: set fault interrupt mode.
max31856: check fault mode ok.
max31856: max31856_set_noise_rejection_filter/max31856_get_noise_rejection_filter test.
max31856: set noise rejection filter 60hz.
max31856: check noise rejection filter ok.
max31856: set noise rejection filter 50hz.
max31856: check noise rejection filter ok.
max31856: max31856_set_sample_average/max31856_get_sample_average test.
max31856: set sample average 1.
max31856: check sample average ok.
max31856: set sample average 2.
max31856: check sample average ok.
max31856: set sample average 4.
max31856: check sample average ok.
max31856: set sample average 8.
max31856: check sample average ok.
max31856: set sample average 16.
max31856: check sample average ok.
max31856: max31856_set_thermocouple_type/max31856_get_thermocouple_type test.
max31856: set thermocouple type b.
max31856: check thermocouple type ok.
max31856: set thermocouple type e.
max31856: check thermocouple type ok.
max31856: set thermocouple type j.
max31856: check thermocouple type ok.
max31856: set thermocouple type k.
max31856: check thermocouple type ok.
max31856: set thermocouple type n.
max31856: check thermocouple type ok.
max31856: set thermocouple type r.
max31856: check thermocouple type ok.
max31856: set thermocouple type s.
max31856: check thermocouple type ok.
max31856: set thermocouple type t.
max31856: check thermocouple type ok.
max31856: set thermocouple voltage gain 8.
max31856: check thermocouple type ok.
max31856: set thermocouple voltage gain 32.
max31856: check thermocouple type ok.
max31856: max31856_set_fault_mask/max31856_get_fault_mask test.
max31856: enable cold junction high fault threshold mask.
max31856: check fault mask ok.
max31856: disable cold junction high fault threshold mask.
max31856: check fault mask ok.
max31856: enable cold junction low fault threshold mask.
max31856: check fault mask ok.
max31856: disable cold junction low fault threshold mask.
max31856: check fault mask ok.
max31856: enable thermocouple temperature high fault threshold mask.
max31856: check fault mask ok.
max31856: disable thermocouple temperature high fault threshold mask.
max31856: check fault mask ok.
max31856: enable thermocouple temperature low fault threshold mask.
max31856: check fault mask ok.
max31856: disable thermocouple temperature low fault threshold mask.
max31856: check fault mask ok.
max31856: enable over voltage or undervoltage input fault mask.
max31856: check fault mask ok.
max31856: disable over voltage or undervoltage input fault mask.
max31856: check fault mask ok.
max31856: enable thermocouple open circuit fault mask.
max31856: check fault mask ok.
max31856: disable thermocouple open circuit fault mask.
max31856: check fault mask ok.
max31856: max31856_set_cold_junction_high_fault_threshold/max31856_get_cold_junction_high_fault_threshold test.
max31856: set cold junction high fault threshold 48.
max31856: check cold junction high fault threshold ok.
max31856: max31856_set_cold_junction_low_fault_threshold/max31856_get_cold_junction_low_fault_threshold test.
max31856: set cold junction low fault threshold -75.
max31856: check cold junction low fault threshold ok.
max31856: max31856_set_temperature_high_fault_threshold/max31856_get_temperature_high_fault_threshold test.
max31856: set temperature high fault threshold 31561.
max31856: check temperature high fault threshold ok.
max31856: max31856_set_temperature_low_fault_threshold/max31856_get_temperature_low_fault_threshold test.
max31856: set temperature low fault threshold -22632.
max31856: check temperature low fault threshold ok.
max31856: max31856_set_cold_junction_temperature_offset/max31856_get_cold_junction_temperature_offset test.
max31856: set cold junction temperature offset -7.
max31856: check cold junction temperature offset ok.
max31856: max31856_set_cold_junction_temperature/max31856_get_cold_junction_temperature test.
max31856: set cold junction temperature 21317.
max31856: check cold junction temperature 21316.
max31856: max31856_clear_fault test.
max31856: check clear fault ok.
max31856: max31856_get_fault_status test.
max31856: status is 0x00.
max31856: max31856_cold_junction_fault_threshold_convert_to_register/max31856_cold_junction_fault_threshold_convert_to_data test.
max31856: set cold junction fault threshold 2.060C.
max31856: check cold junction fault threshold 2.000C.
max31856: max31856_temperature_fault_threshold_convert_to_register/max31856_temperature_fault_threshold_convert_to_data test.
max31856: set temperature fault threshold 48.350C.
max31856: check temperature fault threshold 48.312C.
max31856: max31856_cold_junction_temperature_offset_convert_to_register/max31856_cold_junction_temperature_offset_convert_to_data test.
max31856: set cold junction temperature offset 0.614C.
max31856: check cold junction temperature offset 0.562C.
max31856: max31856_cold_junction_temperature_convert_to_register/max31856_cold_junction_temperature_convert_to_data test.
max31856: set cold junction temperature 15.280C.
max31856: check cold junction temperature 15.277C.
max31856: finish register test.
```

```shell
max31856 -t read --type=K --times=3

max31856: chip is Maxim Integrated MAX31856.
max31856: manufacturer is Maxim Integrated.
max31856: interface is SPI.
max31856: driver version is 1.0.
max31856: min supply voltage is 3.0V.
max31856: max supply voltage is 3.6V.
max31856: max current is 2.00mA.
max31856: max temperature is 125.0C.
max31856: min temperature is -55.0C.
max31856: start read test.
max31856: cold junction temperature read test.
max31856: cold junction temperature is 25.500C.
max31856: cold junction temperature is 25.500C.
max31856: cold junction temperature is 25.500C.
max31856: single read test.
max31856: temperature is 25.102C.
max31856: temperature is 25.172C.
max31856: temperature is 25.133C.
max31856: continuous read test.
max31856: temperature is 25.219C.
max31856: temperature is 25.273C.
max31856: temperature is 25.219C.
max31856: finish read test.
```

```shell
max31856 -t int --type=K --cold-junction-low=20 --cold-junction-high=30 --temperature-low=25 --temperature-high=30 --timeout=5000

max31856: chip is Maxim Integrated MAX31856.
max31856: manufacturer is Maxim Integrated.
max31856: interface is SPI.
max31856: driver version is 1.0.
max31856: min supply voltage is 3.0V.
max31856: max supply voltage is 3.6V.
max31856: max current is 2.00mA.
max31856: max temperature is 125.0C.
max31856: min temperature is -55.0C.
max31856: start interrupt test.
max31856: cold junction low fault threshold 20.000C.
max31856: cold junction high fault threshold 30.000C.
max31856: temperature low fault threshold 25.000C.
max31856: temperature high fault threshold 30.000C.
max31856: irq thermocouple temperature low fault.
max31856: temperature is 24.750C.
max31856: find interrupt.
max31856: finish interrupt test.
```

```shell
max31856 -e read --type=K --times=3

1/3 25.236C.
2/3 24.883C.
3/3 24.969C.
```

```shell
max31856 -e shot --type=K --times=3

1/3 24.766C.
2/3 24.836C.
3/3 24.836C.
```

```shell
max31856 -e int --type=K --cold-junction-low=20 --cold-junction-high=30 --temperature-low=25 --temperature-high=30 --timeout=5000

max31856: cold junction low fault threshold 20.000C.
max31856: cold junction high fault threshold 30.000C.
max31856: temperature low fault threshold 25.000C.
max31856: temperature high fault threshold 30.000C.
max31856: irq thermocouple temperature high fault.
max31856: temperature is 30.008C.
max31856: find interrupt.
```

```shell
max31856 -h

Usage:
  max31856 (-i | --information)
  max31856 (-h | --help)
  max31856 (-p | --port)
  max31856 (-t reg | --test=reg)
  max31856 (-t read | --test=read) [--type=<B | E | J | K | N | R | S | T | GAIN8 | GAIN32>] [--times=<num>]
  max31856 (-t int | --test=int) [--type=<B | E | J | K | N | R | S | T | GAIN8 | GAIN32>]
           [--cold-junction-low=<deg>] [--cold-junction-high=<deg>]
           [--temperature-low=<deg>] [--temperature-high=<deg>]
           [--timeout=<ms>]
  max31856 (-e read | --example=read) [--type=<B | E | J | K | N | R | S | T | GAIN8 | GAIN32>] [--times=<num>]
  max31856 (-e shot | --example=shot) [--type=<B | E | J | K | N | R | S | T | GAIN8 | GAIN32>] [--times=<num>]
  max31856 (-e int | --example=int) [--type=<B | E | J | K | N | R | S | T | GAIN8 | GAIN32>]
           [--cold-junction-low=<deg>] [--cold-junction-high=<deg>]
           [--temperature-low=<deg>] [--temperature-high=<deg>]
           [--timeout=<ms>]

Options:
      --cold-junction-low=<deg>      Set the cold junction low temperature.([default: 20])
      --cold-junction-high=<deg>     Set the cold junction high temperature.([default: 30])
  -e <read | shot | int>, --example=<read | shot | int>
                                     Run the driver example.
  -h, --help                         Show the help.
  -i, --information                  Show the chip information.
  -p, --port                         Display the pin connections of the current board.
  -t <reg | read | int>, --test=<reg | read | int>
                                     Run the driver test.
      --temperature-low=<deg>        Set the low temperature.([default: 25])
      --temperature-high=<deg>       Set the high temperature.([default: 30])
      --type=<B | E | J | K | N | R | S | T | GAIN8 | GAIN32>
                                     Set the thermocouple type.([default: K])
      --timeout=<ms>                 Set the timeout in ms.([default: 5000])
      --times=<num>                  Set the running times.([default: 3])
```

