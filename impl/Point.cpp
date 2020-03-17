//
// Created by rbd on 15.3.2020.
//

#include "Point.h"
#include "properties.h"

//#include <utility>

/** Constructor, creates a Point from a vector of encrypted coordinates **/
Point::Point(KeysServer * keysServer, vector<Vec<Ctxt> > eCoordinates) // decrypt and assign coor
        : keysServer(keysServer), pubKey(keysServer->pubKey), eCoordinates(std::move(eCoordinates)) {}

Point::Point(FHEPubKey * pubKey, vector<Vec<Ctxt> > eCoordinates)
        : pubKey(pubKey), eCoordinates(std::move(eCoordinates)) {}


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
        /** for DBG #1  - todo - remove
        vector<long> slots;
        decryptBinaryNums(slots, eep, *(keysServer->secKey), *(keysServer->pubKey->getContext().ea));
//        cout << "score! " << slots[0] << endl;
         END for DBG #1**/
    }
    Point sumPoint = Point(pubKey, sum);
//    Point sumPoint = Point(keysServer, sum);
    /** for DBG #2 - todo - remove
    cout << "The dec sum Point is: ( ";
    for(auto c : sumPoint.eCoordinates) {
//        Vec<Ctxt> eSum;
        CtPtrs_VecCt eep(c);
        vector<long> slots;
        decryptBinaryNums(slots, eep, *(keysServer->secKey), *(keysServer->pubKey->getContext().ea));
        cout << slots[0] << " ";
    }
    cout << ")" << endl;
     END for DBG #2**/
    return sumPoint; //TODO should return the point with the encCoor
}

Point Point::operator-(const Point &p) const {
//    new coor-vec
    vector<Vec<Ctxt>> pCoordinates = p.eCoordinates;
//    for every coor i
    for(int i = 0; i < DIM; ++i) {//(EncNumber c : eCoordinates) {
//      new coor
        Vec<Ctxt> newCoor;
//      for every bit j
        for(int j = 0; j < BIT_SIZE; ++j) {
//          b = this[i][j] - p[i][j]
            pCoordinates[i][j]-=eCoordinates[i][j];
//          push new   bit   into new   coor
        }
//      push new   coor   into new   coor-vec
    }
//    create & return new point
    Point subPoint = Point(pubKey, pCoordinates);
    cout << "sumPoint.decrypt: " << subPoint.decrypt(*keysServer) << endl;
    return subPoint;
//    return Point(pubKey, pCoordinates); //TODO should return the point with the encCoor
}

Point Point::operator*(const Ctxt & bit) const{
    vector<Vec<Ctxt>> newCoordinates = eCoordinates;
    int i = 0;
    for(EncNumber & c : newCoordinates) {
//        Vec<Ctxt> newCoor;
//        int nBits = (outSize>0 && outSize<2*BIT_SIZE)? outSize : (2*BIT_SIZE);
        //fixme - high potensial for BUG, newCoordinates.bitsize can be bigger than BIT_SIZE
//        for(auto & ct : c) ct.multiplyBy(bit);  // multiplies each bit of the coordinate
//        cout << "-- i --: "  << i++ << endl;
        for(Ctxt & ct : c) {
//            multTwoNumbers();//TODO
            ct *= bit;  // multiplies each bit of the coordinate
//            newCoor.append(ct);
//            EncNumber ctct; ctct.append(ct);
//            cout << "ct * bit: " << decryptNumber(*keysServer, ctct) << endl;
        }
//        newCoordinates.push_back(c);
//        cout << "newCoor: " << decryptNumber(*keysServer, newCoor) << endl;
//        newCoordinates.push_back(newCoor);
//        /** for DBG #1  - todo - remove**/
//        vector<long> slots;
//        cout << "11 " << endl;
//        decryptBinaryNums(slots, CtPtrs_VecCt(c), *(keysServer->secKey), *(keysServer->pubKey->getContext().ea));
//        decryptBinaryNums(slots, CtPtrs_VecCt(c), *(keysServer->secKey), *(keysServer->pubKey->getContext().ea));
//        cout << "11 " << endl;
//        cout << "score! " << slots[0] << endl;
        /* END for DBG #1*/
    }
//    Point sumPoint = Point(keysServer, newCoordinates);

    Point sumPoint = Point(pubKey, newCoordinates);
    cout << "sumPoint.decrypt: " << sumPoint.decrypt(*keysServer) << endl;
    /** for DBG #2 - todo - remove
    cout << "The dec prodPoint Point is: ( ";
    for(Vec<Ctxt> c : sumPoint.eCoordinates) {
//        Vec<Ctxt> eSum;
        CtPtrs_VecCt eep(c);
        vector<long> slots;
        decryptBinaryNums(slots, eep, *(keysServer->secKey), *(keysServer->pubKey->getContext().ea));
        cout << slots[0] << " ";
    }
    cout << ")" << endl;
     END for DBG #2**/
    return sumPoint; //TODO should return the point with the encCoor
}

