//
// Created by rbd on 24.12.2019.
//

#ifndef TRY_IMP1_KMEANS_H
#define TRY_IMP1_KMEANS_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <cmath>       /* pow */
#include <cassert>
#include <helib/FHE.h>
#include <helib/EncryptedArray.h>
#include <helib/intraSlot.h>
#include <helib/binaryCompare.h>
#include <helib/ArgMap.h>
#include <NTL/BasicThreadPool.h>

//#include "Point.h"
//#include "Binary.h"
//#include "Bit.h" // todo add
#include "aux.h"
#include "SKeys.h"

vector<Point> getEncryptedKMeans(vector<Point> points);

vector<vector<Point> > getLeftoverPoints(const vector<Point> &points, const vector<Point> &means);



#endif //TRY_IMP1_KMEANS_H

////    IMPORTANT
/*
 ----    This time it's gonna work   ----
input bitSize=5, running 4 tests for each function
computing key-independent tables... done.
m = 1023, p = 2, phi(m) = 600
  ord(p)=10
  generator 838 has order (== Z_m^*) of 10
  generator 584 has order (== Z_m^*) of 6
 L=300, B=25

computing key-dependent tables... done
pa (random num 1): 10
pb (random num 2): 6
Comparison (without min max) succeeded: (10,6)=> mu=1, ni=0
pa (random num 1): 3
pb (random num 2): 8
Comparison (without min max) succeeded: (3,8)=> mu=0, ni=1

pa (random num 1): 19
pb (random num 2): 19
Comparison (without min max) succeeded: (19,19)=> mu=0, ni=0   <<-----  in case of EQUAL, BOTH bits return as -0- ZERO

 pa (random num 1): 7
pb (random num 2): 13
Comparison (without min max) succeeded: (7,13)=> mu=0, ni=1

Done!

Process finished with exit code 0
 */