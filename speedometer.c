
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/adc.h"
#include "hardware/dma.h"

#include "seven_segment.h"

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

#define PIN_IR_LED0 (14)
#define PIN_IR_LED1 (15)
#define PIN_IR_PT0 (27)
#define PIN_IR_PT1 (26)


void gpio_setting();


uint repeat_alarm_num = 0;
absolute_time_t prev_target;
uint32_t period_us = 2000;

SevenSegment_t sevenSeg;


#define CAPTURE_DEPTH 16
uint8_t capture_buf_0[CAPTURE_DEPTH];
uint8_t capture_buf_1[CAPTURE_DEPTH];

uint dma_ch_0;
bool dma_interrupted = false;

void alarm_callback(uint alarm_num)
{
    prev_target += (uint64_t)period_us;
    hardware_alarm_set_target(repeat_alarm_num, prev_target);

    SevenSegment_Update(&sevenSeg);

}


void dma_handler()
{
    printf("Handler!\n");
    dma_interrupted = true;
    dma_channel_acknowledge_irq0(dma_ch_0);
}

void main(void)
{

    int i = 0;
    float j = 0.0;
    
    stdio_init_all();
    adc_init();
    gpio_setting();
    adc_gpio_init(PIN_IR_PT0);
    adc_gpio_init(PIN_IR_PT1);

    sleep_ms(2000);

    // Start

    gpio_put(PIN_PONSIG, 1);

    gpio_put(PIN_IR_LED0, 0);
    gpio_put(PIN_IR_LED1, 1);

    hardware_alarm_set_callback(repeat_alarm_num, alarm_callback);
    prev_target = time_us_64() + 100000; // 開始だけ遅らせる
    hardware_alarm_set_target(repeat_alarm_num, prev_target);

    adc_select_input(0);

    adc_fifo_setup(true, true, 1, false, true);

    // main clock: 48MHz
    adc_set_clkdiv(48000);

    dma_ch_0 = dma_claim_unused_channel(true);
    dma_channel_config cfg = dma_channel_get_default_config(dma_ch_0);
    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_8);
    channel_config_set_read_increment(&cfg, false);
    channel_config_set_write_increment(&cfg, true);
    // channel_config_set_irq_quiet(&cfg, true);
    channel_config_set_dreq(&cfg, DREQ_ADC);
    dma_channel_configure(dma_ch_0, &cfg, capture_buf_0, &adc_hw->fifo, CAPTURE_DEPTH, false);
    dma_channel_set_irq0_enabled(dma_ch_0, true);
    
    irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
    irq_set_enabled(DMA_IRQ_0, true);


    dma_channel_start(dma_ch_0);

    adc_run(true);
    
    printf("Start.\n");

    while (true) {
        // gpio_put(PIN_LED, 1);
        // sleep_ms(100);
        // if(gpio_get(PIN_PONSW) != 0)
        // {
        //     gpio_put(PIN_LED, 0);
        // }
        // sleep_ms(100);

        // //SevenSegment_SetIntDec(&sevenSeg, i++);
        // j = j + 0.01;

        // uint16_t result = adc_read();
        
        // SevenSegment_SetFloatDec(&sevenSeg, result/4096.0f, 3);



        sleep_ms(100);

        //for(int i = 0; i < CAPTURE_DEPTH; i++)
        //for(int i = 1024-32; i < 1024; i++)

        // for(int i = 0; i < 64; i++)
        // {
        //     if((i & 0x07) == 0)
        //         printf("|");
        //     else
        //         printf(" ");

        //     printf("%02x", capture_buf_0[i]);
        // }
        // printf("\n");

        printf("%d, %d\n", dma_interrupted, dma_channel_get_irq0_status(dma_ch_0));

    }

}



void gpio_setting()
{
    
    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);

    gpio_init(PIN_PONSIG);
    gpio_init(PIN_PONSW);
    gpio_set_dir(PIN_PONSIG, GPIO_OUT);
    gpio_set_dir(PIN_PONSW, GPIO_IN);

    gpio_init(PIN_IR_LED0);
    gpio_init(PIN_IR_LED1);
    gpio_set_dir(PIN_IR_LED0, GPIO_OUT);
    gpio_set_dir(PIN_IR_LED1, GPIO_OUT);
    gpio_put(PIN_IR_LED0, 0);
    gpio_put(PIN_IR_LED1, 0);

    
    SevenSegment_Init_t ssinit;
    ssinit.pin_a = PIN_DISP_A;
    ssinit.pin_b = PIN_DISP_B;
    ssinit.pin_c = PIN_DISP_C;
    ssinit.pin_d = PIN_DISP_D;
    ssinit.pin_e = PIN_DISP_E;
    ssinit.pin_f = PIN_DISP_F;
    ssinit.pin_g = PIN_DISP_G;
    ssinit.pin_dp = PIN_DISP_DP;
    ssinit.pin_com[0] = PIN_DISP_CA1;
    ssinit.pin_com[1] = PIN_DISP_CA2;
    ssinit.pin_com[2] = PIN_DISP_CA3;
    ssinit.dir_anode = 0;
    ssinit.dir_cathode = 0;
    ssinit.com_count = 3;
    SevenSegment_Init(&sevenSeg, &ssinit);

}
