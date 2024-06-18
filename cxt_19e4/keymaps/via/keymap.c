/* Copyright 2023 Brian McKenna
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

#include QMK_KEYBOARD_H
#include "add_keycodes.h"
#include "i2c_master.h"
#include "gpio.h"
#include "quantum.h"
#include "analog.h"
#include "qmk_midi.h"
#include "os_detection.h"
#include "./drivers/sensors/analog_joystick.h"
//#include "analog_joystick.h"




typedef union {
  uint32_t raw;
  struct {
    uint8_t jMode;
    uint8_t kLayer;
  };
} user_config_t;

user_config_t user_config;
uint8_t jMode;
uint8_t kLayer;

typedef struct {
    int8_t jdx;
    int8_t jdy;
} report_jdelta_t;

report_jdelta_t master;

void joysitck_mode_master_INC(void) {
  //if(jMode == _JOYSZ) {
  if(jMode == _JOYSL) {
    jMode = _MOUSE;
  }else{
    jMode++;
  }
}

void keyboard_layer__INC(void) {
  if(kLayer == _ADJUST) {
    kLayer = _BASE;
  }else{
    kLayer++;
  }
}

int16_t xOrigin, yOrigin;

void JoyOriginReset(void){
  xOrigin = analogReadPin(JSH);
  yOrigin = analogReadPin(JSV);
  //XUpThreshold   = xOrigin - _DEAD;
  //XDownThreshold = xOrigin + _DEAD;
  //YUpThreshold   = yOrigin - _DEAD;
  //YDownThreshold = yOrigin + _DEAD;
}

#define ASCII_0 48

#define SCROLL_DIVISOR_H 8.0
#define SCROLL_DIVISOR_V 8.0

#define EISU LALT(KC_GRV)

uint8_t divisor = 0;



#if 1
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[0] = LAYOUT(
        KC_ESC,  KC_F11,  KC_P,       KC_Q,
        KC_A,    KC_B,    KC_C,       KC_D,
        KC_E,    KC_2,    KC_0,       KC_1,
        KC_F,    KC_G,    KC_H,
        KC_I,    KC_J,    KC_K,       KC_L,
        KC_4,    KC_5,    MJMODE,     LAYSW,
        KC_UP,   KC_LEFT, KC_RIGHT,   KC_DOWN
    )
};
#endif


#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = {
        ENCODER_CCW_CW(KC_VOLD, KC_VOLU),
        ENCODER_CCW_CW(SC_UP, SC_DOWN),
        ENCODER_CCW_CW(KC_WH_L, KC_WH_R),
        ENCODER_CCW_CW(KC_WH_U, KC_WH_D)
    },
};
#endif // defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
#if 0
layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    process_record_addedkeycodes(keycode, record);
    return true;
}

void eeconfig_init_user(void) {  // EEPROM is getting reset!

    //bool dpi_change_f = false;
    //user_config.raw = 0;

    user_config.raw = eeconfig_read_user();
    if(user_config.jMode > _JOYSL || user_config.jMode < _MOUSE ){
      user_config.jMode = _MOUSE;
      eeconfig_update_user(user_config.raw);    
      //dpi_change_f = true;
    }
    jMode = user_config.jMode;

    if(user_config.kLayer > _ADJUST || user_config.kLayer < _BASE) {
        user_config.kLayer = _BASE;
        eeconfig_update_user(user_config.raw); 
    }
    kLayer = user_config.kLayer;

}

void matrix_init_user(void){
# ifdef CONSOLE_ENABLE
  dprintf("init MS XY transform value \n");
# endif
  eeconfig_init_user();
}

#if 1
void oled_driver_init(void){

    i2c_init();
#ifdef OLED_RST_PIN
    /* Reset device */
    setPinOutput(OLED_RST_PIN);
    writePinLow(OLED_RST_PIN);
    wait_ms(20);
    writePinHigh(OLED_RST_PIN);
    wait_ms(20);
