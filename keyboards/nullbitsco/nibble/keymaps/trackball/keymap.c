/* Copyright 2021 Jonathan Law, Jay Greco
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
 *
 * Original: j-inc's kyria keymap
 */
#include QMK_KEYBOARD_H
#include "animation_frames.h"
#include "pointing_device.h"
#include <stdio.h>
#include "big_led.h"

#define TRACKBALL_ORIENTATION 3
#include "./features/pimoroni_trackball.h"

enum layer_names {
  _BASE,
  _FUNC,
  _OPTS,
  _MOUS
};

enum custom_keycodes {
  #ifdef VIA_ENABLE
  PROG = USER00,
  #else
  PROG = SAFE_RANGE,
  #endif
  KC_PRVWD,
  KC_NXTWD,
  KC_MAC,
  KC_MGUI,
  KC_MALT,
  _NUM_CUST_KCS,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_all(
              KC_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,
    KC_MAC,   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME,
    KC_F14,   XXXXXXX, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_END,
    MO(_OPTS),KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_PGDN,
    MO(_FUNC),KC_LCTL, KC_MGUI, KC_MALT,                            KC_SPC,                  KC_SPC, KC_RALT, MO(_FUNC), KC_LEFT, KC_DOWN, KC_RGHT
  ),

  [_FUNC] = LAYOUT_all(
              KC_GRV,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, KC_MUTE,
    RGB_TOG,  _______, _______, KC_PGUP, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_VOLU,
    _______,  _______,KC_PRVWD, KC_PGDN,KC_NXTWD, _______, _______, _______, _______, _______, _______, _______, _______,          _______, KC_VOLD,
    _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______,  _______, _______, _______,                            _______,                   _______, _______, _______, KC_MPRV, KC_MPLY, KC_MNXT
  ),

  [_OPTS] = LAYOUT_all(
              _______, KC_MAC,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_SAI,
    _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_SAD,
    _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, RGB_HUI,
    _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, RGB_MODE_FORWARD, RGB_HUD,
    _______,  _______, _______, _______,                            _______,                   _______, _______, _______, RGB_VAD, RGB_MODE_REVERSE, RGB_VAI
  ),

  [_MOUS] = LAYOUT_all(
              _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
    _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______,  _______, _______, _______,                            KC_BTN1,                   KC_BTN2, _______, _______, _______, _______, _______
  ),

};

// user storage
typedef union {
    uint32_t raw;
    struct {
        bool mac_mode :1;
    };
} user_config_t;
user_config_t user_config;

void update_mac_led(void) {
    if (user_config.mac_mode) {
        set_big_LED_rgb(255, 255, 0);
    } else {
        // this shows up as blue because of the i2c interference with the LED
        set_big_LED_rgb(0, 0, 0);
    }
}

void eeconfig_init_user(void) {
    user_config.raw = 0;
    user_config.mac_mode = false;
    eeconfig_update_user(user_config.raw);
    update_mac_led();
}

// trackball
static int16_t mouse_auto_layer_timer = 0;
#define MOUSE_TIMEOUT 600
#define TRACKBALL_TIMEOUT 5

#define TRACKBALL_BRIGHTNESS 128

#define SIGN(x) ((x > 0) - (x < 0))

void keyboard_post_init_user(void) {
    trackball_set_brightness(TRACKBALL_BRIGHTNESS);
    user_config.raw = eeconfig_read_user();
    update_mac_led();
}

// ! this is already done by the oled - doing it twice breaks the oled on reset
// void matrix_init_user() {
//     trackball_init();
// }

void suspend_power_down_user(void) {
    trackball_set_brightness(0);
    // trackball_sleep();
}

void update_member(int8_t* member, int16_t* offset) {//{{{
    if (*offset > 127) {
        *member = 127;
        *offset -= 127;
    } else if (*offset < -127) {
        *member = -127;
        *offset += 127;
    } else {
        *member = *offset;
        *offset = 0;
    }
}//}}}

#define MOUSE_MOVE_SCALE 2

