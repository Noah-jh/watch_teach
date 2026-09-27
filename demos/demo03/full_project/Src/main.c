/* Demo03 full_project main.c - ADC + DMA for MQ-2, joystick, battery
 * Requires CubeMX-generated ADC1 with channels PA0..PA3 and DMA circular
 */

#include "main.h"
#include "cmsis_os2.h"
#include <stdio.h>

extern ADC_HandleTypeDef hadc1;
#define ADC_BUF_LEN 4
uint16_t adc_buf[ADC_BUF_LEN];

osThreadId_t adcTaskHandle;

void ADCTask(void *arg)
{
    // start DMA
    if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buf, ADC_BUF_LEN) != HAL_OK) {
        printf("ADC start DMA failed\r\n");
    }
    for(;;) {
        // print values periodically
        printf("ADC: %u %u %u %u\r\n", adc_buf[0], adc_buf[1], adc_buf[2], adc_buf[3]);
        osDelay(1000);
    }
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_USART1_UART_Init();

    osKernelInitialize();
    const osThreadAttr_t a_attr = { .name = "adc", .priority = osPriorityNormal, .stack_size = 384*4 };
    adcTaskHandle = osThreadNew(ADCTask, NULL, &a_attr);
    osKernelStart();
    while (1) {}
}
