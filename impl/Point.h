// Created by rbd on 15.3.2020.
#ifndef TRY_PointH
#define TRY_PointH

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

static long counter = 0;

/**
 * @class Point
 * @brief Representation of user point: d encrypted coordinates and a ptr to the CA
 ********************************************************************/
class Point {
    //! @var KeysServer * keysServer
    //! points to the corresponding CA
    KeysServer * keysServer{}; //CA - the keys Server

    vector<long> deCoor; /** for DBG **/

protected:
    //! @var Ctxt pubEncrKey
    //! "The public encryption key is an encryption of 0,
    //! relative to the first secret key"
    FHEPubKey * pubKey;  // logic - future use  //todo should be public?
    
    // other kind of ctor?

public:
    long id; // = 0;  //fixme

    //! @var vector<Vec<Ctxt> > eCoordinates
    //! The encrypted coordinates vector (each coor is Vec<Ctxt>)
    vector<Vec<Ctxt> > eCoordinates;
    
    Point(vector<Vec<Ctxt> > eCoordinates, KeysServer * keysServer);
    
    Point(vector<Vec<Ctxt> > eCoordinates, FHEPubKey * pubKey);
    
    //! @brief _Encrypted_ addition of coordinates
    Point operator+(const Point &) const;
    Point operator-(const Point &) const;
    
    //! @brief _Encrypted_ multiplication of coordinates
    Point operator*(const Ctxt& bit) const;
    
    //! @brief compare 2 points
    Ctxt operator>(const Point & p) const;
//    Ctxt operator<(const Point & p) const;
    
    //! @brief The encrypted coordiante
    Vec<Ctxt> operator[](int idx) { return eCoordinates[idx]; }
    
    Vec<Ctxt> operator[](int idx) const { return eCoordinates[idx]; }
    
    /** for DBG **/
    DecryptedPoint decrypt(KeysServer & keysServ = *(new KeysServer())) const ;
    
    long decryptNumber(KeysServer & keysServ, EncNumber c) const ;
    
    friend std::ostream & operator<<(std::ostream & os, const Point & p);

protected:
    virtual void print(ostream & os) const;
    
};

/**
 * @class PointExtended
 * @brief Extention of class Point - allows init with plaintext (long) coordinates, and saves them
 ********************************************************************/
class PointExtended : public Point {
private:
    long dist(DecryptedPoint & point);

protected:
    //! @var vector<long> coordinates
    //! The plaintext coordinates vector (double represented as long)
    vector<long> coordinates; //todo check if overriding public/private is allowed

public:
    PointExtended(const vector<long> & coordinates, KeysServer * keysServer);
//    PointExtended(KeysServer * keysServer, vector<long> & coordinates);
    
    PointExtended(const vector<long> & coordinates, FHEPubKey * pubKey);
    //todo make c'tor private. make Skeys (or some class) a friend and create a factory method
    
    EncNumber getDistanceFromClosestPoint(vector<DecryptedPoint> points);

    EncNumber encryptNum(long min) const;
    
    /** for DBG **/
    void print(ostream & os) const override;
    
    
};


// Encryption and decryption are done by the friends FHE[Pub|Sec]Key

// TODO
//  addManyPoints - collect the points and use addManyNumbers (like you did with 'size')
//      - more efficient than on at a time, and probably more safe
//  subtraction
//  multi by negative
//  --maybe
//  dummy_Point (with random values)
//  copy c'tor
//  copy c'tor from and/or to  PointExtended
//  assignment
//  Encryption and decryption are done by the friends FHE[Pub|Sec]Key


#endif //TRY_PointH
