

#ifndef TRY_PROPERTIES_H
#define TRY_PROPERTIES_H


#include <cmath>
#include <vector>
#include <helib/FHE.h>
#include <NTL/BasicThreadPool.h>

NTL_CLIENT

#define NUM_POINTS 20  // for python
#define DIM 2
#define RANGE_LIM 1  // for python, defines the range of the points (from Bottom_LIM to RANGE_LIM)
#define Bottom_LIM 0
#define EPSILON 0.5
#define DECIMAL_DIGITS 2
#define CONVERSION_FACTOR pow(10, DECIMAL_DIGITS) // for conversion from double to long (helib compatibility)
#define BIT_SIZE 10
#define N_THreads 1

using DecryptedPoint = std::vector<long>;
using Bit = Ctxt;
using EncNumber = Vec<Ctxt>;

//bool DBG = false;  // use PubKey to init Point
static bool DBG = true;  // use KeysServer to init Point
static bool VERBOSE = false;

static ofstream fcout("fcout");  //for DBG

#define points_file "/home/fares/CLionProjects/rbd/io/points"
#define points_copy "/home/fares/CLionProjects/rbd/io/points_copy"
#define chosen_file "/home/fares/CLionProjects/rbd/io/chosen"
#define leftover_file "/home/fares/CLionProjects/rbd/io/leftover"
#define means_file "/home/fares/CLionProjects/rbd/io/means"
#define rands_file "/home/fares/CLionProjects/rbd/io/rand_means"
#define bad_rands_file "/home/fares/CLionProjects/rbd/io/rand_means"



#endif //TRY_PROPERTIES_H


/*  JUST SOME THOUGHTS
 *
 * point
 *      can encrypt
 *      can add/be added to other points
 *      can also be multuplied
 *
 */