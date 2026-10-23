# WhiteMCU ESP32e DevKit V1

A 38-pin ESP32 development board made in India by WhiteMCU. It uses the **ESP32-WROOM-32E** module, has **USB-C**, a **CH340** USB-serial chip, separate **BOOT / RST** buttons and an **onboard RGB LED on GPIO5**.

> Pin labels in this document were read from photos of the board and cross-checked against the standard ESP32-WROOM-32 pin functions. Verify anything critical with a multimeter before wiring.

---

## Specifications

| Item | Details |
|---|---|
| Board | WhiteMCU ESP32e DevKit V1 |
| Module | Espressif ESP32-WROOM-32E (PCB antenna) |
| SoC | ESP32-D0WD-V3 (revision v3.x), dual-core Xtensa LX6, up to 240 MHz |
| Flash | 4 MB |
| SRAM / ROM | 520 KB / 448 KB (no PSRAM) |
| Wi-Fi | 802.11 b/g/n, 2.4 GHz |
| Bluetooth | v4.2 BR/EDR (Classic) + BLE. Supports SPP and A2DP |
| USB | USB-C, CH340 USB-to-UART bridge (no native USB) |
| Logic level | 3.3 V (pins are **not** 5 V tolerant) |
| Regulator | AMS1117-style 3.3 V LDO |
| Pins | 38 (19 per side), 0.1" pitch, breadboard friendly |
| Onboard LEDs | Power LED (3V3), RGB LED on GPIO5 (silkscreen `LED@IO5`) |
| Buttons | `BOOT` (GPIO0), `RST` (EN) |
| Module operating range | 3.0–3.6 V, −40 to +85 °C |

Module shield markings seen on this unit: ESPRESSIF ESP32-WROOM-32E, FCC ID 2AC7Z-ESP32WROOM32E, IC 21098-ESPWROOM32E, CMIIT ID 2020DP2713.

---

## Pinout

Orientation: USB-C connector at the **bottom**, antenna at the **top**.

```
                   ┌─ antenna ─┐
            ┌──────┴───────────┴──────┐
       3V3  o                         o  GND
        EN  o                         o  23
   VP (36)  o                         o  22
   VN (39)  o                         o  TX (1)
        34  o                         o  RX (3)
        35  o       ESP32-WROOM-32E   o  21
        32  o                         o  GND
        33  o                         o  19
        25  o                         o  18
        26  o                         o  5   (RGB LED)
        27  o                         o  17
        14  o                         o  16
        12  o                         o  4
       GND  o                         o  0   (BOOT)
        13  o                         o  2
   SD2 (9)  o                         o  15
  SD3 (10)  o                         o  SD1 (8)
       GND* o                         o  SD2* (7)
        5V  o                         o  CLK (6)
            │  [RST]   [USB-C]  [BOOT] │
            └──────────────────────────┘
```

`*` See "Silkscreen notes" below.
Numbers in brackets are GPIO numbers where the label is not already the GPIO number.

### Left side (top to bottom)

| Label | GPIO | Notes |
|---|---|---|
| 3V3 | – | 3.3 V rail from the regulator |
| EN | – | Chip enable / reset. The `RST` button pulls it low |
| VP | 36 | **Input only**. ADC1_CH0 |
| VN | 39 | **Input only**. ADC1_CH3 |
| 34 | 34 | **Input only**. ADC1_CH6 |
| 35 | 35 | **Input only**. ADC1_CH7 |
| 32 | 32 | ADC1_CH4, touch T9 |
| 33 | 33 | ADC1_CH5, touch T8 |
| 25 | 25 | DAC1, ADC2_CH8 |
| 26 | 26 | DAC2, ADC2_CH9 |
| 27 | 27 | ADC2_CH7, touch T7 |
| 14 | 14 | ADC2_CH6, touch T6, HSPI SCK. Outputs a PWM signal at boot |
| 12 | 12 | ADC2_CH5, touch T5, HSPI MISO. **Strapping pin: must be LOW at boot** |
| GND | – | Ground |
| 13 | 13 | ADC2_CH4, touch T4, HSPI MOSI |
| SD2 | 9 | Connected to flash. **Do not use** |
| SD3 | 10 | Connected to flash. **Do not use** |
| GND* | – | Labelled GND on this board (the CMD / GPIO11 position on standard boards) |
| 5V | – | 5 V from USB (VBUS), or 5 V input when not powered by USB |

### Right side (top to bottom)

| Label | GPIO | Notes |
|---|---|---|
| GND | – | Ground |
| 23 | 23 | VSPI MOSI |
| 22 | 22 | Default I²C SCL |
| TX | 1 | UART0 TX. Shared with the USB-serial chip and boot log |
| RX | 3 | UART0 RX. Shared with the USB-serial chip |
| 21 | 21 | Default I²C SDA |
| GND | – | Ground |
| 19 | 19 | VSPI MISO |
| 18 | 18 | VSPI SCK |
| 5 | 5 | VSPI CS. **Onboard RGB LED data pin.** Strapping pin, outputs PWM at boot |
| 17 | 17 | Default UART2 TX |
| 16 | 16 | Default UART2 RX |
| 4 | 4 | ADC2_CH0, touch T0 |
| 0 | 0 | **BOOT button.** Strapping pin: LOW at reset enters download mode. ADC2_CH1, touch T1 |
| 2 | 2 | Strapping pin. ADC2_CH2, touch T2. Keep floating or LOW at boot |
| 15 | 15 | Strapping pin. ADC2_CH3, touch T3, HSPI CS. LOW at boot silences the boot log |
| SD1 | 8 | Connected to flash. **Do not use** |
| SD2* | 7 | Connected to flash. **Do not use** (standard label is SD0) |
| CLK | 6 | Connected to flash. **Do not use** |

