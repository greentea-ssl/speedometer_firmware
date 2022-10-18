
#include "speed_detect.h"



void SpeedDetect_Init(SpeedDetect_t* h)
{
    h->init.th_high[0] = 150;
    h->init.th_high[1] = 150;
    h->init.th_low[0] = 100;
    h->init.th_low[1] = 100;

    h->init.timeout_threshold = 112500; // 0.09[m] / lowspeed[m/s] * 250E+6[Hz]

    uint16_t init_val = 300;

    for(int ch = 0; ch < 2; ch++)
    {
        h->maf_sum[ch] = init_val*MAF_LEN;
        for(int i = 0; i < MAF_LEN; i++)
        {
            h->maf_buf[ch][i] = init_val;
        }
    }
    h->maf_buf_cursor = 0;

    h->bin_state[0] = false;
    h->bin_state[1] = false;

    h->mes_state = MES_STATE_STANDBY;
    
}


void SpeedDetect_UpdateResponse(SpeedDetect_t* h, uint16_t* buf, uint32_t length)
{
    uint32_t ch_len = length / 2;
    uint16_t u[2];
    uint16_t y[2];
    SpeedDetect_Init_t* init = &h->init;

    for(int i = 0; i < ch_len; i++)
    {   
        for(int ch = 0; ch < 2; ch++)
        {
            u[ch] = buf[i*2 + ch];
            h->maf_sum[ch] -= h->maf_buf[ch][h->maf_buf_cursor];
            h->maf_sum[ch] += u[ch];
            h->maf_buf[ch][h->maf_buf_cursor] = u[ch];
            y[ch] = h->maf_sum[ch] >> MAF_DIV_SHIFT;
            
            #if 0 // 物理Highレベルで検出か?
            if(h->bin_state[ch] && y[ch] < init->th_low)
            {
                h->bin_state[ch] = false;
            }
            else if(y[ch] > init->th_high)
            {
                h->bin_state[ch] = true;
            }
            #else
            if(!h->bin_state[ch] && y[ch] < init->th_low[ch])
            {
                h->bin_state[ch] = true;
            }
            else if(y[ch] > init->th_high[ch])
            {
                h->bin_state[ch] = false;
            }
            #endif
        }
        h->maf_buf_cursor = ++h->maf_buf_cursor & MAF_CURSOR_MASK;

        switch(h->mes_state)
        {
            case MES_STATE_STANDBY:
            if(h->bin_state[0])
            {
                h->mes_start_ch = 0;
                h->mes_period_counter = 0;
                h->mes_state = MES_STATE_MEASURING;
            }
            else if(h->bin_state[1])
            {
                h->mes_start_ch = 1;
                h->mes_period_counter = 0;
                h->mes_state = MES_STATE_MEASURING;
            }
            break;

            case MES_STATE_MEASURING:
            h->mes_period_counter++;
            if(h->mes_period_counter >= init->timeout_threshold)
            {
                h->mes_period_counter = 0;
                h->mes_state = MES_STATE_ENDING;
                break;
            }
            if(h->mes_start_ch == 0 && h->bin_state[1])
            {
                h->mes_state = MES_STATE_ENDING;
            }
            else if(h->mes_start_ch == 1 && h->bin_state[0])
            {
                h->mes_state = MES_STATE_ENDING;
            }
            break;

            case MES_STATE_ENDING:
            if(!h->bin_state[0] && !h->bin_state[1])
            {
                h->mes_state = MES_STATE_STANDBY;
            }
            break;
        }

    }

}