/* 
   Copyright (C) 2000 .. 2024 Achim Bode

   This file is part of epl (error propagation library).

   epl is free software: you can redistribute it and/or modify it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE version 3
   as published by the Free Software Foundation.

   epl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU AFFERO GENERAL PUBLIC LICENSE for more details.

   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE along with epl. If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef ERRORCORRH
#define ERRORCORRH

// Some of these might be an overkill.
#include "config.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <math.h>

#include "epl_correlation.h"
#include "ppoe.h"

namespace epl{
  using namespace std;
  using namespace ppoe;
  template <typename T> class error;

#include "epl_correlation.h"
// Formating Argument to print
// COV            covariance matrix    -
// CORR           correlation matrix    | without descriptive header
// CORR_ERR_COV   corr/error/cov        |
// CORR_ERR_SCOV  corr/error/sqrt(cov) -
// COVH           covariance matrix    -
// CORRH          correlation matrix    | with descriptive header
// CORR_ERR_COVH  corr/error/cov        |
// CORR_ERR_SCOVH corr/error/sqrt(cov) -
enum covarianceformat{COV =-1,CORR =-2,CORR_ERR_COV =-3,CORR_ERR_SCOV =-4,
		       COVH= 1,CORRH= 2,CORR_ERR_COVH= 3,CORR_ERR_SCOVH= 4};

template<typename T> using Error = error<T>; //unused

//typedef template<typename T> T (*FuncObj)(const T &);
template<typename T> void draw(const error<T> &, const error<T>&);
template<typename T> void print(ostream&, const vector<error<T>*>&, covarianceformat=CORR_ERR_SCOVH);
template<typename T> error<T> func(T(*)(T), const error<T> &); 
//template<typename T> T(*FuncObj)(T);
//template<typename FuncObj, typename T> error<T> func(FuncObj, const error<T> &); 
//template<typename FuncObj,typename T> error<T> func(FuncObj           , const error<T> &); 

template<typename FuncObj,typename T> error<T> func(FuncObj const fobj, const error<T> &, const error<T> &); 
template<typename FuncObj,typename T> error<T> func(FuncObj const fobj, const error<T> &, const error<T> &, const error<T> &); 
template<typename T> T covariance(const error<T> &, const error<T> &);
template<typename T> ostream& operator<<(ostream& ,const error<T>&);
template<typename T> error<T> sqrt(const error<T> &);
template<typename T> error<T> sin (const error<T> &);
template<typename T> error<T> cos (const error<T> &);
template<typename T> error<T> tan (const error<T> &);
template<typename T> error<T> asin(const error<T> &);
template<typename T> error<T> acos(const error<T> &);
template<typename T> error<T> atan(const error<T> &);
template<typename T> error<T> sinh(const error<T> &);
template<typename T> error<T> cosh(const error<T> &);
template<typename T> error<T> tanh(const error<T> &);
template<typename T> error<T> exp (const error<T> &);
template<typename T> error<T> log (const error<T> &);
template<typename T> bool  operator==(const error<T>	  &,const error<T>   &);
template<typename T> bool  operator!=(const error<T>	  &,const error<T>   &);
template<typename T> bool  operator<=(const error<T>	  &,const error<T>   &);
template<typename T> bool  operator>=(const error<T>	  &,const error<T>   &);
template<typename T> bool  operator< (const error<T>	  &,const error<T>   &);
template<typename T> bool  operator> (const error<T>	  &,const error<T>   &);

template<typename T> bool  operator==(const T&,const error<T>	  &);
template<typename T> bool  operator!=(const T&,const error<T>	  &);
template<typename T> bool  operator<=(const T&,const error<T>	  &);
template<typename T> bool  operator>=(const T&,const error<T>	  &);
template<typename T> bool  operator< (const T&,const error<T>	  &);
template<typename T> bool  operator> (const T&,const error<T>	  &);

template<typename T> bool  operator==(const error<T>	  &,const T&);
template<typename T> bool  operator!=(const error<T>	  &,const T&);
template<typename T> bool  operator<=(const error<T>	  &,const T&);
template<typename T> bool  operator>=(const error<T>	  &,const T&);
template<typename T> bool  operator< (const error<T>	  &,const T&);
template<typename T> bool  operator> (const error<T>	  &,const T&);

template<typename T> error<T> operator+ (const error<T>   &,const error<T>    &);
template<typename T> error<T> operator- (const error<T>	  &,const error<T>	  &);
template<typename T> error<T> operator* (const error<T>	  &,const error<T>	  &);
template<typename T> error<T> operator/ (const error<T>	  &,const error<T>	  &);

template<typename T> error<T> operator+ (const error<T>	  &,const T&);
template<typename T> error<T> operator- (const error<T>	  &,const T&);
template<typename T> error<T> operator* (const error<T>	  &,const T&);
template<typename T> error<T> operator/ (const error<T>	  &,const T&);

template<typename T> error<T> operator+ (const T&,const error<T>	  &);
template<typename T> error<T> operator- (const T&,const error<T>	  &);
template<typename T> error<T> operator* (const T&,const error<T>	  &);
template<typename T> error<T> operator/ (const T&,const error<T>	  &);  


template <typename T> class error{
  public:
    friend ostream& operator<< <>(ostream& ,const error&);
    friend void print<>(ostream&, const vector<error*>&, covarianceformat );
    friend void draw<>(const error &, const error &);
    friend error func<>(T(*)(T), const error &); 
    //friend error func<>(FuncObj, const error &); 
    //template<typename T> T(*FuncObj)(T);
    //template<typename FuncObj> friend error func(FuncObj, const error &); 
    //template <typename FuncObj> friend error func(FuncObj , const error &); 
    //template <typename FuncObj> friend error func(FuncObj const fobj, const error &); 
    template <typename FuncObj> friend error func(FuncObj const fobj, const error &, const error &); 
    template <typename FuncObj> friend error func(FuncObj const fobj, const error &, const error &, const error &); 
    friend T covariance<>(const error &, const error &);
    friend error sqrt<>(const error &);
    friend error sin <>(const error &);
    friend error cos <>(const error &);
    friend error tan <>(const error &);
    friend error asin<>(const error &);
    friend error acos<>(const error &);
    friend error atan<>(const error &);
    friend error sinh<>(const error &);
    friend error cosh<>(const error &);
    friend error tanh<>(const error &);
    friend error exp <>(const error &);
    friend error log <>(const error &);

    friend bool  operator==<>(const error	  &,const error   &);
    friend bool  operator!=<>(const error	  &,const error   &);
    friend bool  operator<=<>(const error	  &,const error   &);
    friend bool  operator>=<>(const error	  &,const error   &);
    friend bool  operator< <>(const error	  &,const error   &);
    friend bool  operator> <>(const error	  &,const error   &);

    friend bool  operator==<>(const T&,const error	  &);
    friend bool  operator!=<>(const T&,const error	  &);
    friend bool  operator<=<>(const T&,const error	  &);
    friend bool  operator>=<>(const T&,const error	  &);
    friend bool  operator< <>(const T&,const error	  &);
    friend bool  operator> <>(const T&,const error	  &);

    friend bool  operator==<>(const error	  &,const T&);
    friend bool  operator!=<>(const error	  &,const T&);
    friend bool  operator<=<>(const error	  &,const T&);
    friend bool  operator>=<>(const error	  &,const T&);
    friend bool  operator< <>(const error	  &,const T&);
    friend bool  operator> <>(const error	  &,const T&);

    friend error operator+ <>(const error	  &,const error	  &);
    friend error operator- <>(const error	  &,const error	  &);
    friend error operator* <>(const error	  &,const error	  &);
    friend error operator/ <>(const error	  &,const error	  &);

    friend error operator+ <>(const error	  &,const T&);
    friend error operator- <>(const error	  &,const T&);
    friend error operator* <>(const error	  &,const T&);
    friend error operator/ <>(const error	  &,const T&);

    friend error operator+ <>(const T&,const error	  &);
    friend error operator- <>(const T&,const error	  &);
    friend error operator* <>(const T&,const error	  &);
    friend error operator/ <>(const T&,const error	  &);  
    error operator+=(const error    & );
    error operator+=(const T & );
    error operator-=(const error    & );
    error operator-=(const T & );
    error operator*=(const error    & );
    error operator*=(const T & );
    error operator/=(const error    & );
    error operator/=(const T & );
    //explicit --- template ctor are not used for (user defined) conversions
    error(const T v=zero, const T sigma=zero,  const string name="NoName");      
    error(const T v, const string name);
    error(const string name);          
    error(const error & a, string name=""); //copy ctor
    bool operator=(const error<T> & ); //copy assignment
    ~error();                          //dtor
#ifdef EPLDEVELOPMENT
    void assign(const T &mean,const T &err,
		const string name=string("NoName"));
    void     printall(ostream &os); //
#endif
    T   mean() const; //mean/average of distribution)
    T abserr() const; //absolute (correlated) error (sigma)
    T relerr() const; //relative (correlated) error
   protected:
    static const T zero   ;
    static const T one    ;
    static const T onehalf;
    void addtoderv(const error<T> &,const T &);
    void applyderv(const error<T> &,const T &);
  private:
    T     meavg;    //mean/average variable (mean of distribution)
    string name;    //For pretty printing, a name of the variable
    correlation<T> * covmatrix; //pointer to [interface of a]
    // covariance matrix. Right now this pointer is initialized to one 
    // global covariance matrix (per T), maybe we support in 
    // the future multiple covariance matrices by allowing the 
    // initialization of this pointer.
    static correlation<T> covmat;
    int     rowcol;            //number of row/col occupied by this mean
    vector<pair<int,T> > derv; //this vector is usually empty
    //it is used to calc the changes in the covariance matrix.
    //the int is the rowcol of the parameter, the T is
    //the partial derivation of the variable with respect to the
    //parameter.
  };
} //end of namespace

#include "epl_error_def.h"

#endif





