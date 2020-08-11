// Created by rbd on 24.12.2019.
#ifndef TRY_Point__OLD_H
#define TRY_Point__OLD_H


//using Binary = double;
using Binary = long;
//#include "Binary.h"
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
//#include "Skeys.h"

//#include "Bit.h" //todo add
using Bit = bool; //todo remove
using namespace std;

class Point_ {
    vector<long> coordinates;
//    Skeys          &sk; // = (Skeys &) NULL; //TODO should be private. and public key
    Skeys *sk; // = (Skeys &) NULL; //TODO should be private. and public key

public:
    vector<NTL::Vec<Ctxt>> encCoordinates;

    Point_(Skeys * sk, const vector<Binary>& coordinates);
    Point_(Skeys *sk, const vector<Vec < Ctxt> >& encCoordinates);
    
    //    explicit Point_(Skeys &sk, vector<Binary>   coordinates);
    Point_(const Point_ &p);
    
    ~Point_();

//    vector<Binary> getCoordinates() const;
    Vec <Ctxt> operator[](int idx) { return encCoordinates[idx]; }
    
    Vec <Ctxt> operator[](int idx) const { return encCoordinates[idx]; }
    
    friend std::ostream &operator<<(std::ostream &os, const Point_ &p);

//    friend Point_ operator + (const Point_ &, const Point_ &);
    Point_ operator+(const Point_ &);
    
    friend Point_ operator+=(Point_ &, Point_ &);
    
    friend Point_ operator-(const Point_ &, const Point_ &);
    
    friend Point_ operator-=(Point_ &, const Point_ &);

//    friend Point_ operator*(const Point_ &, const Bit &);
//    friend Point_ operator / (const Point_ &p, const Binary &i);
    friend Bit operator>(const Point_ &p1, const Point_ &p2);
    
    friend Bit operator>=(const Point_ &p1, const Point_ &p2); //TODO
    Point_ &operator=(const Point_ &p1); //todo consider returning  Point_&
//    Point_ & operator = ( Point_ p1); //todo consider returning  Point_&
//    static Point_ dummyPoint_(Skeys &sk);
    
    //TODO consider using the function below instead of getDistFromClosestMeanByClient in aux.h
    //friend double getDistFromClosestMean(vector<DecryptedPoint_>); {
    //    return 0;
    //}


//// -------------------------- for DBG --------------------------
    friend class Point__test;
    
};

class Point_Extended_old : public Point_ { // like Point_ with ctor from long instead of Ctxt
public:
    long lcoordinates;
    
};

#endif //TRY_Point__OLD_H
