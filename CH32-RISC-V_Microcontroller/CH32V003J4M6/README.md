# CH32V003J4M6 — Blink (Getting Started)

A minimal GPIO blink project for the **CH32V003J4M6** (WCH RISC-V MCU, SOP8 package),
built and flashed using **MounRiver Studio** and a **WCH-LinkE** programmer.

Same silicon as the CH32V003A4M6, but in a much smaller SOP8 package with far fewer
pins broken out — only 6 GPIO total, and one of those doubles as the debug pin.

## Hardware

| Item | Details |
|---|---|
| MCU board | CH32V003J4M6 (SOP8) |
| Programmer | WCH-LinkE (clone or genuine) |
| Core | QingKe V2A (RISC-V, RV32EC), 48MHz |
| Flash / RAM | 16KB / 2KB |
| Onboard LED pin | Silkscreen **C1** → **PC1** (GPIOC, Pin 1) — confirmed by testing |

## Confirmed board pinout

| Silkscreen label | Function | Notes |
|---|---|---|
| C1 | PC1 | Safe GPIO — used for onboard LED |
| C2 | PC2 | Safe GPIO |
| A2 | PA2 | Safe GPIO |
| D6 | PD6 | Safe GPIO |
| C4 | PC4 | Safe GPIO |
| **D4** | **PD1 / PD4 / PD5 (shared pad)** | **SWIO / debug — do not use as GPIO** |
| GND | VSS | Power |
| VCC | VDD | Power |

The pad labeled `D4` on this board is electrically the same physical pin as PD1/PD5 —
WCH's SOP8 package shares one bond pad across those three pin-function names. Whatever
it's labeled, that pad is the SWIO/debug line and must stay connected to the programmer,
not repurposed as a general GPIO.

## Wiring (WCH-LinkE → board)

| LinkE pin | Board pad |
|---|---|
| 3V3 | VCC |
| GND | GND |
| SWIO | **D4** |

## Toolchain

- **IDE**: MounRiver Studio
- **Chip family in wizard**: CH32V00x → CH32V003J4M6
- **Library**: WCH EVT (Peripheral drivers) — uses `RCC_APB2PeriphClockCmd` /
  `RCC_APB2Periph_GPIOx` naming (same as CH32V003A4M6, different from the CH32V006's
  `RCC_PB2...` naming).

## Code (`User/main.c`)

```c
#include "debug.h"

#define LED_PORT   GPIOC
#define LED_PIN    GPIO_Pin_1

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

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

1. Open the project in MounRiver Studio (chip: CH32V00x → CH32V003J4M6).
2. **Build** (hammer icon) to compile.
3. **Download** (chip + down-arrow icon) to flash over SWIO via the WCH-LinkE (SWIO → D4 pad).
4. LED on PC1 should blink at 1Hz (500ms on / 500ms off).

## Serial output

Since every spare pin matters on this SOP8 package, prefer **SDI Print** over a real UART —
it reuses the existing SWIO debug wire instead of needing two more pins for TX/RX.

```c
#include "debug.h"

int main(void)
{
    SystemCoreClockUpdate();
    Delay_Init();
    SDI_Printf_Enable();   // printf over the SWIO debug line, no extra wiring

    printf("Hello from CH32V003J4M6!\r\n");

    while (1) { }
}
```

View it in MounRiver via **View → SDI Printf Viewer**, with the debugger attached.

## Troubleshooting notes

- **"Chip not found"** on connect → check SWIO wiring to the `D4` pad, and confirm the
  LinkE is in RISC-V mode (not ARM/SWD mode, if your probe has a mode switch/jumper).
- **Never reconfigure the `D4` pad as a GPIO or alternate function (e.g. UART)** — this
  package has no separate NRST pin broken out, so losing SWIO access can brick the chip
  with no way to force it back into programming mode.
- Silkscreen labels can be ambiguous on shared pads (`D4` here is really PD1/PD4/PD5) —
  when in doubt, treat any `D`-prefixed pad near GND/VCC as the debug line and avoid it.

## Next steps

- GPIO input (button press) on one of C2, A2, D6, or C4
- PWM / LED fade
- ADC read (potentiometer)
