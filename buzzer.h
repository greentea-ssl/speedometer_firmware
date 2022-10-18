

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

#define NOTESBUF_LEN 64
#define NOTESBUF_MASK (NOTESBUF_LEN-1)
typedef struct
{
    uint8_t note[NOTESBUF_LEN];
    uint32_t period_us[NOTESBUF_LEN];
    uint32_t head;
    uint32_t tail;
    uint32_t count;
    uint32_t max;
}Buzzer_NotesQueue_t;


typedef struct
{
    Buzzer_Init_t init;

    uint16_t div_x16;
    uint16_t top;
    uint16_t level;
    bool playing;

    uint8_t current_note;
    uint32_t current_period_us;
    uint32_t period_timer_us;

    Buzzer_NotesQueue_t queue;

}Buzzer_t;

void Buzzer_Init(Buzzer_t* h, uint gpio);

void Buzzer_NoteOn(Buzzer_t* h, uint8_t note);

void Buzzer_NoteOff(Buzzer_t* h);

void Buzzer_Update(Buzzer_t* h, uint32_t cycleTime_us);

void Buzzer_SetNote(Buzzer_t* h, uint8_t note, uint32_t period_ms);

void Buzzer_SetNote(Buzzer_t* h, uint8_t note, uint32_t period_ms);

void Buzzer_SetSound_StartUp(Buzzer_t* h);

void Buzzer_SetSound_ShutDown(Buzzer_t* h);



#endif /* _BUZZER_H_ */


