/*  SDLDevil - Sourceport of Devil to libSDL
    Copyright (C) 2012  Florian Feucht (pyro@tagtraummusik.org) */

/*  DEVIL - Descent Editor for Vertices, Items and Levels at all
    Copyright (C) 1995  Achim Stremplat (ubdb@rz.uni-karlsruhe.de) 
    Further info see .c-files. */
void initgrph(int title);
void closegrph(void);
void initdescmap(void);
void changemovemode(enum movetypes mm);
void changecurrmode(enum tagtypes tt);
void changepigfile(char *palname);
void changepogfile(const char *pogfilename);
void newpalette(unsigned char *palette);
