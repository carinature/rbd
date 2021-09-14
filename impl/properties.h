#ifndef TRY_PROPERTIES_H
#define TRY_PROPERTIES_H

#include <cmath>
#include <vector>
#include <helib/FHE.h>
#include <NTL/BasicThreadPool.h>

NTL_CLIENT

//using helib;


#define NUM_POINTS 100  // for python
#define DIM 2
#define RANGE_LIM 1  // for python, defines the range of the points (from BOTTOM_LIM to RANGE_LIM)
#define BOTTOM_LIM 0
#define EPSILON 0.25
#define DECIMAL_DIGITS 1
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

#define points_file "/home/karina/CLionProjects/rbd/io/points"
#define points_copy_file "/home/karina/CLionProjects/rbd/io/points_copy"
#define chosen_file "/home/karina/CLionProjects/rbd/io/chosen"
#define leftover_file "/home/karina/CLionProjects/rbd/io/leftover"
#define means_file "/home/karina/CLionProjects/rbd/io/means"
#define rands_file "/home/karina/CLionProjects/rbd/io/rand_means"
#define rands_bad_file "/home/karina/CLionProjects/rbd/io/rands_bad_file"
#define point_csv_file "/home/karina/CLionProjects/rbd/point_csv"



#endif //TRY_PROPERTIES_H

