/* 
   Copyright (C) 2000 .. 2024 Achim Bode

   This file is part of epl (error propagation library).

   epl is free software: you can redistribute it and/or modify it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE version 3
   as published by the Free Software Foundation.

   epl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU AFFERO GENERAL PUBLIC LICENSE for more details.

   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE along with epl. If not, see <https://www.gnu.org/licenses/>.
*/

#include "config.h"
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include "errlow.h"
/* Hm, I thought that's defined in errno.h ... */
#ifndef errno
extern int errno;
#endif

#ifdef HAVE_ASSERT_H
#define __USE_GNU
#include <assert.h>
#undef __USE_GNU
#ifndef assert_perror
#define assert_perror(A) assert(!A) 
#define __ASSERT_FUNCTION ""
#endif
#else  /* not having assert */
#define assert_perror(A)
#define __ASSERT_FUNCTION ""
#endif /* not having assert */
#define internalerror(A) {fprintf(stderr,"%s:%i:%s Internal error: %s\n",\
            __FILE__,__LINE__,__ASSERT_FUNCTION,A); exit( 1 );}

void print_err_int(char*fmt,  /*string to generate format string*/
		   int *fmvalues, /*the five int's to generate it */
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
	       ){ 

  /* Still some work to do for rounding the error ALWAYS up.
   * Do we want a dynamic errw? like 
   *     122(10)e+02 
   *     122(59)e+02 
   *      12(6)e+03
   *      12(9)e+03
   */

  /* Define low level formatting variables */

  int dbdp_v;     /* position of first nonzero digit of v. */
  int dbdp_e;     /* position of first nonzero digit of e. */
  int f;          /* Internal formatting state */
  char*p="+\0";    

  /* Checking the arguments */	

  if(style!=FORMPM&&style!=FORMPA)
    {
      fprintf(stderr,"In %s at %i invalid argument style= %i\n",
	      __FILE__,__LINE__,style);
      exit( 1 );
    }

  if(expfix!=EXP&&expfix!=FIXED&&expfix!=MIXED)
    {
      fprintf(stderr,"In %s at %i invalid argument expfix= %i\n",
	      __FILE__,__LINE__,expfix);
      exit ( 1 );
    }

  if(magicmix<0)
    {
      fprintf(stderr,"In %s at %i invalid argument magicmix= %i\n",
	      __FILE__,__LINE__,magicmix);
      exit ( 1 );
    }

  if(errw<1)
    {
      fprintf(stderr,"In %s at %i invalid argument errw= %i\n",
	      __FILE__,__LINE__,errw);
      exit ( 1 );
    }

  if(mfw<0||mfw>10)
    {
      fprintf(stderr,"In %s at %i invalid argument mfw= %i\n",
	      __FILE__,__LINE__,mfw);
      exit ( 1 );
    }

  if(!isalpha(E))
    {
      fprintf(stderr,"In %s at %i invalid argument E= %c\n",
	      __FILE__,__LINE__,E);
      exit ( 1 );
    }

  if(pos!=0&&pos!=1)
    {
      fprintf(stderr,"In %s at %i invalid argument pos= %i\n",
	      __FILE__,__LINE__,pos);
      exit ( 1 );
    }

  /* Missing check:
   * There is no way to check if fmt is large enough. Therefore
   * use the asprint_err or snprint_err interface. These 
   * interfaces allocate enough space for fmt.
   */

  if(pos==0) p="\0";

  if(e<0)
    {
      /* We can neglect unused arguments in printf! */
      sprintf(fmt,"error is negative"); 
      return;
    }
  else if(e==0)
    {
      if     (expfix==EXP) 
	{
	  sprintf(fmt,"%%%%%s%s",p,"%1i.%1ie");
	  fmvalues[0]=mfw;
	  fmvalues[1]=errw+EPRECDIFF;
	  fmvalues[2]=0; /*unused */
	  fmvalues[3]=0; /*unused */
	}
      else if(expfix==FIXED)
	{
	  sprintf(fmt,"%%%%%s%s",p,"%1i.%1if");
	  fmvalues[0]=mfw;
	  fmvalues[1]=errw+EPRECDIFF;
	  fmvalues[2]=0; /*unused */
	  fmvalues[3]=0; /*unused */
	}
      else if(expfix==MIXED)
	{
	  sprintf(fmt,"%%%%%s%s",p,"%1i.%1ig");
	  fmvalues[0]=mfw;
	  fmvalues[1]=errw+EPRECDIFF;
	  fmvalues[2]=0; /*unused */
	  fmvalues[3]=0; /*unused */
	}
      else
	{
	  internalerror("cannot determing expfix");
	}
      *scale_v=v;
      return;
    }
  else if(v!=0&&10*e<DBL_EPSILON*(v>0?v:-v))
    {
      /* We can neglect unused arguments in printf! */
      sprintf(fmt,"The value is less precise than error times 10"); 
      return;
    }
  else
    {
      dbdp_e=(log10(e));
      assert_perror(errno);
      dbdp_e-=(e>=1.0)?0:1;     /*check >= =*/
    }
  
  if(v==0)
    {
      dbdp_v=0;    /* check the impact of this setting*/
    }
  else
    {
    dbdp_v=(log10(v>0?v:-v));
    assert_perror(errno);
    dbdp_v-=(v>=1.0)?0:1;     /*check >= = */
    }

  /*Figure the needed low level format: */

  if      (  (style==FORMPM&&expfix==EXP)
	   ||(style==FORMPM&&expfix==MIXED&&dbdp_v!=0))
    {
      f=MWE;  /* "+-" format with exponent*/
    }
  else if (  (style==FORMPM&&expfix==FIXED)
           ||(style==FORMPM&&expfix==MIXED&&dbdp_v==0))
    {
      f=MWF;  /* "+-" format with fixed format */
    }
  else if (  (style==FORMPA&&expfix==EXP)
           ||(style==FORMPA&&expfix==MIXED&&dbdp_v!=0)
           ||(style==FORMPA&&expfix==MIXED&&-dbdp_e+errw-1<0)
           ||(style==FORMPA&&expfix==MIXED&&dbdp_v<=-magicmix))
    {
      f=PWE; /* "()" format with exponent */
    }
  else if (  (style==FORMPA&&expfix==FIXED)
           ||(style==FORMPA&&expfix==MIXED&&dbdp_v==0)
           ||(style==FORMPA&&expfix==MIXED&&
	      (-dbdp_e+errw-1>=0&&dbdp_v>-magicmix)))
    {
      f=PWF; /* "()" format without exponent */
    }
  else
    {
      internalerror("cannot determing f");
    }
  
  /* Do the low level formatting here: */

    if (f == MWE)
    {
      /* sprintf(t,"(%%%s%1i.%1if+-%%%1i.%1if)%c%+03i",
         p,mfw,dbdp_v-dbdp_e+errw-1,mfw,dbdp_v-dbdp_e+errw-1,E,dbdp_v);
      */
      sprintf(fmt, "(%%%%%s%s%c%s", p, "%1i.%1if+-%%%1i.%1if)", E, "%+03i");
      fmvalues[0] = mfw;
      fmvalues[1] = dbdp_v - dbdp_e + errw - 1;
      fmvalues[2] = mfw;
      fmvalues[3] = dbdp_v - dbdp_e + errw - 1;
      fmvalues[4] = dbdp_v;
      *scale_v = v / pow(10., dbdp_v);
      *scale_e = e / pow(10., dbdp_v);
    }
    else if (f == MWF)
    {
      if (-dbdp_e + errw - 1 >= 0)
      {
        /*sprintf(t,"%%%s%1i.%1if+-%%%1i.%1if",
          p,mfw,-dbdp_e+errw-1,mfw,-dbdp_e+errw-1);
        */
        sprintf(fmt, "%%%%%s%s", p, "%1i.%1if+-%%%1i.%1if");
        fmvalues[0] = mfw;
        fmvalues[1] = -dbdp_e + errw - 1;
        fmvalues[2] = mfw;
        fmvalues[3] = -dbdp_e + errw - 1;
        *scale_v = v;
        *scale_e = e;
      }
      else
      {
        /*Ugly branch... the error part gets larger than errw.*/
        /*sprintf(t,"%%%s%1i.%1if+-%%%1i.0f",
          p,mfw,0,errw);
        */
        sprintf(fmt, "%%%%%s%s", p, "%1i.%1if+-%%%1i.0f");
        fmvalues[0] = mfw;
        fmvalues[1] = 0;
        fmvalues[2] = errw;
        fmvalues[3] = 0; /*unused */
        /*
         * No rounding of error required here, since we print
         * more decimals than errw.
         */
        *scale_v = v;
        *scale_e = e * 0.1;
      }
      if(-dbdp_e+errw-1==-1)
	{
	  (*scale_e)*=10;
	}
    }
  else if (f==PWE)
    {
      /*sprintf(t,"%%%s%1i.%1if(%%%1i.0f)%c%+03i",
	p,mfw,dbdp_v-dbdp_e+errw-1,errw,E,dbdp_v);
      */
      sprintf(fmt,"%%%%%s%s%c%s",p,"%1i.%1if(%%%1i.0f)",E,"%+03i");
      fmvalues[0]=mfw;
      fmvalues[1]=dbdp_v-dbdp_e+errw-1;
      fmvalues[2]=errw;
      fmvalues[3]=dbdp_v;
      *scale_v=v/pow(10.,dbdp_v);
      *scale_e=e/pow(10.,dbdp_e-errw+1);
    }
  else if (f==PWF)
    {
      if(-dbdp_e+errw-1>=0)
	{
	  /* sprintf(t,"%%%s%1i.%1if(%%%1i.0f)",
	     p,);
	  */
	  sprintf(fmt,"%%%%%s%s",p,"%1i.%1if(%%%1i.0f)");
	  fmvalues[0]=mfw;
	  fmvalues[1]=-dbdp_e+errw-1;
	  fmvalues[2]=errw; 
	  fmvalues[3]=0; /*unused*/
	  *scale_v=v;
	  *scale_e=e/pow(10.,dbdp_e-errw+1);
	}
      else
	{
	  /*Ugly branch... the error part gets larger than errw.*/
	  /* sprintf(t,"%%%s%1i.%1if(%%%1i.0f)",
	     p,mfw,0,errw); 
	  */
	  sprintf(fmt,"%%%%%s%s",p,"%1i.%1if(%%%1i.0f)");
	  fmvalues[0]=mfw;
	  fmvalues[1]=0;
	  fmvalues[2]=errw;
	  fmvalues[3]=0; /*unused */
	  /* 
	   * No rounding of error required here, since we print
	   * more decimals than errw.
	   */
	  *scale_v=v;
	  *scale_e=e*0.1;
	}
      if(-dbdp_e+errw-1==-1)
	{
	  (*scale_e)*=10;
	}
    }
  else 
    {
      internalerror("cannot determing f");
    }


  /*If we debug, print out the result and 
    the low level formatting variables*/

/*#define INTERNAL_VERBOSE */
#ifdef INTERNAL_VERBOSE
  fprintf(stderr,"%s\n",fmt);
  fprintf(stderr,"int[0]=%i\nint[1]=%i\nint[2]=%i\nint[3]=%i\nint[4]=%i\n",
	  fmvalues[0],fmvalues[1],fmvalues[2],fmvalues[3],fmvalues[4]);
  fprintf(stderr,"dbdp_v   =%i\ndbdp_e   =%i\nscale_v  =%f"
	  "\nscale_e  =%f\nfmtstring=\nin %s\n %i\n"
	  ,dbdp_v,dbdp_e,*scale_v,*scale_e,__FILE__,__LINE__);
#endif    

  if(fmvalues[1]<0)
    {
      fmvalues[1]=0;
      /*
	sprintf(fmt,"Either 0<|value|<<error or internal error"); 
	internalerror("negative field width, maybe 0<|value|<<error?");
      */
    }
  return;
}
/*
 * Interface to errlow where 
 *
 *
 *
 */
