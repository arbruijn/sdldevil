/*  SDLDevil - Sourceport of Devil to libSDL
    sdlconfig.c - graphical configuration for the editor (replacement
    for askcfg)
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
#include "sdlconfig_hotkeys.h"
#include "wins/wins.h"
#include "wins/w_system.h"
#include "userio.h"
#include "version.h"


#define SDLD_MAX_PATH_LEN 255
#define SDLD_ELEMENT_HEIGHT 21

// move lists up / down
void sdld_list_updown(struct w_button **b, int bno, int add)
{
    struct w_b_strlist *bsls = b[bno]->d.sls;
    if (add < -1 || add > 1)
	bsls->start_no += bsls->no_rows / add;
    else
	bsls->start_no += add;
    if (bsls->start_no > bsls->no_strings - bsls->no_rows)
	bsls->start_no = bsls->no_strings - bsls->no_rows;
    if (bsls->start_no < 0)
	bsls->start_no = 0;
    w_drawbutton(b[bno]);
}


// callbacks for buttons which scroll the keymap list
void sdld_keymap_one_up(struct w_button * b) { 
    sdld_list_updown(b->data, 10, -1);
}
void sdld_keymap_one_down(struct w_button * b) { 
    sdld_list_updown(b->data, 10, 1);
}
void sdld_keymap_page_up(struct w_button * b) { 
    sdld_list_updown(b->data, 10, -2);
}
void sdld_keymap_page_down(struct w_button * b) { 
    sdld_list_updown(b->data, 10, 2);
}


// calculate the rank of a key ... the more modifiers, the higher the rank
void sdld_set_rank (struct sdld_hotkey * hotkey) {
    int i;
    hotkey->rank = 0;
    for (i=0; i<5; i++) {
        if (hotkey->kbstat & (1<<i)) 
            (hotkey->rank)++;
    }
}

// generate human readable string for key and modifiers
void sdld_generate_hotkeystring(char * dest, const int key, const int modifiers) {
    int i;
    char * keyname;
    for (i=0; i<SDLD_NUM_KBSTATS; i++) {
        if (modifiers & sdld_kbstats[i].kbstat) {
            strcat(dest, sdld_kbstats[i].txt);
            strcat(dest, "+");
        }
    }
    keyname = ws_getkeyname(key);
    strcat(dest, keyname);
}

// redefine key mapping for given key
void sdld_redefine_key(struct sdld_hotkey * hotkey) {
    int gotevent;
    struct ws_event evt;
    
    evt.buttons = 0;    
    evt.key = 0;
    evt.kbstat = 0;
    
    printmsg("Please press button(s) to map '%s' to\nor mouseclick to cancel", hotkey->txt);
    
    while (1) {
        evt.flags = 0;
                
        gotevent = ws_getevent(&evt, 1);

        // trash mousebutton up event(s) from button click
        if (((evt.flags == ws_f_lbutton) || (evt.flags == ws_f_rbutton)) && !evt.buttons) 
            continue;
        
        // mouseclick cancels
        if (evt.buttons) {
            break;
        }
        
        if (gotevent) {
            hotkey->key = evt.key;
            hotkey->kbstat = evt.kbstat;
            sdld_set_rank(hotkey);
            break;
        }
            
    }
    
}



// callback for change key butten
void sdld_keymap_change(struct w_button * b) {
    int i;
    struct sdld_keychange_data * keychange_data = b->data;
    char buffer[64];
    
    struct w_b_strlist * list =  keychange_data->buttons[10]->d.sls;
    for (i=0; i<list->no_strings; i++) {
        if (list->sel_arr[i]) {
            sdld_redefine_key(keychange_data->hotkeys[i]);
            *buffer = 0;
            sdld_generate_hotkeystring(buffer, keychange_data->hotkeys[i]->key, keychange_data->hotkeys[i]->kbstat);
            strncpy (&(list->strings[i][34]), buffer, 24);
            w_drawbutton(keychange_data->buttons[10]);
            break;
        }
    }
    if (i >= list->no_strings)
        printmsg("Please select a button you want to change first");
    
}


// callback for screen resolution dropdown
void sdld_screenres_change(struct w_button * b) {
    char * p;
    char * s;    
    struct w_b_choose * btn = b->d.ls;
    struct sdld_config_data * config_data = b->data;
    s = MALLOC(sizeof(char) * (strlen(btn->options[btn->selected]) + 1));
    strcpy(s, btn->options[btn->selected]);
    p = strtok(s, "x");
    config_data->screenres_x = atoi(p);
    p = strtok(NULL, "x");
    config_data->screenres_y = atoi(p);
    FREE(s);
}

// callback for descent version dropdown
void sdld_descentversion_change(struct w_button * b) {
    struct w_b_choose * btn = b->d.ls;
    struct sdld_config_data * config_data = b->data;
    config_data->descent_version = sdld_descentversions[btn->selected].descent_version;
}

// callback for fullscreen toggle button
void sdld_fullscreen_change(struct w_button * b) {
    struct w_b_switch * btn = b->d.s;
    struct sdld_config_data * config_data = b->data;
    config_data->fullscreen = btn->on;
}

/* sort comparator for qsort - sort sdld_hotkeys by their rank - highest first
 * aka descending ;-) 
 */
