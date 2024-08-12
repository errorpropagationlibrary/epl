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

extern "C" {
#include "errlow.h"
#if PPOE_INTERNAL_PRINT == ASPRINT
#include <malloc.h>
#endif
}

#include <iostream>
#include <iomanip>
namespace ppoe{
using namespace std;
void print(ostream & os,double const v,double const e){

#ifdef HAS_NO_PPOE
  int osprec(os.precision());
  os << "(" << v << "+-" << setprecision(2) << e << ")" ;
  os.precision(osprec);
#else

  // How to pass the additional formatting options
  // ( '()'/'+-', errw and magicmix) --- via intf?

  // Internal formatting state and help variables
  int style(FORMPA);
  int expfix(FIXED);
  int magicmix(5); //switch mixed from fixed to scientific if
                   //printed zero will exceed magicmix
  int errw(2);  // errw<10: the error must be less than 10 digits precise
  int mfw(2);   // Minimum field width of value -- pretty unimportant
  int E('e');   // e in "2.0e+23"
  int pos(0);   // additional + sign (for showpos)

  //Transforming stream formatting state to internal state:

  // Honor stream formatting flags:
  // floatfield  supported, mixed if (fixed and scientific) or neither
  // precision   supported, used for the precision of the error
  // uppercase   supported
  // showpos     supported  (always +-sign)
  // showpoint   unsupported (always .) -- what's that?
  // adjustfield not used (internal|right|left)

  //Note that style, magicmix and errw are not accessible right now.

  ios::fmtflags osflags(os.flags());
  if     ((  (ios::floatfield&osflags)==ios::scientific)
          &&((ios::floatfield&osflags)!=ios::fixed)){
    expfix=EXP;}
  else if((  (ios::floatfield&osflags)!=ios::scientific)
          &&((ios::floatfield&osflags)==ios::fixed)){
    expfix=FIXED;}
  else if((  (ios::floatfield&osflags)!=ios::scientific)
          &&((ios::floatfield&osflags)!=ios::fixed)){
    expfix=MIXED;}
  else if((  (ios::floatfield&osflags)==ios::scientific)
          &&((ios::floatfield&osflags)==ios::fixed)){
    expfix=MIXED;}
  else{
    //This can never be if iostram is standart conformat.
    cerr << "In " << __FILE__ << " at " << __LINE__ << " strange stream state "<< endl;}
  //the default precision is "6" means here error on two digits.
  if( os.precision()>EPRECDIFF  ){errw=os.precision()-EPRECDIFF;}
  if((ios::showpos&osflags)!=0  ){pos=1;}
  if((ios::uppercase&osflags)!=0){E='E';}

  double scaled_v,scaled_e;

  // Do we have a nicer way to deal with format (TEMPLATES) 
  // from c ?

#if PPOE_INTERNAL_PRINT == ASPRINT	

  char **t;
  asprint_err(t,v,e,style,expfix, magicmix,
	      errw,mfw,E,pos,&scaled_v,&scaled_e); 
  
  //Print the stuff. Is this a g++ extension?
  
  os.form(*t,scaled_v,scaled_e);

  free(*t);

#elif PPOE_INTERNAL_PRINT == SNPRINT	
#define FMTL 300
  int fmtlen=FMTL;
  char fmt[FMTL];
  snprint_err(fmt,fmtlen,v,e,style,expfix,magicmix,errw,mfw,E,pos,&scaled_v,&scaled_e); 

  char buffer[FMTL];
  snprintf(buffer,fmtlen,fmt,scaled_v,scaled_e);

  os<<buffer;
 
  //os.form(fmt,scaled_v,scaled_e);
  
#else
#error "PPOE_INTERNAL_PRINT must be either ASPRINT or SNPRINT, see ppoe.c "
#endif	
#endif 
    return;
}
}

