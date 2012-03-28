/*  DEVIL - Descent Editor for Vertices, Items and Levels at all
    Wins: A Grfx-Windows system for DOS.
    w_system.c - All non-ANSI-C routines
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

#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
// #include <grx20.h>
// #include <dos.h>
//#include <dir.h>
#include <unistd.h>
//#include <fnmatch.h>
#include <dirent.h>
// #include <sys/farptr.h>
#include <sys/stat.h>
#include "wins_int.h"
#include "w_tools.h"

#include <SDL/SDL.h>
#include "SDL_gfxPrimitives.h"
#include "devilfont.h"
//#include <SDL/SDL_gfx_5x7_fnt.h>


//#define DEFAULT_FONT "wins.fnt"

struct ws_internals {
    SDL_Surface *screen;
    //FLOSDL TODO: GrTextOption textopt;
    //FLOSDL TODO: GrPattern pat_box;
    int mousecounter;		/* displaymouse & erasemouse */
} ws_private = 
{
    NULL,
/*    {
    	NULL, {0}, {0}, GR_BYTE_TEXT, GR_TEXT_RIGHT, GR_ALIGN_LEFT, GR_ALIGN_TOP
    }, 
    {0}, */
    0
};

// FLOSDL Added: 
uint32_t ws_palette[256];

SDL_Surface * create_sdl_sw_surface(int width, int height)
{
	return SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32,0,0,0,0);
}

void ws_flip(int x, int y, int width, int height) 
{
    SDL_UpdateRect(ws_private.screen, x, y, width, height);
}


extern FILE *errf;

/* Initialize Grfx-Mode&Mouse.Try to use xres,yres,colors and font <fontname>.
   If you are successful, return a 1.
   If you can't initialize this mode, return a 0. */
int ws_initgrfx(int xres, int yres, int color_depth, const char *fontname)
{
	static unsigned char pixels[2];
/* unsigned char value; */
    

    //FLOSDL TODO: GrBitmap pat_bm = { 0, 2, pixels, 1, 0, 0 };

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(errf, "Can't init SDL: %s\n", SDL_GetError());
		return 0;
	}
	SDL_EnableUNICODE(1);

    /* 
    if ((ws_private.textopt.txo_font = GrLoadFont((char *) (fontname == NULL ? DEFAULT_FONT : fontname))) == NULL) {
	    fprintf(errf, "Can't find font %s\n", fontname);
	    return 0;
    }
    */
	gfxPrimitivesSetFont(gfxDevilFontdata, 6, 14);


	ws_private.screen = SDL_SetVideoMode(xres, yres, color_depth, SDL_SWSURFACE);

	SDL_WM_SetCaption("SDLDevil", "SDLDevil");


    if (ws_private.screen == NULL) {
        fprintf(errf, "Can't init SDL Surface: %s\n", SDL_GetError());
        return 0;
    }

    /* FLOSDL TODO: Unneeded?
    GrClearScreen(0);
    
    if (!GrMouseDetect()) {
	    fprintf(errf, "Can't find mouse\n");
    	return 0;
    }
    GrMouseEventMode(0);
    GrMouseInit();
    GrMouseSetColors(255, 0);
    GrMouseDisplayCursor();
    pixels[0] = 0xaa;
    pixels[1] = 0x55;
    ws_private.pat_box.gp_bitmap = pat_bm;
    */
/* value=_farpeekb(0x40,0x17);
 _farpokeb(0x40,0x17,value^0x20); */
    return 1;
}

/* Setting colors. */
/* FLOSDL DONE */
void ws_setcolor(int i, int r, int g, int b)
{
    ws_palette[i] = (r<<24) | (g << 16) | (b << 8) | 255;
    //GrFreeColor(i);
    //GrSetColor(i, r, g, b);
}

/* Reinit mouse colors (if palette is new) */
/* FLOSDL TODO: */
void ws_resetmousecolors(void)
{
	printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
    //GrMouseEraseCursor();
    //GrMouseSetColors(w_makecolor(255, 255, 255), w_makecolor(0, 0, 0));
    //GrMouseUpdateCursor();
    //GrMouseDisplayCursor();
}

/* Getting color. */
/* FLOSDL TODO */
void ws_getcolor(int i, int *r, int *g, int *b)
{
    //GrQueryColor(i, r, g, b)
	(*r) = (ws_palette[i] >> 24) & 0xff;
	(*g) = (ws_palette[i] >> 16) & 0xff;
	(*b) = (ws_palette[i] >> 8) & 0xff;
}

/* FFE just for those silly arrows.... */
struct ws_bitmap *ws_createbitmap8from(int xsize, int ysize, uint8_t *bm)
{
//	printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
    SDL_Surface *gc;
    struct ws_bitmap *bmap;
    // char *mem[4];
    int x, y;

    if ((bmap = MALLOC(sizeof(struct ws_bitmap))) == NULL)
    	return NULL;

	if ((gc = SDL_CreateRGBSurfaceFrom(bm, xsize, ysize, 8, xsize, 0, 0, 0, 0 )) == NULL) {
		FREE(bmap);
		return NULL;
	}

    bmap->bitmap = (void *) gc;
    bmap->xpos = 0;
    bmap->ypos = 0;
    bmap->xsize = xsize;
    bmap->ysize = ysize;
    bmap->w = NULL;
    return bmap;
}






