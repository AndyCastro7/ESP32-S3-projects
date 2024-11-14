#ifndef SSD1780_H
#define SSD1780_H

#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "driver/i2c_master.h"

typedef struct{
    i2c_port_t i2c_port;
    gpio_num_t sda;
    gpio_num_t scl;
    uint32_t vel;
    uint8_t dir;
}ssd1780_t;

void ssd1780_init(i2c_port_t i2c_port, gpio_num_t sda, gpio_num_t scl, uint32_t velocity_i2c, uint8_t direction);

void ssd1780_command(uint8_t command);

void ssd1780_data(uint8_t data);

void setCursor(uint8_t x, uint8_t y);

void clear_display();

#endif