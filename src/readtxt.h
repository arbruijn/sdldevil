/*  SDLDevil - Sourceport of Devil to libSDL
    Copyright (C) 2012  Florian Feucht (pyro@tagtraummusik.org) */

/*  DEVIL - Descent Editor for Vertices, Items and Levels at all
    Copyright (C) 1995  Achim Stremplat (ubdb@rz.uni-karlsruhe.de) 
    Further info see .c-files. */
void inittxts(void);
int newpigfile(char *pigname,FILE *pogfile);
void readbitmap(unsigned char *dest,struct pig_txt *ptxt,struct ham_txt *txt,int dir);
int cmp_txts(const void *t1,const void *t2);
int readcustomtxts(FILE* pg1file);