/* Create bitmap with bm inside. */
struct ws_bitmap *ws_createbitmap(int xsize, int ysize, uint32_t **bm)
{
//	printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
    SDL_Surface *gc;
    struct ws_bitmap *bmap;
    // char *mem[4];
    int x, y;

// FLOSDL TODO:
//    if (xsize < 0 || ysize < 0 || xsize > GrSizeX() || ysize > GrSizeY())
//    	return NULL;
    if ((bmap = MALLOC(sizeof(struct ws_bitmap))) == NULL)
    	return NULL;
/*
    if ((gc = GrCreateContext(xsize, ysize, bm != NULL ? mem : NULL, NULL)) == NULL) {
        FREE(bmap);
        ireturn NULL;
    }
*/
	if ((gc = create_sdl_sw_surface(xsize, ysize)) == NULL) {
		FREE(bmap);
		return NULL;
	}
	if (bm != NULL) {
        	//mem[0] = bm;
	        //mem[1] = mem[2] = mem[3] = NULL;
		//(*bm) = gc->pixels;
		(*bm) = gc->pixels;
/*
		if (SDL_MUSTLOCK(gc)) {
			if (SDL_LockSurface(gc) < 0) {
				return (NULL);
			}
		}

		for (y=0; y<ysize; y++) {
			for (x=0; x<xsize; x++) {
				fastPixelColorNolock (gc, x, y, ws_palette[bm[y*ysize + x]], 0);
				printf (" === pixeldata: %u\n", bm[y*ysize+x]);
			}
		}
		if (SDL_MUSTLOCK(gc)) {
			SDL_UnlockSurface(gc);
		}
*/	
    	}

/*
    if (bm == NULL) {
        GrSetContext(gc);
        GrClearContext(notes.colindex[cv_winfill]);
        GrSetContext(NULL);
    }
*/
    bmap->bitmap = (void *) gc;
    bmap->xpos = 0;
    bmap->ypos = 0;
    bmap->xsize = xsize;
    bmap->ysize = ysize;
    bmap->w = NULL;
    return bmap;
}

/* get the pointer to the data of the bitmap. */
// FLOSDL TODO:
uint32_t *ws_getbitmapdata(struct ws_bitmap *b)
{
	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
    //return ((GrContext *) (b->bitmap))->gc_baseaddr[0];
	return ((SDL_Surface *)(b->bitmap))->pixels;
}

/* Clear the bitmap (fill it with color c) */
// FLOSDL TODO:
void ws_clearbitmap(struct ws_bitmap *b, int c)
{
	printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
/*
    GrSetContext((GrContext *) b->bitmap);
    GrClearContext(c);
    GrSetContext(NULL);
*/
}

/* Copy from bitmap src xpos,ypos bitmap with size xsize,ysize to bitmap
   dst at position x1,y1 (which must be large enough). bitmap=NULL means
   screen. if withbg=1 the notes.colindex[cv_winfill] color will be
   drawn otherwise this color is not drawn */
void ws_copybitmap(struct ws_bitmap *dst, int x1, int y1,
		   struct ws_bitmap *src, int xpos, int ypos, int xsize,
		   int ysize, int withbg)
{
	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);

	SDL_Rect rect_src;
	rect_src.x = xpos;
	rect_src.y = ypos;
	rect_src.w = xsize;
	rect_src.h = ysize;

	SDL_Rect rect_dst;
	rect_dst.x = x1;
	rect_dst.y = y1;
	rect_dst.w = 0;
	rect_dst.h = 0;

	SDL_Surface *srcSurface = src == NULL ? ws_private.screen : (SDL_Surface *)src->bitmap; 

	if (!withbg) {
		int ckr, ckg, ckb;
		ws_getcolor (notes.colindex[cv_winfill], &ckr, &ckg, &ckb);
		SDL_SetColorKey (srcSurface, SDL_SRCCOLORKEY, SDL_MapRGB(srcSurface->format, ckr, ckg, ckb));
	}

	if (SDL_BlitSurface(srcSurface, &rect_src, dst == NULL ? ws_private.screen : (SDL_Surface *)dst->bitmap, &rect_dst)) {
		printf("copy blit FAILED\n");
	}

	// if destination is main screen, update it
	if (dst == NULL) {
		SDL_UpdateRect (ws_private.screen, x1, y1, xsize, ysize);
	}

	if (!withbg) {
		int ckr, ckg, ckb;
		ws_getcolor (notes.colindex[cv_winfill], &ckr, &ckg, &ckb);
		SDL_SetColorKey (srcSurface, 0, 0);
	}

/*
    GrBitBlt(dst == NULL ? NULL : dst->bitmap, x1, y1,
	     src == NULL ? NULL : src->bitmap, xpos, ypos,
	     xpos + xsize - 1, ypos + ysize - 1,
	     withbg ? GrWRITE : GrIMAGE | notes.colindex[cv_winfill]);
*/
}

/* Copy area (x1,y1)-(x2,y2) from a bitmap (if bm==NULL from the screen)
   and create a structure ws_bitmap with the area in bitmap. Returns NULL if
   not successful, the pointer to the structure otherwise. */
struct ws_bitmap *ws_savebitmap(struct ws_bitmap *bm, int x1, int y1, int xsize, int ysize)
{

