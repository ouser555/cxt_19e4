// Copyright 2023 Colin Kinloch (@ColinKinloch)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#if 0
#define RGB_MATRIX_LED_COUNT 12

#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_TYPING_HEATMAP

#define RGB_MATRIX_KEYPRESSES
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS

#define ENABLE_RGB_MATRIX_TYPING_HEATMAP
#define ENABLE_RGB_MATRIX_DIGITAL_RAIN
#define ENABLE_RGB_MATRIX_MULTISPLASH
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE

#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
#define ENABLE_RGB_MATRIX_SPLASH
#define ENABLE_RGB_MATRIX_MULTISPLASH
#define ENABLE_RGB_MATRIX_SOLID_SPLASH
#define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH

#define RGB_MATRIX_TYPING_HEATMAP_SPREAD 9
#endif
/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT


//#define OLED_DISPLAY_CUSTOM
//#define OLED_DISPLAY_WIDTH 64
//#define OLED_DISPLAY_HEIGHT 48
//#define OLED_MATRIX_SIZE ((OLED_DISPLAY_WIDTH/8)*OLED_DISPLAY_HEIGHT)
//#define  OLED_BLOCK_TYPE uint16_t

#define OLED_DISPLAY_64X48
//#define OLED_RST_PIN 0
#define OLED_RST_PIN D2

#define OLED_FONT_WIDTH 6
#define OLED_FONT_HEIGHT 8


#define ANALOG_JOYSTICK_X_AXIS_PIN B6
#define ANALOG_JOYSTICK_Y_AXIS_PIN F1
#define JSH ANALOG_JOYSTICK_X_AXIS_PIN
#define JSV ANALOG_JOYSTICK_Y_AXIS_PIN

#define POINTING_DEVICE_INVERT_X

#define JOYSTICK_BUTTON_COUNT 0
// Min 0, max 6: X, Y, Z, Rx, Ry, Rz
//#define JOYSTICK_AXIS_COUNT 6
#define JOYSTICK_AXIS_COUNT 2
// Min 8, max 16
//#define JOYSTICK_AXIS_RESOLUTION 10 // for AVR


#define ANALOG_JOYSTICK_AXIS_MAX 800
#define ANALOG_JOYSTICK_AXIS_MIN 200


#if 0
#define ANALOG_JOYSTICK_AXIS_MAX
#define ANALOG_JOYSTICK_AXIS_MIN
#define ANALOG_JOYSTICK_AUTO_AXIS
#define ANALOG_JOYSTICK_SPEED_REGULATOR
#define ANALOG_JOYSTICK_READ_INTERVAL
#define ANALOG_JOYSTICK_SPEED_MAX
#define ANALOG_JOYSTICK_CLICK_PIN
#define ANALOG_JOYSTICK_WEIGHTS
#define ANALOG_JOYSTICK_CUTOFF
#endif

#define _MIN 190
#define _MAX 820
#define _J_REP_MAX 2047
#define _CENTER 550
#define _J_DEADZONE 10
//#define _MAX 1023
//#define _CENTER 512
#define _DEAD 100
#define _SHIFT 15 // last 15 steps upwards

#define _DOWN_TRESHOLD (_CENTER+_DEAD)
#define _UP_TRESHOLD (_CENTER-_DEAD)

#define _TRESHOLD 40


#define SLIDER_PIN F7