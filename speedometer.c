
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pico/stdio.h"
#include "pico/stdlib.h"

#define PIN_LED PICO_DEFAULT_LED_PIN

#define PIN_PONSIG (17)
#define PIN_PONSW (16)

#define PIN_DISP_A (7)
#define PIN_DISP_B (3)
#define PIN_DISP_C (10)
#define PIN_DISP_D (12)
#define PIN_DISP_E (13)
#define PIN_DISP_F (6)
#define PIN_DISP_G (9)
#define PIN_DISP_DP (11)
#define PIN_DISP_CA1 (4)
#define PIN_DISP_CA2 (5)
#define PIN_DISP_CA3 (8)



void main(void)
{
    gpio_init(PIN_LED);
    gpio_init(PIN_PONSIG);
    gpio_init(PIN_PONSW);

    gpio_init(PIN_DISP_A);
    gpio_init(PIN_DISP_B);
    gpio_init(PIN_DISP_C);
    gpio_init(PIN_DISP_D);
    gpio_init(PIN_DISP_E);
    gpio_init(PIN_DISP_F);
    gpio_init(PIN_DISP_G);
    gpio_init(PIN_DISP_DP);
    gpio_init(PIN_DISP_CA1);
    gpio_init(PIN_DISP_CA2);
    gpio_init(PIN_DISP_CA3);

    gpio_set_dir(PIN_LED, GPIO_OUT);
    gpio_set_dir(PIN_PONSIG, GPIO_OUT);
    gpio_set_dir(PIN_PONSW, GPIO_IN);
    
    gpio_set_dir(PIN_DISP_A, GPIO_OUT);
    gpio_set_dir(PIN_DISP_B, GPIO_OUT);
    gpio_set_dir(PIN_DISP_C, GPIO_OUT);
    gpio_set_dir(PIN_DISP_D, GPIO_OUT);
    gpio_set_dir(PIN_DISP_E, GPIO_OUT);
    gpio_set_dir(PIN_DISP_F, GPIO_OUT);
    gpio_set_dir(PIN_DISP_G, GPIO_OUT);
    gpio_set_dir(PIN_DISP_DP, GPIO_OUT);
    gpio_set_dir(PIN_DISP_CA1, GPIO_OUT);
    gpio_set_dir(PIN_DISP_CA2, GPIO_OUT);
    gpio_set_dir(PIN_DISP_CA3, GPIO_OUT);

    gpio_put(PIN_DISP_A, 1);
    gpio_put(PIN_DISP_B, 1);
    gpio_put(PIN_DISP_C, 0);
    gpio_put(PIN_DISP_D, 0);
    gpio_put(PIN_DISP_E, 0);
    gpio_put(PIN_DISP_F, 0);
    gpio_put(PIN_DISP_G, 0);
    gpio_put(PIN_DISP_DP, 0);
    gpio_put(PIN_DISP_CA1, 1);
    gpio_put(PIN_DISP_CA2, 1);
    gpio_put(PIN_DISP_CA3, 1);

    sleep_ms(500);

    gpio_put(PIN_PONSIG, 1);


    while (true) {
        // gpio_put(PIN_LED, 1);
        // sleep_ms(100);
        // if(gpio_get(PIN_PONSW) != 0)
        // {
        //     gpio_put(PIN_LED, 0);
        // }
        // sleep_ms(100);

        gpio_put(PIN_DISP_CA1, 0);
        gpio_put(PIN_DISP_CA2, 1);
        gpio_put(PIN_DISP_CA3, 1);
        sleep_ms(500);

        gpio_put(PIN_DISP_CA1, 1);
        gpio_put(PIN_DISP_CA2, 0);
        gpio_put(PIN_DISP_CA3, 1);
        sleep_ms(500);

        gpio_put(PIN_DISP_CA1, 1);
        gpio_put(PIN_DISP_CA2, 1);
        gpio_put(PIN_DISP_CA3, 0);
        sleep_ms(500);



    }

}