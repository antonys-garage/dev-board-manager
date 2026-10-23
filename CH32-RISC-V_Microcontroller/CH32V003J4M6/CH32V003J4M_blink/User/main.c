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
        Delay_Ms(20);
        GPIO_WriteBit(LED_PORT, LED_PIN, Bit_RESET);
        Delay_Ms(1500);
    }
}