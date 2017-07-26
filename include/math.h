#ifndef __LIB_MATH__
#define __LIB_MATH__

/* has been referenced by other func */
double log(double x);
double pow(double x, double y);
double scalbn (double x, int n);
double copysign(double x, double y);

int finite(double x);
int isnan(double x);
int rem_pio2(double x, double *y);
double log10(double x);
double sqrt(double x);
double fabs(double x);
double floor(double x);
double ceil(double x);
double sin(double x);
double tan(double x);
double cos(double x);
double asin(double x);

#endif // __LIB_MATH__

