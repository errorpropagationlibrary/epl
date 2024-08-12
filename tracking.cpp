/* 
   Copyright (C) 2000 .. 2024 Achim Bode

   This file is part of epl (error propagation library).

   epl is free software: you can redistribute it and/or modify it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE version 3
   as published by the Free Software Foundation.

   epl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU AFFERO GENERAL PUBLIC LICENSE for more details.

   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE along with epl. If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include "epl_error.h"

using namespace epl;

int main(){

  cout << "tracking of correlation" << endl;
  // constructing elements 
  error<double> x(6.0,0.45,"x");
  error<double> y(3.0,0.29,"y");
  error<double> p("x+y"),m("x-y");
  error<double> x2("(p+m)/2"),y2("(p-m)/2");

  // performing the math as usual
  p=x+y;
  m=x-y;
  x2=(p+m)/2.; //Note that a int 2 is not autoconverted to double.
  y2=(p-m)/2.;

  //printing the results:
  cout << "variable x  =            " << x << endl 
       << "variable y  =            " << y << endl
       << "variable p  =    x+y   = " << p << endl
       << "variable m  =    x-y   = " << m << endl
       << "variable x2 = 1/2(p+m) = " << x2 << endl
       << "variable y2 = 1/2(p-m) = " << y2 << endl;

  //The correlations:
  vector<error<double>*> ww;
  ww.push_back(&x);
  ww.push_back(&y);
  ww.push_back(&p);
  ww.push_back(&m);
  ww.push_back(&x2);
  ww.push_back(&y2);
  print(cout,ww);

  cout << "Note the correct correlation between x and x2, y and y2 as well as x2 and y2" << endl <<endl;

  // We can draw the error ellipse too:
  cout << "The error ellipse of two variables can be drawn, it requires gnuplot to be installed" << endl <<endl;
  draw(p,m);

  return 0;
}
