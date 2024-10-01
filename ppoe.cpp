/* 
   Copyright (C) 2000 .. 2024 Achim Bode

   This file is part of epl (error propagation library).

   epl is free software: you can redistribute it and/or modify it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE version 3
   as published by the Free Software Foundation.

   epl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU AFFERO GENERAL PUBLIC LICENSE for more details.

   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE along with epl. If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef __cplusplus
#define _GNU_SOURCE /* required for asprintf, it is not defined by gcc but g++*/
#else
#endif 

#include "ppoe.h"
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <printf.h>
#include <stdio.h>
#include <stdlib.h>

/* format max characters: 9+ DBL_DIG+1+3+4+1+(digistsof(DBL_MAX_10_EXP)+1 = 19+(int)ceil(log10(DBL_DIG+DBL_MAX_10_EXP)) */
const int sizeof_format=20+DBL_DIG+(int)ceil(log10(DBL_MAX_10_EXP));
const char ppoe_internal_error_msg[20] = "Internal ppoe error";

void save_concat_printf(char* format, char const* fmt, ...) {
  if (0==strncmp(format, ppoe_internal_error_msg, sizeof(ppoe_internal_error_msg))) /*encountered prevoiusly an error */ 
    return;
  const int lenght_format=strlen(format);
  va_list arg;
  va_start(arg, fmt);
  const int written = vsnprintf(format+lenght_format, sizeof_format-lenght_format, fmt, arg);
  va_end(arg);
  if (written<0 || written >= sizeof_format-lenght_format) { /*should never happen*/
    sprintf(format, ppoe_internal_error_msg);
  }
}

double ppoe_roundup(double value, int digit) {
  const double scale=pow(10, digit);
  return scale*ceil(value/scale);
}

double ppoe_round(double value, int digit) {
  const double scale=pow(10, digit);
  return scale*round(value/scale);
}
 
