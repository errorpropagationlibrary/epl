# 
#   Copyright (C) 2000 .. 2024 Achim Bode
#
#   This file is part of epl (error propagation library).
#
#   epl is free software: you can redistribute it and/or modify it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE version 3
#   as published by the Free Software Foundation.
#
#   epl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
#   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU AFFERO GENERAL PUBLIC LICENSE for more details.
#
#   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE along with epl. If not, see <https://www.gnu.org/licenses/>.
#
#
#   We plot the ellipsoid defined by
#
#   k= (x,y) alhpa /x\
#                  | |
#                  \y/
#
#         /A  B\
#   alpha=|    |
#         \B  C/
#                 1   / C -B \ / R  S \
#   C=alpha^-1= ----- |      |=|      |
#               AC-BB \-B  A / \ S  T /
#
#   Bounding box of ellipsoid:
#    xrange [-DX:DX]
#    yrange [-DY:DY]
# 
#    DX=sqrt(R*k)
#    DY=sqrt(T*k)
#
#   Values at zeroaxis
#    XZERO=sqrt(k/A)
#    YZERO=sqrt(k/C)
#
# INPUT VALUES A,C,B,k
A = 2.
C = 1.
B = -0.9
k = 10
# DERIVED VALUES D,R,S,T,DX,DY
D = 1
D = A*C-B*B
R =  C/D
S = -B/D
T =  A/D
DX=sqrt(R*k)
DY=sqrt(T*k)
XZERO=sqrt(k/A)
YZERO=sqrt(k/C)

set xrange [-DX*1.1:DX*1.1]
set yrange [-DY*1.1:DY*1.1]

#!grep "^#" `pwd`/ellipsoide.gp | sed 's/#//g' 
#show variables
set xzeroaxis
set yzeroaxis
set nokey
set sample 100

set parametric
# The eigenvalues of C
l=(R+T)/2+sqrt((R-T)*(R-T)/4+S*S)
s=(R+T)/2-sqrt((R-T)*(R-T)/4+S*S)

a=atan((l-R)/S)
ca=cos(a)
sa=sin(a)

l=sqrt(k*l)
s=sqrt(k*s)

# btw: excentrity e
e=sqrt(1-s*s/l/l)

set trange [0:2*pi] 

set size 1.2,1
set terminal pstex size 6in,4in

set output "ellipse.delme.tex" 

plot  l*sin(t)*ca+s*cos(t)*sa,\
     -l*sin(t)*sa+s*cos(t)*ca with lines

set output "ellipse.tex" 

set xtics ( "{$\\overline{X}-\\sigma_X$}"   -DX,    \
	    "{$\\overline{X}-s_X$}"         -XZERO, \
	    "{$\\overline{X}$}"              0,     \
	    "{$\\overline{X}+s_X$}"          XZERO, \
	    "{$\\overline{X}+\\sigma_X$}"    DX)
set ytics ( "{$\\overline{Y}-\\sigma_Y$}"   -DY,    \
	    "{$\\overline{Y}-s_Y$}"         -YZERO, \
	    "{$\\overline{Y}$}"              0,     \
	    "{$\\overline{Y}+s_Y$}"          YZERO, \
	    "{$\\overline{Y}+\\sigma_Y$}"    DY)
replot

set size 1,1
set terminal dumb feed 72 50
set output "ellipse.ascii"
set xtics ( "Xb-sigma_x"   -DX,    \
	    "Xb-s_x"       -XZERO, \
	    "Xb"            0,     \
	    "Xb+s_x"        XZERO, \
	    "Xb+sigma_x"    DX)
set ytics ( "Yb-sigma_y"   -DY,    \
	    "Yb-s_y"       -YZERO, \
	    "Yb"            0,     \
	    "Yb+s_y"        YZERO, \
	    "Yb+sigma_y"    DY)
replot
set output
#remove strange characters.
!tail +2 ellipse.ascii > ellipse.txt
!rm -f ellipse.ascii

set size 0.9,0.9
set terminal jpeg
set termopt enhanced
set output "ellipse.jpg"
set xtics ( "X-σ_x"   -DX,    \
	    "X-s_x"       -XZERO, \
	    "X"            0,     \
	    "X+s_x"        XZERO, \
	    "X+σ_x"    DX)
set ytics ( "Y-σ_y"   -DY,    \
	    "Y-s_y"       -YZERO, \
	    "Y"            0,     \
	    "Y+s_y"        YZERO, \
	    "Y+σ_y"    DY)
replot
#set output
#!gs -dBATCH -dNOPAUSE -sDEVICE=png16 -g750x700 -sOutputFile=ellipse.png ellipse.eps
#!rm -f ellipse.eps
quit

