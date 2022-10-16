
#ifndef _SPEED_DETECT_H_
#define _SPEED_DETECT_H_


#include "pico/stdio.h"
#include "pico/stdlib.h"
#include <stdbool.h>


#define MAF_LEN 8
#define MAF_CURSOR_MASK (MAF_LEN - 1)
#define MAF_DIV_SHIFT 3 // 2^(MAF_DIV_SHIFT) = (MAF_LEN)


typedef enum
{
    MES_STATE_STANDBY,
    MES_STATE_MEASURING,
    MES_STATE_ENDING,
}MeasureState_e;


typedef struct
{
    uint16_t th_high[2];
    uint16_t th_low[2];

    uint32_t timeout_threshold;

}SpeedDetect_Init_t;

typedef struct
{
    SpeedDetect_Init_t init;

    uint32_t maf_sum[2];
    uint16_t maf_buf[2][MAF_LEN];
    uint32_t maf_buf_cursor;

    bool bin_state[2];

    MeasureState_e mes_state;
    int mes_start_ch;
    uint32_t mes_period_counter;

}SpeedDetect_t;


void SpeedDetect_Init(SpeedDetect_t* h);


void SpeedDetect_UpdateResponse(SpeedDetect_t* h, uint16_t* buf, uint32_t length);





#endif /* _SPEED_DETECT_H_ */