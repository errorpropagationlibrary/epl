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

template <typename T> inline T covariance(const error<T> &e1, const error<T> &e2){
  assert(e1.covmatrix==e2.covmatrix); 
  return (*(e1.covmatrix))(e1.rowcol,e2.rowcol); };

template <typename T> ostream& operator<<(ostream& os, const error<T>& e){
  double value=e.mean();
  double err=e.abserr();
  //os << "(" << value << "+-" << setprecision(2) << err << ")" ;
  ppoe::print(os, value, err);
  return os;};

// Since this printing function includes newlines we do not define it as a << operator
template <typename T> void print  (ostream& os,const vector<error<T>*>& vpe, covarianceformat f){
  // fw is the number of characters of each entry
  // Improvments: 
  //  -make sure name.size() <= fw
  //  -extend the formating of a fixpoint float number
  //   to a maximum fieldwith and to a fix "." position
  //   (before extending ostream: defining pretty class
  //   and pretty::operator<< (...,double) )
  //   or using c sprintf function.
  int           osprec(os.precision());
  ios::fmtflags osstat(os.flags());
  if      (f==COVH) {
    os << "covariance" << endl;}
  else if (f==CORRH) {
    os << "correlation" << endl;}
  else if (f==CORR_ERR_COVH) {
    os << "   \\cov    " << endl
       << "corr\\___ error" << endl;}
  else if (f==CORR_ERR_SCOVH) {
    os << "   \\sign(cov)*sqrt|cov|" << endl
       << "corr\\___ error" << endl;}
  static int COV(1),SCOV(2),CORR(3),ERR(4);
  vector<int> sec(3); //upper,diagonal and lower section of matrix 
  switch (f>0?f:-f){
  case COVH           : sec[0]=     sec[1]=    sec[2]=COV ;break;
  case CORRH          : sec[0]=     sec[1]=    sec[2]=CORR;break;
  case CORR_ERR_COVH  : sec[0]=CORR;sec[1]=ERR;sec[2]=COV ;break;
  case CORR_ERR_SCOVH : sec[0]=CORR;sec[1]=ERR;sec[2]=SCOV;break;
  default             : sec[0]=     sec[1]=    sec[2]=COV ;
    cerr << "Wrong 2. arg (f) to print in " << __FILE__ << " at " << __LINE__ <<endl;}
  static int fw;
  typename vector<error<T>*>::const_iterator itr(vpe.begin());
  typename vector<error<T>*>::const_iterator itc(vpe.begin());
  //os.setf(ios::scientific); fw=12; os<< setprecision(fw-7);
  os.setf(ios::fixed); fw=7;     os<< setprecision(fw-4);
  os.setf(ios::right);
  os << setw(fw) << " " << "|" ;
  for(itr=vpe.begin();itr!=vpe.end();itr++){
    os << setw(fw) << ((*itr)->name).c_str() << " ";}
  os << endl;
  {char c((os.fill)('-'));
  for(itr=vpe.begin();itr!=vpe.end();itr++){
    os << setw(fw+1) << "+" ;}
  os << setw(fw) << "-" << endl;
  os.fill(c);}
  for(itr=vpe.begin();itr!=vpe.end();itr++){
    os << setw(fw) << (*itr)->name.c_str() << "|"; 
    int lowdiagup(0);
    for(itc=vpe.begin();itc!=vpe.end();itc++){
      if(itr==itc) lowdiagup++;
      T tmp((*(*itr)->covmatrix)((*itr)->rowcol,(*itc)->rowcol));
      if      (sec[lowdiagup]==COV) {
	os << setw(fw) << tmp;}
      else if (sec[lowdiagup]==SCOV) { 
	T s(tmp>=error<T>::zero?error<T>::one:-error<T>::one);
	os << setw(fw) << s*std::sqrt(s*tmp);}
      else if (sec[lowdiagup]==CORR) { 
	T div=((*itr)->abserr())*((*itc)->abserr());
	if(div!=error<T>::zero){
	  os << setw(fw) << tmp/div;}
	else{
	  os << setw(fw) << "NaN";}}
      else if (sec[lowdiagup]==ERR) { 
	os << setw(fw) << (*itc)->abserr();}
      else{os<<"Internal error";}//this branch is empty by construction
      os << ' ';
      if(itr==itc) lowdiagup++;}
    os << endl;}
  os << endl;
  os.precision(osprec);
  os.flags(osstat);
  return;}

