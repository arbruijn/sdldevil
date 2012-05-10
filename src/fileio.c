/* 
 * File:   fileio.c
 * abstraction of file input/output, mostly for portability - endianess
 * conversion and so on is done here.
 * Author: Florian Feucht
 * Copyright (C) 2012 Florian Feucht
 * Created on 10. Mai 2012, 17:50
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.    
 * This program is distributed in the hope that it will be useful,   
 * but WITHOUT ANY WARRANTY; without even the implied warranty of    
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    
 * GNU General Public License for more details.    
 * You should have received a copy of the GNU General Public License    
 * along with this program (file COPYING); if not, write to the Free Software    
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ignorecase.h"


FILE * fileio_open_ignorecase(char * path, char * fixed_path, char * mode) {
    
    char local_fixed_path[1025];
    char * newpath = fixed_path == NULL ? local_fixed_path : fixed_path;
    strncpy(newpath, path, 1024);
    
    if (icase_locate_one_element(newpath)) {
        return fopen(newpath, mode);
    } else {
        return NULL;
    }
}

size_t fileio_read_uint16(uint16_t * dest, FILE * f) {
    return fread (dest, sizeof(uint16_t), 1, f);
}
size_t fileio_read_int16(int16_t * dest, FILE * f) {
    return fread (dest, sizeof(int16_t), 1, f);
}