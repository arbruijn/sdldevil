/* 
 * File:   sdlconfig_hotkeys.h
 * Author: Florian Feucht
 *
 * Created on 28. April 2012, 19:57
 */

#ifndef SDLCONFIG_HOTKEYS_H
#define	SDLCONFIG_HOTKEYS_H

#include "descdefs.h"

#define SDLD_NUM_KBSTATS 6 
#define SDLD_NUM_HOTKEYS 105
#define SDLD_NUM_DESCENTVERSIONS 5


struct sdld_descentversion {
    enum descent descent_version;
    const char * txt;
}
sdld_descentversions[SDLD_NUM_DESCENTVERSIONS] = {
    { d1_10_reg, "Descent 1 Version 1.0" },
    { d1_14_reg, "Descent 1 Version 1.4+ / D1X-Rebirth / D2X-XL" },
    { d2_10_reg, "Descent 2 Version 1.0" },
    { d2_11_reg, "Descent 2 Version 1.1" },
    { d2_12_reg, "Descent 2 Version 1.2+ / D2X-Rebirth / D2X-XL" }
};

struct sdld_kbstat {
    int kbstat;
    const char *txt;
}
sdld_kbstats[SDLD_NUM_KBSTATS] ={
    { 1, "Shift"},
    { 2, "Alt"},
    { 4, "Ctrl"},
    { 8, "Numlock"},
    { 0x10, "Capslock"},
    { 0x20, "Scrollock"}
};

