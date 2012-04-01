OBJ=devil.o userio.o tools.o tag.o insert.o calctxt.o initio.o config.o \
 askcfg.o plot.o plottxt.o plotsys.o click.o savetool.o readlvl.o \
 readtxt.o do_event.o do_stat.o do_ins.o do_mod.o do_light.o do_move.o \
 do_tag.o do_side.o grfx.o do_opts.o opt_txt.o options.o macros.o title.o\
 lac_cfg.o

WINS_OBJ=wins/w_init.o wins/w_event.o wins/wi_buts.o wins/wi_keys.o wins/wi_winma.o wins/wi_menu.o wins/w_draw.o wins/w_tools.o wins/w_system.o wins/w_list.o wins/SDL_gfxPrimitives.o wins/SDL_rotozoom.o

LIBS=-lSDL
BINARY=sdldevil
CFLAGS=-Wall -g -DGNU_C -DGO32 -ffast-math -Wwrite-strings -I wins -I language


$(BINARY): $(WINS_OBJ) $(OBJ) 
	@gcc $(CFLAGS) -o $(BINARY) $(OBJ) $(WINS_OBJ) $(LIBS)
	@echo "LD $(BINARY)"


%.o: %.c
	@gcc $(CFLAGS) -c -o $@ $<
	@echo "CC $<"


.PHONY: clean
clean:
	rm -f $(OBJ) 
	rm -f $(WINS_OBJ)
	rm -f $(BINARY)	

