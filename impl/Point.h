// Created by rbd on 15.3.2020.
#ifndef TRY_POINT_H
#define TRY_POINT_H

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

//mine
#include <vector>
#include "KeysServer.h"

using namespace std;

class Point {
    /** params **/
    KeysServer * keysServer; //not needed since can get it from pubKey
    FHEPubKey * pubKey;
    vector<long> deCoor; // for DBG
public:
    vector<Vec<Ctxt> > eCoordinates;
    
    /** methods **/
private:
    //other kind of ctor?
public:
    Point(KeysServer * keysServer, vector<Vec<Ctxt> > eCoordinates);
    Point operator+(const Point &) const;
    Vec <Ctxt> operator[](int idx) { return eCoordinates[idx]; }
    Vec <Ctxt> operator[](int idx) const { return eCoordinates[idx]; }
    /** for DBG **/
    friend std::ostream & operator<<(std::ostream & os, const Point & p);
protected:
    virtual void print(ostream & os) const;
    
};

class PointExtended : public Point{
protected:
    vector<long> coordinates;
public:
    PointExtended(KeysServer * keysServer1, const vector<long>& eCoordinates1);
    /** for DBG **/
    void print(ostream & os) const override;
};

#endif //TRY_POINT_H
