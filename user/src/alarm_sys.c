#include "alarm_sys.h"

static alarm_sys_mode current_mode = IDEL;
static uint32_t last_led_toggle_time = 0;
static uint32_t last_buzzer_toggle_time = 0;

static void alarm_all_leds_off(void)
{
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN, GPIO_PIN_RESET);
}

static void alarm_buzzer_off(void)
{
    HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_PIN, GPIO_PIN_RESET);
}

static void alarm_toggle_leds_by_time(uint32_t interval_ms)
{
    uint32_t now = HAL_GetTick();

    if ((now - last_led_toggle_time) >= interval_ms)
    {
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN);
        last_led_toggle_time = now;
    }
}

static void alarm_toggle_buzzer_by_time(uint32_t interval_ms)
{
    uint32_t now = HAL_GetTick();

    if ((now - last_buzzer_toggle_time) >= interval_ms)
    {
        HAL_GPIO_TogglePin(BUZZER_GPIO_PORT, BUZZER_PIN);
        last_buzzer_toggle_time = now;
    }
}

void alarm_sys_func(alarm_sys_mode target_mode)
{
    if (target_mode != current_mode)
    {
        current_mode = target_mode;
        last_led_toggle_time = 0;
        last_buzzer_toggle_time = 0;
        alarm_all_leds_off();
        alarm_buzzer_off();
    }

    switch (current_mode)
    {
    case IDEL:
        alarm_all_leds_off();
        alarm_buzzer_off();
        break;

    case RUN:
        alarm_toggle_leds_by_time(250);
        alarm_buzzer_off();
        break;

    case ALARM:
        alarm_toggle_leds_by_time(50);
        alarm_toggle_buzzer_by_time(50);
        break;

    default:
        current_mode = IDEL;
        alarm_all_leds_off();
        alarm_buzzer_off();
        break;
    }
}
