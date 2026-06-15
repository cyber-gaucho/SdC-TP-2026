#include "pico/stdlib.h"

#define CH1_PIN 16
#define CH2_PIN 17

int main()
{
    stdio_init_all();

    gpio_init(CH1_PIN);
    gpio_set_dir(CH1_PIN, GPIO_OUT);

    gpio_init(CH2_PIN);
    gpio_set_dir(CH2_PIN, GPIO_OUT);

    bool ch1 = false;
    bool ch2 = false;

    absolute_time_t next_ch1 = get_absolute_time();
    absolute_time_t next_ch2 = get_absolute_time();

    while (true)
    {
        if (absolute_time_diff_us(get_absolute_time(), next_ch1) <= 0)
        {
            ch1 = !ch1;
            gpio_put(CH1_PIN, ch1);

            next_ch1 =
                delayed_by_ms(next_ch1, 500);
        }

        if (absolute_time_diff_us(get_absolute_time(), next_ch2) <= 0)
        {
            ch2 = !ch2;
            gpio_put(CH2_PIN, ch2);

            next_ch2 =
                delayed_by_ms(next_ch2, 125);
        }
    }

    return 0;
}
