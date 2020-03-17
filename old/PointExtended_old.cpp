//
// Created by rbd on 15.3.2020.
//

#include "PointExtended_old.h"

PointExtended_old::PointExtended_old(KeysServer * keysServer, const vector<long>& coordinates)
: Point(keysServer->pubKey, vector<Vec<Ctxt> >()), coordinates(coordinates) {
//: Point(keysServer, vector<Vec<Ctxt> >()), coordinates(coordinates) {
    //todo
    // encrypt coor
    // init eCoor field of *this
    for(long c : coordinates) {
        NTL::Vec<Ctxt> encVal;
        FHEPubKey *pubKey = (FHEPubKey *) keysServer->pubKey;
        Ctxt mu(*pubKey);
        resize(encVal, BIT_SIZE, mu);
        for(long i = 0; i < BIT_SIZE; i++) {
            pubKey->Encrypt(encVal[i], ZZX((c >> i)&1)); ////    <----   THE PROBLEM was HERE
//                (*pubKey).Encrypt(encVal[i], ZZX((c >> i)&1));
        }
        eCoordinates.push_back(encVal);
    }
}



void PointExtended_old::print(ostream & os) const {
//    cout << "PointExtended_old" << endl;
    cout << " (" << coordinates[0];
    for(size_t i = 1; i < coordinates.size(); ++i) cout << ", " << coordinates[i];
    cout << ") ";
//    return os;
}