    SDL_Surface *gc;
    struct ws_bitmap *bmap;

	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
   /*
    if (xsize < 0 || ysize < 0 || y1 < 0 || x1 < 0
	|| x1 + xsize > GrSizeX() || y1 + ysize > GrSizeY())
	return NULL;
    */
    checkmem(bmap = MALLOC(sizeof(struct ws_bitmap)));
    checkmem(gc = create_sdl_sw_surface (xsize, ysize));// GrCreateContext(xsize, ysize, NULL, NULL));

    SDL_Rect rect_src;
    rect_src.x = x1;
    rect_src.y = y1;
    rect_src.w = xsize;
    rect_src.h = ysize;

    SDL_Rect rect_dst;
    rect_dst.x = 0;
    rect_dst.y = 0;
    rect_dst.w = xsize;
    rect_dst.h = ysize;

	if (SDL_BlitSurface(bm == NULL ? ws_private.screen : bm->bitmap, &rect_src, gc, NULL)) {
		printf("save blit FAILED\n");
	}



//GrBitBlt(gc, 0, 0, bm == NULL ? NULL : bm->bitmap, x1, y1, x1 + xsize - 1, y1 + ysize - 1, GrWRITE);

    //SDL_UpdateRect(gc, 0, 0, xsize, ysize);

    bmap->bitmap = (void *) gc;
    bmap->xpos = x1;
    bmap->ypos = y1;
    bmap->xsize = xsize;
    bmap->ysize = ysize;
    bmap->w = NULL;
    return bmap;

}

/* Free complete structure bm (with the structure itself) */
void ws_freebitmap(struct ws_bitmap *bm)
{
	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
	my_assert(bm);
	if (bm->bitmap)
		SDL_FreeSurface((SDL_Surface *)bm->bitmap);
	free (bm);
/*
    my_assert(bm);
    if (bm->bitmap)
	GrDestroyContext((GrContext *) bm->bitmap);
    free(bm);
*/
}

/* Restore the content of bitmap on the position saved in bitmap and
   kill bm. */
void ws_restorebitmap(struct ws_bitmap *bm)
{

	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
	my_assert(bm != NULL && bm->bitmap != NULL);

	SDL_Rect rect_src;
	rect_src.x = 0;
	rect_src.y = 0;
	rect_src.w = bm->xsize;
	rect_src.h = bm->ysize;

	SDL_Rect rect_dst;
	rect_dst.x = bm->xpos;
	rect_dst.y = bm->ypos;
	rect_dst.w = bm->xsize;
	rect_dst.h = bm->ysize;

	if (SDL_BlitSurface(bm->bitmap, &rect_src, ws_private.screen, &rect_dst)) {
		printf("restore blit FAILED\n");
	}
	
	SDL_UpdateRect (ws_private.screen, bm->xpos, bm->ypos, bm->xsize, bm->ysize);

	ws_freebitmap(bm);


/*
    my_assert(bm != NULL && bm->bitmap != NULL);
    GrBitBlt(NULL, bm->xpos, bm->ypos, (GrContext *) bm->bitmap, 0, 0,
	     bm->xsize - 1, bm->ysize - 1, GrWRITE);
    ws_freebitmap(bm);
*/
}

/* Draw a line with color c. */
void ws_drawline(int x1, int y1, int x2, int y2, int c, int xor)
{
	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
    //GrLine(x1, y1, x2, y2, c | (xor ? GrXOR : 0));
	int updx;
	int updy;
	int updw;
	int updh;

	lineColor (ws_private.screen, x1, y1, x2, y2, ws_palette[c], xor);


	if (x1 < 0) x1 = 0;
	if (y1 < 0) y1 = 0;
	
	if (x2 < 0) x2 = 0;
	if (y2 < 0) y2 = 0;

	if (x1 < x2) {
		updx = x1;
		updw = (x2-x1)+1;
	} else {
		updx = x2;
		updw = (x1-x2)+1;
	}

	if (y1 < y2) {
		updy = y1;
		updh = (y2-y1)+1;
	} else {
		updy = y2;
		updh = (y1-y2)+1;
	}

	

	SDL_UpdateRect(ws_private.screen, updx, updy, updw, updh);
}

/* Draw a circle with color c. */
void ws_drawcircle(int x, int y, int r, int c, int xor)
{
	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
    //GrCircle(x, y, r, c | (xor ? GrXOR : 0));
	int rx, ry, rw, rh;
	circleColor(ws_private.screen, x, y, r, ws_palette[c], xor);
	
	rx = x-r;
	ry = y-r;
	rw = rh = (r*2)+1;

	if (rx < 0)
		rx = 0;

	if (ry < 0) 
		ry = 0;


	SDL_UpdateRect(ws_private.screen, rx, ry, rw, rh);
	//printf ("x %i y %i w %i h %i\n", x-r, y-r, x+r, y+r);
}

/* Draw a line with color c to bitmap bm. */
void ws_bmdrawline8(struct ws_bitmap *bm, int x1, int y1, int x2, int y2,
		   int c, int xor)
{

	lineColor ((SDL_Surface *)bm->bitmap, x1, y1, x2, y2, c, xor);

}


void ws_bmdrawline(struct ws_bitmap *bm, int x1, int y1, int x2, int y2,
		   int c, int xor)
{

	lineColor ((SDL_Surface *)bm->bitmap, x1, y1, x2, y2, ws_palette[c], xor);

	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
	
/*
    GrSetContext((GrContext *) bm->bitmap);
    GrLine(x1, y1, x2, y2, c | (xor ? GrXOR : 0));
    GrSetContext(NULL);
*/
}

