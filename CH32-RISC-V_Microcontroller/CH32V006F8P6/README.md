# CH32V006F8P6 — Blink (Getting Started)

A minimal GPIO blink project for the **CH32V006F8P6** (WCH RISC-V MCU, TSSOP20 package),
built and flashed using **MounRiver Studio** and a **WCH-LinkE** programmer.

## Hardware

| Item | Details |
|---|---|
| MCU board | CH32V006F8P6 (TSSOP20) |
| Programmer | WCH-LinkE (clone or genuine) |
| Core | QingKe V2A (RISC-V, RV32EC), 48MHz |
| Flash / RAM | 62KB / 8KB |
| Onboard LED pin | Silkscreen **C0** → **PC0** (GPIOC, Pin 0) — matches the silkscreen label directly, confirmed by testing |

## Wiring (WCH-LinkE → board)

| LinkE pin | Board pin |
|---|---|
| 3V3 | VCC |
| GND | GND |
| SWIO | SWIO / DIO (single-wire debug — no clock line needed) |

## Toolchain

- **IDE**: MounRiver Studio
- **Chip family in wizard**: CH32V00x → CH32V006F8P6
- **Library**: WCH EVT (Peripheral drivers) — note that CH32V006 uses `RCC_PB2PeriphClockCmd` /
  `RCC_PB2Periph_GPIOx` naming, which differs from the CH32V003's `RCC_APB2PeriphClockCmd` /
  `RCC_APB2Periph_GPIOx` naming. Check `Peripheral/inc/ch32v00x_rcc.h` in your EVT package if a
  function name doesn't match.

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

#define LED_PORT   GPIOC
#define LED_PIN    GPIO_Pin_0

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_PB2PeriphClockCmd(RCC_PB2Periph_GPIOC, ENABLE);

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

1. Open the project in MounRiver Studio.
2. **Build** (hammer icon) to compile.
3. **Download** (chip + down-arrow icon) to flash over SWIO via the WCH-LinkE.
4. LED on PC0 should blink at 1Hz (500ms on / 500ms off).

## Troubleshooting notes

- **"Chip not found"** on connect → check SWIO wiring, and confirm the LinkE is in
  RISC-V mode (not ARM/SWD mode, if your probe has a mode switch/jumper).
- **Undefined reference to `RCC_PB2PeriphClockCmd`** → your EVT version may use different
  naming; check the actual declaration in `ch32v00x_rcc.h`.
- **LED doesn't light on the expected port** → don't assume a silkscreen label always maps
  cleanly; verify by testing. Confirmed on this board: `C0` = `PC0`.

## Next steps

- GPIO input (button press)
- PWM / LED fade
- ADC read (potentiometer)
- UART (template already present in `USARTx_CFG()` from the default project — multiprocessor
  echo demo on PD5/PD6 at 115200 baud)
