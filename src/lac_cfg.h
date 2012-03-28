/*  SDLDevil - Sourceport of Devil to libSDL
    Copyright (C) 2012  Florian Feucht (pyro@tagtraummusik.org) */

#ifndef LAC_CFG_H
#define LAC_CFG_H

extern int isF2SavingAll;
extern int isAlwaysSmoothing;
extern int theMinDeltaLight;
extern int theMaxLight;
extern int changeCubeEnabled;

void lac_read_cfg(void);

#endif /* LAC_CFG_H */