/* Draw a (not-filled) box with color c. */
void ws_drawbox(int x1, int y1, int xsize, int ysize, int c, int xor)
{
	if (xsize < 0) {
		x1 += xsize;
		xsize = -xsize;
	}
	if (ysize < 0) {
		y1 += ysize;
		ysize = -ysize;
	}

	if (xsize >0 && ysize >0) {
	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
		rectangleColor(ws_private.screen, x1, y1, (x1+xsize)-1, (y1+ysize)-1, ws_palette[c], xor);
    //GrBox(x1, y1, x1 + xsize - 1, y1 + ysize - 1, c | (xor ? GrXOR : 0));
		ws_flip(x1, y1, xsize, ysize);
	}
}

/* Draw a (filled) box with color c. */
void ws_drawfilledbox(int x1, int y1, int xsize, int ysize, int c, int xor)
{
	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
    //GrFilledBox(x1, y1, x1 + xsize - 1, y1 + ysize - 1,	c | (xor ? GrXOR : 0));
	boxColor(ws_private.screen, x1, y1, (x1+xsize)-1, (y1+ysize)-1, ws_palette[c], xor);
	ws_flip(x1, y1, xsize, ysize);

}

/* draw framed box, x,y left upper edge (xs,ys size) of box with frame.
   ltc = left, top color, rbc= right, bottom color of frame.
    inc=fill color. w = width of frame */
void ws_drawframedbox(int x, int y, int xs, int ys, int w, int ltc,
		      int rbc, int inc)
{
	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
	int i;
	for (i=0; i<w; i++) {
		vlineColor(ws_private.screen, x+i, y+i, (y+ys-1)-i, ws_palette[ltc], 0);
		vlineColor(ws_private.screen, (x+xs-1)-i, y+i, (y+ys-1)-i, ws_palette[rbc], 0);

		hlineColor(ws_private.screen, x+i, (x+xs-1)-i, y+i, ws_palette[ltc], 0);

		hlineColor(ws_private.screen, x+i, (x+xs-1)-i, (y+ys-1)-i, ws_palette[rbc], 0);
	}
	boxColor(ws_private.screen, x+w, y+w, (x+xs-w)-1, (y+ys-w)-1, ws_palette[inc], 0);

	ws_flip(x, y, xs, ys);

/*
    GrFBoxColors bc;
    bc.fbx_intcolor = inc;
    bc.fbx_topcolor = bc.fbx_leftcolor = ltc;
    bc.fbx_rightcolor = bc.fbx_bottomcolor = rbc;
    GrFramedBox(x + w, y + w, x + xs - 2 * w, y + ys - 2 * w, w, &bc);
*/
}

/* set clip box. All routines can only draw in this box. if x1==-1 the
 box is the whole screen */
// FLOSDL TODO
void ws_setclipping(int x1, int y1, int x2, int y2)
{
//	printf ("IMPLEMENT ME: %s\n", __FUNCTION__);

	//printf ("CLIPPING: %i,%i  %ix%i\n", x1, y1, x2, y2);
	
	SDL_Rect rect;
	if (x1 < 0) {
		SDL_SetClipRect(ws_private.screen, NULL);
	} else {
		rect.x = x1;
		rect.y = y1;
		rect.w = x2 - x1;
		rect.h = y2 - y1;
		SDL_SetClipRect(ws_private.screen, &rect);
		//rectangleColor(ws_private.screen, x1, y1, x2, y2, 0x00ff00ff, 0);
		//ws_flip(x1, y1, x2, y2);

	}	
}

/*
    if (x1 < 0) {
	GrResetClipBox();
    } else
	GrSetClipBox(x1, y1, x2, y2);
*/


/* String length in pixels */
// FLOSDL TODO
int ws_pixstrlen(const char *txt)
{
	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
//    return txt == NULL ? 0 : GrStringWidth((char *) txt, strlen(txt), &ws_private.textopt);
	if (txt == NULL)
		return 0;
	else
		return strlen(txt) * 6;
}

/* String length in chars which fit in w pixels (assuming all chars
   have an equal width) */
// FLOSDL TODO
int ws_charstrlen(int w)
{
	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
//    return w / GrCharWidth('M', &ws_private.textopt);
	return w / 6;
}

/* Draw text txt to x,y (left, upper edge). maximum pixel width is w.
   fg,bg=fore/background color. if bg=-1 background is transparent. */
// FLOSDL TODO
void ws_drawtext(int x, int y, int w, const char *txt, int fg, int bg)
{
	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
	char buffer[256];
	int clen = ws_charstrlen(w);
	if (clen > 255)
		clen = 255;

	strncpy(buffer, txt, clen);
	buffer[clen] = 0;
	if (strlen(buffer) == 0)
		return;

	
	int plen = ws_pixstrlen(buffer);
	stringColor(ws_private.screen, x, y, buffer, ws_palette[fg]);
	
	//printf("===TEXT: x=%i, y=%i, w=%i\n", x, y ,w);

	//rectangleColor(ws_private.screen, x, y, (x+plen), (y+14), 0x00ff00ff, 0);
	SDL_UpdateRect (ws_private.screen, x, y, plen, 16);

	//printf ("%s @ %i %i\n", txt, x, y);
	
	//SDL_UpdateRect (ws_private.screen, 0, 0, 0, 0);
	
/*
    char buffer[256];
    int clen = ws_charstrlen(w);
    my_assert(txt != NULL);
    if (clen > 255)
	clen = 255;
    strncpy(buffer, txt, clen);
    buffer[clen] = 0;
    if (strlen(buffer) == 0)
	return;
    ws_private.textopt.txo_bgcolor.v = bg == -1 ? GrOR : bg;
    ws_private.textopt.txo_fgcolor.v = fg;
    GrDrawString(buffer, strlen(buffer), x, y, &ws_private.textopt);
*/
}

