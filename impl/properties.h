//
// Created by rbd on 15.2.2020.
//

#ifndef TRY_PROPERTIES_H
#define TRY_PROPERTIES_H

#include <cmath>

#define DIM 2
#define EPSILON 0.2
#define DECIMAL_DIGITS 2
#define FACTOR pow(10, DECIMAL_DIGITS)
#define BIT_SIZE 5
#define N_THreads 1

using DecryptedPoint =  vector<long>;  // moved to prop file

static bool verbose=false;


#endif //TRY_PROPERTIES_H


/*  JUST SOME THOUGHTS
 *
 * point
 *      can encrypt
 *      can add/be added to other points
 *      can also be multuplied
 *
 */