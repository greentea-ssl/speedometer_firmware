

#include "buzzer.h"

#include <math.h>
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

const uint16_t table_div_x16[128] = {3903, 3574, 3335, 3455, 3529, 2926, 2792, 3484, 2409, 2424, 2159, 2396, 2222, 2507, 2862, 2019, 2615, 2070, 1396, 1742, 1787, 1212, 1387, 1198, 1111, 1787, 1431, 1287, 2615, 1035, 698, 871, 1283, 606, 1387, 599, 1807, 618, 1019, 1289, 469, 1463, 349, 360, 1283, 303, 403, 599, 236, 309, 236, 1289, 469, 265, 443, 180, 190, 281, 403, 158, 118, 118, 118, 102, 421, 172, 443, 90, 95, 281, 403, 79, 59, 59, 59, 51, 421, 86, 44, 45, 95, 281, 36, 55, 59, 59, 47, 26, 32, 43, 22, 31, 24, 281, 18, 16, 59, 121, 15, 13, 16, 43, 11, 10, 12, 281, 9, 8, 59, 7, 7, 29, 8, 43, 9, 5, 6, 5, 9, 4, 5, 7, 7, 11, 4, 43, 13, 4, };
const uint16_t table_top[128] = {62676, 64604, 65348, 59538, 55018, 62632, 61954, 46862, 63970, 60006, 63590, 54084, 55046, 46050, 38074, 50942, 37124, 44266, 61954, 46862, 43118, 60006, 49492, 54084, 55046, 32302, 38074, 39958, 18562, 44266, 61954, 46862, 30028, 60006, 24746, 54084, 16922, 46702, 26734, 19948, 51748, 15658, 61954, 56690, 15014, 60006, 42584, 27042, 64784, 46702, 57716, 9974, 25874, 43222, 24404, 56690, 50692, 32352, 21292, 51260, 64784, 61148, 57716, 63022, 14412, 33296, 12202, 56690, 50692, 16176, 10646, 51260, 64784, 61148, 57716, 63022, 7206, 33296, 61426, 56690, 25346, 8088, 59588, 36814, 32392, 30574, 36226, 61810, 47402, 33296, 61426, 41146, 50164, 4044, 59588, 63274, 16196, 7454, 56754, 61810, 47402, 16648, 61426, 63776, 50164, 2022, 59588, 63274, 8098, 64424, 60808, 13854, 47402, 8324, 37538, 63776, 50164, 56818, 29794, 63274, 47778, 32212, 30404, 18262, 47402, 4162, 12994, 39860, };


void Buzzer_CalcTable(Buzzer_t* h)
{
    Buzzer_Init_t* init = &h->init;

    float f_clk = 125E+6;

    // for(uint16_t note = 0; note < 128; note++)
    // {
    //     uint16_t opt_div_x16;
    //     uint16_t opt_top;
    //     float opt_freq_err = 10000;
    //     float f_cmd = 440.0f * powf(2.0, (note-69) / 12.0f);

    //     for(uint16_t div_x16 = 1; div_x16 < 4096; div_x16++)
    //     {
    //         uint32_t top = (uint32_t)(f_clk / f_cmd * 16 / div_x16 / 2 + 0.5) * 2;
    //         if(top <= 65535)
    //         {
    //             float f_repro = f_clk / (top * div_x16 / 16.0f);
    //             float f_err = fabs(f_repro - f_cmd);
    //             if(f_err < opt_freq_err)
    //             {
    //                 opt_div_x16 = div_x16;
    //                 opt_top = top;
    //                 opt_freq_err = f_err;
    //             }
    //         }
    //     }
    //     init->table_div_x16[note] = opt_div_x16;
    //     init->table_top[note] = opt_top;
    //     printf("%f\n", opt_freq_err);
    // }


    // for(uint16_t note = 0; note < 128; note++)
    // {
    //     float f_cmd = 440.0f * powf(2.0, (note-69) / 12.0f);
    //     float f_repro = f_clk / (table_top[note] * table_div_x16[note] / 16.0f);
    //     float f_err = fabs(f_repro - f_cmd);
    //     printf("[%d]: %f\n", note, f_err);
    // }


}


void Buzzer_Init(Buzzer_t* h, uint gpio)
{
    Buzzer_Init_t* init = &h->init;

    init->gpio = gpio;

    gpio_set_function(gpio, GPIO_FUNC_PWM);

    Buzzer_CalcTable(h);

    init->slice_num = pwm_gpio_to_slice_num(gpio);
    init->channel = pwm_gpio_to_channel(gpio);
    pwm_set_clkdiv(init->slice_num, 255.00f);
    pwm_set_wrap(init->slice_num, 1000);
    pwm_set_chan_level(init->slice_num, init->channel, 0);
    pwm_set_enabled(init->slice_num, true);

    Buzzer_Sound_StartUp(h);

}


void Buzzer_NoteOn(Buzzer_t* h, uint8_t note)
{
    uint8_t div_i = table_div_x16[note] >> 4;
    uint8_t div_f = table_div_x16[note] & 0x0F;
    pwm_set_clkdiv_int_frac(h->init.slice_num, div_i, div_f);
    pwm_set_wrap(h->init.slice_num, table_top[note]);
    pwm_set_chan_level(h->init.slice_num, h->init.channel, table_top[note] >> 1);
    pwm_set_enabled(h->init.slice_num, true);
}

void Buzzer_NoteOff(Buzzer_t* h)
{
    pwm_set_enabled(h->init.slice_num, false);
}


void Buzzer_SingleTone(Buzzer_t* h, uint8_t note, uint32_t period_ms)
{
    Buzzer_NoteOn(h, note);
    sleep_ms(period_ms);
    Buzzer_NoteOff(h);
}

void Buzzer_Sound_StartUp(Buzzer_t* h)
{
    Buzzer_SingleTone(h, 72 + 12, 100);
    Buzzer_SingleTone(h, 74 + 12, 100);
    Buzzer_SingleTone(h, 76 + 12, 100);
}


void Buzzer_Sound_ShutDown(Buzzer_t* h)
{
    Buzzer_SingleTone(h, 76 + 12, 100);
    Buzzer_SingleTone(h, 74 + 12, 100);
    Buzzer_SingleTone(h, 72 + 12, 100);
}

