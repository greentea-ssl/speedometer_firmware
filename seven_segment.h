

#ifndef _SEVEN_SEGMENT_H_
#define _SEVEN_SEGMENT_H_

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include <stdbool.h>

#define MAX_DIGIT 8

typedef struct{
    uint pin_a;
    uint pin_b;
    uint pin_c;
    uint pin_d;
    uint pin_e;
    uint pin_f;
    uint pin_g;
    uint pin_dp;
    uint pin_com[MAX_DIGIT];
    uint com_count;
    uint dir_cathode;
    uint dir_anode;
}SevenSegment_Init_t;

typedef struct{
    SevenSegment_Init_t init;

    uint8_t digit_bits[MAX_DIGIT];

    int digit_state;

    
}SevenSegment_t;


void SevenSegment_Init(SevenSegment_t* h, SevenSegment_Init_t* init);


void SevenSegment_Update(SevenSegment_t* h);


void SevenSegment_SetIntDec(SevenSegment_t* h, int val);


void SevenSegment_SetFloatDec(SevenSegment_t* h, float val, int dp);


void SevenSegment_Clear(SevenSegment_t* h);


#endif /* _SEVEN_SEGMENT_H_ */
