/*  SDLDevil - Sourceport of Devil to libSDL
    sdlconfig.c - graphical configuration for the editor
    Copyright (C) 2012  Florian Feucht (pyro@tagtraummusik.org) 

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program (file COPYING); if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */



#include "structs.h"
#include "sdlconfig.h"
#include "wins/w_system.h"

#define MAX_PATH_LEN 255
#define ELEMENT_HEIGHT 21

void sdlconfigdialog(void)
{
	int i, no, dno, no_rows, xsize;
	struct w_window w, *ow;
	struct w_button *b[20];

	int screenmodes_count;
	char ** screenmodes = ws_getscreenmodes(&screenmodes_count);
//	char ** screenmodes = { "bla", "blubb"};
//	screenmodes_count = 2;

	char ** keys = { "move forward", "move backward", "do whatever", "dominate world" };
	int keys_count = 4;


	char d1datapath[MAX_PATH_LEN], d1missionpath[MAX_PATH_LEN], d1binarypath[MAX_PATH_LEN];
	char d2datapath[MAX_PATH_LEN], d2missionpath[MAX_PATH_LEN], d2binarypath[MAX_PATH_LEN];

	*d1datapath = 0;
	*d1missionpath = 0;
	*d1binarypath = 0;
	*d2datapath = 0;
	*d2missionpath = 0;
	*d2binarypath = 0;

	struct w_b_press b_ok, b_cancel, b_keymapup, b_keymapdown;

	// Fields for Descent Paths
	struct w_b_string b_d1datapath, b_d1missionpath, b_d1binarypath;
	struct w_b_string b_d2datapath, b_d2missionpath, b_d2binarypath;

	struct w_b_strlist b_keymap;

	struct w_b_string b_winxsize, b_winysize;
	struct w_b_choose b_descentversion, b_fullscreenresolution;

	struct w_b_switch b_keyrepeat, b_fullscreen;

	// init window
	w.xpos = w.ypos = -1;
	w.xsize = 400;
	w.ysize = w_ymaxwinsize() * 3 / 4;
	w.maxxsize = w.maxysize = -1;
	w.shrunk = 0;
	w.title = "SDLDevil configuration";
	w.buttons = wb_drag;
	w.refresh = wr_normal;
	w.refresh_routine = NULL;
	w.close_routine = NULL;
	w.click_routine = NULL;
	checkmem(ow = w_openwindow(&w));

	// init pith buttons
	b_d1datapath.allowed_char = isprint;
	b_d1datapath.str = d1datapath;
	b_d1datapath.max_length = MAX_PATH_LEN;
	b_d1datapath.l_char_entered = b_d1datapath.r_char_entered = b_d1datapath.l_string_entered = b_d1datapath.r_string_entered = NULL;

	b_d1missionpath = b_d1datapath;
	b_d1missionpath.str = d1missionpath;

	b_d1binarypath = b_d1datapath;
	b_d1binarypath.str = d1binarypath;

	b_d2datapath = b_d1datapath;
	b_d2datapath.str = d2datapath;

	b_d2missionpath = b_d1datapath;
	b_d2missionpath.str = d2missionpath;

	b_d2binarypath = b_d1datapath;
	b_d2binarypath.str = d2binarypath;

	
	b_fullscreen.l_routine = b_fullscreen.r_routine = NULL;
	
	
	b_fullscreenresolution.num_options = screenmodes_count;
	b_fullscreenresolution.options = screenmodes;
	b_fullscreenresolution.selected = 0;
	b_fullscreenresolution.d_xsize = 64;
	b_fullscreenresolution.select_lroutine = b_fullscreenresolution.select_rroutine = NULL;

	b_keymap.max_selected = 1;
	b_keymap.no_rows = keys_count;
	b_keymap.strings = keys;
	b_keymap.no_strings = keys_count;
	b_keymap.start_no = 0;
	b_keymap.l_string_selected = b_keymap.r_string_selected = NULL;

	i = 0;
	checkmem(b[0] = w_addstdbutton(ow, w_b_string, 0, i, w.xsize-2, -1, "Descent 1 data path      ", &b_d1datapath, 1));
	checkmem(b[1] = w_addstdbutton(ow, w_b_string, 0, i+=ELEMENT_HEIGHT, w.xsize-2, -1, "Descent 1 mission path   ", &b_d1missionpath, 1));
	checkmem(b[2] = w_addstdbutton(ow, w_b_string, 0, i+=ELEMENT_HEIGHT, w.xsize-2, -1, "Descent 1 executable path", &b_d1binarypath, 1));
	checkmem(b[3] = w_addstdbutton(ow, w_b_string, 0, i+=ELEMENT_HEIGHT, w.xsize-2, -1, "Descent 2 data path      ", &b_d2datapath, 1));
	checkmem(b[4] = w_addstdbutton(ow, w_b_string, 0, i+=ELEMENT_HEIGHT, w.xsize-2, -1, "Descent 2 mission path   ", &b_d2missionpath, 1));
	checkmem(b[5] = w_addstdbutton(ow, w_b_string, 0, i+=ELEMENT_HEIGHT, w.xsize-2, -1, "Descent 2 executable path", &b_d2binarypath, 1));
	checkmem(b[6] = w_addstdbutton(ow, w_b_switch, 200, i+=ELEMENT_HEIGHT, 198, -1, "Fullscreen", &b_fullscreen, 1));
	b[10] = w_addstdbutton(ow, w_b_list, 0, i, 200, -1, NULL, &b_keymap, 1);
	checkmem(b[7] = w_addstdbutton(ow, w_b_choose, 200, i+=ELEMENT_HEIGHT, 198, -1, "Screen res", &b_fullscreenresolution, 1));


	w_handleuser(1, &b[16], 1, &ow, 0, NULL, NULL);

	w_closewindow(ow);
}
