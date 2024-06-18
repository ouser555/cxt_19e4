// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"


enum layers {
    _BASE = 0,
    _LOWER,
    _RAISE,
    _ADJUST,
    _EXTRA01,
    //_EXTRA02
};

#define LAYER_CYCLE_END 0
#define LAYER_CYCLE_START 3

enum jmodes {
    _MOUSE = 0,
    _SCROLL,
    _WASD,
    _UWASD,
    _JOYSL,
    //_JOYSR,
    //_JOYSZ,
};

enum custom_keycodes {
#ifdef VIA_ENABLE
  //QWERTY = USER00,
  BASE = QK_KB_0,
  //QWERTY = CS00, // qmk v1.1.1
#else
  QWERTTY = SAFE_RANGE,
#endif
  LOWER,
  RAISE,
  ADJUST,
  EXTRA01,
  LAYSW,
  MJMODE,
  SJMODE,
  JOYRST,
  UNDO,
  REDO,
  COPY,
  CUT,
  PASTE,
  SC_UP,
  SC_DOWN,
  SC_RESET,
  CAPTCHA,
  SAVE,
  SAVEAS,
  NEXTTAB,
  PREVTAB,
  CLOSETAB,
  RSTRTAB,
  NEXTAPP,
  PREVAPP,
  SEARCH,
  IPADHOME,
  IPADDOCK,
  CMD_CTL
};

bool process_record_addedkeycodes(uint16_t keycode, keyrecord_t *record);