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
#include "structs.h"
#include "fileio.h"



fileio_file_t * fileio_fopen_ignorecase(const char * path, char * fixed_path, const char * mode) {
    
    char local_fixed_path[1025];
    char * newpath = fixed_path == NULL ? local_fixed_path : fixed_path;
    strncpy(newpath, path, 1024);
    
    if (icase_locate_one_element(newpath)) {
        return fopen(newpath, mode);
    } else {
        return NULL;
    }
}

fileio_file_t * fileio_fopen(const char * path, const char * mode) {
    return fopen(path, mode);
}

int fileio_fclose(fileio_file_t * f) {
    return fclose(f);
}

int fileio_fseek(fileio_file_t * f, long int offset, int whence) {
    return fseek(f, offset, whence);
}

int fileio_fgetc(fileio_file_t * f) {
    return fgetc(f);
}

size_t fileio_fread_char(char * dest, size_t length, fileio_file_t * f) {
    return fread (dest, sizeof(char), length, f);
}
size_t fileio_fwrite_char(char * dest, size_t length, fileio_file_t * f) {
    return fwrite (dest, sizeof(char), length, f);
}
size_t fileio_fread_uchar(unsigned char * dest, size_t length, fileio_file_t * f) {
    return fread (dest, sizeof(unsigned char), length, f);
}
size_t fileio_fwrite_uchar(unsigned char * dest, size_t length, fileio_file_t * f) {
    return fwrite (dest, sizeof(unsigned char), length, f);
}


size_t fileio_fread_int16(int16_t * dest, size_t length, fileio_file_t * f) {
    return fread (dest, sizeof(int16_t), length, f);
}
size_t fileio_fwrite_int16(int16_t * dest, size_t length, fileio_file_t * f) {
    return fwrite (dest, sizeof(int16_t), length, f);
}
size_t fileio_fread_uint16(uint16_t * dest, size_t length, fileio_file_t * f) {
    return fread (dest, sizeof(uint16_t), length, f);
}
size_t fileio_fwrite_uint16(uint16_t * dest, size_t length, fileio_file_t * f) {
    return fwrite (dest, sizeof(uint16_t), length, f);
}


size_t fileio_fread_int32(int32_t * dest, size_t length, fileio_file_t * f) {
    return fread (dest, sizeof(int32_t), length, f);
}
size_t fileio_fwrite_int32(int32_t * dest, size_t length, fileio_file_t * f) {
    return fwrite (dest, sizeof(int32_t), length, f);
}
size_t fileio_fread_uint32(uint32_t * dest, size_t length, fileio_file_t * f) {
    return fread (dest, sizeof(uint32_t), length, f);
}
size_t fileio_fwrite_uint32(uint32_t * dest, size_t length, fileio_file_t * f) {
    return fwrite (dest, sizeof(uint32_t), length, f);
}

int fileio_fread_d1minedata(struct D1_minedata * dest, fileio_file_t * f) {
    if (fileio_fread_uchar(&dest->version, 1, f) != 1)
        return 0;
    
    if (fileio_fread_uint16(&dest->numpts, 1, f) != 1)
        return 0;
    
    if (fileio_fread_uint16(&dest->numcubes, 1, f) != 1)
        return 0;
    
    return 1;
}

int fileio_fwrite_d1minedata(struct D1_minedata * dest, fileio_file_t * f) {
    if (fileio_fwrite_uchar(&dest->version, 1, f) != 1)
        return 0;
    
    if (fileio_fwrite_uint16(&dest->numpts, 1, f) != 1)
        return 0;
    
    if (fileio_fwrite_uint16(&dest->numcubes, 1, f) != 1)
        return 0;
    
    return 1;
}