int sdld_qs_comphotkeyrank(const void * a, const void * b) {
    return (*(struct sdld_hotkey **)b)->rank - (*(struct sdld_hotkey **)a)->rank;
}

/* Check if the user selected the right descent version
 * quite important since wrong selections would crash the editor
 */
int sdld_check_descentversion(struct sdld_config_data * config_data) {
    
    enum descent actual_version;
    uint32_t d;
    char buffer[SDLD_MAX_PATH_LEN+1];
    FILE * f;
    
    *buffer = 0;
    
    if (config_data->descent_version >= d2_10_sw) {
        // descent 2 selected
        // TODO: proper version checks
    } else {
        // descent 1 selected, do the pigfile check like original devil did
        
        strcat(buffer, config_data->d1_data_path);
        strcat(buffer, "/");
        strcat(buffer, "descent.pig");
        
        f = fopen(buffer, "rb");
        if (f == NULL) {
            waitmsg("No Descent 1 found in path %s.\n Please make sure that you selected the right directory where descent.pig resides in", config_data->d1_data_path);
            return 0;
        }
        
        my_assert(fread(&d, sizeof (uint32_t), 1, f) == 1);
        fclose(f);
        if (d >= 0x10000)  
            actual_version = d1_14_reg;
        else
            actual_version = d1_10_reg;
        
        if (actual_version != config_data->descent_version) {
            if (actual_version == d1_10_reg) {
                waitmsg("You selected Descent v1.4+, but the data is from Descent 1.0.\nPlease select the right Descent version or update your Descent installation (the latter is strongly recommonded)");
                return 0;
            } else {
                waitmsg("You selected Descent v1.0, but the data is from Descent 1.4+.\nPlease select the right Descent version.");
                return 0;
            }
        }
    }
    
    return 1;
}


// write the config file
int sdld_write_config(struct sdld_config_data * config_data)  {
    
    int i;
    FILE * f;
    
    if ((f = fopen(init.cfgname, "w")) == NULL) {
        printmsg("ERROR opening file: %s", init.cfgname);
        return 0;
    }
    
    fprintf(f, "%s\n", VERSION);

    /* save path information - NOTE: this differs from original devil:
     * not every single version has one path, but 3 individual paths
     * for data, missions and binary (executable) for each Descent 1 and 
     * Descent 2 
     */
    fprintf(f, ":DESCENTPATHS 6\n", VERSION);
    fprintf(f, "{%s}\n", config_data->d1_data_path);
    fprintf(f, "{%s}\n", config_data->d1_mission_path);
    fprintf(f, "{%s}\n", config_data->d1_binary_path);
    fprintf(f, "{%s}\n", config_data->d2_data_path);
    fprintf(f, "{%s}\n", config_data->d2_mission_path);
    fprintf(f, "{%s}\n", config_data->d2_binary_path);
    
    // the ham file - hardcoded like in original askcfg
    fprintf(f, "{descent2.ham}\n");
    
    // other parameters
    fprintf(f, ":DESCENTVERSION %i\n", config_data->descent_version);
    fprintf(f, ":FULLSCREEN %i\n", config_data->fullscreen);
    fprintf(f, ":SCREENRES_X %i\n", config_data->screenres_x);
    fprintf(f, ":SCREENRES_Y %i\n", config_data->screenres_y);
    fprintf(f, ":KEYREPEAT %i\n", config_data->keyrepeat);
    
    // hotkeys configuration
    fprintf(f, ":KEYS %i\n", config_data->num_hotkeys);
    qsort(config_data->hotkeys, config_data->num_hotkeys, sizeof(struct sdld_hotkey *), sdld_qs_comphotkeyrank);
    for (i=0; i<config_data->num_hotkeys; i++) {
        fprintf (f, "%i %i %i {%s}\n", 
                config_data->hotkeys[i]->kbstat, 
                config_data->hotkeys[i]->key, 
                config_data->hotkeys[i]->event, 
                config_data->hotkeys[i]->txt);
    }
    
    config_data->restart_required = 1;
    fclose(f);
    
    return 1;
    
}