#endif

}
#endif

#if 0
void keyboard_post_init_user(void) {
//#if 0
        setPinOutput(OLED_RESET_PIN);
        //setPinOutputOpenDrain(OLED_RESET_PIN);
        writePinHigh(OLED_RESET_PIN);
        //_delay_ms(1);
        wait_ms(1);
        writePinLow(OLED_RESET_PIN);
        //_delay_ms(10);
        wait_ms(10);
        writePinHigh(OLED_RESET_PIN);
        wait_ms(10);

    oled_clear();
//#endif
}
#endif

#if 0
void oled_render_boot(bool bootloader) {
    oled_clear();
    for (int i = 0; i < 16; i++) {
        oled_set_cursor(0, i);
        if (bootloader) {
            oled_write_P(PSTR("Awaiting New Firmware "), false);
        } else {
            oled_write_P(PSTR("Rebooting "), false);
        }
    }

    oled_render_dirty(true);
}
#endif


void oled_str2num(uint32_t volume){

    uint8_t numbers[4];

    numbers[3]=volume/1000+ASCII_0;
    numbers[2]=volume%1000/100+ASCII_0;
    numbers[1]=volume%1000%100/10+ASCII_0;
    numbers[0]=volume%1000%100%10+ASCII_0;
    oled_write_char(numbers[3], false);
    oled_write_char(numbers[2], false);
    oled_write_char(numbers[1], false);
    oled_write_char(numbers[0], false);

}


#ifdef OLED_ENABLE
bool oled_task_user(void) {
    // Host Keyboard Layer Status

    oled_write_P(PSTR("LYER:"), false);

    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_P(PSTR(" BASE"), false);
            break;
        case 1:
            oled_write_P(PSTR("LOWER"), false);
            break;
        case 2:
            oled_write_P(PSTR("RAISE"), false);
            break;
        case 3:
            oled_write_P(PSTR("ADJST"), false);
            break;
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            //oled_write_ln_P(PSTR("UNDEF"), false);
    }
#if 1
    oled_write_P(PSTR("JMOD:"), false);

    switch (jMode) {
        case _MOUSE:
            oled_write_P(PSTR("MOUSE"), false);
            break;
        case _SCROLL:
            oled_write_P(PSTR("SCROL"), false);
            break;
        case _WASD:
            oled_write_P(PSTR(" WASD"), false);
            break;
        case _UWASD:
            oled_write_P(PSTR("UWASD"), false);
            break;
        case _JOYSL:
            oled_write_P(PSTR("JOYSL"), false);
            break;
#if 0
        case _JOYSR:
            oled_write_P(PSTR("JOYSR"), false);
            break;
        case _JOYSZ:
            oled_write_P(PSTR("JOYSZ"), false);
            break;
#endif
        default:
            // Or use the write_ln shortcut over adding '\n' to the end of your string
            //oled_write_ln_P(PSTR("UNDEF"), false);
    }
#endif
    oled_write_P(PSTR("----------"), false);



#if 1
    // show 3 analog value
    uint32_t volumes = 1024-analogReadPin(SLIDER_PIN);
    uint32_t volumex = analogReadPin(JSH);
    uint32_t volumey = analogReadPin(JSV);
    //uint32_t volumex = (uint32_t)master.jdx;
    //uint32_t volumey = (uint32_t)master.jdy;
    //master.jdx = volumex;
    //master.jdy = volumey;

    // show joystick xy 
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM:@|") : PSTR("NUM: |"), false);
    oled_str2num(volumes);

    oled_write_P(led_state.caps_lock ? PSTR("CAP:@|") : PSTR("CAP: |"), false);
    oled_str2num(volumex);

    oled_write_P(led_state.scroll_lock ? PSTR("SCR:@|") : PSTR("SCR: |"), false);
    oled_str2num(volumey);

