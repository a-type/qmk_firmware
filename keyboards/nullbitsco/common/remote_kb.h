/* Copyright 2021 Jay Greco
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "quantum.h"

#define SERIAL_UART_BAUD 153600 //low error rate for 32u4 @ 16MHz

#define UART_PREAMBLE 0x69
#define UART_MSG_LEN  7
#define UART_NULL     0

#define IDX_MSG_FLAG 1

// key message indexes
#define IDX_PREAMBLE  0
#define IDX_KCLSB     2
#define IDX_KCMSB     3
#define IDX_PRESSED   4
#define IDX_EMPTY     5
#define IDX_CHECKSUM  6

#define KC_FLAG     1

// led message indexes
#define IDX_RGB 2
#define IDX_CHECKSUM 6

#define RGB_FLAG 2

#define IS_HID_KC(x) ((x > 0) && (x < 0xFF))
#define IS_RM_KC(x) ((x >= RM_BASE) && (x <= 0xFFFF))

#define RM_BASE 0xFFFF-16
enum remote_macros {
  RM_1 = RM_BASE,
  RM_2,  RM_3,
  RM_4,  RM_5,
  RM_6,  RM_7,
  RM_8,  RM_9,
  RM_10, RM_11,
  RM_12, RM_13,
  RM_14, RM_15,
};


// Public functions
void
 matrix_init_remote_kb(void),
 process_record_remote_kb(uint16_t keycode, keyrecord_t *record),
 matrix_scan_remote_kb(void),
 tap_remote_key(uint16_t keycode),
 sync_leds(void);
