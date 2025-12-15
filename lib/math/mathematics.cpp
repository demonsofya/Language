#include <math.h>
#include <stdio.h>

#include "mathematics.h"

bool CompareDoubles(double a, double b) {

    return (fabs(a - b) < EPS);
}
