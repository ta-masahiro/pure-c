#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <complex.h>
#include <limits.h>
#include <signal.h>
#include <gc.h>
#include <gmp.h>
#include <mpfr.h>

#include "CException.h"

#define malloc(size) GC_malloc(size)
#define realloc(p, size) GC_realloc(p, size)
#define calloc(n,m) GC_malloc(n*m)
#define free GC_free
 
#define TRUE    1
#define FALSE   0
//#define PR(n) (printf("### %d ###\n",n))
#define MAX(a,b) (a>=b ? a :b )
#define MIN(a,b) (a<=b ? a :b )