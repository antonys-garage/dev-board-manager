# CH32V003A4M6 — Blink (Getting Started)

A minimal GPIO blink project for the **CH32V003A4M6** (WCH RISC-V MCU, SOP16 package),
built and flashed using **MounRiver Studio** and a **WCH-LinkE** programmer.

## Hardware

| Item | Details |
|---|---|
| MCU board | CH32V003A4M6 (SOP16) |
| Programmer | WCH-LinkE (clone or genuine) |
| Core | QingKe V2A (RISC-V, RV32EC), 48MHz |
| Flash / RAM | 16KB / 2KB |
| Onboard LED pin | **Not yet confirmed** — verify against this board's silkscreen before trusting any assumed pin |

## Wiring (WCH-LinkE → board)

| LinkE pin | Board pin |
|---|---|
| 3V3 | VCC |
| GND | GND |
| SWIO | SWIO / DIO (single-wire debug — no clock line needed) |

## Toolchain

- **IDE**: MounRiver Studio
- **Chip family in wizard**: CH32V00x → CH32V003A4M6
- **Library**: WCH EVT (Peripheral drivers) — uses `RCC_APB2PeriphClockCmd` /
  `RCC_APB2Periph_GPIOx` naming (different from the CH32V006's `RCC_PB2...` naming).

## Project structure

```
YourProject/
├── User/
│   └── main.c          ← application code (only file you need to edit)
├── Core/                (core_riscv.c etc. — don't touch)
├── Debug/                (UART printf helpers)
├── Peripheral/           (EVT library — GPIO, RCC, USART, etc.)
├── Startup/              (startup_ch32v00x.S)
└── Ld/                   (linker script)
```

## Code (`User/main.c`)

```c
#include "debug.h"

#define LED_PORT   GPIOD
#define LED_PIN    GPIO_Pin_4   // placeholder — confirm against actual board silkscreen

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); // match the port you use

    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
}

int main(void)
{
    SystemCoreClockUpdate();
    Delay_Init();

    LED_Init();

    while (1)
    {
        GPIO_WriteBit(LED_PORT, LED_PIN, Bit_SET);
        Delay_Ms(500);
        GPIO_WriteBit(LED_PORT, LED_PIN, Bit_RESET);
        Delay_Ms(500);
    }
}
```

## Build & Flash

1. Open the project in MounRiver Studio (chip: CH32V00x → CH32V003A4M6).
2. **Build** (hammer icon) to compile.
3. **Download** (chip + down-arrow icon) to flash over SWIO via the WCH-LinkE.
4. LED should blink at 1Hz once the correct pin is confirmed.

## Troubleshooting notes

- **"Chip not found"** on connect → check SWIO wiring, and confirm the LinkE is in
  RISC-V mode (not ARM/SWD mode, if your probe has a mode switch/jumper).
- **LED doesn't light on the expected port** → try `GPIOA` or `GPIOC` with the same pin
  number, same trial-and-error approach used to find `PC0` on the V006 board.

## Next steps

- GPIO input (button press)
- PWM / LED fade
- ADC read (potentiometer)
