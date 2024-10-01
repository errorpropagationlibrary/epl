# Generated automatically from Makefile.in by configure.
# Makefile for ppoe.	-*- Indented-Text -*-
#/*
#   Copyright (C) 2000 .. 2024 Achim Bode
#
#   This file is part of epl (error propagation library).
#
#   epl is free software: you can redistribute it and/or modify it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE version 3
#   as published by the Free Software Foundation.
#
#   epl is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even th implied warranty of MERCHANTABILITY
#   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU AFFERO GENERAL PUBLIC LICENSE for more details.
#
#   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE along with epl. If not, #see <https://www.gnu.org/licenses/>.
#
#*/
#### Start of system configuration section. ####

srcdir = .

CC = gcc

LIBS = -lfmt 

CFLAGS = -g -Wall  -Wno-format
LDFLAGS = -g -Wall
CXXFLAGS = -g -Wall -Wno-format

prefix = /usr/local
exec_prefix = $(prefix)

bindir = $(exec_prefix)/bin
infodir = $(prefix)/info

# Prefix to be prepended to each installed program, normally empty or `g'.
binprefix = 

#### End of system configuration section. ####

SHELL = /bin/sh

PPOESRC  = ppoe.c
PPOEHDR  = ppoe.h
PPOEOBJ  = ppoe.o

all: demoC demo
.PHONY: all

#.c.o:
#	$(CXX) -c $(CXXFLAGS) $< 

demo:
	$(CXX) -c $(CXXFLAGS) ppoe.c  
	$(CXX) -c $(CXXFLAGS) demo.c  
	$(CXX) $(LDFLAGS) -o $@ demo.o ppoe.o $(LIBS)

demoC:
	$(CC) -c $(CCFLAGS) ppoe.c  
	$(CC) -c $(CCFLAGS) demo.c  
	$(CC) $(LDFLAGS) -o $@ demo.o ppoe.o -lm

#Makefile: Makefile.in config.status
#	$(SHELL) config.status

.PHONY: clean mostlyclean distclean realclean dist

clean:
	rm -f ppoe.o demo.o demo demoC

mostlyclean: clean

distclean: clean
	rm -f Makefile 

# Prevent GNU make v3 from overflowing arg limit on SysV.
.NOEXPORT:

