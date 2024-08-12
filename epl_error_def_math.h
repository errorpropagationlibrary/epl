/* 
   Copyright (C) 2000 .. 2024 Achim Bode

   This file is part of epl (error propagation library).

   epl is free software: you can redistribute it and/or modify it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE version 3
   as published by the Free Software Foundation.

   epl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU AFFERO GENERAL PUBLIC LICENSE for more details.

   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE along with epl. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef ERRORCORRDEFH 
#error There is no point in including this file directly, it is used by epl_error.h
#endif

#include <math.h>

template <typename T> epl::error<T> epl::sqrt(const error<T> &e){
  error<T> result(::sqrt(e.meavg)); 
  result.applyderv(e, error<T>::onehalf/result.meavg);
  return result; };

template <typename T> epl::error<T> epl::sin(const error<T> &e){
#ifdef USE_SINCOS
  //Speed improvement possible via call of sincos (GNU libC)
  T s, c; ::sincos(e.meavg, &s, &c);
  error<T> result(s);
  result.applyderv(e, c);
#else
  error<T> result(::sin(e.meavg));
  result.applyderv(e, ::cos(e.meavg));
#endif  
  return result; };

template <typename T> epl::error<T> epl::cos(const error<T> &e){
#ifdef USE_SINCOS
  //Speed improvement possible via call of sincos (GNU libC)
  T s,c; ::sincos(e.meavg, &s, &c);
  error<T> result(c);
  result.applyderv(e,-s);
#else
  error<T> result(::cos(e.meavg));
  result.applyderv(e,-::sin(e.meavg));
#endif  
  return result; };

template <typename T> epl::error<T> epl::tan(const error<T> &e){
  error<T> result(::tan(e.meavg)); 
  result.applyderv(e,error<T>::one+result.meavg*result.meavg);
  return result; };

template <typename T> epl::error<T> epl::asin(const error<T> &e){
  error<T> result(::asin(e.meavg)); 
  result.applyderv(e,error<T>::one/::sqrt(error<T>::one-e.meavg*e.meavg));
  return result; };

template <typename T> epl::error<T> epl::acos(const error<T> &e){
  error<T> result(::acos(e.meavg)); 
  result.applyderv(e,-error<T>::one/::sqrt(error<T>::one-e.meavg*e.meavg));
  return result; };

template <typename T> epl::error<T> epl::atan(const error<T> &e){
  error<T> result(::atan(e.meavg)); 
  result.applyderv(e,error<T>::one/(error<T>::one+e.meavg*e.meavg));
  return result; };

template <typename T> epl::error<T> epl::sinh(const error<T> &e){
  error<T> result(::sinh(e.meavg)); 
  result.applyderv(e,::cosh(e.meavg));
  return result; };

template <typename T> epl::error<T> epl::cosh(const error<T> &e){
  error<T> result(cosh(e.meavg)); 
  result.applyderv(e,::sinh(e.meavg));
  return result; };

template <typename T> epl::error<T> epl::tanh(const error<T> &e){
  error<T> result(::tanh(e.meavg)); 
  result.applyderv(e,error<T>::one-result.meavg*result.meavg);
  return result; };

template <typename T> epl::error<T> epl::exp(const error<T> &e){
  error<T> result(::exp(e.meavg)); 
  result.applyderv(e,result.meavg);
  return result; };

template <typename T> epl::error<T> epl::log(const error<T> &e){
  error<T> result(::log(e.meavg)); 
  result.applyderv(error<T>::one/e.meavg,result.meavg);
  return result; };


// T (*FuncObj)(const T &)
//template<typename T> T(*FuncObj)(T);
//template<typename FuncObj> friend error func(FuncObj, const error<T> &); 
//template <typename FuncObj, typename T> epl::error<T> epl::func(FuncObj fobj , const error<T> &e1){
template <typename T> epl::error<T> epl::func(T(*fobj)(T) , const epl::error<T> &e1){
  T err    (e1.abserr());
  T fplus  (fobj(e1.meavg+err));
  T fminus (fobj(e1.meavg-err));  
  error<T> result(fobj(e1.meavg)); 
  result.applyderv(e1,error<T>::onehalf*(fplus-fminus)/err);
  return result; };

template <typename T> epl::error<T> epl::func(T(*fobj)(T,T), const error<T> &e1, const error<T> &e2){
  error<T> result(fobj(e1.meavg,e2.meavg));
  T err    (e1.abserr());
  T fplus  (fobj(e1.meavg+err, e2.meavg));
  T fminus (fobj(e1.meavg-err, e2.meavg));
  result.addtoderv(e1,error<T>::onehalf*(fplus-fminus)/err);

  err   =e2.abserr();
  fplus =fobj(e1.meavg, e2.meavg+err);
  fminus=fobj(e1.meavg, e2.meavg-err);
  result.applyderv(e2,error<T>::onehalf*(fplus-fminus)/err);
  return result; };

template <typename T> epl::error<T> epl::func(T(*fobj)(T,T,T) , const error<T> &e1, const error<T> &e2, const error<T> &e3){
  error<T> result(fobj(e1.meavg,e2.meavg, e3.meavg)); 
  T err    (e1.abserr());
  T fplus  (fobj(e1.meavg+err, e2.meavg, e3.meavg));
  T fminus (fobj(e1.meavg-err, e2.meavg, e3.meavg));
  result.addtoderv(e1,error<T>::onehalf*(fplus-fminus)/err);

  err   =e2.abserr();
  fplus =fobj(e1.meavg, e2.meavg+err, e3.meavg);
  fminus=fobj(e1.meavg, e2.meavg-err, e3.meavg);
  result.addtoderv(e2,error<T>::onehalf*(fplus-fminus)/err);

  err   =e3.abserr();
  fplus =fobj(e1.meavg, e2.meavg, e3.meavg+err);
  fminus=fobj(e1.meavg, e2.meavg, e3.meavg-err);
  result.applyderv(e3,error<T>::onehalf*(fplus-fminus)/err);
  return result; };

namespace epl {
template<typename T> error<T> operator+(const error<T>& e1, const error<T>& e2){
    error<T> result(e1.meavg + e2.meavg);
    result.addtoderv(e1, error<T>::one);
    result.applyderv(e2, error<T>::one);
    return result; };

template<typename T> error<T> operator+(const error<T>&e1, const T&b){
  error<T> result(e1.meavg+b);
  result.applyderv(e1,error<T>::one);
  return result; };

template<typename T> error<T> operator+(const T&b, const error<T>&e1){
  error<T> result(b+e1.meavg);
  result.applyderv(e1,error<T>::one);
  return result; };

template<typename T> error<T> operator-(const error<T>&e1, const T&b){
  error<T> result(e1.meavg-b);
  result.applyderv(e1,error<T>::one);
  return result; };

template<typename T>  error<T> operator-(const T&b, const error<T>&e1){
  error<T> result(b-e1.meavg);
  result.applyderv(e1,-error<T>::one);
  return result; }

template<typename T> error<T> operator-(const error<T>&e1, const error<T>&e2){
    error<T> result(e1.meavg-e2.meavg);
    result.addtoderv(e1,error<T>::one);
    result.applyderv(e2,-error<T>::one);
    return result; };

template<typename T> error<T> operator*(const error<T>&e1, const error<T>&e2){
  error<T> result(e1.meavg*e2.meavg);
  result.addtoderv(e1,e2.meavg);
  result.applyderv(e2,e1.meavg);
  return result; };

template<typename T> error<T> operator*(const error<T>&e1, const T&b){
  error<T> result(e1.meavg*b);
  result.applyderv(e1,b);
  return result; };

template<typename T> error<T> operator*(const T&b, const error<T>&e1){
  error<T> result(b*e1.meavg);
  result.applyderv(e1,b);
  return result; };

template<typename T> error<T> operator/(const error<T>&e1, const error<T>&e2){
  T tmp(error<T>::one/e2.meavg);
  error<T> result(e1.meavg*tmp);
  result.addtoderv(e1,tmp);
  result.applyderv(e2,-result.meavg*tmp);
  return result; };

template<typename T> error<T> operator/(const error<T>&e1, const T&b){
  T tmp(error<T>::one/b);
  error<T> result(e1.meavg*tmp);
  result.applyderv(e1,tmp);
  return result; };

template<typename T> error<T> operator/(const T&b, const error<T>&e1){
  T tmp(error<T>::one/e1.meavg);
  error<T> result(b*tmp);
  result.applyderv(e1,-result.meavg*tmp);
  return result; };


template<typename T> bool operator==(const error<T>&e1,const error<T>&e2)
{return e1.meavg==e2.meavg;};

template<typename T> bool operator==(const error<T>&e1,const       T &b  )
{return e1.meavg==b;};

template<typename T> bool operator==(const       T & b,const error<T> &e1)
{return e1.meavg==b;};

template<typename T> bool operator!=(const error<T>&e1,const error<T>&e2)
{return e1.meavg!=e2.meavg;};

template<typename T> bool operator!=(const error<T>&e1,const       T &b  )
{return e1.meavg!=b;};

template<typename T> bool operator!=(const       T & b,const error<T> &e1)
{return e1.meavg!=b;};

template<typename T> bool operator<=(const error<T>&e1,const error<T>&e2)
{return e1.meavg<=e2.meavg;};

template<typename T> bool operator<=(const error<T>&e1,const       T &b  )
{return e1.meavg<=b;};

template<typename T> bool operator<=(const       T & b,const error<T> &e1)
{return e1.meavg<=b;};

template<typename T> bool operator>=(const error<T>&e1,const error<T>&e2)
{return e1.meavg>=e2.meavg;};

template<typename T> bool operator>=(const error<T>&e1,const       T &b  )
{return e1.meavg>=b;};

template<typename T> bool operator>=(const       T & b,const error<T> &e1)
{return e1.meavg>=b;};

template<typename T> bool operator<(const error<T>&e1,const error<T>&e2)
{return e1.meavg<e2.meavg;};

template<typename T> bool operator<(const error<T>&e1,const       T &b  )
{return e1.meavg<b;};

template<typename T> bool operator<(const       T & b,const error<T> &e1)
{return e1.meavg<b;};

template<typename T> bool operator>(const error<T>&e1,const error<T>&e2)
{return e1.meavg>e2.meavg;};

template<typename T> bool operator>(const error<T>&e1,const       T &b  )
{return e1.meavg>b;};

template<typename T> bool operator>(const       T & b,const error<T> &e1)
{return e1.meavg>b;};

template <typename T> error<T> error<T>::operator+=(const error<T> &e){
  *this=*this+e;
  error<T> result(*this);
  return result; };

template <typename T> error<T> error<T>::operator+=(const T &e){
  *this=*this+e;
  error<T> result(*this);
  return result; };

template <typename T> error<T> error<T>::operator-=(const error<T> &e){
  *this=*this-e;
  error<T> result(*this);
  return result; };

template <typename T> error<T> error<T>::operator-=(const T &e){
  *this=*this-e;
  error<T> result(*this);
  return result; };

template <typename T> error<T> error<T>::operator*=(const error<T> &e){
  *this=*this*e;
  error<T> result(*this);
  return result; };

template <typename T> error<T> error<T>::operator*=(const T &e){
  *this=*this*e;
  error<T> result(*this);
  return result; };

template <typename T> error<T> error<T>::operator/=(const error<T> &e){
  *this=*this/e;
  error<T> result(*this);
  return result; };

template <typename T> error<T> error<T>::operator/=(const T &e){
  *this=*this/e;
  error<T> result(*this);
  return result; };

}
