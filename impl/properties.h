

#ifndef TRY_PROPERTIES_H
#define TRY_PROPERTIES_H


#include <cmath>
#include <vector>
#include <helib/FHE.h>
#include <NTL/BasicThreadPool.h>

NTL_CLIENT

#define NUM_POINTS 80
#define DIM 2
#define EPSILON 0.15
#define DECIMAL_DIGITS 2
#define FACTOR pow(10, DECIMAL_DIGITS) // for conversion from double to long (helib compatibility)
#define BIT_SIZE 10
#define N_THreads 1

using DecryptedPoint = std::vector<long>;
using Bit = Ctxt;
using EncNumber = Vec<Ctxt>;

static bool verbose = false;

static bool dbg = true;  // use KeysServer to init Point
//bool dbg = false;  // use PubKey to init Point


#endif //TRY_PROPERTIES_H


/*  JUST SOME THOUGHTS
 *
 * point
 *      can encrypt
 *      can add/be added to other points
 *      can also be multuplied
 *
 */