void print_error(char** result,  /*location whhere string is located, must be freed*/
                 double const value,         /*value*/
                 double const error,         /*error of value*/
                 bool flag_alternate_form,   /*always with decimal point */
                 char paddingchar,       /* not supported */        
                 short side_adjustment,    /* supported */
                 bool flag_startwithblank,   
                 bool flag_sign,             /* supported */
                 bool flag_thousandseperator, /*no test case available */
                 int fieldwith,              /* mimimum field width, supported*/
                 int precision,              /* presision of error, if -1 or 0: 2 for error <3, allowed -1,0,1,2 */
                 char length,                /* only double is suppored, therefore it must be 'l' */
                 char conversion){           /* p, q, r while simelar to p~e, q~f, r~g and P, Q, R */
  
  //#define  OUTPUT_ARGS_TO_STDIO
#ifdef OUTPUT_ARGS_TO_STDIO
  char input[10000];
  sprintf(input              , "## input\n");
  sprintf(input+strlen(input), "## flag_alternate_form          %s\n", flag_alternate_form?"true":"false"); 
  sprintf(input+strlen(input), "## paddingchar                  %s\n", paddingchar!='\0'?"true":"false");
  sprintf(input+strlen(input), "## side_adjustment              %i\n", side_adjustment>=0?"true":"false");
  sprintf(input+strlen(input), "## flag_startwithblank          %s\n", flag_startwithblank?"true":"false");
  sprintf(input+strlen(input), "## flag_sign                    %s\n", flag_sign?"true":"false");
  sprintf(input+strlen(input), "## flag_thousandseperatorinput  %s\n", flag_thousandseperator?"true":"false");
  sprintf(input+strlen(input), "## fieldwith                    %i\n", fieldwith);
  sprintf(input+strlen(input), "## precision                    %i\n", precision);
  sprintf(input+strlen(input), "## length                       %c\n", length);
  sprintf(input+strlen(input), "## conversion                   %c\n", conversion);
  sprintf(input+strlen(input), "## value                       %lf\n", value);
  sprintf(input+strlen(input), "## error                       %lf\n", error);

  char input2[10000];
  sprintf(input2,"%%");
  if (flag_alternate_form)    sprintf(input2+strlen(input2),"#"); 
  if (paddingchar!='\0')  sprintf(input2+strlen(input2),"0"); 
  if (side_adjustment==1)   sprintf(input2+strlen(input2),"-"); 
  if (flag_startwithblank)    sprintf(input2+strlen(input2)," "); 
  if (flag_sign)              sprintf(input2+strlen(input2),"+"); 
  if (flag_thousandseperator) sprintf(input2+strlen(input2),"'"); 
  sprintf(input2+strlen(input2),"%i.%i%c%c",fieldwith,precision,length,conversion);
  //printf("%s  ", input);
  //printf("-->%s<--", input2);
  asprintf(result, "DEBUG ONLY -->%s<--", input2);
  return;
#endif

  /* Checking the arguments */  
  if (tolower(conversion)!='p' && tolower(conversion)!='q' && tolower(conversion)!='r') {
    asprintf(result, "invalid converstion parameter %c", conversion); 
    return;
  }  

  if (value!=value){
    asprintf(result, "value is not a number"); 
    return;
  } else if (isinf(value)){
    asprintf(result, "value is infinit"); 
    return;
  }

  if(error!=error){
    asprintf(result, "error is not a number"); 
    return;
  } else if (isinf(error)){
    asprintf(result, "error is infinit"); 
    return;
  }

  char fillchar=(paddingchar=='\0'?' ':paddingchar);

  if (!isalnum(fillchar)&&!isblank(fillchar)) {
    asprintf(result, "invalid fill character '%c'", fillchar); 
    return;
  }

  /* validate and figure the precision of error out */
  if (error<0.0) {
    asprintf(result, "error is negative"); 
    return;
  } else if (error==0.0) {
    asprintf(result, "error is zero");
    return;
  } else /* if (error>0.0) */ {
    if (precision<-1 || precision > 2 ) { /* make no sense */
      asprintf(result, "invalid precision= %i", precision);
      return;
    } else if (precision<=0) {
      double error_leading_digit = ceil(error*pow(10, -1.0*floor(log10(error))));/*could be done with a loop */
      if (error_leading_digit>3)
            precision=1; 
      else
            precision=2;
    } /* precsion == 1 or precision == 2 */
  }
  
  /* Define low level formatting variables */
  int dbdp_v;     /* position of first nonzero digit of v. */
  int dbdp_e;     /* position of first nonzero digit of e. */
  char format[sizeof_format];
  format[0]='\0';

  save_concat_printf(format, "%s"); /* for left adjusted */ 
  save_concat_printf(format, "%%"); 
  if (flag_alternate_form) /*always with decimal point */ 
    strcat(format, "#");
  if (paddingchar!='\0' && (side_adjustment<=0)) /* not supported (ignored because low level format uses 1 as min field width)*/
    save_concat_printf(format, "0");
  if (flag_startwithblank)
    save_concat_printf(format, " ");
  if (flag_sign)
    save_concat_printf(format, "+");
  
  dbdp_e=floor(log10(error)); /*highest digit of error */
  double rounded_error = ppoe_roundup(error, dbdp_e-precision+1);   /* always round error up. */
  dbdp_e=floor(log10(rounded_error)); /* recalculate dbdp_e from rounded value */
  int scaled_error=lround(rounded_error*pow(10, precision-dbdp_e-1) ); /* rounding required because of numerical errors from scaling */

  double rounded_value = ppoe_round(value, dbdp_e-precision+1); /* must round here on the digit of the dbdp_e-precision */ 
  if (rounded_value==0) {
    dbdp_v=0; 
  } else { 
    dbdp_v=floor(log10(fabs(rounded_value)));
  }

  if (dbdp_v-dbdp_e>0 && dbdp_v-dbdp_e > DBL_DIG) {
    asprintf(result, "double precision less than error"); 
    return;
  }
  
  /*Figure the needed low level format: */
  if (conversion=='r' && dbdp_v>=6) conversion='p'; 
  if (conversion=='R' && dbdp_v>=6) conversion='P';
  if (conversion=='r' && dbdp_v<6) conversion='q'; 
  if (conversion=='R' && dbdp_v<6) conversion='Q';
  if (conversion=='q' && dbdp_e-precision>=0 ) conversion='p';
  if (conversion=='Q' && dbdp_e-precision>=0 ) conversion='P';

  double scaled_value=rounded_value;
  if (conversion== 'q' || conversion== 'Q') { /* fixed */
    if (dbdp_e-precision>=0) {
      save_concat_printf(format, "%i.%ilf", 1, 0);
    } else {
      save_concat_printf(format, "%i.%ilf", 1, -dbdp_e+precision-1);
    }
    save_concat_printf(format, "(%i)", scaled_error);
  } else {  /* (conversion== 'p'|| conversion== 'P') {  with exponent */
    save_concat_printf(format, "%i.%ilf", 1, dbdp_v-dbdp_e+precision-1);
    save_concat_printf(format, "(%i)", scaled_error);
    char exponent=(conversion== 'p'?'e':'E');
    save_concat_printf(format, "%c%i", exponent, dbdp_v);
    scaled_value*=pow(10,-dbdp_v);
  }
   
  save_concat_printf(format, "%%s");  

  length = snprintf(NULL, 0, format, "", scaled_value, ""); 

  int padding_len = (fieldwith-length>0?fieldwith-length:0);
  int padding_len_left;
  int padding_len_right;

  if (side_adjustment>0) {
    padding_len_left  = 0;
    padding_len_right = padding_len;
  } else if (side_adjustment<0) {
    padding_len_left  = padding_len;
    padding_len_right = 0;
  } else { /*side_adjustment==0 */
    padding_len_right = padding_len/2;
    padding_len_left  = padding_len_right+padding_len % 2;
  }

  char padding_string_left [padding_len_left +1];
  char padding_string_right[padding_len_right+1];

  padding_string_left [padding_len_left ] = '\0';
  padding_string_right[padding_len_right] = '\0';
  for (; padding_len_left > 0;)
    padding_string_left[--padding_len_left]   = fillchar;
  for (; padding_len_right > 0;)
    padding_string_right[--padding_len_right] = fillchar;

  int r = asprintf(result, format, padding_string_left, scaled_value, padding_string_right); 

  if (r==-1 && *result!=NULL){
    free(*result);
    *result=NULL;
  }
//#define OUTPUT_INTERNAL_VARIABLES_TO_STDIO
#ifdef OUTPUT_INTERNAL_VARIABLES_TO_STDIO
  printf(" error : %20lf %20lf %10i %4i\n", error, rounded_error, scaled_error, dbdp_e);
  printf(" value : %20lf %20lf %20lf %4i\n", value, rounded_value, scaled_value, dbdp_v);
  printf(" format: %s (ppoe_internal_error_msg)\n", format);
  printf(" result: %s \n", *result);
#endif

  return;
}

