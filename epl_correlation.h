/* 
   Copyright (C) 2000 .. 2024 Achim Bode

   This file is part of epl (error propagation library).

   epl is free software: you can redistribute it and/or modify it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE version 3
   as published by the Free Software Foundation.

   epl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU AFFERO GENERAL PUBLIC LICENSE for more details.

   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE along with epl. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef CORRELATIONH
#define CORRELATIONH
#include "epl_matrix.h"
#include <vector>
#include <bits/stdc++.h> 
using namespace std;

template <typename T> class correlation;
//
//Protect class against any attempt to copy or assign!
//
template <typename T> class correlation{
public:
  correlation();
  //access elements
  T& operator()(const int index1, const int index2);
  //alloc space for new variable( if necssary), init the space and
  //return the index
  int  insert(T e);
  void erase(int index);
  bool isindex(int index);
  int  indexend();
  void print(ostream &os);
private:
  //returns an unused row/column or zero if there is none.
  int getfree();
  //free might be better realized with a heap
  vector<int> free;
  matrix<T> covmatrix;
protected:
  //Where to put these constants within the class
  //and having them defined in all member AND friend functions?
  //having them here make them defined only for members. Why?
  static const T zero   ;
  //static const T one    ;
  //static const T onehalf;
};

template <typename T> const T correlation<T>::zero   =static_cast<T>(0);
template <typename T> correlation<T>::correlation(): free(0){};

template <typename T> inline T& correlation<T>::operator()(const int index1,const int index2){
// Uncommenting the index check promotes this member to a function
// which can not be used in a arthmetic expression in ANSI C++. And
// exactly this is done in class error. Hence do not uncomment the check.
//  if(!isindex(index1)&&!isindex(index1)){
//    cerr<<"Wrong index to correlation matrix"<<endl;
//    exit -1;}
  return covmatrix(index1,index2);}

template <typename T> inline int correlation<T>::insert(T esquare){
  int index=getfree();
  if(0==index){
    covmatrix.push_back(1,zero);
    covmatrix.push_back(2,zero);
    index=covmatrix.col()-1;}
  else{
    //This sets also the unused row/column to zero
    for(int x=0;x!=covmatrix.row();x++){
      covmatrix(x,index)=zero;}
    for(int y=0;y!=covmatrix.col();y++){
      covmatrix(index,y)=zero;}}
  covmatrix(index,index)=esquare;
  return index;}

#define FREECOVMATRIX 0 //currently matrix::erase is disabled.

template <typename T> inline void correlation<T>::erase(int index){
  //erase the entry in the map,
  //erase the space in  matrix iff it is last element,
  //otherwise we would have to update rowcol for
  //each error object. We are not able to do that here!
  if(covmatrix.col()-1==index&&FREECOVMATRIX){
    covmatrix.erase(1,index);
    covmatrix.erase(2,index);}
  else{
    free.push_back(index); // store the free index of matrix for later use
    std::sort(free.begin(), free.end(), greater<int>() ); // keep free sorted, is that necessary? 
  }
  return;}

template <typename T> inline int correlation<T>::getfree(){
  int i=0;
  //the lowest matindex in free is the last element since free is always sorted
  if(free.size()>0) {
    i=*(free.end()-1);
    free.pop_back();}
  return i;}

template <typename T> inline bool correlation<T>::isindex(int index){
  return free.end()==find(free.begin(),free.end(),index);}

template <typename T> inline int  correlation<T>::indexend(){
  return covmatrix.row();}

#define BARECORRMAT
template <typename T> void correlation<T>::print(ostream& os){
#ifdef BARECORRMAT
  os<<"Unused entrys are ";
  for(int c=0;c!=indexend();c++)
    if(! isindex(c)){
      os<<c<<", ";}
  os<<endl;
  for(int c=0;c!=indexend();c++){
      os<<c<<"\t, ";}
  os<<endl;
  covmatrix.print(os);
#else
  for(int c=0;c!=indexend();c++)
    if(isindex(c)){
      for(int r=0;r!=indexend();r++)
	if(isindex(r)){
	  os << covmatrix(c,r);
	  //This condition is wrong since indexend()-1 need not to
	  //be isindex(). Fix it -- or not.
	  if(r+1!=indexend()) os << " , ";}
      os << endl;}
#endif
return;}
#endif
