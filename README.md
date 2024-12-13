# Universal-IO using ADS1247   
Reference impleentation from https://www.ti.com/lit/ug/slau520a/slau520a.pdf


Code Uses settingd from the  Table 12. ADS1247 Register Settings in page 30
### Table 12: ADS1247 Register Settings

| **Register Address** | **Register Name** | **Default Value (Hex)** | **Bit 7** | **Bit 6** | **Bit 5** | **Bit 4** | **Bit 3** | **Bit 2** | **Bit 1** | **Bit 0** | **Description**                                                                                                                             |
|----------------------|--------------------|-------------------------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|------------------------------------------------------------------------------------------------------------------------------------------|
| 0x00                 | STATUS             | 0x08                    | POR        | RDATAC   |   -       |  -        |  -       |  -       |  -        |    -      | Power-on-reset (POR) flag, RDATAC enable/disable flag                                                                                             |
| 0x01                 | MUX0               | 0x01                    | AINP2      | AINP1      | AINP0      |  AINN2      |  AINN1     | AINN0       |  -        |  -        | Positive input selection, negative input selection                                                                                                   |
| 0x02                 | MUX1               | 0x00                    |  -       |  -       |  -       |  -       |  -       |   VREF       |  -     |  -    |  Reference selection                                                                                                                            |
| 0x03                 | SYS                | 0x00                    |  -       |  -       |  -       |  -       |  -       | CLK_OUT     | MODE1      | MODE0      | Clock output enable, operating mode selection                                                                                                 |
| 0x04                 | IDAC0              | 0x00                    |   IDAC2   | IDAC1     | IDAC0     |  -        |  -       |  -       |  -        |  -     | Current source magnitude                                                                                                                    |
| 0x05                 | IDAC1              | 0x00                    | IDAC_E  | IDAC_DIR  |  -      |  -        |  -       |  -       |  -        |   -        | Current source enable, current source direction.                                                                                          |
| 0x06                 | PGA                | 0x00                    | PGA2      | PGA1      | PGA0      |  -        |  -       |  -       |  -        | -       |  Programmable Gain Amplifier (PGA) gain selection                                                                                     |
| 0x07                 | DATA               | Not Applicable          |      -    |    -     |    -     |  -        |  -        |  -       |  -        |  -     | Read-only data register                                                                                                                |
| 0x08                 | GPIOCON            | 0x00                    | GPIO_7       | GPIO_6      | GPIO_5      |  GPIO_4      |  GPIO_3      |  GPIO_2       | GPIO_1      |   GPIO_0        |  General-Purpose Input/Output (GPIO) configuration of the associated digital pins as input or output                                                  |
| 0x09                 | GPIODIR            | 0x00                    | GPIO_7_DIR | GPIO_6_DIR| GPIO_5_DIR| GPIO_4_DIR |  GPIO_3_DIR | GPIO_2_DIR| GPIO_1_DIR  |  GPIO_0_DIR  | GPIO pin direction configuration; (1= output, 0 = input)       |
| 0x0A                | GPIODATA           | 0x00                    | GPIO_7_OUT| GPIO_6_OUT | GPIO_5_OUT| GPIO_4_OUT | GPIO_3_OUT| GPIO_2_OUT | GPIO_1_OUT  |  GPIO_0_OUT | GPIO data read/write access                                           |
| 0x0B                  | OFFSET             | 0x00                    |  -        |  -       |  -       |  -        |  -       |  -       | -          |    -       | Offset correction data ( read only)                                  |
| 0x0C                  | GAIN               | 0x00                    | -         |  -       | -        | -        | -        |  -       |   -       |  -    | Gain calibration data (read only)                                 |

**Notes:**
*   " - " indicates the bit is reserved or not applicable.
*   Data register (0x07) is not settable.
*   The offset and gain registers (0x0B and 0x0C) are read-only.
