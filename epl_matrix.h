/*
   Copyright (C) 2000 .. 2024 Achim Bode

   This file is part of epl (error propagation library).

   epl is free software: you can redistribute it and/or modify it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE version 3
   as published by the Free Software Foundation.

   epl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU AFFERO GENERAL PUBLIC LICENSE for more details.

   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE along with epl. If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef MATRIXH
#define MATRIXH
#include <vector>
#include <iostream>
#include <iomanip>


using namespace std;

template <typename T> class matrix;

template <typename T>
class matrix{
 public:
  explicit matrix(const int r=0, const int c=0, const int cap=225 );
  ~matrix();
  void print(ostream & os);
  //extend matrix with one row/column and intialise the elements
  void push_back(const int dim, const T =zero);
  //delete a row or column.
  void erase(const int dim, const int r);
  //element access, range : 0<=c<col 0<=r<row
  T& operator()(const int,const int);  
  int col() const;  //number of column in matrix
  int row() const;  //number of rows in matrix
  void insert(const int col,const int row,const T& d);
 protected:
  void resize(int rownew=0, int colnew=0);
 private:
  static const T zero;
  int rows;        //number of rows in the matrix
  int cols;        //number of columns in the matrix
  int rowcapacity; //number of row where space is allready allocated
  int colcapacity; //number of columns where space is allready allocated
  vector<T> mat;   //The space where the matrix is stored
};
//
//Since the class is a template we have to include the code
//

template<typename T>
const T matrix<T>::zero=static_cast<T>(0);


template <typename T>
matrix<T>::matrix(const int r, const int c, const int cap)
  : rows(r), cols(c), 
  rowcapacity(max(rows,cap)), 
  colcapacity(max(cols,cap)),
  mat(rowcapacity*colcapacity,zero){}

template <typename T>
matrix<T>::~matrix(){
  return;}

template <typename T>
inline void matrix<T>::print(ostream& os) {
  for(int r=0; r!= rows; r++){
    for(int c=0; c!=cols; c++){
      os << (*this)(r,c) ;
      if(c+1!=cols) os << "\t, ";}
    os << endl;}
  return;}
    
template <typename T>
inline T& matrix<T>::operator()(const int r, const int c) {
#ifdef NDEBUG
  if(r<0||r>=rows) {cerr<<"Wrong row arg in "<<__FILE__<<" at "
			<<__LINE__<<":"<<r<<"("<<rows<<")"<<endl;}
  if(c<0||c>=cols) {cerr<<"Wrong col arg in "<<__FILE__<<" at "
			<<__LINE__<<":"<<c<<"("<<cols<<")"<<endl;}
#endif
  return *(mat.begin()+colcapacity*r+c);}

template <typename T>
inline int matrix<T>::col() const{return cols;}

template <typename T>
inline int matrix<T>::row() const{return rows;}

template <typename T>
inline void matrix<T>::insert(const int c,const int r,const T& d){
  mat(c,r)=d;
  *(mat.begin()+colcapacity*r+c)=d;
}

template <typename T>
inline void matrix<T>::resize(int rownew, int colnew){
  // Note: 1.5*1.5 approx 2, the capacity increase in vector.
  if(colnew==0) colnew=static_cast<int>(colcapacity*1.5);
  if(rownew==0) rownew=static_cast<int>(rowcapacity*1.5);
#if 0  //This doesn't work for unknown reasons.
  mat.reserve(colnew*rownew);
#endif
  if(colnew>colcapacity){ //Alloc new columns inbetween of vector
    for(int i=0;i!=rows;i++){
      mat.insert(mat.begin()+i*colnew+colcapacity,
		  (colnew-colcapacity),zero);
    }
    colcapacity=colnew;
  }

  if(rownew>rowcapacity){    //Alloc new rows at end of vector
    mat.insert(mat.begin()+colcapacity*rowcapacity,
		colcapacity*(rownew-rowcapacity),zero);
    rowcapacity=rownew;
  }
  return;
}

template <typename T>
inline void matrix<T>::push_back(const int dim, const T e){
  if(dim==1&&(cols==colcapacity||rows==rowcapacity)) resize();
  if(dim==1){
    //resize(++rows,0);
     rows++;
    for(int c=0;c!=cols;c++) 
      *(mat.begin()+colcapacity*rows+c)=e;}
  else if(dim==2){
    //resize(0,++cols);
    cols++;
    for(int r=0;r!=rows;r++) 
      *(mat.begin()+colcapacity*r+cols)=e;}
  else{
    cerr<<"Wrong dim arg in "<<__FILE__<<" at "
	<<__LINE__<<":"<<dim<<endl;}
  return;
}
    
template <typename T>
inline void matrix<T>::erase(const int dim, int r){
  //erase on matrix is disabled!
  return;
  //todo: adjust colcapacity and rowcapacity
  if      (dim==1) {
    mat.erase(mat.begin()+cols*r,mat.begin()+cols*(r+1));
    rows--;}
  else if (dim==2) {
    for(int i=0;i!=rows;i++){
      mat.erase(mat.begin()+cols*i+r-i);}
    cols--;}
  else             
    {cerr<<"Wrong dim arg in "<<__FILE__<<" at "<<__LINE__<<":"<<dim<<endl;}
  return;}
    
#endif
