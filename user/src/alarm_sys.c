#include "alarm_sys.h"

static alarm_sys_mode current_mode = IDEL;
static uint32_t last_blink_time = 0;
 void alarm_sys_func(alarm_sys_mode target_mode)
{
    if (current_mode != target_mode)
    {
        current_mode = target_mode;
        last_blink_time = HAL_GetTick();
    }

    switch (current_mode)
    {
        case IDEL:
            led_off();
            buzzer_off();
            break;

        case RUN:
            if (HAL_GetTick() - last_blink_time >= 500)
            {
                led_on();
                last_blink_time = HAL_GetTick();
            }

            buzzer_off();
            break;

        case ALARM:
            if (HAL_GetTick() - last_blink_time >= 50)
            {
                led_on();
                last_blink_time = HAL_GetTick();
            }

            buzzer_on();
            break;

        default:
            break;
    }
}