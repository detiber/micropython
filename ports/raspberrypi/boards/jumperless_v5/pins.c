// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2024 Scott Shawcroft for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#include "shared-bindings/board/__init__.h"

static const mp_rom_map_elem_t board_module_globals_table[] = {
    CIRCUITPYTHON_BOARD_DICT_STANDARD_ITEMS

    // Peripheral reset
    { MP_ROM_QSTR(MP_QSTR_PERIPH_RESET), MP_ROM_PTR(&pin_GPIO16) },

    // Neopixels
    { MP_OBJ_NEW_QSTR(MP_QSTR_NEOPIXEL), MP_ROM_PTR(&pin_GPIO17) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_NEOPIXEL0), MP_ROM_PTR(&pin_GPIO3) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_NEOPIXEL1), MP_ROM_PTR(&pin_GPIO2) },

    // Primary uart
    { MP_ROM_QSTR(MP_QSTR_TX), MP_ROM_PTR(&pin_GPIO0) },
    { MP_ROM_QSTR(MP_QSTR_RX), MP_ROM_PTR(&pin_GPIO1) },

    // Analog pins
    { MP_OBJ_NEW_QSTR(MP_QSTR_A0), MP_ROM_PTR(&pin_GPIO40) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_A1), MP_ROM_PTR(&pin_GPIO41) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_A2), MP_ROM_PTR(&pin_GPIO42) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_A3), MP_ROM_PTR(&pin_GPIO43) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_A4), MP_ROM_PTR(&pin_GPIO44) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_A5), MP_ROM_PTR(&pin_GPIO45) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_A6), MP_ROM_PTR(&pin_GPIO46) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_A7), MP_ROM_PTR(&pin_GPIO47) },

    // GP Pins
    { MP_ROM_QSTR(MP_QSTR_GP6), MP_ROM_PTR(&pin_GPIO6) },
    { MP_ROM_QSTR(MP_QSTR_GP7), MP_ROM_PTR(&pin_GPIO7) },
    { MP_ROM_QSTR(MP_QSTR_GP20), MP_ROM_PTR(&pin_GPIO20) },
    { MP_ROM_QSTR(MP_QSTR_GP21), MP_ROM_PTR(&pin_GPIO21) },
    { MP_ROM_QSTR(MP_QSTR_GP22), MP_ROM_PTR(&pin_GPIO22) },
    { MP_ROM_QSTR(MP_QSTR_GP23), MP_ROM_PTR(&pin_GPIO23) },
    { MP_ROM_QSTR(MP_QSTR_GP24), MP_ROM_PTR(&pin_GPIO24) },
    { MP_ROM_QSTR(MP_QSTR_GP25), MP_ROM_PTR(&pin_GPIO25) },
    { MP_ROM_QSTR(MP_QSTR_GP26), MP_ROM_PTR(&pin_GPIO26) },
    { MP_ROM_QSTR(MP_QSTR_GP27), MP_ROM_PTR(&pin_GPIO27) },

    // primary i2c
    { MP_OBJ_NEW_QSTR(MP_QSTR_SDA), MP_ROM_PTR(&pin_GPIO4) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SCL), MP_ROM_PTR(&pin_GPIO5) },

    // primary spi
    { MP_ROM_QSTR(MP_QSTR_MISO), MP_ROM_PTR(&pin_GPIO20) },
    { MP_ROM_QSTR(MP_QSTR_CS), MP_ROM_PTR(&pin_GPIO21) },
    { MP_ROM_QSTR(MP_QSTR_SCK), MP_ROM_PTR(&pin_GPIO22) },
    { MP_ROM_QSTR(MP_QSTR_MOSI), MP_ROM_PTR(&pin_GPIO23) },

    // secondary i2c (shared with primary spi)
    { MP_OBJ_NEW_QSTR(MP_QSTR_SDA1), MP_ROM_PTR(&pin_GPIO22) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_SCL1), MP_ROM_PTR(&pin_GPIO23) },

    // Secondary uart (shared with secondary spi)
    { MP_ROM_QSTR(MP_QSTR_TX1), MP_ROM_PTR(&pin_GPIO24) },
    { MP_ROM_QSTR(MP_QSTR_RX1), MP_ROM_PTR(&pin_GPIO25) },

    // secondary spi
    { MP_ROM_QSTR(MP_QSTR_MISO1), MP_ROM_PTR(&pin_GPIO24) },
    { MP_ROM_QSTR(MP_QSTR_CS1), MP_ROM_PTR(&pin_GPIO25) },
    { MP_ROM_QSTR(MP_QSTR_SCK1), MP_ROM_PTR(&pin_GPIO26) },
    { MP_ROM_QSTR(MP_QSTR_MOSI1), MP_ROM_PTR(&pin_GPIO27) },

    { MP_ROM_QSTR(MP_QSTR_I2C), MP_ROM_PTR(&board_i2c_obj) },
    { MP_ROM_QSTR(MP_QSTR_SPI), MP_ROM_PTR(&board_spi_obj) },
    { MP_ROM_QSTR(MP_QSTR_UART), MP_ROM_PTR(&board_uart_obj) },
};
MP_DEFINE_CONST_DICT(board_module_globals, board_module_globals_table);
