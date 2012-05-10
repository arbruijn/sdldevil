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

FILE * fileio_open_ignorecase(char * path, char * fixed_path, char * mode);

#endif	/* FILEIO_H */

