/* my-config.h from epl, error propagation library.
   Copyright (C) 2000, 2001 Achim Bode
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */
#ifndef MY_CONFIG_H
#define MY_CONFIG_H

// iff defined the printing the correlation matrix includes
// unused row/columns.
#define BARECORRMAT

//used only in class matrix
#define NDEBUG

//gcc seems to have problems with nonmemberoperators like +

// used by ppoe: err.cpp
#define HAS_NO_PPOE
#define ASPRINT 0
#define SNPRINT 1
//#define PPOE_INTERNAL_PRINT SNPRINT
#define PPOE_INTERNAL_PRINT SNPRINT

//Define only if applies :)
//#define EPL_DEVELOPMENT
#endif
