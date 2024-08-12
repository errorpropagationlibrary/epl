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
#include <iostream>
#include "epl_error.h"

using namespace epl;

float myfunc(float a){return ::sqrt(sinh(a));};
float dsqrt (float a){return ::sqrt(a);};

int main(){

  // We can use more than one T, but everything
  // is independent between them!
  error<float>  x(0.1,0.05,"x"),
                y( 9 ,0.2,"y");
  error<double> a(10 ,0.02,"a"),
                b(10 ,0.2,"b");

  cout << "variable x (based on float)  = " << x << endl 
       << "variable y (based on float)  = " << y << endl
       << "variable a (based on double) = " << a << endl
       << "variable b (based on double) = " << b << endl;

    //x=x+0.2f*y;

  cout << "exp("<<x<<") =" << exp(x) << " //many functions are defined for error variables" << endl;
  cout << "sqrt(sinh(" << x << ")= " << sqrt(sinh(x))  << " //nested function are no problem" << endl;
  cout << "myfunc(" << x << ")   = " << func(myfunc, x) << " //derivation by numerical approximation" << endl;
  cout << "sqrt(" << x << ")= " << sqrt(x) << " //another implemented function" << endl;
  cout << "approximation with func sqrt(" << x << ")= " << func(sqrt,x)<< " //the numerical approximation only differes in the error part"<< endl;
  typedef  float (*fptype)(float);
  fptype fp(sqrt);
  cout << "approximation with func sqrt(" << x << ")= " << func(fp,x) << " //function pointer work as well" << endl;   // Note that function<->pointer conversation is done by C++



    cout << "Show all formatting options of the correlation matrix" << endl << endl;

    vector<error<float>*> v;
    v.push_back(&x);
    v.push_back(&y);
    cout << "Default formatting of covariance matrix" << endl;
    print(cout,v);
    cout << "Printing the correlation matrix" << endl;
    print(cout,v,CORRH);
    cout << "Printing the covariance matrix" << endl;
    print(cout,v,COVH);
    cout << "Printing the first hybrid" << endl;
    print(cout,v,CORR_ERR_COVH);
    cout << "Printing the second hybrid" << endl;
    print(cout,v,CORR_ERR_SCOVH);
    cout << "Certainly there are all available without header" << endl;

  cout << "but the formatting of the numbers is not adjustable." << endl
       << "Certainly you can read the covariance matrix by yourself"<<endl
       << "cov(x,y)=" << covariance(x,y)<<endl;
  
  // access to mean and error:
  cout << "x                   = " << x << endl
       << "mean of x           = " << x.mean()<<endl
       << "absolut error of x  = " << x.abserr()<<endl
       << "relative error of x = " << x.relerr()<<endl;

  return 0;
}
