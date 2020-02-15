//
// Created by rbd on 24.12.2019.
//

#ifndef TRY_POINT_H
#define TRY_POINT_H

//#include "Skeys.h"

//using Binary = long;
#include "Binary.h"
#include <vector>
//#include <iostream>

//helib
#include <helib/FHE.h>
#include <helib/EncryptedArray.h>
#include <helib/intraSlot.h>
#include <helib/binaryArith.h>
#include <helib/binaryCompare.h>
#include <helib/ArgMap.h>
#include <NTL/BasicThreadPool.h>
#include <numeric>
#include <cmath>
#include <unistd.h>

NTL_CLIENT
//static std::vector<zzX> unpackSlotEncoding; // a global variable


class Skeys;
//#include "Bit.h" //todo add
using Bit = bool; //todo remove
using namespace std;

class Point {
    vector<NTL::Vec<Ctxt>> encCoordinates;
    vector<Binary> coordinates;
    Skeys          &sk; // = (Skeys &) NULL; //TODO should be private. and public key
    
public:
    explicit Point(Skeys &sk, vector<Binary>  coordinates);
    Point(const Point &p);
//    Point();
    vector<Binary> getCoordinates() const;
    Binary& operator [] (int idx) { return coordinates[idx]; }
    const Binary& operator [] (int idx) const { return coordinates[idx]; }
    friend std::ostream& operator << (std::ostream& os, const Point& p);
    friend Point operator + (Point &, Point &);
    friend Point operator += (Point &, Point &);
    friend Point operator - (const Point &, const Point &);
    friend Point operator -= (Point &, const Point &);
    friend Point operator * (const Point &, const Bit &);
    friend Point operator / (const Point &p, const Binary &i);
    friend Bit operator >= (const Point &p1, const Point &p2);
    Point & operator = (const Point &p1); //todo consider returning  Point&
//    Point & operator = ( Point p1); //todo consider returning  Point&
//    static Point dummyPoint(Skeys &sk);

    //TODO consider using the function below instead of getDistFromClosestMeanByClient in aux.h
    //friend double getDistFromClosestMean(vector<DecryptedPoint>); {
    //    return 0;
    //}


//// -------------------------- for DBG --------------------------
    friend class Point_test;
};


#endif //TRY_POINT_H
