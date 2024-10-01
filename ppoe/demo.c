#include "ppoe.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef __cplusplus
#include <string.h>
#else
#include <iostream>
void FormatCPP(const char* f){
  double v=92345.678;
  double e=    0.345;
  std::pair<double,double> ppoe(v, e);
  int s = 30-strlen(f);
  int l = 40-fmt::format(f, v).length();
  std::cout<<"FormatCPP(\""<<f<<"\");";
  while(0<s--) {
    std::cout<<" ";
  }
  std::cout<<" // leads to "<<fmt::format(f, v);
  while(0<l--) {
    std::cout<<" ";
  }
  std::cout<<" "<<fmt::format(f, ppoe)<<std::endl;
}

void compare_format(const char* c_format, double value, double error){
  if (c_format==NULL) { 
    std::cout<<"c-format  c++-format  c-format(value)  c++-format(value) c-format(error) cpp-format(error)"<<std::endl;
  } else {
    double v=92345.678;
    double e=    0.345;
    std::pair<double,double> ppoe(v, e);
    char c_format_str1[300]; 
    char c_format_str2[300];
    char c_format_err[300];
    char cpp_format[200];


    sprintf(c_format_str1,c_format,v);

    strcpy(cpp_format, "{");
    strcpy(cpp_format+1, c_format);
    *strchr(cpp_format,'%')=':';
    strcpy(cpp_format+strlen(cpp_format),"}");

    strcpy(c_format_err,c_format);
    if(strchr(c_format_err,'e'))
      *strchr(c_format_err,'e')='p';
    if(strchr(c_format_err,'f'))
      *strchr(c_format_err,'f')='q';
    if(strchr(c_format_err,'g'))
      *strchr(c_format_err,'g')='r';
    sprintf(c_format_str2,c_format_err,v, e);

    std::cout<<c_format     <<"   "<<cpp_format<<"  ";//<<std::endl;
    std::cout<<c_format_str1<<"   "<<fmt::format(cpp_format,v)<<"   "
	     <<c_format_str2<<"   "<<fmt::format(cpp_format,ppoe)<<std::endl;
  }
  return;
}
#endif

#ifdef PRINTF_CUSTOMIZATION
void FormatC(const char* f){
  double v=92345.678;
  double e=    0.345;
  char f2[300];
  strcpy(f2,f);
  if(strchr(f2,'e'))
    *strchr(f2,'e')='p';
  if(strchr(f2,'f'))
    *strchr(f2,'f')='q';
  if(strchr(f2,'g'))
    *strchr(f2,'g')='r';
  if(strchr(f2,'E'))
    *strchr(f2,'E')='P';
  if(strchr(f2,'F'))
    *strchr(f2,'F')='Q';
  if(strchr(f2,'G'))
    *strchr(f2,'G')='R';

  int s = 30-strlen(f);
  int l = 40-snprintf(NULL, 0, f, v);
  printf("FormatC(\"%s\");",f);
  while(0<s--) {
    printf(" ");
  }
  printf(" // leads to "); printf(f, v);
  while(0<l--) {
    printf(" ");
  }
  printf(" "); printf(f2, v, e); printf("\n");
}
#endif

