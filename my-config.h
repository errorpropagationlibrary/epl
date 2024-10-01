/*
   Copyright (C) 2000 .. 2024 Achim Bode

   This file is part of epl (error propagation library).

   epl is free software: you can redistribute it and/or modify it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE version 3
   as published by the Free Software Foundation.

   epl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU AFFERO GENERAL PUBLIC LICENSE for more details.

   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE along with epl. If not, see <https://www.gnu.org/licenses/>.
*/
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
