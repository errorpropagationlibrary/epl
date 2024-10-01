/* 
   Copyright (C) 2000 .. 2024 Achim Bode

   This file is part of epl (error propagation library).

   epl is free software: you can redistribute it and/or modify it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE version 3
   as published by the Free Software Foundation.

   epl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU AFFERO GENERAL PUBLIC LICENSE for more details.

   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE along with epl. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef PPOE_H
#define PPOE_H

#ifndef __cplusplus
#include <stdio.h> /*defines __GLIBC__ if GLIBC*/
#include <stdbool.h>
#else
#define private protected //hack fmt to resue the float formatter.
#include <fmt/format.h>
#undef private
#endif
#ifdef __GLIBC__
#define PRINTF_CUSTOMIZATION
#endif 

void print_error(char** result,  /*location whhere string is located, must be freed*/
                 double const value,       /*value*/
                 double const error,       /*error of value*/
                 bool flag_alternate_form, /*  (always 0 are ., G trailing zero not removed */
                 char paddingchar,          
                 short side_adjustment,   /*1 left, 0 middle, -1 right*/
                 bool flag_startwithblank,
                 bool flag_sign,
                 bool flag_thousandseperator,
                 int fieldwith, /*       mimimum....*/
                 int precision, /*       -> of error, if not given 2 for error <3 */
                 char length,  /*(type)*/
                 char conversion); 
#ifdef PRINTF_CUSTOMIZATION
int ppoe_register_with_printf();
#endif //PRINTF_CUSTOMIZATION
#ifdef __cplusplus
namespace fmt{
  template <> struct formatter<std::pair<double, double>>:formatter<float>{
    appender format(std::pair<double, double>& value_error, format_context& context) const; 
  };
}
namespace ppoe{
  void print(std::ostream & os, double v, double e);
}
#endif //__cplusplus
#endif
