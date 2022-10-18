

#ifndef _BUZZER_H_
#define _BUZZER_H_


#include "pico/stdio.h"
#include "pico/stdlib.h"


typedef struct
{
    uint gpio;
    uint slice_num;
    uint channel;

}Buzzer_Init_t;

typedef struct
{
    Buzzer_Init_t init;

    uint16_t div_x16;
    uint16_t top;
    uint16_t level;

}Buzzer_t;

void Buzzer_Init(Buzzer_t* h, uint gpio);

void Buzzer_NoteOn(Buzzer_t* h, uint8_t note);

void Buzzer_NoteOff(Buzzer_t* h);

void Buzzer_SingleTone(Buzzer_t* h, uint8_t note, uint32_t period_ms);

void Buzzer_Sound_StartUp(Buzzer_t* h);

void Buzzer_Sound_ShutDown(Buzzer_t* h);



#endif /* _BUZZER_H_ */


