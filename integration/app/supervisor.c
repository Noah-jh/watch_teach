/* integration/app/supervisor.c - supervisor task that manages watchdog and heartbeats
 * - Registers tasks' heartbeats
 * - Refreshes IWDG at regular intervals if system healthy
 */

#include "watchdog.h"
#include "cmsis_os2.h"
#include "stm32f4xx_hal.h"
#include <string.h>

static osThreadId_t supTaskHandle;
static volatile uint32_t last_heartbeat = 0;

void supervisor_task(void *arg)
{
    (void)arg;
    const uint32_t kick_interval_ms = 500;
    while (1) {
        if ((HAL_GetTick() - last_heartbeat) < 2000) {
            watchdog_kick();
        } else {
            // No heartbeat from system tasks - let watchdog expire or take recovery action
        }
        osDelay(kick_interval_ms);
    }
}

void supervisor_notify_heartbeat(void)
{
    last_heartbeat = HAL_GetTick();
}

void supervisor_init(void)
{
    const osThreadAttr_t attr = { .name = "supervisor", .stack_size = 256*2, .priority = osPriorityHigh };
    supTaskHandle = osThreadNew(supervisor_task, NULL, &attr);
}
