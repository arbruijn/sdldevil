BASE_OBJ=devil.o userio.o tools.o tag.o insert.o calctxt.o initio.o config.o \
 plot.o plottxt.o plotsys.o click.o savetool.o readlvl.o \
 readtxt.o do_event.o do_stat.o do_ins.o do_mod.o do_light.o do_move.o \
 do_tag.o do_side.o grfx.o do_opts.o opt_txt.o options.o macros.o title.o\
 lac_cfg.o sdlconfig.o ignorecase.o fileio.o

WINS_OBJ=wins/w_init.o wins/w_event.o wins/wi_buts.o wins/wi_keys.o \
 wins/wi_winma.o wins/wi_menu.o wins/w_draw.o wins/w_tools.o wins/w_system.o \
 wins/w_list.o wins/SDL_gfxPrimitives.o wins/SDL_rotozoom.o

BASE_LIBS=-lSDL -lm
CFLAGS=-Wall -g -pedantic -std=gnu99 -ffast-math -Wwrite-strings -I wins -I language
CFLAGS+=-Wno-address-of-packed-member -Werror-implicit-function-declaration

WINDOWS_OBJ=res/sdldevil_icon.res


ifdef SYSTEMDRIVE
    # Windows build
    BINARY = sdldevil.exe
    LIBS   = $(BASE_LIBS) -liberty
    OBJ    = $(WINS_OBJ) $(BASE_OBJ) $(WINDOWS_OBJ)
else
    # UNIX build
    BINARY = sdldevil
    LIBS   = $(BASE_LIBS)
    OBJ	   = $(WINS_OBJ) $(BASE_OBJ)
endif


$(BINARY): $(OBJ) 
	@gcc $(CFLAGS) -o $(BINARY) $(OBJ) $(LIBS)
	@echo "LD $(BINARY)"

%.o: %.c
	@gcc $(CFLAGS) -c -o $@ $<
	@echo "CC $<"
	
%.res: %.rc
	@windres -o $@ $< -O coff
	@echo "RES $<"


.PHONY: clean
clean:
	rm -f $(OBJ) 
	rm -f $(WINS_OBJ)
	rm -f $(BINARY)	

