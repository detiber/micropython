// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2021 Scott Shawcroft for Adafruit Industries
//
// SPDX-License-Identifier: MIT

// #include "common-hal/microcontroller/Pin.h"
#include "supervisor/board.h"
#include "hardware/gpio.h"
#include "shared-bindings/board/__init__.h"
// #include "supervisor/shared/board.h"
#include "shared-bindings/busio/I2C.h"

// Use the MP_WEAK supervisor/shared/board.c versions of routines not defined here.

#define CROSSBAR_RESET_PIN_NUMBER 16
#define MCP4728_I2CADDR 0x60
#define LDAC_PIN_NUMBER 8

bool init_dac(void);

bool board_reset_pin_number(uint8_t pin_number) {
    // Take the Crossbars out of reset.
    if (pin_number == CROSSBAR_RESET_PIN_NUMBER) {
        gpio_put(pin_number, 0);
        gpio_set_dir(pin_number, GPIO_OUT);
        gpio_set_function(pin_number, GPIO_FUNC_SIO);

        return true;
    }
    return false;
}

void board_init(void) {
    // Put the crossbars into reset
    gpio_put(CROSSBAR_RESET_PIN_NUMBER, 1);
    gpio_set_dir(CROSSBAR_RESET_PIN_NUMBER, GPIO_OUT);
    gpio_set_function(CROSSBAR_RESET_PIN_NUMBER, GPIO_FUNC_SIO);

    init_dac();

    // mp_hal_delay_us(1);
    // Release the crossbars from reset
    board_reset_pin_number(CROSSBAR_RESET_PIN_NUMBER);
}

bool init_dac(void) {
    busio_i2c_obj_t *i2c = common_hal_board_create_i2c(0);
    if (!i2c) {
        return false;
    }

    if (!common_hal_busio_i2c_try_lock(i2c)) {
        return false;
    }

    bool probed = common_hal_busio_i2c_probe(i2c, MCP4728_I2CADDR);
    if (probed) {
        gpio_put(LDAC_PIN_NUMBER, 1);
        gpio_set_dir(LDAC_PIN_NUMBER, GPIO_OUT);
        gpio_set_function(LDAC_PIN_NUMBER, GPIO_FUNC_SIO);

        uint16_t channel_a_value = 1650;
        uint16_t channel_b_value = 1650;
        uint16_t channel_c_value = 1660;
        uint16_t channel_d_value = 1641;

        uint8_t output_buffer[8];
        output_buffer[0] = channel_a_value >> 8;
        output_buffer[1] = channel_a_value & 0xFF;

        output_buffer[2] = channel_b_value >> 8;
        output_buffer[3] = channel_b_value & 0xFF;

        output_buffer[4] = channel_c_value >> 8;
        output_buffer[5] = channel_c_value & 0xFF;

        output_buffer[6] = channel_d_value >> 8;
        output_buffer[7] = channel_d_value & 0xFF;

        uint8_t err = common_hal_busio_i2c_write(i2c, MCP4728_I2CADDR, output_buffer, sizeof(output_buffer));
        if (err) {
            return false;
        }

        gpio_put(LDAC_PIN_NUMBER, 0);
        gpio_set_dir(LDAC_PIN_NUMBER, GPIO_OUT);
        gpio_set_function(LDAC_PIN_NUMBER, GPIO_FUNC_SIO);
    }

    common_hal_busio_i2c_unlock(i2c);

    return true;
}
