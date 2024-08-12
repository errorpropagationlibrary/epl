/* 
   Copyright (C) 2000 .. 2024 Achim Bode

   This file is part of epl (error propagation library).

   epl is free software: you can redistribute it and/or modify it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE version 3
   as published by the Free Software Foundation.

   epl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU AFFERO GENERAL PUBLIC LICENSE for more details.

   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE along with epl. If not, see <https://www.gnu.org/licenses/>.
*/
/*
*********************************************
* Calling gnuplot from C and fortran        *
*********************************************
* The usefullness rely on gnuplot version 3.6 or beta 3.6 (>???)
  since we have the plot '-' feature
*********************************************
* Achim 19.07.97
*
* Read this before using:
*
* void gnuplot(gnuplotcommand) is sending a string to the command line of gnuplot
* gnuplot is called automatically, a pipe to gnuplot is automatically opened. 

* Finishing gnuplot (e.g. via gnuplot("quit") ) or killing can cause a 'Broken pipe' error (Not catched).
* The gnuplot command pause -1 doesn't work as naive expected (See examples for emulation)

*********************************************
* What fortran fans must do                 *
*********************************************
* The fortran routine must be stored in an seperated fortran source file
  eg gnuplotfortran.f:
       subroutine gnupin(string)
       character string*(*)
       integer lenght
       lenght=len(string)
       call gnupf(lenght,xstring)
       return
       end
* compile this file with f77 gnuplotfortan.f and link with f77 gnuplotfortran.o  thisfile.o yourfortanexample.o
* if you get unresolved symbols see wrappers below in the code.
**********************************************
* How to use the routines:                   *
**********************************************
* A sample C program
      int main(){
      	 gnuplot("plot '-'");
      	 gnuplot(" 0 10");
      	 gnuplot(" 10 0");
      	 gnuplot(" 5 5");
      	 gnuplot("E");
      	 fprintf(stderr,"press enter to continue...");
      	 fflush(stderr);
      	 getchar();
      	 return (1);}

* A sample fortran program
      program gnutes
      integer i
      character*100 fmt
      call gnupin("set title 'Sample title'")
      call gnupin("plot '-' w l , sin(x)*exp(x/50.)")
      do i=1,100
         write(fmt,'(2E10.3)') real(i),exp(real(i)/50.0)
         call gnupin(fmt)
      enddo
      call gnupin("E")
      write(*,'(A26)') "press enter to continue..."
      read(*,*)
      stop
      end
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define MAXARGSTRLEN 1024

static FILE *command=NULL;

void gnuplot(gnuplotcommand) 
     char* gnuplotcommand;
{ 
  if ( NULL == command && NULL == (command = popen("gnuplot","w")) )
    perror("popen gnuplot failed");
  fprintf(command, gnuplotcommand);
  fflush(command);
  return;
}
void gnupf(len,gnuplotcommand) 
     int *len; char* gnuplotcommand;
{ 
  char help[MAXARGSTRLEN];
  if((*len)+2>MAXARGSTRLEN) {
    fprintf(stderr,"string given to gnupf to long");
    exit(-1);};
  strncpy(help,gnuplotcommand,*len);
  help[(*len)  ]='\n';
  help[(*len)+1]='\0';
  gnuplot(help);
  return;
}

/* function wrapper for some fortran calling c conventions */

void gnupf_(len,gnuplotcommand) 
     int *len; char* gnuplotcommand;
{ gnupf(len,gnuplotcommand); return;}

void GNUPF_(len,gnuplotcommand) 
     int *len; char* gnuplotcommand;
{ gnupf(len,gnuplotcommand); return;}

void GNUPF(len,gnuplotcommand) 
     int *len; char* gnuplotcommand;
{ gnupf(len,gnuplotcommand); return;}
