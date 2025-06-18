// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2021 Scott Shawcroft for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#include "supervisor/board.h"
#include "hardware/gpio.h"
#include "shared-bindings/busio/SPI.h"
#include "shared-bindings/busio/I2C.h"
#include "mpconfigboard.h"
#include "py/mphal.h"
#include "shared-bindings/board/__init__.h"

// Use the MP_WEAK supervisor/shared/board.c versions of routines not defined here.

#define CROSSBAR_RESET_PIN_NUMBER 24
#define REV2_DAC0_MCP4725_I2CADDR 0x62
#define REV2_DAC1_MCP4725_I2CADDR 0x63
#define MCP4822_CS_PIN_NUMBER 1

digitalio_digitalinout_obj_t rev3_dac_cs_pin_obj;

bool init_dac_rev2(void);
bool init_dac_rev3(void);

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

    init_dac_rev2();
    init_dac_rev3();

    // mp_hal_delay_us(1);
    // Release the crossbars from reset
    board_reset_pin_number(CROSSBAR_RESET_PIN_NUMBER);
}

bool init_dac_rev2(void) {
    uint8_t err;
    uint8_t output_buffer[3];
    uint8_t input_buffer[1];

    busio_i2c_obj_t *i2c = common_hal_board_create_i2c(0);
    if (!i2c) {
        return false;
    }

    if (!common_hal_busio_i2c_try_lock(i2c)) {
        return false;
    }

    bool dac1_probed = common_hal_busio_i2c_probe(i2c, REV2_DAC1_MCP4725_I2CADDR);
    if (dac1_probed) {
        // dac1_8V.setInputCode(offset[1] + calib[1], MCP4725_EEPROM_Mode,
        //                  MCP4725_PowerDown_Off);
        // eeprom mode = 0x60
        // power down off = 0x00
        output_buffer[0] = 0x60;

        // input code is offset (1932) + calib (150) = 2082 = 0x822
        // values taken from jumperless v1 upstream firmware
        // input code is a 12 bit value split over two byte segments
        // input code high bytes
        output_buffer[1] = 0x82;
        // input code low byte, least significant 4 bits are unused.
        output_buffer[2] = 0x20;

        err = common_hal_busio_i2c_write(i2c, REV2_DAC0_MCP4725_I2CADDR, output_buffer, sizeof(output_buffer));
        if (err) {
            return false;
        }

        // Poll status register until EEPROM write is complete
        for (int i = 0; i < 2; ++i) {
            err = common_hal_busio_i2c_read(i2c, REV2_DAC0_MCP4725_I2CADDR, input_buffer, 1);
            if (err) {
                mp_hal_delay_ms(100);
                continue;
            }

            if (input_buffer[0] & 0x80) {
                break;
            }

            mp_hal_delay_ms(100);
        }
    }

    bool dac0_probed = common_hal_busio_i2c_probe(i2c, REV2_DAC0_MCP4725_I2CADDR);
    if (dac0_probed) {
        // dac0_5V.setVoltage(0.00, MCP4725_EEPROM_Mode, MCP4725_PowerDown_Off);
        // eeprom mode = 0x60
        // power down off = 0x00
        output_buffer[0] = 0x60;
        // input code for voltage of 0.00 is 0x0000
        output_buffer[1] = 0x00;
        output_buffer[2] = 0x00;

        err = common_hal_busio_i2c_write(i2c, REV2_DAC0_MCP4725_I2CADDR, output_buffer, sizeof(output_buffer));
        if (err) {
            return false;
        }

        // Poll status register until EEPROM write is complete
        for (int i = 0; i < 2; ++i) {
            err = common_hal_busio_i2c_read(i2c, REV2_DAC0_MCP4725_I2CADDR, input_buffer, 1);
            if (err) {
                mp_hal_delay_ms(100);
                continue;
            }

            if (input_buffer[0] & 0x80) {
                break;
            }

            mp_hal_delay_ms(100);
        }
    }

    common_hal_busio_i2c_unlock(i2c);

    return true;
}


bool init_dac_rev3(void) {
    uint8_t output_buffer[2];

    gpio_put(MCP4822_CS_PIN_NUMBER, 1);
    gpio_set_dir(MCP4822_CS_PIN_NUMBER, GPIO_OUT);
    gpio_set_function(MCP4822_CS_PIN_NUMBER, GPIO_FUNC_SIO);

    busio_spi_obj_t *spi = common_hal_board_create_spi(0);
    if (!spi) {
        return false;
    }

    if (!common_hal_busio_spi_try_lock(spi)) {
        return false;
    }

    if (!common_hal_busio_spi_configure(spi, 16000000, 0, 0, 8)) {
        return false;
    }

    // setDac0_5Vvoltage(0.00);
    // voltageCode = voltage * 4095 / 5 = 0
    // transfer(0x3000 | value);
    gpio_put(MCP4822_CS_PIN_NUMBER, 0);

    // write to DAC A, 1x Gain, active mode operation and 4 most significant bits of voltage code
    output_buffer[0] = 0x30;
    // 8 least significant bits of voltage code
    output_buffer[1] = 0x00;
    common_hal_busio_spi_write(spi, output_buffer, sizeof(output_buffer));

    gpio_put(MCP4822_CS_PIN_NUMBER, 1);

    // setDac1_8VinputCode(4095);
    // transfer(0xB000 | value);
    gpio_put(MCP4822_CS_PIN_NUMBER, 0);

    // write to DAC B, 1x Gain, active mode operation and 4 most significant bits of voltage code
    output_buffer[0] = 0xBF;
    // 8 least significant bits of voltage code
    output_buffer[1] = 0xFF;
    common_hal_busio_spi_write(spi, output_buffer, sizeof(output_buffer));

    gpio_put(MCP4822_CS_PIN_NUMBER, 1);

    common_hal_busio_spi_unlock(spi);

    return true;
}