// FLOSDL TODO
void ws_bmdrawtext(struct ws_bitmap *bm, int x, int y, int w,
		   const char *txt, int fg, int bg)
{
	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);

	int clen = ws_charstrlen(w);
	stringColor((SDL_Surface *)bm->bitmap, x, y, txt, ws_palette[fg]);

    /*
    char buffer[256];
    int clen = ws_charstrlen(w);
    my_assert(txt != NULL);
    if (clen > 255)
	clen = 255;
    strncpy(buffer, txt, clen);
    buffer[clen] = 0;
    if (strlen(buffer) == 0)
	return;
    ws_private.textopt.txo_bgcolor.v = bg == -1 ? GrOR : bg;
    ws_private.textopt.txo_fgcolor.v = fg;
    GrSetContext((GrContext *) bm->bitmap);
    GrDrawString(buffer, strlen(buffer), x, y, &ws_private.textopt);
    GrSetContext(NULL);
*/
}

/* Switch to normal textmode */
// FLOSDL TODO
void ws_textmode(void)
{
//	printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
	SDL_Quit ();
/*
    GrMouseUnInit();
    GrSetMode(GR_default_text);
*/
}

int altkeys_az[26] =
    { 286, 304, 302, 288, 274, 289, 290, 291, 279, 292, 293, 294,
    306, 305, 280, 281, 272, 275, 287, 276, 275, 303, 273, 301, 300, 277
};
int altkeys_09[10] = { 385, 376, 377, 378, 379, 380, 381, 382, 383, 384 };

/* get an event (key or mousebutton pressed/released).
   if wait>0 wait for an event. 
   if wait==0 don't wait for an event. if there's no event return 0.
   if wait<0 wait -wait/100 seconds for an event.
   Translate the event in DEVIL - format. See w_system.h.
   make an event for pressing&for releasing the mousebutton but only
   one for pressing a key. If the Ctrl or the Alt key is pressed you must
   return the normal code for the letter. This must work for the Ctrl for the
   letters and for the Alt key for the numbers+the letters. */
// FLOSDL TODO
int ws_getevent(struct ws_event *se, int wait)
{
    	SDL_Event me;
	Uint32 starttime;
	int got_event = 0;
	me.type = 0;

	if (wait) {
		if (wait < 0) {

			//SDL_Delay(100);
			starttime = SDL_GetTicks();
			//printf("start\n");

//				got_event = SDL_WaitEvent(&me);
			do {
				got_event = 0;		
				got_event = SDL_PollEvent(&me);
				if ((SDL_GetTicks() - starttime) > (-wait) * 10) {
					//printf("vorbei\n");
					got_event = 0;
					break;
				}
			} while ((me.type != SDL_KEYDOWN) && (me.type != SDL_MOUSEBUTTONDOWN) && (me.type != SDL_MOUSEBUTTONUP));
			if (got_event) { printf("gotevent\n"); }
		
		} else {
			do {
				got_event = SDL_WaitEvent(&me);
			} while ((me.type != SDL_KEYDOWN) && (me.type != SDL_MOUSEBUTTONDOWN) && (me.type != SDL_MOUSEBUTTONUP));
		}
	} else {
		got_event = SDL_PollEvent(&me);
	}

	se->buttons = ws_bt_none;
	se->flags = ws_f_none;
	se->key = 0;

	if (got_event) {
		switch(me.type) {

		case SDL_MOUSEBUTTONUP:	
		case SDL_MOUSEBUTTONDOWN:
			se->buttons = ws_bt_none;
			switch (me.button.button) {
			case SDL_BUTTON_LEFT:
				if (me.type == SDL_MOUSEBUTTONDOWN)
					se->buttons |= ws_bt_left;
				se->flags = ws_f_lbutton;
				break;

			case SDL_BUTTON_RIGHT:
				if (me.type == SDL_MOUSEBUTTONDOWN)
					se->buttons |= ws_bt_right;
				se->flags = ws_f_rbutton;
				break;
			}
		
			se->x = me.button.x;
			se->y = me.button.y;


			break;

		case SDL_KEYDOWN:
			se->key = me.key.keysym.sym;
			se->unicode = me.key.keysym.unicode;
			se->flags = ws_f_keypress;
			se->kbstat = 0;	

			if ( (me.key.keysym.mod & KMOD_LSHIFT) || (me.key.keysym.mod & KMOD_RSHIFT)) 
				se->kbstat |= 1;
			
			if ( (me.key.keysym.mod & KMOD_LALT) || (me.key.keysym.mod & KMOD_RALT)) 
				se->kbstat |= 2;
			
			if ( (me.key.keysym.mod & KMOD_LCTRL) || (me.key.keysym.mod & KMOD_RCTRL)) 
				se->kbstat |= 4;
			
			if (me.key.keysym.mod & KMOD_NUM) 
				se->kbstat |= 8;
			
			if (me.key.keysym.mod & KMOD_CAPS) 
				se->kbstat |= 16;
			
			break;

		case SDL_KEYUP:

		default:
			got_event = 0;
			break;			
		}

		
	} 

	Uint8 ms = SDL_GetMouseState(&se->x, &se->y);
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1)) 
		se->buttons |= ws_bt_left;

	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3)) 
		se->buttons |= ws_bt_right;

	se->kbstat = 0;
	Uint8 * keystate = SDL_GetKeyState(NULL);
	if (keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT]) 
		se->kbstat|=1;
	if (keystate[SDLK_LALT] || keystate[SDLK_RALT]) 
		se->kbstat|=2;
	if (keystate[SDLK_LCTRL] || keystate[SDLK_RCTRL]) 
		se->kbstat|=4;
	/*
	if (keystate[SDLK_NUM] || keystate[SDLK_RCTRL]) 
		se->kbstat|=8;
	if (keystate[SDLK_CAPS] || keystate[SDLK_RCTRL]) 
		se->kbstat|=16;
	*/


	/*
	if (got_event) {	
		printf ("event - flags=%i, key=%i, unicode=%i, kbstat=%i, x=%i, y=%i, buttons=%i, wait=%i\n", se->flags, se->key, se->unicode, se->kbstat, se->x, se->y, se->buttons, wait);
	}
	*/

	return got_event;

}

