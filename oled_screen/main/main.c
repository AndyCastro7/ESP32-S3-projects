#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ssd1780.h"

void app_main(void)
{
    vTaskDelay(pdMS_TO_TICKS(100));
    ssd1780_init(I2C_NUM_0, GPIO_NUM_8, GPIO_NUM_9, 100000, 0x3C);
    for(uint16_t i = 0 ; i < 128 ; i++){
        ssd1780_data(0b10101010);
        ssd1780_data(0b01010101);
    }


    
    while(true){
        vTaskDelay(pdMS_TO_TICKS(100));
    }

}
