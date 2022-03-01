#include <stdio.h>
#include "cmdConsole.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pid.hpp"

extern "C" void app_main(void)
{
    #ifdef CONFIG_CONSOLE_ENABLE
    xTaskCreate((TaskFunction_t)&taskConsole, "Console", 8192, NULL, 3, NULL);
    #endif
}