#ifdef PRINTF_CUSTOMIZATION
static int ppoe_printf(FILE *stream, const struct printf_info *info, const void *const args[]) {
  char* result;
  double value = *(double *)args[0];
  double error = *(double *)args[1];
  print_error(&result,  value, error, info->alt, (char) info->pad, (info->left==1?1:-1) , info->space , info->showsign, info->group , info->width, info->prec, 'l', (char) info->spec); 
    
  if (result==NULL)
    return -1;

  int len = fputs(result, stream);
  free(result);
  return len;
}

static int ppoe_arginf_sz(const struct printf_info *info, size_t n, int argtypes[/*n*/], int size[/*n*/] /*unused, no custom sizes*/) {
  /* this callback is called twice if first call returns > 1 */
  if (info->is_long_double) /* more assumptions about info possible*/
    return -1;

  argtypes[0] = PA_DOUBLE;
  argtypes[1] = PA_DOUBLE;
  return 2;
}

int ppoe_register_with_printf() {
  char specifiers[7]="pPqQrR";
  for (unsigned int i=0; i<sizeof(specifiers); i++) {
    if (register_printf_specifier(specifiers[i], ppoe_printf, ppoe_arginf_sz)==-1) {
      printf("error in register_printf_specifier('%c', ppoe_printf, ppoe_arginf_sz)",specifiers[i]);
      return -1;
    }
  }
  return 0;   
}
#endif

#ifdef __cplusplus
namespace fmt{
  /* not required since formatter<std::pair<double, double>> is inherited from formatter<Float>.  
     constexpr auto template <> struct formatter<std::pair<double, double>>:formatter<float>::parse (format_parse_context& context) {    
     auto i = context.begin();
     auto end = context.end();
     if (i != end && *i != '}' ) {
     i++;
     }
     if (i != end && *i != '}') {
     throw format_error("invalid format");
     }
     return i;  
     } */

  appender  formatter<std::pair<double, double>>::format(std::pair<double, double>& value_error, format_context& context) const {
    char *t;

    bool flag_thousandseperator=specs_.localized; /* no test case available */

    char conversion; /* p, q, r while simelar to p~e, q~f, r~g and P, Q, R */
    switch (specs_.type)
      {
      case presentation_type::exp_lower:
        conversion = 'p';
        break;
      case presentation_type::exp_upper:
            conversion = 'P';
        break;
      case presentation_type::fixed_lower:
            conversion = 'q';
            break;
      case presentation_type::fixed_upper:
            conversion = 'Q';
            break;
      case presentation_type::general_lower:
            conversion = 'r';
            break;
      case presentation_type::general_upper:
            conversion = 'R';
          break;
        default:
            conversion = 'z'; // Will be handled by print_error
      }

    char padding_character = specs_.fill[0]; //no UTF

    short side_adjustment = 0; /* supported */
    switch (specs_.align)
      {
      case align::type::left:
            side_adjustment = 1;
            break;
     case align::type::center:
           side_adjustment = 0;
       break;
     case align::type::numeric: //???
     case align::type::none:
     case align::type::right:
     default:
           side_adjustment = -1;
      }

    bool flag_sign = false; /* supported */
    char flag_startwithblank = '\0';
    switch (specs_.sign)
      {
      case sign::type::none:
      case sign::type::minus:
            flag_sign = false;
        break;
      case sign::type::space:
            flag_sign = false;
            flag_startwithblank=' ';
            break;
      case sign::type::plus:
      default:
            flag_sign = true;
      }

    print_error(&t, value_error.first, value_error.second, specs_.alt, padding_character, side_adjustment, flag_startwithblank, flag_sign, flag_thousandseperator, specs_.width, specs_.precision, 'l', conversion);

    if(t!=NULL) {
      fmt::appender it = context.out();
      for(int i=0; t[i]!='\0'; i++){
        *it++ = t[i];
      }
      free(t) ;
      return it;
    } else { /* error case */
      fmt::appender it = context.out();
      for(int i=0; ppoe_internal_error_msg[i]!='\0'; i++){
        *it++ = ppoe_internal_error_msg[i];
      }
      return it;
    }
     
  };
} //namespace fmt
 
void ppoe::print(std::ostream& os, double v,double e){
  os << fmt::format("{:.2f}", std::pair<double, double>(v, e)) ;
}
#endif // __cplusplus

