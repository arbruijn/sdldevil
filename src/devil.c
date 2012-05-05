/*  SDLDevil - Sourceport of Devil to libSDL
    Copyright (C) 2012  Florian Feucht (pyro@tagtraummusik.org) */

/*  DEVIL - Descent Editor for Vertices, Items and Levels at all
    devil.c - main()
    Copyright (C) 1995  Achim Stremplat (ubdb@rz.uni-karlsruhe.de)

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

#include <stdlib.h>
#include "structs.h"
#include "userio.h"
#include "tools.h"
#include "grfx.h"
#include "readlvl.h"
#include "initio.h"
#include "tag.h"
#include "config.h"
#include "plottxt.h"
#include "version.h"
#include "do_event.h"
//#include "askcfg.h"
#include "sdlconfig.h"
#ifdef _WIN32
#include <windows.h>
#endif

const char *extnames[desc_number] ={"SDL", "RDL", "RDL", "SL2", "RL2", "RL2", "RL2"};
#ifdef GER
const char *ininames[desc_number] ={NULL, "d1r_g.ini", "d1r_g.ini", NULL, "d2r_g_10.ini", "d2r_g_11.ini",
    "d2r_g_11.ini"};
#else
const char *ininames[desc_number] ={NULL, "d1reg.ini", "d1reg.ini", NULL, "d2reg_10.ini", "d2reg_11.ini",
    "d2reg_11.ini"};
#endif
const char *vernames[desc_number] ={"Descent 1 V1.0 shareware", "Descent 1 V1.0 registered",
    "Descent 1 V1.4 (or higher) registered", "Descent 2 V1.0 shareware",
    "Descent 2 V1.0 registered", "Descent 2 V1.1 registered",
    "Descent 2 V1.2 (or higher) registered"};

struct viewdata view;
struct initdata init;
struct pigdata pig;
struct palette palettes[NUM_PALETTES];
struct leveldata *l;

void my_exit(void) {
    static int no_loop = 0;
    if (!no_loop) {
        no_loop = 1;
        fprintf(errf, "Severe bug. Trying to save current work...");
        savestatus(-1);
        fprintf(errf, "Done. Maybe you are a lucky guy.\n");
    }
    ws_textmode();
    releasetimer();
    // FFE TODO: make better way to report this on modern systems (message box)
    printf("Severe bug.\n");
    printf("This is the SDLDevil sourceport of Devil made by\n");
    printf("Florian \"v66r\" Feucht.\n");
    printf("This version is NOT maintained by Achim Stremplat or Lars\n");
    printf("Christensen. Please contact me (Florian) for reporting this\n");
    printf("bug.\n");
    printf("How to reach me:\n");
    printf("On official SDLDevil Forum in the DXX-Rebirth Forums:\n");
    printf("  http://www.dxx-rebirth.com\n");
    printf("Via E-Mail:\n");
    printf("  sdldevil@kr4d.de\n");
    //ws_waitforkey();
    exit(2);
}

const char *signame[5] = {"Unknown", "Floating point exception",
    "Illegal opcode", "Segment violation", "Terminate"};

void my_abort(int sigcode) {
    int x;
    switch (sigcode) {
        case SIGFPE:
            x = 1;
            break;
        case SIGILL:
            x = 2;
            break;
        case SIGSEGV:
            x = 3;
            break;
        case SIGTERM:
            x = 4;
            break;
        default:
            x = 0;
    }
    if (errf != NULL) {
        fprintf(errf, "Unexpected signal: %s\n", signame[x]);
        fflush(errf);
    } else
        printf("Unexpected signal: %s\n", signame[x]);
    if (x == 1)
        my_exit();
    else
        exit(2);
}

enum cmdline_params {
    clp_new, clp_notitle, clp_config,
    num_cmdlineparams
};
const char *cmdline_switches[num_cmdlineparams] = {"NEW", "NOTITLE", "CONFIG"};

const char *cmdline_txts[num_cmdlineparams] ={TXT_CMDSTARTNEW, TXT_CMDDONTSHOWTITLE, TXT_CMDCONFIG};

#ifdef _WIN32

// Win32 entry point
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    // generate UNIX like commandline parameters on windows
    char * cmdline = (char *)lpCmdLine;
    char * tp = NULL;
    char ** argv = NULL;

    int argc = 1;

    argv = (char **)MALLOC(sizeof(char *) * argc);
    argv[0] = NULL;     // binary name not needed

    tp = strtok(cmdline, " ");
    if (tp) {
        do {
            argv = (char **)REALLOC(argv, sizeof(char *) * (argc + 1));
            argv[argc] = (char *)MALLOC(strlen(tp) + 1);
            strcpy (argv[argc], tp);
            argc++;
        } while (tp = strtok(NULL, " "));
    }
    
#else

// UNIX entry point
int main(int argc, char *argv[]) {
    
#endif
    
    int i, j, title = 1;
    int with_cfg = 1, reconfig = 0;
    char buffer[128];
    char *load_file_name = NULL;
    
    signal(SIGFPE, my_abort);
    signal(SIGILL, my_abort);
    signal(SIGSEGV, my_abort);
    signal(SIGTERM, my_abort);
    
    //printf("SDLDevil %s\nCompiler: GNU GCC %s\nCompiled: %s %s\n", VERSION, SYS_COMPILER_NAME, __DATE__, __TIME__);
    /*
        if (sizeof(float) != 4 || sizeof(long int) != 8
            || sizeof(short int) != 2 || sizeof(int) != 4
            || sizeof(char) != 1) {
            printf("Wrong float/int size. Check your compiler flags. float: %u, long int: %u, short int: %u, int: %u, char: %u\n", sizeof(float), sizeof(long int), sizeof(short int), sizeof(int), sizeof(char));
            exit(2);
        }
     */
    errf = stdout;
    for (j = 1; j < argc; j++) {
        sscanf(argv[j], " %s", buffer);
        for (i = 0; i < strlen(buffer); i++)
            buffer[i] = toupper(buffer[i]);
        if (buffer[0] == '/') {
            for (i = 0; i < num_cmdlineparams; i++) {
                if (strcmp(&buffer[1], cmdline_switches[i]) == 0) {
                    switch (i) {
                        case clp_new:
                            with_cfg = 0;
                            break;
                        case clp_notitle:
                            //printf("Devil is sponsored by PC Player!\n");
                            title = 0;
                            break;
                        case clp_config:
                            reconfig = 1;
                            break;
                    }
                    break;
                }
            }
            if (i == num_cmdlineparams) {
                printf(TXT_CMDUNKNOWNPARAM, &buffer[1]);
                for (i = 0; i < num_cmdlineparams; i++)
                    printf("%s -- %s\n", cmdline_switches[i],
                        cmdline_txts[i]);
                exit(1);
            }
        } else {
            load_file_name = argv[j];
        }
    }
    
#ifdef _WIN32
    
    // free our "artificial" UNIX commandline params
    for (i=0; i<argc; i++) {
            if (argv[i])
                    FREE(argv[i]);
    }
    FREE(argv);

#endif
    initeditor(INIFILE, title);
    
    if (!readconfig() || reconfig) {
        // FFE init rudimentary graphics for initial configuration
        if (!w_initwins(640, 480, 32, 0, 0)) {
            printf(TXT_CANTINITWINS);
            my_exit();
        }        
        newpalette(palettes[0].palette);
        sdld_configdialog();
        exit(0);

    }
    
    initgrph(title);
    
    //ws_disablectrlc(); FFE no more needed
    
    // initialize current level pointer 
    l = NULL;
    
    if (with_cfg)
        readstatus(load_file_name);
    else
        printmsg(TXT_NOCFGFILE);
    /*
       if(strlen(load_file_name))
       {
       for(n=view.levels.head;n->next!=NULL;n=n->next)
       closelevel(n->d.lev,1);
       openlevel(load_file_name);
       }
     */

    w_handleuser(0, NULL, 0, NULL, view.num_keycodes, view.ec_keycodes, do_event);

    return 0;
}
