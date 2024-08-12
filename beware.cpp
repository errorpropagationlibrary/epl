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
    
  cout << "error variables are different from scalar variables," << endl
       << "be aware of the differences" <<endl;

  error<double> x(6.0,0.45,"x");
  error<double> y(6.0,0.45,"y");
  error<double> d(x-y,"w"); 
  error<double> dq(d*d,"dq");
  error<double> sdq(sqrt(dq),"sdq");
  error<double> z(x-x,"z"); 
  error<double> zq(z*z,"zq");
  error<double> szq(sqrt(zq),"szq");

  cout << "variable x        =             " << x << endl
       << "variable y        =             " << y << " // same distribution as x, but independent" << endl 
       << "variable z        =     x-x   = " << z << " // this has a zero error (kronecer delta)"  << endl 
       << "variable zq       =     z^2   = " << zq <<" // same, error is zero" << endl
       << "variable sqrt(zq) = sqrt(z^2) = " << szq << " //fails because the derivation of sqrt at 0 is infinit" << endl;
 
  vector<error<double>*> ww;
  ww.push_back(&x);
  ww.push_back(&z);
  ww.push_back(&zq);
  ww.push_back(&szq);
  cout<<"Correlation"<<endl;
  print(cout,ww);

  cout << "variable d        =     x-y   = " << d << " // differnt distributions, the difference is also zero, but not the error" << endl 
       << "variable dq       =     d^2   = " << dq <<" // same, error is zero because the derivation of x^2 at 0 is 0" << endl
       << "variable sqrt(dq) = sqrt(d^2) = " << sdq << " //fails because the derivation of sqrt at 0 is infinit" << endl;
   
  ww.clear();
  ww.push_back(&x);
  ww.push_back(&y);
  ww.push_back(&d);
  ww.push_back(&dq);
  ww.push_back(&sdq);
  cout<<"Correlation"<<endl;
  print(cout,ww);

  cout << "compare and logical operators" << endl;
 
  double ce(6.2);
  double cb(6.47);
  error<double> xw(6.0,0.50,"xw");
  cout << "x =" <<  x << endl;
  cout << "xw=" << xw << endl;
  cout << "y =" <<  y << endl;
  cout << "ce=" << ce << endl;
  cout << "cb=" << cb << endl;

  if (x==y) {cout << "x==y";}else{cout << "x!=y";} cout << endl;
  if (x==xw) {cout << "x==xw";}else{cout << "NOT x==xw";} cout << endl;

  if (x ==ce) {cout << "x ==ce";}else{cout << "NOT x ==ce";} cout << endl;
  if (x ==cb) {cout << "x ==cb";}else{cout << "NOT x ==cb";} cout << endl;
  if (ce== x) {cout << "ce== x";}else{cout << "NOT ce== x";} cout << endl;
  if (cb== x) {cout << "cb== x";}else{cout << "NOT cb== x";} cout << endl;
  if (cb==ce) {cout << "cb==ce";}else{cout << "NOT cb==ce";} cout << endl;
  if (x >=ce) {cout << "x >=ce";}else{cout << "NOT x >=ce";} cout << endl;
  if (x <=ce) {cout << "x <=ce";}else{cout << "NOT x <=ce";} cout << endl;
  if (x >=cb) {cout << "x >=cb";}else{cout << "NOT x >=ce";} cout << endl;
  if (x <=cb) {cout << "x <=cb";}else{cout << "NOT x <=cb";} cout << endl;
  if (xw>=cb) {cout << "xw>=cb";}else{cout << "NOT xw>=cb";} cout << endl;
  if (xw<=cb) {cout << "xw<=cb";}else{cout << "NOT xw<=cb";} cout << endl;

  return 0;
}