#endif
#if 0
    // show 3 analog value
    uint32_t volumes = 1024-analogReadPin(SLIDER_PIN);
    uint32_t volumex = analogReadPin(JSH);
    uint32_t volumey = analogReadPin(JSV);
    //uint32_t volumex = (uint32_t)master.jdx;
    //uint32_t volumey = (uint32_t)master.jdy;
    master.jdx = volumex;
    master.jdy = volumey;
    uint8_t numbers[4];
    uint8_t numberx[4];
    uint8_t numbery[4];

    numbers[3]=volumes/1000+ASCII_0;
    numbers[2]=volumes%1000/100+ASCII_0;
    numbers[1]=volumes%1000%100/10+ASCII_0;
    numbers[0]=volumes%1000%100%10+ASCII_0;

    numberx[3]=volumex/1000+ASCII_0;
    numberx[2]=volumex%1000/100+ASCII_0;
    numberx[1]=volumex%1000%100/10+ASCII_0;
    numberx[0]=volumex%1000%100%10+ASCII_0;

    numbery[3]=volumey/1000+ASCII_0;
    numbery[2]=volumey%1000/100+ASCII_0;
    numbery[1]=volumey%1000%100/10+ASCII_0;
    numbery[0]=volumey%1000%100%10+ASCII_0;

    // show joystick xy 
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM:@|") : PSTR("NUM: |"), false);
    oled_write_char(numbers[3], false);
    oled_write_char(numbers[2], false);
    oled_write_char(numbers[1], false);
    oled_write_char(numbers[0], false);

    oled_write_P(led_state.caps_lock ? PSTR("CAP:@|") : PSTR("CAP: |"), false);
    //oled_write_P(buffer, false);
    oled_write_char(numberx[3], false);
    oled_write_char(numberx[2], false);
    oled_write_char(numberx[1], false);
    oled_write_char(numberx[0], false);

    oled_write_P(led_state.scroll_lock ? PSTR("SCR:@|") : PSTR("SCR: |"), false);
    oled_write_char(numbery[3], false);
    oled_write_char(numbery[2], false);
    oled_write_char(numbery[1], false);
    oled_write_char(numbery[0], false);
#endif

#if 0
    // show joystick xy
    uint32_t volumex = analogReadPin(JSH);
    uint32_t volumey = analogReadPin(JSV);
    uint8_t numberx[4];
    uint8_t numbery[4];
    numberx[3]=volumex/1000+ASCII_0;
    numberx[2]=volumex%1000/100+ASCII_0;
    numberx[1]=volumex%1000%100/10+ASCII_0;
    numberx[0]=volumex%1000%100%10+ASCII_0;

    numbery[3]=volumey/1000+ASCII_0;
    numbery[2]=volumey%1000/100+ASCII_0;
    numbery[1]=volumey%1000%100/10+ASCII_0;
    numbery[0]=volumey%1000%100%10+ASCII_0;

    // show joystick xy 
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM:@|JYXY") : PSTR("NUM: |JYSY"), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAP:@|") : PSTR("CAP: |"), false);
    //oled_write_P(buffer, false);
    oled_write_char(numberx[3], false);
    oled_write_char(numberx[2], false);
    oled_write_char(numberx[1], false);
    oled_write_char(numberx[0], false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR:@|") : PSTR("SCR: |"), false);
    oled_write_char(numbery[3], false);
    oled_write_char(numbery[2], false);
    oled_write_char(numbery[1], false);
    oled_write_char(numbery[0], false);

#endif
#if 0
    // show slider value

    uint32_t volume = 1024-analogReadPin(SLIDER_PIN);
    uint8_t number10[4];
    uint16_t numberPerc[3];
    number10[3]=volume/1000+ASCII_0;
    number10[2]=volume%1000/100+ASCII_0;
    number10[1]=volume%1000%100/10+ASCII_0;
    number10[0]=volume%1000%100%10+ASCII_0;

    volume=volume*100/1024;
    numberPerc[2]=volume/100+ASCII_0;
    numberPerc[1]=volume%100/10+ASCII_0;
    numberPerc[0]=volume%100%10+ASCII_0;

    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM:@|SLDR") : PSTR("NUM: |SLDR"), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAP:@|") : PSTR("CAP: |"), false);
    //oled_write_P(buffer, false);
    oled_write_char(number10[3], false);
    oled_write_char(number10[2], false);
    oled_write_char(number10[1], false);
    oled_write_char(number10[0], false);
    //oled_write_ln_P(PSTR("\n"), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR:@|") : PSTR("SCR: |"), false);
    oled_write_char(numberPerc[2], false);
    oled_write_char(numberPerc[1], false);
    oled_write_char(numberPerc[0], false);
    oled_write_char('%', false);

