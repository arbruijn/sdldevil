/* 
 * File:   fileio.h
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

#ifndef FILEIO_H
#define	FILEIO_H

#include "structs.h"

typedef FILE fileio_file_t;

fileio_file_t * fileio_fopen_ignorecase(const char * path, char * fixed_path, const char * mode);
fileio_file_t * fileio_fopen(const char * path, const char * mode);

int fileio_fclose(fileio_file_t * f);
int fileio_fseek(fileio_file_t * f, long int offset, int whence);

size_t fileio_fread_char(char * dest, size_t length, fileio_file_t * f);
size_t fileio_fwrite_char(char * dest, size_t length, fileio_file_t * f);

size_t fileio_fread_int16(int16_t * dest, size_t length, fileio_file_t * f);
size_t fileio_fwrite_int16(int16_t * dest, size_t length, fileio_file_t * f);

size_t fileio_fread_int32(int32_t * dest, size_t length, fileio_file_t * f);
size_t fileio_fwrite_int32(int32_t * dest, size_t length, fileio_file_t * f);



#endif	/* FILEIO_H */

