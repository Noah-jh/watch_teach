# demos/demo03_adc_dma/main.c

#include "main.h"
#include "cmsis_os2.h"
#include <stdio.h>

osThreadId_t adcTaskHandle;
volatile uint16_t adc_buf[4];

void ADCTask(void *argument)
{
    for (;;) {
        osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);
        printf("ADC0=%d ADC1=%d ADC2=%d ADC3=%d\r\n",
               adc_buf[0], adc_buf[1], adc_buf[2], adc_buf[3]);
    }
}

void MX_FREERTOS_Init(void)
{
    const osThreadAttr_t adc_attr = {
        .name = "ADC_Task",
        .priority = (osPriority_t)osPriorityNormal,
        .stack_size = 256 * 4
    };
    adcTaskHandle = osThreadNew(ADCTask, NULL, &adc_attr);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1) {
        osThreadFlagsSet(adcTaskHandle, 0x01);
    }
}

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_ADC1_Init();
    MX_DMA_Init();
    MX_FREERTOS_Init();

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buf, 4);
    osKernelStart();

    while (1) {
    }
}

