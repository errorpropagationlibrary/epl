
The Pretty Printing of Errors PPOE allows errors to be printed as in the
form of

   4543(3)

It compiles with C and C++ (with the fmt library (https://fmt.dev/11.0/)
(which is integrated into C++ 20))

ppoe provides an interface to glibc extension to printf formats.

ppoe.h and ppoe.c is all it takes. demo.c compiles also with C and C++.

The format specifier for printf extension use p, q and r instead
of e, f and g. The precision is used to specify the significat
digits of the error. Even with q and exponent might be used. All other
format specifies are as for double. 

demo demonstrates the interface the use of the format specifiers, there
are basically the same as for floating point numbers. demo is compiled
with C++, demoC. The provided Makefile compiles both from the same source.

Only double numbers are supported.

Integration with thr glibc printd extension can be disabled by removing the
#define PRINTF_CUSTOMIZATION
from ppoe.h.

C++ is only tested with the fmt library v9. In fact a hack is used to
allow inheritance of formatter<float>. It is not tested with C++20.  
The thousand seperator is not tested at all and likely broken.