#endif
 
    return false;
}
#endif

void slider(void) {
    if (divisor++) { // only run the slider function 1/256 times it's called
        return;
    }

    midi_send_cc(&midi_device, 2, 0x3E, 0x7F - (analogReadPin(SLIDER_PIN) >> 3));
}

#if 1
//joystick config
joystick_config_t joystick_axes[JOYSTICK_AXIS_COUNT] = {
    JOYSTICK_AXIS_VIRTUAL,
    JOYSTICK_AXIS_VIRTUAL,
    //JOYSTICK_AXIS_VIRTUAL,
    //JOYSTICK_AXIS_VIRTUAL,
    //JOYSTICK_AXIS_VIRTUAL,
    //JOYSTICK_AXIS_VIRTUAL
    //JOYSTICK_AXIS_IN(JSH, _MAX, _CENTER, _MIN),
    //JOYSTICK_AXIS_IN(JSV, _MIN, _CENTER, _MAX)  
};

void pointing_device_driver_init(void) {

  analog_joystick_init();
  JoyOriginReset();

}

int8_t subaxisCoordinate(pin_t pin, uint16_t origin) {
    int8_t  direction;
    int16_t distanceFromOrigin;
    int16_t range;

    int16_t position = analogReadPin(pin);

    if (origin == position) {
        return 0;
    } else if (origin > position) {
        distanceFromOrigin = origin - position;
        range              = origin - ANALOG_JOYSTICK_AXIS_MIN;
        direction          = -1;
    } else {
        distanceFromOrigin = position - origin;
        range              = ANALOG_JOYSTICK_AXIS_MAX - origin;
        direction          = 1;
    }

    float   percent    = (float)distanceFromOrigin / range;
    int16_t coordinate = (int16_t)(percent * 100);
    if (coordinate < 0) {
        return 0;
    } else if (coordinate > 100) {
        return 100 * direction;
    } else {
        return coordinate * direction;
    }
}