Ctxt Point::operator>(const Point & p) const {
    vector<Vec<Ctxt> > p1dec = eCoordinates, p2dec = p.eCoordinates;
    Ctxt mu(*pubKey), ni(*pubKey);
    vector<long> slotsMin, slotsMax, slotsMu, slotsNi;
    Vec<Ctxt> eMax, eMin, enca = p1dec[DIM - 1], encb = p2dec[DIM - 1];
    compareTwoNumbers(mu, ni, CtPtrs_VecCt(enca), CtPtrs_VecCt(encb), &unpackSlotEncoding);
    /** for DBG #2 - todo - remove
    Vec<Ctxt> mumu,nini;
    mumu.append(mu);
    nini.append(ni);
    vector<long> slots1, slots2;
    cout << "The dec mumu Point is: ( ";
    decryptBinaryNums(slots1, CtPtrs_VecCt(mumu), *(keysServer->secKey), *(keysServer->pubKey->getContext().ea));
    cout << slots1[0] << " ";
    cout << ")" << endl;
    cout << "The dec nini Point is: ( ";
    decryptBinaryNums(slots2, CtPtrs_VecCt(nini), *(keysServer->secKey), *(keysServer->pubKey->getContext().ea));
    cout << slots2[0] << " ";
    cout << ")" << endl;
     END for DBG #2**/
    return mu; // todo check results
//    return mu && !ni; // todo check results
    //! Convenience method: XOR and nXOR with arbitrary plaintext space:
    //! a xor b = a+b-2ab = a + (1-2a)*b,
    //! a nxor b = 1-a-b+2ab = (b-1)(2a-1)+a
//    void xorConstant(const DoubleCRT& poly, double size=-1.0)

}

PointExtended::PointExtended(FHEPubKey * pubKey, const vector<long> & coordinates)
        : Point(pubKey, vector<Vec<Ctxt> >()), coordinates(coordinates) {
    for(long c : coordinates) {
        NTL::Vec<Ctxt> encVal;
        Ctxt mu(*pubKey);
        resize(encVal, BIT_SIZE, mu);
        for(long i = 0; i < BIT_SIZE; i++) {
            pubKey->Encrypt(encVal[i], ZZX((c >> i)&1)); ////    <----   THE PROBLEM was HERE
        }
        eCoordinates.push_back(encVal);
    }
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

DecryptedPoint Point::decrypt(KeysServer & keysServer) {
    DecryptedPoint dp;
    for(EncNumber c : eCoordinates) {
        CtPtrs_VecCt eep(c);
        vector<long> slots;
        //todo replace with keysServer.decrypt - secKey should not be public
        decryptBinaryNums(slots, eep, *(keysServer.secKey), *(pubKey->getContext().ea));
        dp.push_back(slots[0]);
    }
//    cout << "The dec Point is: ";
//    cout << dp << endl;
    return dp;
}

long Point::decryptNumber(KeysServer & keysServer, EncNumber c) {
//    DecryptedPoint dp;
//    for(EncNumber c : eCoordinates) {
    CtPtrs_VecCt eep(c);
    vector<long> slots;
    //todo replace with keysServer.decrypt - secKey should not be public
    decryptBinaryNums(slots, eep, *(keysServer.secKey), *(pubKey->getContext().ea));
//        dp.push_back(slots[0]);
//    }
//    cout << "The dec Point is: ";
//    cout << dp << endl;
    return slots[0];
}


void PointExtended::print(ostream & os) const {
    cout << " (" << coordinates[0];
    for(size_t i = 1; i < coordinates.size(); ++i) cout << ", " << coordinates[i];
    cout << ") ";
}

