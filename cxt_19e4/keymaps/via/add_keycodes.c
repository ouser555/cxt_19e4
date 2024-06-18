// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "add_keycodes.h"
#include "quantum.h"
#include "os_detection.h"


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

        case MJMODE:
            if (record->event.pressed) {
                joysitck_mode_master_INC();
                user_config.MjMode = MjMode;
                eeconfig_update_user(user_config.raw); 
            }
            //dprintf("mjoymode \n");
            return false;

        case SJMODE:
            if (record->event.pressed) {
                //joysitck_mode_slave_INC();
                zoom_mode_slave_INC();
                user_config.SjMode = SjMode;
                eeconfig_update_user(user_config.raw); 
            }
            return false;

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