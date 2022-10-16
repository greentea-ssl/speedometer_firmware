

#include "seven_segment.h"

#include <math.h>
#include <string.h>

/*
0123
4567
89ab
cdef
*/
const int digit_table[16] = {
    0b00111111, 0b00000110, 0b01011011, 0b01001111, 
    0b01100110, 0b01101101, 0b01111101, 0b00100111,
    0b01111111, 0b01101111, 0b01110111, 0b01111100, 
    0b01011000, 0b01011110, 0b01111001, 0b01110001};


void SevenSegment_Init(SevenSegment_t* h, SevenSegment_Init_t* init)
{
    
    gpio_init(init->pin_a);
    gpio_init(init->pin_b);
    gpio_init(init->pin_c);
    gpio_init(init->pin_d);
    gpio_init(init->pin_e);
    gpio_init(init->pin_f);
    gpio_init(init->pin_g);
    gpio_init(init->pin_dp);
    gpio_init(init->pin_com[0]);
    gpio_init(init->pin_com[1]);
    gpio_init(init->pin_com[2]);
    
    gpio_set_dir(init->pin_a, GPIO_OUT);
    gpio_set_dir(init->pin_b, GPIO_OUT);
    gpio_set_dir(init->pin_c, GPIO_OUT);
    gpio_set_dir(init->pin_d, GPIO_OUT);
    gpio_set_dir(init->pin_e, GPIO_OUT);
    gpio_set_dir(init->pin_f, GPIO_OUT);
    gpio_set_dir(init->pin_g, GPIO_OUT);
    gpio_set_dir(init->pin_dp, GPIO_OUT);
    gpio_set_dir(init->pin_com[0], GPIO_OUT);
    gpio_set_dir(init->pin_com[1], GPIO_OUT);
    gpio_set_dir(init->pin_com[2], GPIO_OUT);

    bool init_c = !init->dir_cathode;
    bool init_a = !init->dir_anode;
    gpio_put(init->pin_a, init_c);
    gpio_put(init->pin_b, init_c);
    gpio_put(init->pin_c, init_c);
    gpio_put(init->pin_d, init_c);
    gpio_put(init->pin_e, init_c);
    gpio_put(init->pin_f, init_c);
    gpio_put(init->pin_g, init_c);
    gpio_put(init->pin_dp, init_c);
    gpio_put(init->pin_com[0], init_a);
    gpio_put(init->pin_com[1], init_a);
    gpio_put(init->pin_com[2], init_a);

    memcpy(&h->init, init, sizeof(SevenSegment_Init_t));

}


void SevenSegment_Update(SevenSegment_t* h)
{
    SevenSegment_Init_t* init = &h->init;

    uint8_t bin;
    if(h->digit_state < init->com_count)
        bin = h->digit_bits[h->digit_state];
    else
        bin = 0;

    if(!init->dir_cathode)
    {
        bin = ~bin;
    }

    gpio_put(init->pin_a, (bin & (1 << 0)) != 0);
    gpio_put(init->pin_b, (bin & (1 << 1)) != 0);
    gpio_put(init->pin_c, (bin & (1 << 2)) != 0);
    gpio_put(init->pin_d, (bin & (1 << 3)) != 0);
    gpio_put(init->pin_e, (bin & (1 << 4)) != 0);
    gpio_put(init->pin_f, (bin & (1 << 5)) != 0);
    gpio_put(init->pin_g, (bin & (1 << 6)) != 0);
    gpio_put(init->pin_dp, (bin & (1 << 7)) != 0);

    switch(h->digit_state)
    {
    case 0: 
        gpio_put(init->pin_com[0], 0);
        gpio_put(init->pin_com[1], 1);
        gpio_put(init->pin_com[2], 1);
        break;
    case 1:
        gpio_put(init->pin_com[0], 1);
        gpio_put(init->pin_com[1], 0);
        gpio_put(init->pin_com[2], 1);
        break;
    case 2:
        gpio_put(init->pin_com[0], 1);
        gpio_put(init->pin_com[1], 1);
        gpio_put(init->pin_com[2], 0);
        break;
    default: 
        gpio_put(init->pin_com[0], 1);
        gpio_put(init->pin_com[1], 1);
        gpio_put(init->pin_com[2], 1);
        break;
    }

    h->digit_state++;
    if(h->digit_state > 5) h->digit_state = 0;

}


void SevenSegment_SetIntDec(SevenSegment_t* h, int val)
{
    SevenSegment_Init_t* init = &h->init;


    for(int i = 0; i < init->com_count; i++)
    {
        int digit = val;
        val = val / 10;
        digit = digit - val * 10;
        h->digit_bits[i] = digit_table[digit];
    }
}


void SevenSegment_SetFloatDec(SevenSegment_t* h, float val, int dp)
{

    for(int i = 0; i < dp; i++)
    {
        val *= 10;
    }
    SevenSegment_SetIntDec(h, (int)(val + 0.5));

    if(dp >= 0 && dp < 3)
    {
        h->digit_bits[dp] |= 0b10000000;
    }

}


void SevenSegment_Clear(SevenSegment_t* h)
{
    SevenSegment_Init_t* init = &h->init;

    for(int i = 0; i < init->com_count; i++)
    {
        h->digit_bits[i] = 0x00;
    }

    SevenSegment_Update(h);

}