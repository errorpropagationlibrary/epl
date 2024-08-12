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

namespace epl{

  template <typename T> correlation<T> error<T>::covmat;
  template <typename T> const T error<T>::zero   =static_cast<T>(0);
  template <typename T> const T error<T>::one    =static_cast<T>(1);
  template <typename T> const T error<T>::onehalf=static_cast<T>(1./2.);

  template <typename T> error<T>::error(const T v,const T e,const string name): meavg(v), name(name), covmatrix(&covmat), derv(0) {
    rowcol=covmatrix->insert(e*e);
    return;}

  template <typename T> error<T>::error(const T v, const string name):  meavg(v), name(name), covmatrix(&covmat), derv(0) {
    rowcol=covmatrix->insert(zero);
    return;}

  template <typename T> error<T>::error(const string name):   meavg(zero), name(name), covmatrix(&covmat), derv(0) {
    rowcol=covmatrix->insert(zero);
    return;}

template <typename T> error<T>::error(const error<T> & a, string newname): //The COPY function
  meavg(a.meavg),name(newname!=""?newname:a.name), covmatrix(&covmat),derv(0) {
  if(this==&a){
    cerr<<"In error: copyconstructor: selfasignment"<<endl;
    return;}
  rowcol=covmatrix->insert((*covmatrix)(a.rowcol,a.rowcol));
  for(int r=0;r!=covmatrix->indexend();r++){
    if(covmatrix->isindex(r)&&r!=rowcol)
      (*covmatrix)(r,rowcol)=(*covmatrix)(r,a.rowcol);}
  for(int c=0;c!=covmatrix->indexend();c++){
    if(covmatrix->isindex(c)&&c!=rowcol)
      (*covmatrix)(rowcol,c)=(*covmatrix)(a.rowcol,c);}
  //(*covmatrix)   (  rowcol,  rowcol)= //This assignment is done by insert
  (*covmatrix)(  rowcol,a.rowcol)
    =(*covmatrix)(a.rowcol,  rowcol)
    =(*covmatrix)(a.rowcol,a.rowcol);
  return;}

template <typename T> error<T>::~error(){
  covmatrix->erase(rowcol);
  return;}

#ifdef EPLDEVELOPMENT
// Undocumented and untested function: 
template <typename T> void error<T>::assign(const T &mean,
			     const T &err,
			     string name){
  //Clear the relvant rows/columns of the correlation matrix before  
  for(int rc=0;rc!=covmatrix->indexend();rc++)
    if(rc!=rowcol&&covmatrix->isindex(rc)&&(
      (*covmatrix)(rc,rowcol)!=zero||(*covmatrix)(rowcol,rc)!=zero)){
      (*covmatrix)(rc,rowcol)=zero;
      (*covmatrix)(rowcol,rc)=zero;}
  meavg=meavg;
  name=name;
  (*covmatrix)(rowcol,rowcol)=err*err;
  return;}

template <typename T> void error<T>::printall(ostream &os) {
  covmatrix->print(os);
  return;}
#endif

template <typename T> inline void error<T>::addtoderv(const error<T> &e, const T &df){
  derv.push_back(pair<int,T>(e.rowcol,df));
  return;}

template <typename T> void error<T>::applyderv(const error<T> &e,const T &df){
  addtoderv(e,df);
  typename vector<pair<int, T> >::const_iterator it; 
  for(int r=0;r!=covmatrix->indexend();r++){
    if(covmatrix->isindex(r)&&r!=rowcol){
      //I can add directly in the cov element!
       T sum=zero;
       for(it=derv.begin();it!=derv.end();it++){
 	sum+=(it->second) * ((*covmatrix)(it->first,r));}
       (*covmatrix)(rowcol,r)=sum;
       (*covmatrix)(r,rowcol)=sum;
     };};
   //This can be optimized!
   typename vector<pair<int,T> >::const_iterator itr;
   typename vector<pair<int,T> >::const_iterator itc;
   T sum=zero;
   for(itr=derv.begin();itr!=derv.end();itr++){
     for(itc=derv.begin();itc!=derv.end();itc++){
       sum+=(itc->second)
	 *((*covmatrix)(itc->first,itr->first))
	 *(itr->second);}}
   (*covmatrix)(rowcol,rowcol)=sum;
   derv.erase(derv.begin(),derv.end());
   return;}

template <typename T> inline T error<T>::mean() const { return meavg ;}

template <typename T> inline T  error<T>::abserr() const{
  T result = (*covmatrix)(rowcol,rowcol);
  //A negative errorsquare is an error, so we can just
  //return this mean indicating the error.
  //this condition forbids implicite any non-scalar T
  if(result>=zero) result=std::sqrt(result);
  return result ;}

template <typename T> inline T error<T>::relerr() const {
  if(meavg==zero){
    cerr<<"In relerr: Value is zero";
    exit(-1);}
  if(abserr()<zero){ 
    cerr<<"In relerr: absolute error is negative";
    exit(-1);}
  return abserr()/meavg;
}

template <typename T> bool error<T>::operator=(const error<T> & a) {
  if( this==&a) {
    cerr<<"In = of error: selfassignment";
    return false;}
  meavg=a.meavg;
  //name=a.name;
  //The constructor allready add a colum/row to the matrix:
  //Clear the relevant rows/columns of the correlation matrix before  
  for(int rc=0;rc!=covmatrix->indexend();rc++)
    if(rc!=rowcol&&covmatrix->isindex(rc)&&(
      (*covmatrix)(rc,rowcol)!=zero||(*covmatrix)(rowcol,rc)!=zero)){
      (*covmatrix)(rc,rowcol)=zero;
      (*covmatrix)(rowcol,rc)=zero;}
  for(int r=0;r!=covmatrix->indexend();r++){
    if(covmatrix->isindex(r)&&r!=rowcol)
      (*covmatrix)(r,rowcol)=(*covmatrix)(r,a.rowcol);}
  for(int c=0;c!=covmatrix->indexend();c++){
    if(covmatrix->isindex(c)&&c!=rowcol)
       (*covmatrix)(rowcol,c)=(*covmatrix)(a.rowcol,c);}
  (*covmatrix)   (  rowcol,  rowcol)
    =(*covmatrix)(  rowcol,a.rowcol)
    =(*covmatrix)(a.rowcol,  rowcol)
    =(*covmatrix)(a.rowcol,a.rowcol);
  return true;};

}
