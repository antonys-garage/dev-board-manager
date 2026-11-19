# EDU PICO (V1 board) demo code for Raspberry Pi Pico 2 W

CircuitPython demo code for the **EDU PICO V1** board running a **Raspberry Pi Pico 2 W**.

<p align="center">
  <img src="https://raw.githubusercontent.com/CytronTechnologies/Cytron-EDU-PICO/main/images/edu-pico-top-800x800.png" alt="EDU PICO V1 board, top view" width="500">
</p>
<p align="center"><em>EDU PICO V1 board (image: Cytron Technologies)</em></p>

Based on the official Cytron demo code:
- Pico 2 version: https://github.com/CytronTechnologies/Cytron-EDU-PICO2/tree/main/demo_code/circuitpython
- V1 version: https://github.com/CytronTechnologies/Cytron-EDU-PICO/tree/main/demo_code/circuitpython

## Why this exists

| Problem | Cause | Fix |
| --- | --- | --- |
| Sensor tests fail with the Pico 2 demo code | The Pico 2 demo targets the V2 board (PAJ7620 + OPT4048 sensors). The V1 board has an **APDS9960**. | Use the V1 `edu_apds9960_*` modules. |
| APDS9960 library will not load on Pico 2 W | The V1 repo ships `.mpy` files compiled for CircuitPython 8. Pico 2 W needs CircuitPython 9.2+. | Ship the library as plain `.py` source (`adafruit_apds9960` v3.1.19). |

## What was changed

- `code.py` imports `edu_apds9960_proximity`, `edu_apds9960_light`, `edu_apds9960_gesture` and `edu_apds9960_colour` instead of the PAJ7620/OPT4048 modules.
- `edu_pico_lib/` contains the four V1 APDS9960 modules (the V2 sensor modules were removed).
- `lib/adafruit_apds9960/` contains the library as `.py` files.
- Everything else comes unchanged from the Pico 2 demo code.

The I2C pins are the same on both boards (SDA = GP4, SCL = GP5), so no pin changes were needed.

## Installation

1. Install the CircuitPython firmware for the **Raspberry Pi Pico 2 W** from https://circuitpython.org/board/raspberry_pi_pico2_w/ (hold BOOTSEL while plugging in USB, then copy the `.uf2`).
2. Open the **CIRCUITPY** drive.
3. Copy the full contents of this folder onto it, replacing existing files:
   - `code.py`
   - `settings.toml`
   - `font5x8.bin` and `font5x8.bin.license`
   - `edu_pico_lib/`
   - `images/`
   - `sd/`
   - `lib/`

   The `docs/` folder and this README are only for documentation, so you can leave them off the board.
4. The board restarts and the demo starts on the OLED.

## Required libraries

Already included in `lib/`:
- `adafruit_apds9960` (as `.py`)
- `adafruit_displayio_ssd1306.mpy`

Not included. Get these from the Adafruit CircuitPython Library Bundle matching your CircuitPython version (https://circuitpython.org/libraries):
- `adafruit_ssd1306`, `adafruit_framebuf`
- `adafruit_ahtx0`
- `adafruit_display_text`, `adafruit_imageload`
- `adafruit_motor`, `adafruit_sdcard`
- `neopixel`, `simpleio`
- `adafruit_bus_device` (only if your firmware does not already include it)

## Demo tests

| Test | Hardware |
| --- | --- |
| Buttons | Buttons A and B |
| NeoPixel | RGB LEDs |
| Temperature and humidity | AHT20 |
| Proximity, light, gesture, colour | APDS9960 |
| Microphone | PDM microphone |
| Potentiometer | Onboard potentiometer |
| Audio and buzzer | Buzzer or audio jack |
| SD card | Micro SD socket |
| Servo, motor, USB relay | Servo ports, motor terminals, USB relay |

Use button A for BACK and button B for NEXT, as shown on the OLED.

## Troubleshooting

- **`ImportError: no module named ...`**: copy the missing library from the bundle into `lib/`. Use the bundle that matches your CircuitPython major version (9.x or 10.x).
- **`ValueError: Invalid ... mpy`**: a `.mpy` file was built for a different CircuitPython version. Replace it with one from the matching bundle, or use the `.py` source.
- **APDS9960 not found / I2C error**: check that the Pico 2 W is seated correctly and the board is powered on. A I2C scan should show the OLED at `0x3C` and the AHT20 at `0x38`.

## Credits

Original demo code and hardware by Cytron Technologies. The APDS9960 library is by Adafruit (MIT License).