/* gives all filenames at 'path' with extension(s) 'ext' (the extensions
   must be seperated with a '.'). In no is the number
   of files found returned. If no==0 no file found, no==-1 path not found,
   no==-2 invalid path or extension. path="" or path=NULL or ext=NULL is not
   allowed. You must free filenames[i] and filenames. */

char **ws_getallfsentries (const char *path, const char *exts, int *no, int types) {
	int i;
	char **filenames = NULL;
	char *ext;
	char extbuffer[32];
	char pathbuffer[1024];

	if (path == NULL || strlen(path) == 0) {
		*no = -2;
        	return NULL;
	}

	DIR *dp;
	struct dirent *ep;
	struct stat fileinfo;

	dp = opendir(path);
	if (dp == NULL) {
		*no = -1;
		return NULL;
	}

	*no = 0;

	while (ep = readdir(dp)) {
		strcpy(pathbuffer, path);
		strcat(pathbuffer, "/");
		strcat(pathbuffer, ep->d_name);
		stat (pathbuffer, &fileinfo);
		//if (!(ep->d_type & types)) {
		if (!(fileinfo.st_mode & types)) {
			continue;
		}
		if (exts) {
			
			if (strncmp(exts, "*", 3)) {
				int foundext = 0;
				strncpy(extbuffer, exts, 31);
				ext = strtok (extbuffer, ".,");
				while (ext != NULL) {
					foundext = 1;
					for (i=0; i<strlen(ext); i++) {
						if (i >= strlen(ep->d_name))  // do not overflow the buffer
							break;
				
						if (tolower(ext[i]) != tolower(ep->d_name[strlen(ep->d_name) - strlen(ext) + i])) {
							foundext = 0;
							break;
						}
					
					}
					if (foundext) {
						break;
					}
					ext = strtok (NULL, ".,");
				}
				if (!foundext) {
					continue;
				}

			}
		} 

		(*no)++;
		if (filenames == NULL) {
			checkmem(filenames = MALLOC(sizeof(char *) * (*no)));
		} else {
			checkmem(filenames = REALLOC(filenames, sizeof(char *) * (*no)));
		}
		checkmem(filenames[*no - 1] = MALLOC((strlen(ep->d_name) + 1) * sizeof(char)));
		strcpy(filenames[*no - 1], ep->d_name);
	}

	closedir(dp);
	return filenames;

}


char **ws_getallfilenames(const char *path, const char *ext, int *no)
{
	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
	
	return ws_getallfsentries(path, ext, no, S_IFREG);

/*
    struct ffblk files;
    char buffer[255], real_path[255], **filenames, new_ext[4];
    const char *ext_pos, *old_pos = ext;
*/


/*
    strcpy(buffer, path);

    if (buffer[strlen(buffer) - 1] == '/' || buffer[strlen(buffer) - 1] == '\\')
        buffer[strlen(buffer) - 1] = 0;
    
    if (strlen(buffer) != 2 || buffer[1] != ':')
        if (findfirst(buffer, &files, FA_ARCH | FA_RDONLY | FA_DIREC) || (files.ff_attrib & FA_DIREC) == 0) {
            *no = -1;
            return NULL;
	    }

    *no = 0;
    filenames = NULL;
    strcpy(real_path, buffer);

    do {
        ext_pos = strchr(old_pos, '.');
        if (ext_pos == NULL)
            ext_pos = &old_pos[strlen(old_pos)];
        if (ext_pos - old_pos > 3)
            return filenames;

        strncpy(new_ext, old_pos, ext_pos - old_pos);
        new_ext[ext_pos - old_pos] = 0;
        old_pos = ext_pos + 1;
        strcpy(buffer, real_path);
//        strcat(buffer, "/*.");
        strcat(buffer, new_ext);
        if (!findfirst(buffer, &files, FA_ARCH | FA_RDONLY))
            do {
            checkmem(filenames = REALLOC(filenames, sizeof(char *) * (++*no)));
            checkmem(filenames[*no - 1] = MALLOC((strlen(files.ff_name) + 1) * sizeof(char)));
            strcpy(filenames[*no - 1], files.ff_name);
        }
	    while (!findnext(&files));
    }

    while (strlen(ext_pos) > 0);
    return filenames;
*/

}

