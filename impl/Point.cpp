//
// Created by rbd on 15.3.2020.
//

#include "Point.h"
#include "properties.h"

//#include <utility>\

Point::Point(KeysServer * keysServer, vector<Vec<Ctxt> > eCoordinates) : keysServer(keysServer),
        pubKey(keysServer->pubKey), eCoordinates(std::move(eCoordinates)) {
    //todo :
    // decrypt and assign coor
}


Point Point::operator+(const Point & p) const {
//    vector<Vec<Ctxt>> sum(p.eCoordinates);
//    for(size_t i = 0; i < this->eCoordinates.size(); ++i) {
////        const Vec<Ctxt> & c1 = (*this)[i]; //todo check shallow or deep copy?
//        const Vec<Ctxt> & c2 = p[i];
//        Vec<Ctxt> eSum(sum[i]);
    vector<Vec<Ctxt>> sum;
    for(size_t i = 0; i < this->eCoordinates.size(); ++i) {
        Vec<Ctxt> c1 = (*this)[i]; //todo check shallow or deep copy?
        Vec<Ctxt> c2 = p[i];
        Vec<Ctxt> eSum;
        CtPtrs_VecCt eep(eSum);  // A wrapper around the output vector
//        int nBits = (outSize>0 && outSize<2*BIT_SIZE)? outSize : (2*BIT_SIZE);
        //TODO - high potensial for BUG, sum.bitsize can be bigger than BIT_SIZE
        addTwoNumbers(eep, CtPtrs_VecCt(c1), CtPtrs_VecCt(c2), BIT_SIZE, &unpackSlotEncoding);
//        eSum
        sum.push_back(eSum);
        vector<long> slots;
        decryptBinaryNums(slots, eep, *(keysServer->secKey), *(keysServer->pubKey->getContext().ea));
        cout << "score! " << slots[0] << endl;
    }
    Point sumPoint = Point(keysServer, sum);
    return sumPoint; //TODO should return the point with the encCoor
}


PointExtended::PointExtended(KeysServer * keysServer, const vector<long> & coordinates)
        : Point(keysServer, vector<Vec<Ctxt> >()), coordinates(coordinates) {
    //todo
    // encrypt coor
    // init eCoor field of *this
    for(long c : coordinates) {
        NTL::Vec<Ctxt> encVal;
        FHEPubKey * pubKey = (FHEPubKey *) keysServer->pubKey;
        Ctxt mu(*pubKey);
        resize(encVal, BIT_SIZE, mu);
        for(long i = 0; i < BIT_SIZE; i++) {
            pubKey->Encrypt(encVal[i], ZZX((c >> i)&1)); ////    <----   THE PROBLEM was HERE
//                (*pubKey).Encrypt(encVal[i], ZZX((c >> i)&1));
        }
        eCoordinates.push_back(encVal);
    }
}


/** for DBG **/

void Point::print(ostream & os) const {
    cout << " ep(" << eCoordinates[0]._vec__rep;
    for(size_t i = 1; i < eCoordinates.size(); ++i) cout << ", " << eCoordinates[i]._vec__rep;
    cout << ") ";
}

std::ostream & operator<<(std::ostream & os, const Point & p) {
    p.print(os);
}

void PointExtended::print(ostream & os) const {
    cout << " (" << coordinates[0];
    for(size_t i = 1; i < coordinates.size(); ++i) cout << ", " << coordinates[i];
    cout << ") ";
}