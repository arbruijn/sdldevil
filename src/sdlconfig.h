#ifndef SDLCONFIG_H
#define SDLCONFIG_H


struct sdld_config_data {
    struct sdld_hotkey ** hotkeys;
    int num_hotkeys;
    enum descent descent_version;
    char * d1_data_path;
    char * d1_mission_path;
    char * d1_binary_path;
    char * d2_data_path;
    char * d2_mission_path;
    char * d2_binary_path;
    int fullscreen;
    int keyrepeat;
    int screenres_x;
    int screenres_y;
    int restart_required;
};


void sdld_configdialog(void);


#endif
