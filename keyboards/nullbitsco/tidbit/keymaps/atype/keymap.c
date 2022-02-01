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
#define KEYBOARD_REMOTE true

#include QMK_KEYBOARD_H

#define _BASE     0
#define _VIA1     1
#define _VIA2     2
#define _VIA3     3

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT(
           KC_F13,KC_F14, KC_F15,
  KC_MUTE, KC_P7, KC_P8,  KC_P9,
  KC_MPLY, KC_P4, KC_P5,  KC_P6,
  KC_F16,   KC_P1, KC_P2,  KC_P3,
  KC_F17,   KC_P0, KC_PDOT,KC_ENT
  ),

  [_VIA1] = LAYOUT(
           KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),

  [_VIA2] = LAYOUT(
           KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),

  [_VIA3] = LAYOUT(
           KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  process_record_remote_kb(keycode, record);
  return true;
}

void matrix_init_user(void) {
  matrix_init_remote_kb();
  set_bitc_LED(LED_OFF);
}

void matrix_scan_user(void) {
  matrix_scan_remote_kb();
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            tap_remote_key(KC_VOLU);
        } else {
            tap_remote_key(KC_VOLD);
        }
    } else if (index == 1) {
        if (clockwise) {
            tap_remote_key(KC_MPRV);
        } else {
            tap_remote_key(KC_MNXT);
        }
    }
    return true;
}
