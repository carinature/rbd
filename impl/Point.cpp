//
// Created by rbd on 15.3.2020.
//

#include "Point.h"
#include "properties.h"

//#include <utility>

/** Constructor, creates a Point from a vector of encrypted coordinates **/
Point::Point(KeysServer * keysServer, vector<Vec<Ctxt> > eCoordinates) : keysServer(keysServer),
        pubKey(keysServer->pubKey), eCoordinates(std::move(eCoordinates)) {
    //todo :
    // decrypt and assign coor
}

/** Addition of 2 encrypted points, each coordinate with the corresponding one,
 *      using helib's addTwoNumbers
 * Returns a sum Point, with the same keysServer **/
Point Point::operator+(const Point & p) const {
    vector<Vec<Ctxt>> sum;
    for(size_t i = 0; i < this->eCoordinates.size(); ++i) {
        Vec<Ctxt> c1 = (*this)[i], c2 = p[i], eSum;
        CtPtrs_VecCt eep(eSum);
//        int nBits = (outSize>0 && outSize<2*BIT_SIZE)? outSize : (2*BIT_SIZE);
        //fixme - high potensial for BUG, sum.bitsize can be bigger than BIT_SIZE
        addTwoNumbers(eep, CtPtrs_VecCt(c1), CtPtrs_VecCt(c2), BIT_SIZE, &unpackSlotEncoding);
        sum.push_back(eSum);
        /** for DBG #1  - todo - remove **/
        vector<long> slots;
        decryptBinaryNums(slots, eep, *(keysServer->secKey), *(keysServer->pubKey->getContext().ea));
//        cout << "score! " << slots[0] << endl;
        /** END for DBG #1**/
    }
    Point sumPoint = Point(keysServer, sum);
    /** for DBG #2 - todo - remove **/
    cout << "The dec sum Point is: ( ";
    for(auto c : sumPoint.eCoordinates) {
//        Vec<Ctxt> eSum;
        CtPtrs_VecCt eep(c);
        vector<long> slots;
        decryptBinaryNums(slots, eep, *(keysServer->secKey), *(keysServer->pubKey->getContext().ea));
        cout << slots[0] << " ";
    }
    cout << ")" << endl;
    /** END for DBG #2**/
    return sumPoint; //TODO should return the point with the encCoor
}

Point Point::operator*(const Ctxt & bit) {
    vector<Vec<Ctxt>> prodPoint;
    for(size_t i = 0; i < eCoordinates.size(); ++i) {
        Vec<Ctxt> c = eCoordinates[i];
//        int nBits = (outSize>0 && outSize<2*BIT_SIZE)? outSize : (2*BIT_SIZE);
        //fixme - high potensial for BUG, prodPoint.bitsize can be bigger than BIT_SIZE
        for (auto & ct : c) ct.multiplyBy(bit);  // multiplies each bit of the coordinate
        prodPoint.push_back(c);
        /** for DBG #1  - todo - remove **/
        vector<long> slots;
        decryptBinaryNums(slots, CtPtrs_VecCt(c), *(keysServer->secKey), *(keysServer->pubKey->getContext().ea));
//        cout << "score! " << slots[0] << endl;
        /** END for DBG #1**/
    }
    Point sumPoint = Point(keysServer, prodPoint);
    /** for DBG #2 - todo - remove **/
    cout << "The dec prodPoint Point is: ( ";
    for(Vec<Ctxt> c : sumPoint.eCoordinates) {
//        Vec<Ctxt> eSum;
        CtPtrs_VecCt eep(c);
        vector<long> slots;
        decryptBinaryNums(slots, eep, *(keysServer->secKey), *(keysServer->pubKey->getContext().ea));
        cout << slots[0] << " ";
    }
    cout << ")" << endl;
    /** END for DBG #2**/
    return sumPoint; //TODO should return the point with the encCoor
}

Ctxt Point::operator>(const Point & p) {
    vector<Vec<Ctxt> > p1dec = eCoordinates, p2dec = p.eCoordinates;
    Ctxt mu(*pubKey), ni(*pubKey);
    vector<long> slotsMin, slotsMax, slotsMu, slotsNi;
    Vec<Ctxt> eMax, eMin, enca = p1dec[DIM - 1], encb = p2dec[DIM - 1];
    compareTwoNumbers(mu, ni, CtPtrs_VecCt(enca), CtPtrs_VecCt(encb), &unpackSlotEncoding);
    
//    Vec<Ctxt> mumu,nini;
//    mumu.append(mu);
//    nini.append(ni);
//    /** for DBG #2 - todo - remove **/
//    vector<long> slots1, slots2;
//    cout << "The dec mumu Point is: ( ";
//    decryptBinaryNums(slots1, CtPtrs_VecCt(mumu), *(keysServer->secKey), *(keysServer->pubKey->getContext().ea));
//    cout << slots1[0] << " ";
//    cout << ")" << endl;
//    cout << "The dec nini Point is: ( ";
//    decryptBinaryNums(slots2, CtPtrs_VecCt(nini), *(keysServer->secKey), *(keysServer->pubKey->getContext().ea));
//    cout << slots2[0] << " ";
//    cout << ")" << endl;
//    /** END for DBG #2**/
    
    return mu; // todo check results
//    return mu && !ni; // todo check results
    //! Convenience method: XOR and nXOR with arbitrary plaintext space:
    //! a xor b = a+b-2ab = a + (1-2a)*b,
    //! a nxor b = 1-a-b+2ab = (b-1)(2a-1)+a
//    void xorConstant(const DoubleCRT& poly, double size=-1.0)

}

/** Constructor, creates a PointExtended from a vector of plaintext coordinates **/
PointExtended::PointExtended(KeysServer * keysServer, const vector<long> & coordinates)
        : Point(keysServer, vector<Vec<Ctxt> >()), coordinates(coordinates) {
    for(long c : coordinates) {
        NTL::Vec<Ctxt> encVal;
        FHEPubKey * pubKey = (FHEPubKey *) keysServer->pubKey;
        Ctxt mu(*pubKey);
        resize(encVal, BIT_SIZE, mu);
        for(long i = 0; i < BIT_SIZE; i++) {
            pubKey->Encrypt(encVal[i], ZZX((c >> i)&1)); ////    <----   THE PROBLEM was HERE
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

DecryptedPoint Point::decrypt() {
    DecryptedPoint dp;
    cout << "The dec Point is: ";
    for(auto c : eCoordinates) {
        CtPtrs_VecCt eep(c);
        vector<long> slots;
        //todo replace with keysServer.decrypt - secKey should not be public
        decryptBinaryNums(slots, eep, *(keysServer->secKey), *(keysServer->pubKey->getContext().ea));
        dp.push_back(slots[0]);
    }
    cout << dp << endl;
    return dp;
}

void PointExtended::print(ostream & os) const {
    cout << " (" << coordinates[0];
    for(size_t i = 1; i < coordinates.size(); ++i) cout << ", " << coordinates[i];
    cout << ") ";
}