// callback for save button
void sdld_save_click(struct w_button * b) {
    struct sdld_config_data * config_data = b->data;
    if (sdld_check_descentversion(config_data)) {
        sdld_write_config(config_data);
    }
}


void sdld_browse_file(struct w_button * b) {
    struct w_button * btn = b->data;
    
    char * path = MALLOC(strlen(btn->d.str->str) + 1);
    char * file = MALLOC(256);
    //strcpy(path, btn->d.str->str);
    
    ws_splitpath(btn->d.str->str, NULL, path, file, NULL);
    
    char * res;
    res = getfilename(&path, file, NULL, "select file", 0);
    
    if (res){
        strncpy(btn->d.str->str, res, SDLD_MAX_PATH_LEN);
        FREE(res);
    }
    
    FREE(path);
    
    w_drawbutton(btn);
}

void sdld_browse_dir(struct w_button * b) {
    struct w_button * btn = b->data;
    getdirname(&btn->d.str->str, "select directory");
    w_drawbutton(btn);
}





// show sdldevil config dialog
void sdld_configdialog(void) {

    int i, j, key, kbstat, event, dlg_result;
    struct w_window w, *ow;
    struct w_button *b[26];

    int screenmodes_count;
    char ** screenmodes = ws_getscreenmodes(&screenmodes_count);
    int current_screenmode = 0;
    char buffer[64];

    char ** descentversions;
    int current_descentversion = 0;
    
    char ** keys;
    char * keys_sel_array;
    int num_keys;

    struct sdld_hotkey ** new_hotkeys;
    struct sdld_keychange_data * keychange_data;
    keychange_data = MALLOC(sizeof(struct sdld_keychange_data));

    char d1datapath[SDLD_MAX_PATH_LEN+1], d1missionpath[SDLD_MAX_PATH_LEN+1], d1binarypath[SDLD_MAX_PATH_LEN+1];
    char d2datapath[SDLD_MAX_PATH_LEN+1], d2missionpath[SDLD_MAX_PATH_LEN+1], d2binarypath[SDLD_MAX_PATH_LEN+1];

    *d1datapath = 0;
    *d1missionpath = 0;
    *d1binarypath = 0;
    *d2datapath = 0;
    *d2missionpath = 0;
    *d2binarypath = 0;

    struct w_b_press b_save, b_cancel, b_keymapup, b_keymapdown, b_keymapchange;

    // Fields for Descent Paths
    struct w_b_string b_d1datapath, b_d1missionpath, b_d1binarypath;
    struct w_b_string b_d2datapath, b_d2missionpath, b_d2binarypath;
    
    struct w_b_press b_d1datapath_browse, b_d1missionpath_browse, b_d1binarypath_browse;
    struct w_b_press b_d2datapath_browse, b_d2missionpath_browse, b_d2binarypath_browse;

    struct w_b_strlist b_keymap;

    //struct w_b_string b_winxsize, b_winysize;
    struct w_b_choose b_descentversion, b_fullscreenresolution;

    struct w_b_switch b_keyrepeat, b_fullscreen;

    struct sdld_config_data * config_data = MALLOC(sizeof(struct sdld_config_data));
    
    // find current screenmode
    sprintf(buffer, "%ix%i", init.xres, init.yres);
    for (i=0; i<screenmodes_count; i++) {
        if (!strcmp(buffer, screenmodes[i])) {
            current_screenmode = i;
            break;
        }
    }
    
    
    // load existing paths if any
    if (init.descentpaths[0])
        strncpy(d1datapath, init.descentpaths[0], SDLD_MAX_PATH_LEN);
    
    if (init.descentpaths[1])
        strncpy(d1missionpath, init.descentpaths[1], SDLD_MAX_PATH_LEN);
    
    if (init.descentpaths[2])
        strncpy(d1binarypath, init.descentpaths[2], SDLD_MAX_PATH_LEN);
    
    if (init.descentpaths[3])
        strncpy(d2datapath, init.descentpaths[3], SDLD_MAX_PATH_LEN);
    
    if (init.descentpaths[4])
        strncpy(d2missionpath, init.descentpaths[4], SDLD_MAX_PATH_LEN);
    
    if (init.descentpaths[5])
        strncpy(d2binarypath, init.descentpaths[5], SDLD_MAX_PATH_LEN);

    
    
    // init hotkeys config
    num_keys = view.ec_keycodes == NULL ? SDLD_NUM_HOTKEYS : view.num_keycodes;

    keys = MALLOC(sizeof(char*) * num_keys);
    keys_sel_array = MALLOC(sizeof(char) * num_keys);
    new_hotkeys = MALLOC(sizeof(struct sdld_hotkey *) * num_keys);

    for (i=0; i<num_keys; i++) {
        keys[i] = MALLOC(sizeof(char) * 64);
        new_hotkeys[i] = MALLOC(sizeof(struct sdld_hotkey));

        if (view.ec_keycodes == NULL) {
            strncpy(keys[i], sdld_hotkeys[i].txt, 32);    
            new_hotkeys[i]->txt = sdld_hotkeys[i].txt;
        } else {
            strncpy(keys[i], view.txt_keycode[i], 32); 
            new_hotkeys[i]->txt = view.txt_keycode[i];
        }

        while (strlen(keys[i]) < 34)
            strcat(keys[i], " ");

        /* in initial config there might be no keycodes defined..
         * use the defaults then 
         */
        if (view.ec_keycodes == NULL) {
            key = sdld_hotkeys[i].key;
            kbstat = sdld_hotkeys[i].kbstat;
            event = sdld_hotkeys[i].event;
        } else {
            key = view.ec_keycodes[i].key;
            kbstat = view.ec_keycodes[i].kbstat;
            event = view.ec_keycodes[i].event;
        }

        new_hotkeys[i]->key = key;
        new_hotkeys[i]->kbstat = kbstat;
        new_hotkeys[i]->event = event;

        sdld_set_rank(new_hotkeys[i]);

        *buffer = 0;

        sdld_generate_hotkeystring(buffer, key, kbstat);
        strncpy (&(keys[i][34]), buffer, 24);

        keys_sel_array[i] = 0;
    }


    keychange_data->buttons = b;
    keychange_data->hotkeys = new_hotkeys;

    // init Descent versions
    descentversions = MALLOC(sizeof(char*) * SDLD_NUM_DESCENTVERSIONS);
    for (i=0 ; i<SDLD_NUM_DESCENTVERSIONS; i++) {
        descentversions[i] = MALLOC(sizeof(char) * 64);
        strncpy(descentversions[i], sdld_descentversions[i].txt, 63);
        // find current descent version
        if (sdld_descentversions[i].descent_version == init.d_ver) 
            current_descentversion = i;
    }

    config_data->fullscreen = init.fullscreen;
    config_data->keyrepeat = init.keyrepeat;
    config_data->screenres_x = init.xres;
    config_data->screenres_y = init.yres;
    config_data->d1_data_path = d1datapath;    
    config_data->d1_mission_path = d1missionpath;
    config_data->d1_binary_path = d1binarypath;
    config_data->d2_data_path = d2datapath;
    config_data->d2_mission_path = d2missionpath;
    config_data->d2_binary_path = d2binarypath;
    config_data->hotkeys = new_hotkeys;
    config_data->num_hotkeys = num_keys;
    config_data->descent_version = sdld_descentversions[current_descentversion].descent_version;
    config_data->restart_required = 0;

    // init window
    w.xpos = w.ypos = -1;
    w.xsize = 600;
    w.ysize = w_ymaxwinsize() * 3 / 3.5;
    w.maxxsize = w.maxysize = -1;
    w.shrunk = 0;
    w.title = "SDLDevil configuration";
    w.buttons = wb_drag;
    w.refresh = wr_normal;
    w.refresh_routine = NULL;
    w.close_routine = NULL;
    w.click_routine = NULL;
    checkmem(ow = w_openwindow(&w));

    // init path buttons
    b_d1datapath.allowed_char = isprint;
    b_d1datapath.str = d1datapath;
    b_d1datapath.max_length = SDLD_MAX_PATH_LEN;
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


    b_keymapup.delay = b_keymapdown.delay = 40;
    b_keymapup.repeat = b_keymapdown.repeat = 15;

    b_keymapup.l_pressed_routine = b_keymapup.l_routine = sdld_keymap_one_up;
    b_keymapup.r_pressed_routine = b_keymapup.r_routine = sdld_keymap_page_up;

    b_keymapdown.l_pressed_routine = b_keymapdown.l_routine = sdld_keymap_one_down;
    b_keymapdown.r_pressed_routine = b_keymapdown.r_routine = sdld_keymap_page_down;

    b_keymapchange.l_pressed_routine = b_keymapchange.r_pressed_routine = b_keymapchange.l_routine = b_keymapchange.r_routine = sdld_keymap_change;


    b_cancel.l_pressed_routine = b_cancel.r_pressed_routine = b_cancel.l_routine = b_cancel.r_routine = NULL;
    b_save.l_pressed_routine = b_cancel.r_pressed_routine = NULL;
    b_save.l_routine = b_save.r_routine = sdld_save_click;
    
    // browsebuttons
    b_d1binarypath_browse = b_d2binarypath_browse = b_save;

    b_d1binarypath_browse.l_routine = b_d1binarypath_browse.r_routine = sdld_browse_file;
    b_d2binarypath_browse.l_routine = b_d2binarypath_browse.r_routine = sdld_browse_file;

    b_d1datapath_browse = b_d1binarypath_browse;
    b_d1datapath_browse.l_routine = b_d1datapath_browse.r_routine = sdld_browse_dir;
    b_d2datapath_browse = b_d1missionpath_browse = b_d2missionpath_browse = b_d1datapath_browse;
    
    b_fullscreen.l_routine = b_fullscreen.r_routine = sdld_fullscreen_change;
    b_fullscreen.on = init.fullscreen;
    if (b_fullscreen.on > 1)
        b_fullscreen.on = 1;


    b_fullscreenresolution.num_options = screenmodes_count;
    b_fullscreenresolution.options = screenmodes;
    b_fullscreenresolution.selected = current_screenmode;
    b_fullscreenresolution.d_xsize = 64;
    b_fullscreenresolution.select_lroutine = b_fullscreenresolution.select_rroutine = sdld_screenres_change;

    b_descentversion.num_options = SDLD_NUM_DESCENTVERSIONS;
    b_descentversion.options = descentversions;
    b_descentversion.selected = current_descentversion;
    b_descentversion.d_xsize = 128;
    b_descentversion.select_lroutine = b_descentversion.select_rroutine = sdld_descentversion_change;

    
    b_keymap.max_selected = 1;
    b_keymap.no_strings = SDLD_NUM_HOTKEYS;
    b_keymap.strings = keys;
    b_keymap.sel_arr = keys_sel_array;
    b_keymap.no_rows = 16;
    b_keymap.start_no = 0;
    b_keymap.l_string_selected = b_keymap.r_string_selected = NULL;

    // create the buttons on the window
    i = 0;
    checkmem(b[0] = w_addstdbutton(ow, w_b_string, 0, i, w.xsize-18, -1, "Descent 1 data path      ", &b_d1datapath, 1));
    checkmem(b[20] = w_addstdbutton(ow, w_b_press,w.xsize-18, i, 16, SDLD_ELEMENT_HEIGHT, "...", &b_d1datapath_browse, 1));
    b[20]->data = b[0];
    
    checkmem(b[1] = w_addstdbutton(ow, w_b_string, 0, i+=SDLD_ELEMENT_HEIGHT, w.xsize-18, -1, "Descent 1 mission path   ", &b_d1missionpath, 1));
    checkmem(b[21] = w_addstdbutton(ow, w_b_press,w.xsize-18, i, 16, SDLD_ELEMENT_HEIGHT, "...", &b_d1missionpath_browse, 1));
    b[21]->data = b[1];
    
    
    checkmem(b[2] = w_addstdbutton(ow, w_b_string, 0, i+=SDLD_ELEMENT_HEIGHT, w.xsize-18, -1, "Descent 1 executable path", &b_d1binarypath, 1));
    checkmem(b[22] = w_addstdbutton(ow, w_b_press,w.xsize-18, i, 16, SDLD_ELEMENT_HEIGHT, "...", &b_d1binarypath_browse, 1));
    b[22]->data = b[2];
    
    
    checkmem(b[3] = w_addstdbutton(ow, w_b_string, 0, i+=SDLD_ELEMENT_HEIGHT, w.xsize-18, -1, "Descent 2 data path      ", &b_d2datapath, 1));
    checkmem(b[23] = w_addstdbutton(ow, w_b_press,w.xsize-18, i, 16, SDLD_ELEMENT_HEIGHT, "...", &b_d2datapath_browse, 1));
    b[23]->data = b[3];
    
    checkmem(b[4] = w_addstdbutton(ow, w_b_string, 0, i+=SDLD_ELEMENT_HEIGHT, w.xsize-18, -1, "Descent 2 mission path   ", &b_d2missionpath, 1));
    checkmem(b[24] = w_addstdbutton(ow, w_b_press,w.xsize-18, i, 16, SDLD_ELEMENT_HEIGHT, "...", &b_d2missionpath_browse, 1));
    b[24]->data = b[4];

    
    checkmem(b[5] = w_addstdbutton(ow, w_b_string, 0, i+=SDLD_ELEMENT_HEIGHT, w.xsize-18, -1, "Descent 2 executable path", &b_d2binarypath, 1));
    checkmem(b[25] = w_addstdbutton(ow, w_b_press,w.xsize-18, i, 16, SDLD_ELEMENT_HEIGHT, "...", &b_d2binarypath_browse, 1));
    b[25]->data = b[5];
    

    w_drawbuttonbox(ow, 0, i+SDLD_ELEMENT_HEIGHT, w.xsize-2, w.ysize-i-SDLD_ELEMENT_HEIGHT*2+4);
    checkmem(b[8] = w_addstdbutton(ow, w_b_choose, 0, i+=SDLD_ELEMENT_HEIGHT, w.xsize-2, -1,  "Descent version used     ", &b_descentversion, 1));
    checkmem(b[6] = w_addstdbutton(ow, w_b_switch, 0, i+=SDLD_ELEMENT_HEIGHT, 200, SDLD_ELEMENT_HEIGHT, "Fullscreen", &b_fullscreen, 1));
    checkmem(b[7] = w_addstdbutton(ow, w_b_choose, 200, i, 198, -1, "screen resolution", &b_fullscreenresolution, 1));

    b[6]->data = b[7]->data = b[8]->data = config_data;

    
    int no_rows = (w_ywininsize(ow) - (i )) / (w_titlebarheight() + 2) - 4;
    b_keymap.no_rows = no_rows;
    checkmem(b[10] = w_addstdbutton(ow, w_b_list, 0, i+SDLD_ELEMENT_HEIGHT, w.xsize-2, no_rows * (w_titlebarheight() + 2), NULL, &b_keymap, 1)); 
    i += no_rows * (w_titlebarheight() + 2) + SDLD_ELEMENT_HEIGHT;
    checkmem(b[11] = w_addstdbutton(ow, w_b_press, 0, i, 64, -1, "/\\", &b_keymapup, 1));
    checkmem(b[12] = w_addstdbutton(ow, w_b_press, 64, i, 64, -1, "\\/", &b_keymapdown, 1));
    checkmem(b[13] = w_addstdbutton(ow, w_b_press, 128, i, 64, -1, "change", &b_keymapchange, 1));
    b[11]->data = b[12]->data = b;
    b[13]->data = keychange_data;

    checkmem(b[16] = w_addstdbutton(ow, w_b_press, w.xsize / 2 - 70, w.ysize - 36, 64, -1, "Cancel", &b_cancel, 1));
    checkmem(b[17] = w_addstdbutton(ow, w_b_press, w.xsize / 2 + 14, w.ysize - 36, 64, -1, "Save", &b_save, 1));
    b[17]->data = config_data;
    
    // run dialog
    while(1) {
        dlg_result = w_handleuser(2, &b[16], 1, &ow, 0, NULL, NULL);
        if (dlg_result == 0) {
            // cancel clicked
            break;
        } else {
            // save clicked, if restart_required is not set, something went wrong... stay in the dialog then
            if (config_data->restart_required) 
                break;
        }
    }

    w_closewindow(ow);
    
    for (i=0 ; i<SDLD_NUM_DESCENTVERSIONS; i++)
        FREE(descentversions[i]);

    FREE(descentversions);

    for (i=0; i<SDLD_NUM_HOTKEYS; i++) {
        FREE(keys[i]);
        FREE(new_hotkeys[i]);
    }

    FREE(keys);

    FREE(keys_sel_array);
    FREE(new_hotkeys);
    FREE(keychange_data);
    
    if (config_data->restart_required) {
        if (init.lastname)
                remove(init.lastname);
    
        waitmsg("Successfully saved configuration, exiting SDLDevil\nRestart SDLDevil for the changes to take effect");
        ws_textmode();
        
        FREE(config_data);
        exit(0);

    } else {
        FREE(config_data);
    }
    
}
