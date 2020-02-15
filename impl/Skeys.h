//
// Created by rbd on 31.1.2020.
//

#ifndef TRY_SKEYS_H
#define TRY_SKEYS_H

//#include "aux.h"
#include "Point.h"

//#include <iostream>
#include <NTL/BasicThreadPool.h>
NTL_CLIENT

#include <helib/FHE.h>
#include <helib/EncryptedArray.h>
#include <helib/binaryCompare.h>
#include <helib/intraSlot.h>

static std::vector<zzX> unpackSlotEncoding; // a global variable

using DecryptedPoint =  vector<double>;

class SKeys {
    //// todo put here the CA functions , as STATIC
    //      maybe later consider:
    //  1. having a CA/Skeys object
    //  2. using Skeys server - all communication will be user-server -
    //      this way you don't have to init helib parameters every time
    
    long mValue[15] =
// { p, phi(m),   m,   d, m1, m2, m3,    g1,   g2,   g3, ord1,ord2,ord3, B,c}
    {2, 48,    105,   12, 3,  35,  0,   71,    76,    0,     2,  2,  0,   25, 2};
//    {{2, 48,    105,   12, 3,  35,  0,   71,    76,    0,     2,  2,  0,   25, 2},
//    {2, 600,   1023,  10, 11, 93,  0,   838,   584,   0,     10, 6,  0,   25, 2},
//    {2, 2304,  4641,  24, 7,  3,   221, 3979,  3095,  3760,  6,  2,  -8,  25, 3},
//    {2, 15004, 15709, 22, 23, 683, 0,   4099,  13663, 0,     22, 31, 0,   25, 3},
//    {2, 27000, 32767, 15, 31, 7,   151, 11628, 28087, 25824, 30, 6,  -10, 28, 4}
//};
    
    FHEcontext * context;
    FHESecKey * secKey;

public:
    SKeys(int argc, char *argv[]);
    Point calculateAvgPointByCA(const Point &point, Binary amount);
    Binary calculateThresholdByCA(vector<Binary> distSum, int amount);
    DecryptedPoint decryptPointByCA(const Point& p);
    vector<DecryptedPoint> decryptPointsByCA(const vector<Point> &reps); //todo consider static, and writing enc info to file

};

#endif //TRY_SKEYS_H
