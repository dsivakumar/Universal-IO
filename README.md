# Universal-IO using ADS1247   
Reference impleentation from https://www.ti.com/lit/ug/slau520a/slau520a.pdf


Code uses settings from the  Table 12. ADS1247 Register Settings in page 30
### Table 12: ADS1247 Register Settings
**Table 12. ADS1247 Register Settings**

| Register | Address | Value | Description                                                                 |
|----------|---------|-------|-----------------------------------------------------------------------------|
| MUX0     | 00h     | 0Ah   | Select AIN0 as positive input and AIN3 as negative input                     |
| VBIAS    | 01h     | 00h   | Disable bias voltage                                                         |
| MUX1     | 02h     | 20h   | Enable internal reference and select REFP0 and REFN0 as reference inputs     |
| SYS0     | 03h     | 22h   | Set gain to 1 and data rate to 20 SPS                                        |
| OFC0     | 04h     | xxh   | Offset calibration least significant byte (value depends on calibration)     |
| OFC1     | 05h     | xxh   | Offset calibration middle byte (value depends on calibration)                |
| OFC2     | 06h     | xxh   | Offset calibration most significant byte (value depends on calibration)      |
| FSC0     | 07h     | xxh   | Full-scale calibration least significant byte (value depends on calibration) |
| FSC1     | 08h     | xxh   | Full-scale calibration middle byte (value depends on calibration)            |
| FSC2     | 09h     | xxh   | Full-scale calibration most significant byte (value depends on calibration)  |
| IDAC0    | 0Ah     | 96h   | Set IDAC current magnitude to 1 mA                                           |
| IDAC1    | 0Bh     | 03h   | Route IDAC1 to AIN0 and IDAC2 to AIN3                                        |
| GPIOCFG  | 0Ch     | 00h   | Configure all GPIO pins as inputs                                            |
| GPIODIR  | 0Dh     | 00h   | Set all GPIO pins to input direction                                         |
| GPIODAT  | 0Eh     | 00h   | Clear all GPIO data                                                          |

*Note:* Registers with values marked as 'xx' change depending on the calibration results.

Ref https://github.com/Flydroid/ADS12xx-Library but this code didn't run off the shelf!

![ADS12xx Diagram](https://github.com/dsivakumar/Universal-IO/blob/main/ads12xx.png)