report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report){

    report_analog_joystick_t joym = analog_joystick_read();

    master.jdx = -subaxisCoordinate(JSH, xOrigin);
    master.jdy = subaxisCoordinate(JSV, yOrigin);
    //master.jdx = axisCoordinate(JSH, uint16_t origin, uint8_t axis);
    //master.jdy = axisCoordinate(JSV, uint16_t origin, uint8_t axis);

    if(jMode == _MOUSE){
        mouse_report.x = joym.x;
        mouse_report.y = joym.y;
        //master.jdx = 0;
        //master.jdy = 0;
    }else{
        //master.jdx = joym.x;
        //master.jdy = joym.y;
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
     //mouse_report.x = subaxisCoordinate(JSH, xOrigin);
     //mouse_report.y = subaxisCoordinate(JSV, yOrigin);
     //mouse_report.x = axisCoordinate(JSH, xOrigin);
     //mouse_report.y = axisCoordinate(JSV, yOrigin);

     return mouse_report; 
}
#endif

#if 0
void matrix_scan_user(void) {
    slider();
}
#else


bool LyDownHeld = false;
bool LyUpHeld = false;
bool LxLeftHeld = false;
bool LxRightHeld = false;

void matrix_scan_user(void) {
    slider();

    if(jMode == _SCROLL) {

        if(!LyUpHeld    && master.jdy <=  - _TRESHOLD){
          LyUpHeld = true;
          register_code(KC_WH_U);
        }else if(LyUpHeld && master.jdy > - _TRESHOLD){
          LyUpHeld = false;
          unregister_code(KC_WH_U);
        }else if(!LyDownHeld  && master.jdy >= _TRESHOLD){
          LyDownHeld = true;
          register_code(KC_WH_D);
        }else if(LyDownHeld && master.jdy < _TRESHOLD){
          LyDownHeld = false;
          unregister_code(KC_WH_D);
        }
        // x axis
        if(!LxLeftHeld  && master.jdx <=  - _TRESHOLD){
          LxLeftHeld = true;
          register_code(KC_WH_L);
        }else if(LxLeftHeld && master.jdx > - _TRESHOLD){
          LxLeftHeld = false;
          unregister_code(KC_WH_L);
        }else if(!LxRightHeld && master.jdx >= _TRESHOLD){
          LxRightHeld = true;
          register_code(KC_WH_R);
        }else if(LxRightHeld && master.jdx < _TRESHOLD){
          LxRightHeld = false;
          unregister_code(KC_WH_R);
        }

    }else if(jMode == _WASD){

        if(!LyUpHeld    && master.jdy <=  - _TRESHOLD){
          LyUpHeld = true;
          register_code(KC_W);
        }else if(LyUpHeld && master.jdy > - _TRESHOLD){
          LyUpHeld = false;
          unregister_code(KC_W);
        }else if(!LyDownHeld  && master.jdy >= _TRESHOLD){
          LyDownHeld = true;
          register_code(KC_S);
        }else if(LyDownHeld && master.jdy < _TRESHOLD){
          LyDownHeld = false;
          unregister_code(KC_S);
        }
        // x axis
        if(!LxLeftHeld  && master.jdx <= - _TRESHOLD){
          LxLeftHeld = true;
          register_code(KC_A);
        }else if(LxLeftHeld && master.jdx > - _TRESHOLD){
          LxLeftHeld = false;
          unregister_code(KC_A);
        }else if(!LxRightHeld && master.jdx >= _TRESHOLD){
          LxRightHeld = true;
          register_code(KC_D);
        }else if(LxRightHeld && master.jdx < _TRESHOLD){
          LxRightHeld = false;
          unregister_code(KC_D);
        }

    }else if (jMode == _UWASD){

        if(!LyUpHeld    && master.jdy <= - _TRESHOLD){
          LyUpHeld = true;
          register_code(dynamic_keymap_get_keycode(biton32(layer_state),6,0));
        }else if(LyUpHeld && master.jdy > - _TRESHOLD){
          LyUpHeld = false;
          unregister_code(dynamic_keymap_get_keycode(biton32(layer_state),6,0));
        }else if(!LyDownHeld  && master.jdy >= _TRESHOLD){
          LyDownHeld = true;
          register_code(dynamic_keymap_get_keycode(biton32(layer_state),6,3));
        }else if(LyDownHeld && master.jdy < _TRESHOLD){
          LyDownHeld = false;
          unregister_code(dynamic_keymap_get_keycode(biton32(layer_state),6,3));
        }
        // x axis
        if(!LxLeftHeld  && master.jdx <= - _TRESHOLD){
          LxLeftHeld = true;
          register_code(dynamic_keymap_get_keycode(biton32(layer_state),6,1));
        }else if(LxLeftHeld && master.jdx > - _TRESHOLD){
          LxLeftHeld = false;
          unregister_code(dynamic_keymap_get_keycode(biton32(layer_state),6,1));
        }else if(!LxRightHeld && master.jdx >= _TRESHOLD){
          LxRightHeld = true;
          register_code(dynamic_keymap_get_keycode(biton32(layer_state),6,2));
        }else if(LxRightHeld && master.jdx < _TRESHOLD){
          LxRightHeld = false;
          unregister_code(dynamic_keymap_get_keycode(biton32(layer_state),6,2));
        }

    }else if(jMode == _JOYSL){
          //joystick_set_axis( 0, master.jdx*2048/100);
          //joystick_set_axis( 1, master.jdy*2048/100);
          joystick_set_axis( 0, master.jdx);
          joystick_set_axis( 1, master.jdy);
#if 0          
    }else if(jMode == _JOYSR){
          joystick_set_axis( 3, master.jdx*2048/100);
          joystick_set_axis( 4, master.jdy*2048/100);
    }else if(jMode == _JOYSZ){
          joystick_set_axis( 2, master.jdx*2048/100);
          joystick_set_axis( 5, master.jdy*2048/100);
#endif
    //----------------------mouse mode------------------------------
    }else if(jMode == _MOUSE){

    }
}
#endif



bool process_record_addedkeycodes(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case BASE:
            if (record->event.pressed) {
            //print("mode just switched to qwerty and this is a huge string\n");
            set_single_persistent_default_layer(_BASE);
            }
            return false;
            //break;
        case LOWER:
            if (record->event.pressed) {
                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }  else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            //break;
        case RAISE:
            if (record->event.pressed) {
                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            //break;
        case ADJUST:
            if (record->event.pressed) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            return false;
            //break;
        case LAYSW:
            if (record->event.pressed) { 
 #if 0
                uint8_t current_layer = get_highest_layer(layer_state);
                // Check if we are within the range, if not quit
                if (current_layer > LAYER_CYCLE_END || current_layer < LAYER_CYCLE_START) {
                    return false;
                }
                uint8_t next_layer = current_layer + 1;
                if (next_layer > LAYER_CYCLE_END) {
                    next_layer = LAYER_CYCLE_START;
                }
                layer_move(next_layer);
#endif
                keyboard_layer__INC();
                user_config.kLayer = kLayer;
                layer_move(kLayer);
                eeconfig_update_user(user_config.raw);
            }            
            return false;

        case MJMODE:
            if (record->event.pressed) {
                joysitck_mode_master_INC();
                user_config.jMode = jMode;
                eeconfig_update_user(user_config.raw);
            }
            //dprintf("mjoymode \n");
            return false;
#if 0
        case SJMODE:
            if (record->event.pressed) {
                //joysitck_mode_slave_INC();
                zoom_mode_slave_INC();
                user_config.SjMode = SjMode;
                eeconfig_update_user(user_config.raw); 
            }
            return false;
#endif
        case JOYRST:
            if (record->event.pressed) {
                JoyOriginReset();
            }
            return false;

        case CMD_CTL:
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                }
            } else {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    unregister_code(KC_LGUI);
                } else {
                    unregister_code(KC_LCTL);
                }
            }
            return false;
            break;
        case UNDO:
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_Z);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                    tap_code(KC_Z);
                    unregister_code(KC_LCTL);
                }
            }
            return false;
            break;
        case REDO:
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LSFT);
                    register_code(KC_LGUI);
                    tap_code(KC_Z);
                    unregister_code(KC_LGUI);
                    unregister_code(KC_LSFT);
                } else {
                    register_code(KC_LSFT);
                    register_code(KC_LCTL);
                    tap_code(KC_Z);
                    unregister_code(KC_LCTL);
                    unregister_code(KC_LSFT);
                }
            }
            return false;
            break;
        case COPY:
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_C);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                    tap_code(KC_C);
                    unregister_code(KC_LCTL);
                }
            }
            return false;
            break;
        case CUT:
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_X);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                    tap_code(KC_X);
                    unregister_code(KC_LCTL);
                }
            }
            return false;
            break;
        case PASTE:
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_V);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                    tap_code(KC_V);
                    unregister_code(KC_LCTL);
                }
            }
            return false;
            break;
        case SC_UP:
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_PPLS);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                    tap_code(KC_PPLS);
                    unregister_code(KC_LCTL);
                }
            }
            return false;
            break;
        case SC_DOWN:
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_PMNS);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                    tap_code(KC_PMNS);
                    unregister_code(KC_LCTL);
                }
            }
            return false;
            break;
        case SC_RESET:
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_0);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                    tap_code(KC_0);
                    unregister_code(KC_LCTL);
                }
            }
            return false;
            break;
        case CAPTCHA:
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LSFT);
                    register_code(KC_LGUI);
                    tap_code(KC_4);
                    unregister_code(KC_LGUI);
                    unregister_code(KC_LSFT);
                } else {
                    register_code(KC_LSFT);
                    register_code(KC_LGUI);
                    tap_code(KC_S);
                    unregister_code(KC_LGUI);
                    unregister_code(KC_LSFT);
                }
            }
            return false;
            break;
        case SAVE:
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_S);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                    tap_code(KC_S);
                    unregister_code(KC_LCTL);
                }
            }
            return false;
            break;
        case SAVEAS:
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LSFT);
                    register_code(KC_LGUI);
                    tap_code(KC_S);
                    unregister_code(KC_LGUI);
                    unregister_code(KC_LSFT);
                } else {
                    register_code(KC_LSFT);
                    register_code(KC_LCTL);
                    tap_code(KC_S);
                    unregister_code(KC_LCTL);
                    unregister_code(KC_LSFT);
                }
            }
            return false;
            break;
        case NEXTTAB:
            if (record->event.pressed) {
                register_code(KC_LCTL);
                tap_code(KC_TAB);
                unregister_code(KC_LCTL);
            }
            return false;
            break;
        case PREVTAB:
            if (record->event.pressed) {
                register_code(KC_LSFT);
                register_code(KC_LCTL);
                tap_code(KC_TAB);
                unregister_code(KC_LCTL);
                unregister_code(KC_LSFT);
            }
            return false;
            break;
        case CLOSETAB:
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_W);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                    tap_code(KC_W);
                    unregister_code(KC_LCTL);
                }
            }
            return false;
            break;
        case RSTRTAB:
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LSFT);
                    register_code(KC_LGUI);
                    tap_code(KC_T);
                    unregister_code(KC_LGUI);
                    unregister_code(KC_LSFT);
                } else {
                    register_code(KC_LSFT);
                    register_code(KC_LCTL);
                    tap_code(KC_T);
                    unregister_code(KC_LCTL);
                    unregister_code(KC_LSFT);
                }
            }
            return false;
            break;
        case NEXTAPP:
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_TAB);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LALT);
                    tap_code(KC_TAB);
                    unregister_code(KC_LALT);
                }
            }
            return false;
            break;
        case PREVAPP:
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LSFT);
                    register_code(KC_LGUI);
                    tap_code(KC_TAB);
                    unregister_code(KC_LGUI);
                    unregister_code(KC_LSFT);
                } else {
                    register_code(KC_LSFT);
                    register_code(KC_LALT);
                    tap_code(KC_TAB);
                    unregister_code(KC_LALT);
                    unregister_code(KC_LSFT);
                }
            }
            return false;
            break;
        case SEARCH:
            if (record->event.pressed) {
                if (detected_host_os() == OS_MACOS || detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_SPC);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LGUI);
                    tap_code(KC_S);
                    unregister_code(KC_LGUI);
                }
            }
            return false;
            break;
        case IPADHOME:
            if (record->event.pressed) {
                if (detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    tap_code(KC_H);
                    unregister_code(KC_LGUI);
                }
            }
            return false;
            break;
        case IPADDOCK:
            if (record->event.pressed) {
                if (detected_host_os() == OS_IOS){
                    register_code(KC_LGUI);
                    register_code(KC_LALT);
                    tap_code(KC_D);
                    unregister_code(KC_LALT);
                    unregister_code(KC_LGUI);
                }
            }
            return false;
            break;
    }
   
    return true;
}