---

## Usable GPIO summary

| Type | GPIOs |
|---|---|
| Safe general-purpose I/O | 4, 13, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33 |
| Usable with care (strapping / boot behavior) | 0, 2, 5, 12, 14, 15 |
| Input only (no output, no internal pull-up/down) | 34, 35, 36 (VP), 39 (VN) |
| Reserved for UART0 / USB serial | 1 (TX), 3 (RX) |
| Connected to flash, never use | 6, 7, 8, 9, 10, 11 |

### Default peripheral pins (Arduino-ESP32)

| Peripheral | Pins |
|---|---|
| I²C (`Wire`) | SDA = 21, SCL = 22 |
| SPI (VSPI, default `SPI`) | SCK = 18, MISO = 19, MOSI = 23, CS = 5 |
| SPI (HSPI) | SCK = 14, MISO = 12, MOSI = 13, CS = 15 |
| UART0 (`Serial`) | TX = 1, RX = 3 (USB serial) |
| UART2 (`Serial2`) | TX = 17, RX = 16 |
| DAC | 25 (DAC1), 26 (DAC2) |
| Touch | 4, 0, 2, 15, 13, 12, 14, 27, 33, 32 |
| ADC1 (works with Wi-Fi) | 32, 33, 34, 35, 36, 39 |
| ADC2 (**does not work while Wi-Fi is on**) | 0, 2, 4, 12, 13, 14, 15, 25, 26, 27 |

Almost any output-capable GPIO can be remapped for PWM, UART, I²C and SPI through the GPIO matrix.

---

## Power

- **USB-C:** 5 V goes through the regulator to 3.3 V for the ESP32. This is the normal way to power and program the board.
- **5V pin:** can be used as a 5 V input (when USB is not connected) or as a 5 V output when powered from USB.
- **3V3 pin:** 3.3 V output. If you feed 3.3 V in from outside, do not connect USB at the same time.
- Keep peripheral current modest. The AMS1117-style regulator is not suited to heavy loads, and Wi-Fi transmit peaks can briefly draw several hundred mA.
- Use a good USB data cable. Charge-only cables will not show a serial port.

---

## Getting started (Arduino IDE)

1. Install the **esp32** board package by Espressif (Boards Manager).
2. Install the **CH340** USB driver if your OS doesn't detect the port.
3. Select **Tools → Board → ESP32 Dev Module**.
4. Select the correct COM port.
5. Upload speed: `921600`, or `115200` if uploads fail.
6. Flash size: **4MB**. If a sketch is too large, choose a partition scheme such as *Huge APP*.

**If upload fails with "Failed to connect":** hold **BOOT**, tap **RST**, release **BOOT** when "Connecting..." appears, then let the upload continue.

### Example: onboard RGB LED (GPIO5)

```cpp
#define RGB_PIN 5

void setup() {}

void loop() {
  neopixelWrite(RGB_PIN, 32, 0, 0);  // red
  delay(500);
  neopixelWrite(RGB_PIN, 0, 32, 0);  // green
  delay(500);
  neopixelWrite(RGB_PIN, 0, 0, 32);  // blue
  delay(500);
}
```

The LED sits on a single data pin, so drive it like a WS2812 / NeoPixel. If the colors come out swapped, the color order differs from what the library assumes. GPIO5 is also the VSPI CS pin, so avoid using it for an SPI device at the same time.

### Example: Bluetooth Classic serial (SPP)

```cpp
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32e-WhiteMCU");
}

void loop() {
  if (Serial.available()) SerialBT.write(Serial.read());
  if (SerialBT.available()) Serial.write(SerialBT.read());
}
```

---

## Verify the chip with esptool

```bash
pip install esptool
esptool --port COM5 chip_id
esptool --port COM5 flash_id
```

(Use `chip-id` / `flash-id` on newer esptool versions, and `esptool.py` on older ones. Replace `COM5` with your port.)

Expected for this board:
- Chip: **ESP32-D0WD-V3 (revision v3.x)**, dual core, 40 MHz crystal
- Flash: **4 MB**

---

## Cautions

- Pins are 3.3 V only. Use a level shifter for 5 V signals.
- Don't drive GPIO 0, 2, 12 or 15 into the wrong level at reset, or the board may not boot or may enter download mode.
- Don't connect anything to GPIO 6–11. They are used by the onboard flash.
- GPIO 34, 35, 36 and 39 are input only and have no internal pull-ups.
- Avoid ADC2 pins for analog reads when Wi-Fi is active.
- A metal enclosure or a hand over the antenna end of the board reduces Wi-Fi and Bluetooth range.

---

## Silkscreen notes

Two labels on this board differ from the standard 38-pin ESP32 DevKit layout. Check them on your own unit.

- On the right side, next to `SD1` and `CLK`, the label reads `SD2`. On a standard layout this pin is `SD0` (GPIO7).
- On the left side, next to `5V`, the label reads `GND`. On a standard layout this is `CMD` (GPIO11).

Both are flash pins that you shouldn't use anyway, so this won't affect normal projects.

---

## Status

- [x] Chip and flash identified
- [x] RGB LED on GPIO5
- [ ] Pinout checked pin by pin with a multimeter
- [ ] Wi-Fi range test
- [ ] Auto-reset reliability over many uploads