static int16_t x_offset = 0;
static int16_t y_offset = 0;
static int16_t v_offset = 0;
static int16_t h_offset = 0;
static int16_t tb_timer = 0;
void pointing_device_task() {
    report_mouse_t mouse = pointing_device_get_report();

    if (trackball_get_interrupt() && (!tb_timer || timer_elapsed(tb_timer) > TRACKBALL_TIMEOUT)) {
        tb_timer = timer_read() | 1;
        trackball_state_t state = trackball_get_state();

        uint8_t mods;
        if (state.x || state.y) {
            // trigger_tapping();
            mods = get_mods();
        }

        if (state.button_triggered) {
            if (state.button_down) {
                mouse.buttons |= MOUSE_BTN1;
            } else {
                mouse.buttons &= ~MOUSE_BTN1;
            }
            pointing_device_set_report(mouse);
            pointing_device_send();
        } else {
            #ifdef TRACKBALL_DEFAULT_POINTER
            bool is_scroll = layer_state_is(_FUNC);
            #else
            bool is_scroll = !layer_state_is(_FUNC);
            #endif
            if (is_scroll) {
                h_offset += state.x;
                v_offset += -state.y;
            } else if ((state.x || state.y) && !state.button_down) {
                uint8_t scale = MOUSE_MOVE_SCALE;
                // precision mode when ctrl is held
                if (mods & MOD_MASK_CTRL) scale = scale - 1;
                x_offset += state.x * state.x * state.x * /*SIGN(state.x) * */scale;
                y_offset += state.y * state.y * state.y * /*SIGN(state.y) * */scale;
            }
        }
    }

    while (x_offset || y_offset || h_offset || v_offset) {
        update_member(&mouse.x, &x_offset);
        update_member(&mouse.y, &y_offset);
        update_member(&mouse.v, &v_offset);
        update_member(&mouse.h, &h_offset);

        pointing_device_set_report(mouse);
        pointing_device_send();
    }
}

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    if ((keycode < KC_BTN1 || keycode > KC_BTN5) && layer_state_is(_MOUS) && record->event.pressed) {
        layer_off(_MOUS);
        mouse_auto_layer_timer = 0;
    }
    return true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer = get_highest_layer(state);
    switch(layer) {
        case _FUNC:
            trackball_set_rgbw(0, TRACKBALL_BRIGHTNESS, 0, 0);
            break;
        case _OPTS:
            trackball_set_rgbw(0, TRACKBALL_BRIGHTNESS, TRACKBALL_BRIGHTNESS, 0);
            break;
        case _MOUS:
            trackball_set_rgbw(TRACKBALL_BRIGHTNESS, TRACKBALL_BRIGHTNESS, 0, 0);
            break;
        default:
            trackball_set_rgbw(0, 0, 0, TRACKBALL_BRIGHTNESS);
            break;
    }
    return state;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    // rotary is next/prev word by default,
    // volume on FUNC layer
    if (layer_state_is(_FUNC)) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else {
        if (clockwise) {
            if (user_config.mac_mode) {
                register_mods(MOD_LALT);
                tap_code(KC_RIGHT);
                unregister_mods(MOD_LALT);
            } else{
                register_mods(MOD_RCTL);
                tap_code(KC_RIGHT);
                unregister_mods(MOD_RCTL);
            }
        } else {
            if (user_config.mac_mode) {
                register_mods(MOD_LALT);
                tap_code(KC_LEFT);
                unregister_mods(MOD_LALT);
            } else{
                register_mods(MOD_RCTL);
                tap_code(KC_LEFT);
                unregister_mods(MOD_RCTL);
            }
        }
    }
    return true;
}

#define IDLE_FRAME_DURATION 200 // Idle animation iteration rate in ms

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

uint32_t anim_timer         = 0;
uint32_t anim_sleep         = 0;
uint8_t  current_idle_frame = 0;

bool tap_anim        = false;
bool tap_anim_toggle = false;


// Decompress and write a precompressed bitmap frame to the OLED.
// Documentation and python compression script available at:
// https://github.com/nullbitsco/squeez-o
#ifdef USE_OLED_BITMAP_COMPRESSION
static void oled_write_compressed_P(const char* input_block_map, const char* input_block_list) {
    uint16_t block_index = 0;
    for (uint16_t i=0; i<NUM_OLED_BYTES; i++) {
        uint8_t bit = i%8;
        uint8_t map_index = i/8;
        uint8_t _block_map = (uint8_t)pgm_read_byte_near(input_block_map + map_index);
        uint8_t nonzero_byte = (_block_map & (1 << bit));
        if (nonzero_byte) {
            const char data = (const char)pgm_read_byte_near(input_block_list + block_index++);
            oled_write_raw_byte(data, i);
        } else {
            const char data = (const char)0x00;
            oled_write_raw_byte(data, i);
        }
    }
}
#endif

