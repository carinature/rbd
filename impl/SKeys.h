//
// Created by rbd on 31.1.2020.
//

#ifndef TRY_SKEYS_H
#define TRY_SKEYS_H

#include "aux.h"

class SKeys {
    //// todo put here the CA functions , as STATIC
    //      maybe later consider:
    //  1. having a CA/Skeys object
    //  2. using Skeys server - all cominucation will be user-server -
    //      this way yoou don't have to init helib parameters every time

public:
    static Point calculateAvgPointByCA(const Point &point, Binary amount);
    static Binary calculateThresholdByCA(vector<Binary> distSum, int amount);
    static DecryptedPoint decryptPointByCA(const Point& p);
    static vector<DecryptedPoint> decryptPointsByCA(const vector<Point> &reps);

};



#endif //TRY_SKEYS_H

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