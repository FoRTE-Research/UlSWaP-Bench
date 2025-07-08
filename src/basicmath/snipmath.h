/* +++Date last modified: 05-Jul-1997 */

/*
**  SNIPMATH.H - Header file for SNIPPETS math functions and macros
*/

#ifndef SNIPMATH__H
#define SNIPMATH__H

#include <math.h>
#include <stdint.h>
#include "sniptype.h"

/*
**  Callable library functions begin here
*/

void SolveCubic(double a, double b, double c, /* Cubic.C        */
                double d, uint32_t *solutions,
                double *x);
void Cubic(double a, double b, double c, double d, /* Cubic.C        */
           uint32_t *solutions, double *x);
void PrintCubic(double a, double b, double c, double d, /* Cubic.C        */
                uint32_t *solutions, double *x);
double rad2deg(double rad); /* Rad2Deg.C      */
double deg2rad(double deg); /* Rad2Deg.C      */

#include "pi.h"
#ifndef PHI
#define PHI ((1.0 + sqrt(5.0)) / 2.0) /* the golden number    */
#define INV_PHI (1.0 / PHI)           /* the golden ratio     */
#endif

/*
**  File: ISQRT.C
*/

struct int_sqrt
{
    uint32_t sqrt;
    uint32_t frac;
};

void usqrt(uint32_t x, struct int_sqrt *q);

#endif /* SNIPMATH__H */