static void render_anim(void) {
    // Idle animation
    void animation_phase(void) {
        if (!tap_anim) {
            current_idle_frame = (current_idle_frame + 1) % NUM_IDLE_FRAMES;
            uint8_t idx = abs((NUM_IDLE_FRAMES - 1) - current_idle_frame);
            #ifdef USE_OLED_BITMAP_COMPRESSION
            oled_write_compressed_P(idle_block_map[idx], idle_frames[idx]);
            #else
            oled_write_raw_P(idle_frames[idx], NUM_OLED_BYTES);
            #endif
        }
    }

    // Idle behaviour
    if (get_current_wpm() != 000) {  // prevent sleep
        oled_on();
        if (timer_elapsed32(anim_timer) > IDLE_FRAME_DURATION) {
            anim_timer = timer_read32();
            animation_phase();
        }
        anim_sleep = timer_read32();
    } else {  // Turn off screen when timer threshold elapsed or reset time since last input
        if (timer_elapsed32(anim_sleep) > OLED_TIMEOUT) {
            oled_off();
        } else {
            if (timer_elapsed32(anim_timer) > IDLE_FRAME_DURATION) {
                anim_timer = timer_read32();
                animation_phase();
            }
        }
    }
}

void oled_task_user(void) {
    render_anim();
    oled_set_cursor(0, 14);
    // sprintf(wpm_str, ">%04d", get_current_wpm());
    oled_write_ln(user_config.mac_mode ? "mac" : "win", false);
}

// Animate tap
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Check if non-mod
    if ((keycode >= KC_A && keycode <= KC_0) || (keycode >= KC_TAB && keycode <= KC_SLASH)) {
        if (record->event.pressed) {
            // Display tap frames
            tap_anim_toggle = !tap_anim_toggle;
            #ifdef USE_OLED_BITMAP_COMPRESSION
            oled_write_compressed_P(tap_block_map[tap_anim_toggle], tap_frames[tap_anim_toggle]);
            #else
            oled_write_raw_P(tap_frames[tap_anim_toggle], NUM_OLED_BYTES);
            #endif
        }
    }

    switch(keycode) {
        case KC_PRVWD:
            if (record->event.pressed) {
                if (user_config.mac_mode) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_LEFT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_LEFT);
                }
            } else {
                if (user_config.mac_mode) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_LEFT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_LEFT);
                }
            }
            break;
        case KC_NXTWD:
             if (record->event.pressed) {
                if (user_config.mac_mode) {
                    register_mods(mod_config(MOD_LALT));
                    register_code(KC_RIGHT);
                } else {
                    register_mods(mod_config(MOD_LCTL));
                    register_code(KC_RIGHT);
                }
            } else {
                if (user_config.mac_mode) {
                    unregister_mods(mod_config(MOD_LALT));
                    unregister_code(KC_RIGHT);
                } else {
                    unregister_mods(mod_config(MOD_LCTL));
                    unregister_code(KC_RIGHT);
                }
            }
            break;

        case KC_MAC:
            if (record->event.pressed) {
                user_config.mac_mode ^= 1;
                eeconfig_update_user(user_config.raw);
                update_mac_led();
            }
            return false;

        case KC_MGUI:
            if (record->event.pressed) {
                if (user_config.mac_mode) {
                    register_code(KC_LALT);
                } else {
                    register_code(KC_LGUI);
                }
            } else {
                if (user_config.mac_mode) {
                    unregister_code(KC_LALT);
                } else {
                    unregister_code(KC_LGUI);
                }
            }
            return false;

        case KC_MALT:
            if (record->event.pressed) {
                if (user_config.mac_mode) {
                    register_code(KC_LGUI);
                } else {
                    register_code(KC_LALT);
                }
            } else {
                if (user_config.mac_mode) {
                    unregister_code(KC_LGUI);
                } else {
                    unregister_code(KC_LALT);
                }
            }
            return false;

        default:
        break;
    }

    return true;
}