/* gives all directories at 'path' with extension(s) 'ext' (the extensions
   must be seperated by '.'). In no is the number
   of dirs found returned. If no==0 no file found, no==-1 path not found,
   no==-2 invalid path or extension. path="" or path=NULL or ext=NULL is not
   allowed. You must free filenames[i] and filenames. */
// FLOSDL TODO
char **ws_getalldirs(const char *path, const char *ext, int *no)
{
	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
	return ws_getallfsentries(path, ext, no, S_IFDIR);
/*
    struct ffblk files;
    char buffer[255], real_path[255], **filenames, new_ext[4];
    const char *ext_pos, *old_pos = ext;
    if (path == NULL || strlen(path) == 0) {
	*no = -2;
	return NULL;
    }
    strcpy(buffer, path);
    if (buffer[strlen(buffer) - 1] == '/'
	|| buffer[strlen(buffer) - 1] == '\\')
	buffer[strlen(buffer) - 1] = 0;
    if (strlen(buffer) != 2 || buffer[1] != ':')
	if (findfirst(buffer, &files, FA_ARCH | FA_RDONLY | FA_DIREC) ||
	    (files.ff_attrib & FA_DIREC) == 0) {
	    *no = -1;
	    return NULL;
	}
    *no = 0;
    filenames = NULL;
    strcpy(real_path, buffer);
    do {
	ext_pos = strchr(old_pos, '.');
	if (ext_pos == NULL)
	    ext_pos = &old_pos[strlen(old_pos)];
	if (ext_pos - old_pos > 3)
	    return filenames;
	strncpy(new_ext, old_pos, ext_pos - old_pos);
	new_ext[ext_pos - old_pos] = 0;
	old_pos = ext_pos + 1;
	strcpy(buffer, real_path);
//	strcat(buffer, "/*.");
	strcat(buffer, new_ext);
	if (!findfirst(buffer, &files, FA_ARCH | FA_RDONLY | FA_DIREC))
	    do {
		if (files.ff_attrib & FA_DIREC) {
		    checkmem(filenames =
			     REALLOC(filenames, sizeof(char *) * (++*no)));
		    checkmem(filenames[*no - 1] =
			     MALLOC((strlen(files.ff_name) +
				     2) * sizeof(char)));
		    strcpy(filenames[*no - 1], files.ff_name);
		    filenames[*no - 1][strlen(files.ff_name)] = '/';
		    filenames[*no - 1][strlen(files.ff_name) + 1] = 0;
		}
	    }
	    while (!findnext(&files));
    }
    while (strlen(ext_pos) > 0);
    return filenames;
*/
}


// FFE quick n dirty replacement for djgpp's _fixpath
void ws_fixpath(char * in, char * out)
{

	char * part;
	out[0] = '\0';
	char buffer[1024];

	strncpy(buffer, in, 1023);

	// deal with unix path
	if (buffer[0] == '/')
		strcat(out, "/");
			
	part = strtok(buffer, "/");
	while (part != NULL) {
		if (!strcmp(part, "..")) {
			if (strchr(out, '/') != strrchr(out, '/')) {
				do {
					
out[strlen(out) - 1] = '\0';
				} while (out[strlen(out) - 1] != '/');
			}
		} else {
			strcat(out, part);
			strcat(out, "/");
		}

		part = strtok(NULL, "/");
	}

	// remove trailing / if not root directory
	if (strlen(out) > 1) 
		out[strlen(out) - 1] = '\0';

}


/* Make the most simple path from oldpath and store it in newpath.
   Therefore remove all '..' etc. */
// FLOSDL TODO
void ws_makepath(const char *oldpath, char *newpath)
{

    char *p, *hp, buffer[300];
    if (newpath != oldpath)
		p = hp = newpath;
    else
		p = hp = buffer;
    ws_fixpath(oldpath, hp);
    //strncpy(hp, oldpath, 300);
    if (strlen(hp) == 0) 
	strcat (hp, "/");

    while (*p != 0) {
		if (*p == '\\')
			*p = '/';
		//*p = toupper(*p);
		p++;
    }
    strcpy(newpath, hp);

}

/* Split fullpath in drive,path,name,ext. */
// FLOSDL TODO
void ws_splitpath(char *fullpath, char *drive, char *path, char *name,
		  char *ext)
{

	char buffer[256];
	char buffer2[1024];	
	char * pos;
	struct stat fileinfo;

	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
    //fnsplit(fullpath, drive, path, name, ext);

	path[0] = '\0';
	name[0] = '\0';
	ext[0] = '\0';
	drive[0] = '\0';
	
	//if (fullpath[strlen(fullpath)-1] != '/') {
		stat (fullpath, &fileinfo);
		if (!S_ISDIR(fileinfo.st_mode)) {
			strncpy(buffer2, fullpath, 1023);
			if (pos = strrchr(buffer2, '/')) {
				*pos = '\0';
				strncpy(buffer, pos + 1, 255);
				strcpy(path, buffer2);
			} else {
				strcpy(buffer, fullpath);
				path[0] = '\0';
			}
			if (pos = strrchr(buffer, '.')) {
				strcpy(ext, pos);
				*pos = '\0';
			}
			strcpy (name, buffer);
		} else {
			strcpy (path, fullpath);
		}
		
	//}
	
#ifdef _WIN32
	strcpy (buffer2, path);
	if (path[1] == ':') {
		strncpy(drive, buffer2,2);
		drive[2] = '\0';
		if (strlen(buffer2) > 2) {
			strcpy(path, &buffer2[2]);
		} else {
			path[0] ='\0';
		}
	}
	//printf("windoze - drive:%s,path:%s,file:%s,ext:%s\n", drive, path, name, ext);
#endif
}