// FFE: rank is for sorting the keys - those with the most modifiers first,
// since the event system needs to fall through to the version with less
// modifiers when combination with modifiers is not defined
struct sdld_hotkey {
    int kbstat, key, event, rank;
    const char *txt;
}
sdld_hotkeys[SDLD_NUM_HOTKEYS] = {
    { 0x01, 27, 59,     0, "fast quit"},
    { 0x00, 27, 0,      0, "quit"},
    { 0x01, 9, 84,      0, "wireframe on/off"},
    { 0x05, 32, 116,    0, "reinit graphics"},
    { 0x00, 9, 58,      0, "change movemode"},
    { 0x00, 100, 117,   0, "change single/double"},
    { 0x01, 49, 118,    0, "rendering off"},
    { 0x01, 50, 119,    0, "render current cube"},
    { 0x01, 245, 120,   0, "fly through"},
    { 0x01, 51, 120,    0, "fly through"},
    { 0x01, 52, 121,    0, "fly with collision"},
    { 0x00, 53, 28,     0, "beam to current object"},
    { 0x00, 55, 29,     0, "goto wall"},
    { 0x01, 265, 30,    0, "goto side cube"},
    { 0x02, 32, 20,     0, "tag special"},
    { 0x01, 32, 72,     0, "tag all"},
    { 0x00, 32, 73,     0, "tag"},
    { 0x01, 127, 11,    0, "delete tagged"},
    { 0x00, 127, 10,    0, "delete current"},
    { 0x00, 8, 12,      0, "delete special"},
    { 0x01, 277, 7,     0, "insert fast"},
    { 0x00, 277, 6,     0, "insert"},
    { 0x00, 270, 13,    0, "enlarge object"},
    { 0x00, 269, 14,    0, "shrink object"},
    { 0x00, 109, 3,     0, "make group"},
    { 0x01, 77, 16,     0, "set connection side/exit"},
    { 0x00, 103, 61,    0, "grid on/off"},
    { 0x04, 99, 53,     0, "mode: cube"},
    { 0x04, 115, 54,    0, "mode: side"},
    { 0x04, 101, 126,   0, "mode: edge"},
    { 0x04, 112, 55,    0, "mode: point"},
    { 0x04, 116, 56,    0, "mode: thing"},
    { 0x04, 119, 57,    0, "mode: wall"},
    { 0x01, 108, 82,    0, "previous level"},
    { 0x00, 108, 83,    0, "next level"},
    { 0x01, 99, 62,     0, "previous cube"},
    { 0x00, 99, 63,     0, "next cube"},
    { 0x01, 115, 64,    0, "previous side"},
    { 0x00, 115, 65,    0, "next side"},
    { 0x01, 101, 125,   0, "previous edge"},
    { 0x00, 101, 124,   0, "next edge"},
    { 0x01, 112, 66,    0, "previous point"},
    { 0x00, 112, 67,    0, "next point"},
    { 0x01, 116, 68,    0, "previous thing"},
    { 0x00, 116, 69,    0, "next thing"},
    { 0x01, 119, 70,    0, "previous wall"},
    { 0x00, 119, 71,    0, "next wall"},
    { 0x00, 48, 129,    0, "set side&edge to 0"},
    { 0x05, 120, 34,    0, "align to neg. x-axis"},
    { 0x04, 120, 31,    0, "align to pos. x-axis"},
    { 0x05, 121, 35,    0, "align to neg. y-axis"},
    { 0x04, 121, 32,    0, "align to pos. y-axis"},
    { 0x05, 122, 36,    0, "align to neg. z-axis"},
    { 0x04, 122, 33,    0, "align to pos. z-axis"},
    { 0x01, 42, 76,     0, "increase rotation angle"},
    { 0x01, 47, 77,     0, "decrease rotation angle"},
    { 0x00, 42, 74,     0, "increase movefactor"},
    { 0x00, 47, 75,     0, "decrease movefactor"},
    { 0x01, 97, 78,     0, "increase visibility"},
    { 0x01, 122, 79,    0, "decrease visibility"},
    { 0x01, 121, 79,    0, "decrease visibility"},
    { 0x04, 282, 90,    0, "save pos 1"},
    { 0x04, 283, 91,    0, "save pos 2"},
    { 0x04, 284, 92,    0, "save pos 3"},
    { 0x04, 285, 93,    0, "save pos 4"},
    { 0x04, 286, 94,    0, "save pos 5"},
    { 0x04, 287, 95,    0, "save pos 6"},
    { 0x04, 288, 96,    0, "save pos 7"},
    { 0x04, 289, 97,    0, "save pos 8"},
    { 0x04, 290, 98,    0, "save pos 9"},
    { 0x04, 291, 99,    0, "save pos 10"},
    { 0x04, 292, 100,   0, "overview"},
    { 0x04, 293, 101,   0, "corridor overview"},
    { 0x01, 282, 102,   0, "goto pos 1"},
    { 0x01, 283, 103,   0, "goto pos 2"},
    { 0x01, 284, 104,   0, "goto pos 3"},
    { 0x01, 285, 105,   0, "goto pos 4"},
    { 0x01, 286, 106,   0, "goto pos 5"},
    { 0x01, 287, 107,   0, "goto pos 6"},
    { 0x01, 288, 108,   0, "goto pos 7"},
    { 0x01, 289, 109,   0, "goto pos 8"},
    { 0x01, 290, 110,   0, "goto pos 9"},
    { 0x01, 291, 111,   0, "goto pos 10"},
    { 0x01, 292, 112,   0, "back from overview"},
    { 0x01, 293, 113,   0, "back from cor. overview"},
    { 0x00, 121, 41,    0, "move backward"},
    { 0x00, 122, 41,    0, "move backward"},
    { 0x00, 257, 41,    0, "move backward"},
    { 0x00, 97, 43,     0, "move forward"},
    { 0x00, 259, 43,    0, "move forward"},
    { 0x01, 264, 48,    0, "turn down"},
    { 0x01, 260, 50,    0, "turn left"},
    { 0x01, 258, 51,    0, "turn up"},
    { 0x01, 262, 52,    0, "turn right"},
    { 0x00, 264, 42,    0, "move down"},
    { 0x00, 260, 44,    0, "move left"},
    { 0x00, 258, 45,    0, "move up"},
    { 0x00, 262, 46,    0, "move right"},
    { 0x01, 257, 47,    0, "bank left"},
    { 0x01, 259, 49,    0, "bank right "},
    { 0x00, 282, 39,    0, "hot keys"},
    { 0x00, 283, 60,    0, "fast save"},
    { 0x00, 284, 25,    0, "make room"},
    { 0x00, 285, 26,    0, "split cube"},
    { 0x00, 286, 23,    0, "all lines on/off"}

};

struct sdld_keychange_data {
    struct w_button ** buttons;
    struct sdld_hotkey ** hotkeys;
};



#endif	/* SDLCONFIG_HOTKEYS_H */

