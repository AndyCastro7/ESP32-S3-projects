#include "ssd1780.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


static ssd1780_t my_ssd1780;
static i2c_master_bus_handle_t bus_handle;
static i2c_master_dev_handle_t dev_handle;

static void ssd1780_i2c_init();

void ssd1780_init(i2c_port_t i2c_port, gpio_num_t sda, gpio_num_t scl, uint32_t velocity_i2c, uint8_t direction)
{
    my_ssd1780.i2c_port = i2c_port;
    my_ssd1780.sda = sda;
    my_ssd1780.scl = scl;
    my_ssd1780.vel = velocity_i2c;
    my_ssd1780.dir = direction;

    ssd1780_i2c_init();

    vTaskDelay(pdMS_TO_TICKS(10));

    // set mux ratio
    ssd1780_command(0xA8); // command
    ssd1780_command(0x3F); // 64 pixeles

    // set display offset
    ssd1780_command(0xD3); // command
    ssd1780_command(0x00); // 0

    // set display start line
    ssd1780_command(0x40); // 0

    // set segment re-map 
    ssd1780_command(0xA0); // column address 0 is mapped to seg 0

    // set com output scan direction
    ssd1780_command(0xC0); // normal mode com0 to com N-1

    // set com pins hardware configuration
    ssd1780_command(0xDA); // good
    ssd1780_command(0x02);

    // set contrast control
    ssd1780_command(0x81);
    ssd1780_command(0x7F);

    // disable entire display on
    ssd1780_command(0xA4); 

    // set normal display
    ssd1780_command(0xA6);

    // set osc frequency
    ssd1780_command(0xD5);
    ssd1780_command(0x80);

    // set memory addressing mode
    ssd1780_command(0x20);
    ssd1780_command(0x02); 

    // enable charge pump regulator
    ssd1780_command(0x8D);
    ssd1780_command(0x14);

    // display on
    ssd1780_command(0xAF);

    clear_display();

    

}

void ssd1780_command(uint8_t command)
{
    const uint8_t command_control_byte = 0x80;
    uint8_t buffer[] = {command_control_byte, command};
    i2c_master_transmit(dev_handle, buffer, sizeof(buffer), -1);
}

void ssd1780_data(uint8_t data){
    const uint8_t data_control_byte = 0x40;
    uint8_t buffer[] = {data_control_byte, data};
    i2c_master_transmit(dev_handle, buffer, sizeof(buffer), -1);
}

void setCursor(uint8_t x, uint8_t y){
    ssd1780_command(0x00 + (x & 0x0F));
    ssd1780_command(0x10 + ((x >> 4) & 0x0F));
    ssd1780_command(0xB0 + y);
}

void clear_display(){
    for(uint8_t page = 0 ; page < 8 ; page++){
        setCursor(0, page);
        for(uint8_t column = 0 ; column < 128 ; column++){
            ssd1780_data(0x00);
        }
    }
    setCursor(0,0);
}


static void ssd1780_i2c_init(){
    i2c_master_bus_config_t i2c_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = my_ssd1780.i2c_port,
        .scl_io_num = my_ssd1780.scl,
        .sda_io_num = my_ssd1780.sda,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true
    };

    i2c_new_master_bus(&i2c_mst_config, &bus_handle);

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = my_ssd1780.dir,
        .scl_speed_hz = my_ssd1780.vel
    };

    i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle);   
}