#define MIN_FMT_SIZE 100
#if PPOE_INTERNAL_PRINT == ASPRINT
void asprint_err(char **t,               /*format string*/
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
  ){
  char fmt[MIN_FMT_SIZE];
  int  fmtv[5];
  print_err_int(fmt,fmtv,v,e,style,expfix,magicmix,errw,mfw,E,pos,scale_v,scale_e);
  asprintf(t,fmt,fmtv[0],fmtv[1],fmtv[2],fmtv[3],fmtv[4]);
  assert_perror(errno);
#ifdef INTERNAL_VERBOSE
  fprintf(stderr,"format string(asprintf)=%s\n",fmt);
  fprintf(stderr,"format string(asprintf)=%s\n",*t);
  assert_perror(errno);
#endif
#elif PPOE_INTERNAL_PRINT == SNPRINT
void snprint_err(char*t,   /*format string*/
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
  ){
  char fmt[MIN_FMT_SIZE]; 
  int  fmtv[5];
  print_err_int(fmt,fmtv,v,e,style,expfix,magicmix,errw, mfw,E,pos,scale_v,scale_e);
  assert_perror(errno);
  if(n<=snprintf(t,n,fmt,fmtv[0],fmtv[1],fmtv[2],fmtv[3],fmtv[4])){
      fprintf(stderr,"In %s at %i format string not large enough, it is only %i chars long\n",__FILE__,__LINE__,n);
      exit(EXIT_FAILURE);
    }
#ifdef INTERNAL_VERBOSE
  fprintf(stderr,"format string(snprintf)=%s\n",t);
  assert_perror(errno);
#endif
#endif
  return;
}
