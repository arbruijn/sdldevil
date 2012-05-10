/*  SDLDevil - Sourceport of Devil to libSDL
    Copyright (C) 2012  Florian Feucht (pyro@tagtraummusik.org) */

/*  DEVIL - Descent Editor for Vertices, Items and Levels at all
    Copyright (C) 1995  Achim Stremplat (ubdb@rz.uni-karlsruhe.de) 
    Further info see .c-files. */
void printmsg(const char *txt,...);
void waitmsg(const char *txt,...);
int yesnomsg(const char *txt,...);
int isfloat(int x);
int getfloat(const char *txt,const char *help,int length,int digits,
 float *input);
char *askstringlist(const char **strs,const char *defname,int nostrs,
 const char *title);
char *getfilename(char **path,const char *defname,const char *ext,
 const char *title,int overwrite_warning);
char *getdirname(char **path, const char *title);
void hogfilemanager(void);
