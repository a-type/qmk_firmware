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
// #include "pointing_device.h"
#include <stdio.h>
// #include "big_led.h"

// #define TRACKBALL_ORIENTATION 3
// #include "./features/pimoroni_trackball.h"

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
  KC_DVTL,
  // VS Code pane
  KC_VCPN,
  _NUM_CUST_KCS,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_all(
              KC_GESC, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,
    KC_MAC,   KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME,
    KC_F12,   KC_GRV, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_END,
    MO(_OPTS),KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_UP,   KC_PGDN,
    MO(_FUNC),KC_LCTL, KC_MGUI, KC_MALT,                            KC_SPC,                  KC_SPC, KC_RALT, MO(_FUNC), KC_LEFT, KC_DOWN, KC_RGHT
  ),

  [_FUNC] = LAYOUT_all(
              KC_GRV,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, KC_MUTE,
    RGB_TOG,  _______, _______, KC_PGUP, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_VOLU,
    KC_VCPN,  _______,KC_PRVWD, KC_PGDN,KC_NXTWD, _______, _______, _______, _______, _______, _______, _______, _______,          _______, KC_VOLD,
    _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______,  _______, KC_LGUI, KC_LALT,                            _______,                   _______, _______, _______, KC_MPRV, KC_MPLY, KC_MNXT
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

void eeconfig_init_user(void) {
    user_config.raw = 0;
    user_config.mac_mode = false;
    eeconfig_update_user(user_config.raw);
}

#define SIGN(x) ((x > 0) - (x < 0))

void keyboard_post_init_user(void) {
    // trackball_set_brightness(TRACKBALL_BRIGHTNESS);
    user_config.raw = eeconfig_read_user();
    // update_mac_led();
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

uint8_t mod_state;

bool win_mac_mode_key(uint16_t swap_key, uint16_t input_key, keyrecord_t *record) {
    if (swap_key != input_key) {
        return true;
    }

    if (record->event.pressed) {
        if (!user_config.mac_mode && mod_state & MOD_BIT(KC_LALT)) {
            del_mods(KC_LALT);
            add_mods(MOD_LCTL);
            register_code(swap_key);
            set_mods(mod_state);
            return false;
        }
    }

    return true;
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

    mod_state = get_mods();

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

        // devtools shortcut
        case KC_DVTL:
            if (record->event.pressed) {
                if (user_config.mac_mode) {
                    register_mods(mod_config(MOD_LALT | MOD_LGUI));
                    register_code(KC_J);
                } else {
                    register_mods(mod_config(MOD_LCTL | MOD_LSFT));
                    register_code(KC_J);
                }
            } else {
                if (user_config.mac_mode) {
                    unregister_mods(mod_config(MOD_LALT | MOD_LGUI));
                    unregister_code(KC_J);
                } else {
                    unregister_mods(mod_config(MOD_LCTL | MOD_LSFT));
                    unregister_code(KC_J);
                }
            }
            return false;

        case KC_VCPN:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL | MOD_LSFT));
                register_code(KC_GRV);
            } else {
                unregister_mods(mod_config(MOD_LCTL | MOD_LSFT));
                unregister_code(KC_GRV);
            }
            return false;

        case KC_MAC:
            if (record->event.pressed) {
                user_config.mac_mode ^= 1;
                eeconfig_update_user(user_config.raw);
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
                    register_code(KC_RCTL);
                }
            } else {
                if (user_config.mac_mode) {
                    unregister_code(KC_LGUI);
                } else {
                    unregister_code(KC_RCTL);
                }
            }
            return false;

        // Windows mode: rebind RCtrl+Tab to Alt+Tab
        case KC_TAB:
            if (user_config.mac_mode) {
                break;
            }
            // detect RCtrl+Tab on Windows; rebind to Alt+Tab
            if (record->event.pressed) {
                if ((mod_state & MOD_BIT(KC_RCTL)) == MOD_BIT(KC_RCTL)) {
                    del_mods(KC_RCTL);
                    add_mods(MOD_LALT);
                    register_code(KC_TAB);
                    set_mods(mod_state);
                    return false;
                }
            }

        // Windows mode: rebind RCtrl+Arrow to Alt+Arrow
        case KC_LEFT:
        case KC_RIGHT:
        case KC_UP:
        case KC_DOWN:
            if (user_config.mac_mode) {
                break;
            }
            if (record->event.pressed) {
                if ((mod_state & MOD_BIT(KC_RCTL)) == MOD_BIT(KC_RCTL)) {
                    unregister_mods(KC_RCTL);
                    register_mods(MOD_LALT);
                    register_code(keycode);
                    set_mods(mod_state);
                    return false;
                }
            }

        // Windows mode: rebind RCtrl+F4 to Alt+F4
        case KC_F4:
            if (user_config.mac_mode) {
                break;
            }
            if (record->event.pressed) {
                if ((mod_state & MOD_BIT(KC_RCTL)) == MOD_BIT(KC_RCTL)) {
                    unregister_mods(KC_RCTL);
                    register_mods(MOD_LALT);
                    register_code(keycode);
                    set_mods(mod_state);
                    return false;
                }
            }

        default:
        break;
    }

    return true;
}
