/* 
   Copyright (C) 2000 .. 2024 Achim Bode

   This file is part of epl (error propagation library).

   epl is free software: you can redistribute it and/or modify it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE version 3
   as published by the Free Software Foundation.

   epl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU AFFERO GENERAL PUBLIC LICENSE for more details.

   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE along with epl. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef ERRLOW_H
#define ERRLOW_H
#include <stdio.h>
/*
 * Internal formatting and low level internal formatting states
 */
enum styletypes {FORMPM=0, FORMPA};
enum expfixtypes{EXP=0, FIXED, MIXED};
enum intfortypes{MWE=0, MWF, PWE, PWF};

/* 
 * Precision of Error + EPRCDIFF = convential precision in format 
 * Used in errC.cpp to calculate the errw from precision and in the
 * fallback of zero error in errlow.c.
 */
#define EPRECDIFF 4 

/*
 * Figure if a interface is choosen and do it if not.
 */
#ifndef   PRINT
#define   PRINT -1
#endif
#ifndef ASPRINT
#define ASPRINT  0
#endif
#ifndef SNPRINT
#define SNPRINT  1
#endif
#ifndef PPOE_INTERNAL_PRINT
#define PPOE_INTERNAL_PRINT ASPRINT
#endif

/* For security (buffer overflow) we do not implement this function. */
#if PPOE_INTERNAL_PRINT == PRINT
void print_err(char*t,               /*format string*/
               double const v,       /*value*/
               double const e,       /*error of value*/
               int    const style,   /* +- or () format*/
               int    const expfix,  /* scientific/fixed or mixed*/
               int    const magicmix,/* used for mixed*/
               int    const errw,    /* precision of error*/
               int    const mfw,     /* minimum field width, set to 2*/
               int    const E,       /* char for exponent indication*/
               int    const pos,     /* always a + sign? either 0 or 1*/     
               double      *scale_v, /*scaled value for printing*/
               double      *scale_e  /*scaled error for printing*/
  );
#elif PPOE_INTERNAL_PRINT == ASPRINT
void asprint_err(char**t,               /*format string*/
		 double const v,       /*value*/
		 double const e,       /*error of value*/
		 int    const style,   /* +- or () format*/
		 int    const expfix,  /* scientific/fixed or mixed*/
		 int    const magicmix,/* used for mixed*/
		 int    const errw,    /* precision of error*/
		 int    const mfw,     /* minimum field width, set to 2*/
		 int    const E,       /* char for exponent indication*/
		 int    const pos,     /* always a + sign? either 0 or 1*/     
		 double      *scale_v, /*scaled value for printing*/
		 double      *scale_e  /*scaled error for printing*/
  );
#elif PPOE_INTERNAL_PRINT == SNPRINT
void snprint_err(char*t,               /*format string*/
		 int n,                /*lenght of format string*/
		 double const v,       /*value*/
		 double const e,       /*error of value*/
		 int    const style,   /* +- or () format*/
		 int    const expfix,  /* scientific/fixed or mixed*/
		 int    const magicmix,/* used for mixed*/
		 int    const errw,    /* precision of error*/
		 int    const mfw,     /* minimum field width, set to 2*/
		 int    const E,       /* char for exponent indication*/
		 int    const pos,     /* always a + sign? either 0 or 1*/     
		 double      *scale_v, /*scaled value for printing*/
		 double      *scale_e  /*scaled error for printing*/
  );
#else
#error "PPOE_INTERNAL_PRINT" is not defined properly in "errlow.h"
#endif
#endif