// FFE fnmerge replacement
void ws_mergepath(char *fullpath, const char *drive, const char *path, const char *name, const char *ext) 
{
	fullpath[0] = '\0';
	if (drive != NULL)
		strcat(fullpath, drive);

	if (path != NULL) {
		if (path[0] != '/')
			strcat(fullpath, "/");

		strcat(fullpath, path);
	}

	if (name != NULL) {
		if (fullpath[strlen(fullpath)-1] != '/') 
			strcat(fullpath, "/");

		strcat(fullpath, name);

		if (ext != NULL)
			strcat(fullpath, ext);
	}
}


/* Test if wildcard string matches name (no path allowed, only the filename)*/
int ws_matchname(char *wildcard, char *name)
{
    return !fnmatch(wildcard, name, 0);
}

/* Disable control-c. (remember: Cube-menu) */
// FLOSDL TODO: UNNEEDED?
void ws_disablectrlc(void)
{
	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
//    setcbrk(0);
//    signal(SIGINT, SIG_IGN);
}

/* Plot a dot-non-dot-non filled Box */
// FLOSDL TODO
void ws_drawpatternedbox(int x1, int y1, int xsize, int ysize, int c)
{

	int x, y, m;
	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
/* This is not yet working in my release from grx 2.0:
 ws_private.pat_box.gp_bitmap.bmp_fgcolor=c|GrXOR;
 ws_private.pat_box.gp_bitmap.bmp_bgcolor=GrXOR;
 GrPatternFilledBox(x1,y1,x1+xsize,y1+ysize,&ws_private.pat_box); */

    //m = GrMouseBlock(NULL, x1, y1, x1 + xsize, y1 + ysize);
    for (y = y1; y < y1 + ysize; y++)
	for (x = x1 + (y & 1); x < x1 + xsize; x += 2)
		fastPixelColor(ws_private.screen, x, y, gfxWriteModeXOR | c);

	SDL_UpdateRect (ws_private.screen, x1, y1, xsize, ysize);
    //GrMouseUnBlock(m);

}

/* remove the mouse from the screen, so that the drawing is faster. */
void ws_erasemouse(void)
{
	
//    GrMouseEraseCursor();
    ws_private.mousecounter--;
	SDL_ShowCursor(0);
}

void ws_displaymouse(void)
{
	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);

    if (++ws_private.mousecounter >= 0) {
	ws_private.mousecounter = 0;
	//GrMouseDisplayCursor();
	SDL_ShowCursor(1);
    }

}

/* beams the mouse to x,y */
// FLOSDL TODO
void ws_mousewarp(int x, int y)
{
	//printf ("MOUSEWARP: %s\n", __FUNCTION__);
	SDL_WarpMouse(x, y);
	//    GrMouseWarp(x, y);
}

// FLOSDL TODO
int ws_waitforkey(void)
{
	printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
	//getchar();
}

// FLOSDL TODO
int ws_testforkey(void)
{
	printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
	//return getchar();
}

// FLOSDL unneeded 
void ws_setdriver(const char *name)
{
	//printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
//    GrSetDriver((char *) name);
}

/* init cursor structure (which is whatever you want). w=width, h=height,
 xo,yo=hot spot, colortable maps colors in data to colors on screen. */
// FLOSDL TODO
ws_cursor *ws_initcursor(char *data, int w, int h, int xo, int yo,
			 long *colortable)
{
	printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
//    return (ws_cursor *) GrBuildCursor(data, w, w, h, xo, yo, colortable);
}

/* set cursor (cursor==NULL is allowed) */
// FLOSDL TODO
void ws_changecursor(ws_cursor * cursor)
{

	printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
	// ==> SDL_CreateCursor ... 
//    if (cursor) {
//	GrMouseEraseCursor();	/* GrMouseSetCursor((GrCursor *)cursor); */
//	GrMouseDisplayCursor();
//    } else
//	ws_resetmousecolors();
}

/* kill cursor */
// FLOSDL TODO
void ws_killcursor(ws_cursor * cursor)
{
//    GrDestroyCursor((GrCursor *) cursor);
	printf ("IMPLEMENT ME: %s\n", __FUNCTION__);
}




// translate pixels from palette to native format
void ws_translatepixels (unsigned char* src, uint32_t* dst, int length)
{
	int i, r, g, b;
	for (i=0; i<length; i++) {
		ws_getcolor(src[i], &r, &g, &b);
		dst[i] = SDL_MapRGB(ws_private.screen->format, r, g, b);
	}
}

// fetch native pixelformat color for indexed color
uint32_t ws_getNativeColor(unsigned char i)
{
	int r, g, b;
	ws_getcolor(i, &r, &g, &b);
	return SDL_MapRGB(ws_private.screen->format, r, g, b);
}

// translate rgb to native pixelformat
uint32_t ws_getNativeColorRGB(int r, int g, int b)
{
	return SDL_MapRGB(ws_private.screen->format, r, g, b);
}

// give the rest of our timeslice to the os
void ws_sleep(void)
{
	SDL_Delay(0);
}