int fileio_fread_d1gamedata(struct D1_gamedata * dest, fileio_file_t *f) {
    if (!fileio_fread_uint16(&dest->signature, 1, f)) 
        return 0;
    
    if (!fileio_fread_uint16(&dest->version, 1, f)) 
        return 0;
    
    if (!fileio_fread_int32(&dest->sizedir, 1, f))
        return 0;
    
    if (fileio_fread_char(&dest->mine_filename, 15, f) != 15)
        return 0;
    
    if (!fileio_fread_int32(&dest->level, 1, f))
        return 0;
    
    if (!fileio_fread_uint32(&dest->posplayer, 1, f))
        return 0;
    if (!fileio_fread_uint32(&dest->sizeplayer, 1, f))
        return 0;
    
    if (!fileio_fread_uint32(&dest->posthings, 1, f))
        return 0;
    if (!fileio_fread_uint32(&dest->numthings, 1, f))
        return 0;
    if (!fileio_fread_uint32(&dest->sizethings, 1, f))
        return 0;
    
    if (!fileio_fread_uint32(&dest->posdoors, 1, f))
        return 0;    
    if (!fileio_fread_uint32(&dest->numdoors, 1, f))
        return 0;
    if (!fileio_fread_uint32(&dest->sizedoors, 1, f))
        return 0;
    
    if (!fileio_fread_uint32(&dest->posopendoors, 1, f))
        return 0;
    if (!fileio_fread_uint32(&dest->numopendoors, 1, f))
        return 0;
    if (!fileio_fread_uint32(&dest->sizeopendoors, 1, f))
        return 0;
    
    if (!fileio_fread_uint32(&dest->possdoors, 1, f))
        return 0;
    if (!fileio_fread_uint32(&dest->numsdoors, 1, f))
        return 0;
    if (!fileio_fread_uint32(&dest->sizesdoors, 1, f))
        return 0;

    if (fileio_fread_uint32(&dest->stuff2, 3, f) != 3)
        return 0;    
    
    if (!fileio_fread_uint32(&dest->posedoors, 1, f))
        return 0;
    if (!fileio_fread_uint32(&dest->numedoors, 1, f))
        return 0;
    if (!fileio_fread_uint32(&dest->sizeedoors, 1, f))
        return 0;
    
    if (!fileio_fread_uint32(&dest->posproducer, 1, f))
        return 0;
    if (!fileio_fread_uint32(&dest->numproducer, 1, f))
        return 0;
    if (!fileio_fread_uint32(&dest->sizeproducer, 1, f))
        return 0;

    return 1;
}


int fileio_fwrite_d1gamedata(struct D1_gamedata * dest, fileio_file_t *f) {
    if (!fileio_fwrite_uint16(&dest->signature, 1, f)) 
        return 0;
    
    if (!fileio_fwrite_uint16(&dest->version, 1, f)) 
        return 0;
    
    if (!fileio_fwrite_int32(&dest->sizedir, 1, f))
        return 0;
    
    if (fileio_fwrite_char(&dest->mine_filename, 15, f) != 15)
        return 0;
    
    if (!fileio_fwrite_int32(&dest->level, 1, f))
        return 0;
    
    if (!fileio_fwrite_uint32(&dest->posplayer, 1, f))
        return 0;
    if (!fileio_fwrite_uint32(&dest->sizeplayer, 1, f))
        return 0;
    
    if (!fileio_fwrite_uint32(&dest->posthings, 1, f))
        return 0;
    if (!fileio_fwrite_uint32(&dest->numthings, 1, f))
        return 0;
    if (!fileio_fwrite_uint32(&dest->sizethings, 1, f))
        return 0;
    
    if (!fileio_fwrite_uint32(&dest->posdoors, 1, f))
        return 0;    
    if (!fileio_fwrite_uint32(&dest->numdoors, 1, f))
        return 0;
    if (!fileio_fwrite_uint32(&dest->sizedoors, 1, f))
        return 0;
    
    if (!fileio_fwrite_uint32(&dest->posopendoors, 1, f))
        return 0;
    if (!fileio_fwrite_uint32(&dest->numopendoors, 1, f))
        return 0;
    if (!fileio_fwrite_uint32(&dest->sizeopendoors, 1, f))
        return 0;
    
    if (!fileio_fwrite_uint32(&dest->possdoors, 1, f))
        return 0;
    if (!fileio_fwrite_uint32(&dest->numsdoors, 1, f))
        return 0;
    if (!fileio_fwrite_uint32(&dest->sizesdoors, 1, f))
        return 0;

    if (fileio_fwrite_uint32(&dest->stuff2, 3, f) != 3)
        return 0;    
    
    if (!fileio_fwrite_uint32(&dest->posedoors, 1, f))
        return 0;
    if (!fileio_fwrite_uint32(&dest->numedoors, 1, f))
        return 0;
    if (!fileio_fwrite_uint32(&dest->sizeedoors, 1, f))
        return 0;
    
    if (!fileio_fwrite_uint32(&dest->posproducer, 1, f))
        return 0;
    if (!fileio_fwrite_uint32(&dest->numproducer, 1, f))
        return 0;
    if (!fileio_fwrite_uint32(&dest->sizeproducer, 1, f))
        return 0;

    return 1;
}