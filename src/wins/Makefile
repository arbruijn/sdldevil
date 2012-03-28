OBJ=w_init.o w_event.o wi_buts.o wi_keys.o wi_winma.o wi_menu.o w_draw.o w_tools.o w_system.o w_list.o SDL_gfxPrimitives.o SDL_rotozoom.o

CFLAGS= -Wall -g -DGNU_C -DGO32 -c

wins: $(OBJ)

%.o: %.c
	gcc $(CFLAGS) -o $@ $<

w_system.o: w_system.c wins.h wins_int.h w_system.h w_list.h
	gcc -Wall -g -DGNU_C -c w_system.c -o w_system.o



.PHONY: clean
clean:
	rm -f $(OBJ)
	rm -f w_test