// Spawning a gnuplot process to view the error ellipsoide. This routine needs a lot of error catching. 
extern "C" { // gnuplotad.c has no corresponding header file, only the declartion of gnuplot is required
void gnuplot(const char*);
}

#define GPHELP(STR)      \
        if(len<=snprintf(str,len,STR)){\
           cerr<<"Format string not large enough"<<endl;exit(1);};\
        gnuplot(str);\
        snprintf(str,len,"\n");gnuplot(str)   
#define GPHELP2(STR,A)   \
        if(len<=snprintf(str,len,STR,A)){\
           cerr<<"Format string not large enough"<<endl;exit(1);};\
         gnuplot(str);\
         snprintf(str,len,"\n");gnuplot(str)      
#define GPHELP3(STR,A,B)  \
        if(len<=snprintf(str,len,STR,A,B)){\
           cerr<<"Format string not large enough"<<endl;exit(1);};\
        gnuplot(str);\
        snprintf(str,len,"\n");gnuplot(str)    

template<typename T> void draw(const error<T> &e1, const error<T>&e2){
    T sigmalevel(error<T>::one);
// The Gnu C++ lib doesn't know stringstream (sstream)... that would help here!
#define FMTL 500
    int  len=FMTL;
    char str[FMTL];
    GPHELP2("R  = %e",covariance(e1,e1));
    GPHELP2("T  = %e",covariance(e2,e2));
    GPHELP2("S  = %e",covariance(e1,e2));
    GPHELP2("k  = %e",sigmalevel);
    GPHELP2("x0 = %e",e1.mean());
    GPHELP2("y0 = %e",e2.mean());
    GPHELP3("set title 'The error ellipsoid for %s and %s'", e1.name.c_str(),e2.name.c_str());
    GPHELP("l=(R+T)/2+sqrt((R-T)*(R-T)/4+S*S)");
    GPHELP("s=(R+T)/2-sqrt((R-T)*(R-T)/4+S*S)");
    GPHELP("a =atan((l-R)/S)");
    GPHELP("ca=cos(a)");
    GPHELP("sa=sin(a)");
    GPHELP("l=sqrt(k*l)");
    GPHELP("s=sqrt(k*s)");
    GPHELP("xrm=x0-1.1*sqrt(R*k)"); // 1.1 defines 10% "internal" margin:
    GPHELP("xrp=x0+1.1*sqrt(R*k)"); // margin between ellipsoid and border
    GPHELP("yrm=y0-1.1*sqrt(T*k)"); // of plot.
    GPHELP("yrp=y0+1.1*sqrt(T*k)");
    GPHELP("set xrange [xrm:xrp]");
    GPHELP("set yrange [yrm:yrp]");
    GPHELP("set nokey");
    GPHELP("set sample 1000");
    GPHELP("set parametric");
    GPHELP("plot [0:2*pi] l*sin(t)*ca+s*cos(t)*sa+x0, -l*sin(t)*sa+s*cos(t)*ca+y0,\
     	                    xrm+t/(2*pi)*(xrp-xrm),y0,\
      	                  x0,yrm+t/(2*pi)*(yrp-yrm)");
    //GPHELP("pause 10");
    cout<<"press enter to continue..."<<endl;
    getchar();
    //Be carful here: The gnuplot process exits but the calling process,
    //means this one, can not detect that.
    //GPHELP("quit");
    return; 
  };
}