int main(){

  double value = -9876543.0;
  double error =    1234.0;

  if (false){
#define FORLOOP(A) for(bool A = 0; *(char*)&A != 2 ; *(char*)&A=*(char*)&A+1)
    FORLOOP(flag_alternate_form) {
      FORLOOP(padding) {
	char paddchar=(padding?'W':'\0'); 
	FORLOOP(side_adjustment) {
	  FORLOOP(flag_startwithblank) {
	    FORLOOP(flag_sign) {
	      FORLOOP(flag_thousandseperator) {
		for(int fieldwith=-1;fieldwith!=30; fieldwith++){
		  for(int precision=-1;precision!=3; precision++){
		    char length='l';
		    char conversionE[4]= "pqr";
		    for(int conv=0;conv!=3; conv++){
		      char* result;
                    
		      print_error(&result,  /*location whhere string is located, must be freed*/
				  value,       /*value*/
				  error,       /*error of value*/
				  flag_alternate_form, /*  (always 0 are ., G trailing zero not removed */
				  paddchar,           
				  side_adjustment,
				  flag_startwithblank,
				  flag_sign,
				  flag_thousandseperator,
				  fieldwith, /*       mimimum....*/
				  precision, /*       -> of error, if not given 2 for error <3 */
				  length,  /*(type)*/
				  conversionE[conv]);
		      printf("%s\n", result);
		      free (result);
		    }}}}}}}}}
  }
 
  //for(value = 99987654321.01234567; value>=99987654321.01234567 ; value*=0.1){
  for(value = 99987654321.01234567; value>0.0001 ; value*=0.0001){
    for(error = value; error>1e-20 ; error*=0.00003){
      if(value<0.01)
	value=0.00000001;

      char* result;
      print_error(&result,  /*location whhere string is located, must be freed*/
		  value,       /*value*/
		  error,       /*error of value*/
		  false, /*  (always 0 are ., G trailing zero not removed */
		  '\0',       
		  false,
		  false,
		  false,
		  false,
		  20, /*       mimimum....*/
		  2, /*       -> of error, if not given 2 for error <3 */
		  'l',  /*(type)*/
		  'q');
      printf("\nv=%30.15lf\ne=%30.15lf   r=%s\n", value, error, result);
      free (result);
    }
  }
 
#ifdef PRINTF_CUSTOMIZATION
  if (-1==ppoe_register_with_printf())
    return -1;

  double v=92345.678;
  double e=    0.345;
  printf("sample value=%25.15lf\n       error=%25.15lf\n       ppoe =%25.2lp\n",v, e, v, e);
  printf("sample value=%25.15lf\n       error=%25.15lf\n       ppoe =%25.2lP\n",v, e, v, e);
  printf("sample value=%25.15lf\n       error=%25.15lf\n       ppoe =%25.2lq\n",v, e, v, e);
  printf("sample value=%25.15lf\n       error=%25.15lf\n       ppoe =%25.2lr\n",1.0/0.0, e, 1.0/0.0, e);
  printf("sample value=%25.15lf\n       error=%25.15lf\n       ppoe =%25.2lr\n",v, 1.0/0.0, v, 1.0/0.0);
  printf("sample value=%25.15lf\n       error=%25.15lf\n       ppoe =%25.2lr\n",sqrt(-1), e, sqrt(-1), e);
  printf("sample value=%25.15lf\n       error=%25.15lf\n       ppoe =%25.2lr\n",v, sqrt(-1), v, sqrt(-1));
  printf("sample value=%25.15lf\n       error=%25.15lf\n       ppoe =%25.2lp\n       ppoe =%25.2lP\n       ppoe =%25.2lq\n       ppoe =%25.2lr\n",v, e, v, e, v, e, v, e, v, e);
  FormatC("%25.2lf");
  FormatC("%-25.2lf");
  FormatC("%-+25.2lf");
  FormatC("%- 25.2lf");
  FormatC("%-025.2lf");
  FormatC("%-+025.2lf");
  FormatC("%- 025.2lf");
  FormatC("% 25.2lf");
  FormatC("%#25.2lf");
  FormatC("%025.2lf");
  FormatC("%0-25.2lf");
  FormatC("%0+025.2lf");
  FormatC("%0 25.2lf");
  FormatC("%0-#25.2lf");
  FormatC("%.2lf");


  FormatC("%25.2lf");                        // leads to                  92345.68                             92345.68(35)
  FormatC("%-25.2lf");                       // leads to 92345.68                                 92345.68(35)
  FormatC("%-+25.2lf");                      // leads to +92345.68                                +92345.68(35)
  FormatC("%- 25.2lf");                      // leads to  92345.68                                 92345.68(35)
  FormatC("%-025.2lf");                      // leads to 92345.68                                 92345.68(35)
  FormatC("%-+025.2lf");                     // leads to +92345.68                                +92345.68(35)
  FormatC("%- 025.2lf");                     // leads to  92345.68                                 92345.68(35)
  FormatC("% 25.2lf");                       // leads to                  92345.68                             92345.68(35)
  FormatC("%#25.2lf");                       // leads to                  92345.68                             92345.68(35)
  FormatC("%025.2lf");                       // leads to 0000000000000000092345.68                000000000000092345.68(35)
  FormatC("%0-25.2lf");                      // leads to 92345.68                                 92345.68(35)
  FormatC("%0+025.2lf");                     // leads to +000000000000000092345.68                000000000000+92345.68(35)
  FormatC("%0 25.2lf");                      // leads to  000000000000000092345.68                000000000000 92345.68(35)
  FormatC("%0-#25.2lf");                     // leads to 92345.68                                 92345.68(35)
  FormatC("%.2lf");                          // leads to 92345.68                                 92345.68(35)

#ifdef __cplusplus
  std::cout<<"Using C++ stream"<<std::endl;
  std::cout << fmt::format("C++ format string uses fFeEgG {:20.2f} ", std::pair<double,double>(v,e) )<<std::endl;
  FormatCPP("plain       {:20.2E}");           // leads to plain                   9.23E+04         plain             9.234568(35)E4
  FormatCPP("left        {0:X<20.2E}");        // leads to left        9.23E+04XXXXXXXXXXXX         left        9.234568(35)E4XXXXXX
  FormatCPP("right       {0:X>20.2E}");        // leads to right       XXXXXXXXXXXX9.23E+04         right       XXXXXX9.234568(35)E4
  FormatCPP("middle      {0:X^20.2E}");        // leads to middle      XXXXXX9.23E+04XXXXXX         middle      XXX9.234568(35)E4XXX
  FormatCPP("always plus {0:+.2E}");           // leads to always plus +9.23E+04                    always plus +9.234568(35)E4
  FormatCPP("only minus  {0:-.2E}");           // leads to only minus  9.23E+04                     only minus  9.234568(35)E4
  FormatCPP("leading ' ' {0: .2E}");           // leads to leading ' '  9.23E+04                    leading ' '  9.234568(35)E4
  FormatCPP("mixed       {0:20.2g}");          // leads to mixed                    9.2e+04         mixed               92345.68(35)
  FormatCPP("fixed       {0:20.2f}");          // leads to fixed                   92345.68         fixed               92345.68(35)
  FormatCPP("exponent    {0:20.2e}");          // leads to exponent                9.23e+04         exponent          9.234568(35)e4
  //FormatCPP("Local       {0:L20.5E}"); //fails
  //FormatCPP("Local       {0:X+20.5E}"); //fails
  std::cout<<"fixed format with ppoe::print "; 
  ppoe::print(std::cout, v,e);
  std::cout<<std::endl;
#endif
#ifdef X //usused since -- comparing c and c++ formats are not the job of this demo
         //                translating c to c++ formats are not completed (e.g. - to >)
  compare_format(NULL, v, e);
  compare_format("%20.2e", v, e);
  compare_format("%-20.2e", v, e);
  compare_format("%0-20.2e", v, e);
  compare_format("%#-20.2e", v, e);
#endif
#endif //__cplusplus
  return 0;
}
 
