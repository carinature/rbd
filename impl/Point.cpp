

#include "Point.h"
#include "properties.h"

//#include <utility>

/** Constructor, creates a Point from a vector of encrypted coordinates **/
Point::Point(vector<Vec<Ctxt> > eCoordinates, KeysServer * keysServer) // decrypt and assign coor
        : keysServer(keysServer), pubKey(keysServer->pubKey), eCoordinates(std::move(eCoordinates)), id(counter++) {}

Point::Point(vector<Vec<Ctxt> > eCoordinates, FHEPubKey * pubKey)
        : pubKey(pubKey), eCoordinates(std::move(eCoordinates)), id(counter++) {}


/** Addition of 2 encrypted points, each coordinate with the corresponding one,
 *      using helib's addTwoNumbers
 * Returns a sum Point, with the same keysServer **/
Point Point::operator+(const Point & p) const {
    vector<Vec<Ctxt>> sum;
    for(size_t i = 0; i < this->eCoordinates.size(); ++i) {
//        cout << p << endl;
//        cout << *this << endl;
//        cout << "eCoordinates" << endl;
//        cout << p.eCoordinates << endl;
//        cout << eCoordinates << endl;
        
        Vec<Ctxt> coor1 = eCoordinates[i], coor2 = p[i], eSum;
//        Vec<Ctxt> coor1 = (*this)[i], coor2 = p[i], eSum;

//        cout << "decryptNumber" << endl;
//        cout << decryptNumber(*keysServer, coor1) << endl;
//        cout << decryptNumber(*keysServer, coor2) << endl;
        CtPtrs_VecCt eep(eSum);
//        cout << "here comes trouble" << endl;
//        int nBits = (outSize>0 && outSize<2*BIT_SIZE)? outSize : (2*BIT_SIZE);
        //fixme - high potensial for BUG, sum.bitsize can be bigger than BIT_SIZE
//        addTwoNumbers(eep, CtPtrs_VecCt(coor1), CtPtrs_VecCt(coor2), BIT_SIZE, &unpackSlotEncoding);
        addTwoNumbers(eep, CtPtrs_VecCt(coor1), CtPtrs_VecCt(coor2));//, BIT_SIZE, &unpackSlotEncoding);
//        cout << "5" << endl;
        sum.push_back(eSum);
        /** for DBG #1  - todo - remove
        vector<long> slots;
        decryptBinaryNums(slots, eep, *(keysServer->secKey), *(keysServer->pubKey->getContext().ea));
//        cout << "score! " << slots[0] << endl;
         END for DBG #1**/
    }
//    cout << "6" << endl;
    Point sumPoint = Point(sum, pubKey);
//    Point sumPoint = Point(keysServer, sum);
//    cout << "7" << endl;
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

Point Point::operator*(Ctxt bit) const {
    vector<Vec<Ctxt>> prodPoint = eCoordinates;
    vector<Vec<Ctxt>> newprodPoint;
    for(EncNumber & c : prodPoint) {
//        int nBits = (outSize>0 && outSize<2*BIT_SIZE)? outSize : (2*BIT_SIZE);
        //fixme - high potensial for BUG, prodPoint.bitsize can be bigger than BIT_SIZE
        for(auto & ct : c) ct.multiplyBy(bit);  // multiplies each bit of the coordinate
//        for(Ctxt & ct : c) ct *= bit;  // multiplies each bit of the coordinate
        newprodPoint.push_back(c);
    }
//    Point mulPoint = Point(keysServer, prodPoint);
    Point mulPoint = Point(newprodPoint, pubKey);
    
    return mulPoint; //TODO should return the point with the encCoor
/*    vector<Vec<Ctxt>> newCoordinates = eCoordinates;
//    int i = 0;
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

    }
//    Point mulPoint = Point(keysServer, newCoordinates);
    
    Point mulPoint = Point(pubKey, newCoordinates);

    return mulPoint; //TODO should return the point with the encCoor*/
}

Ctxt Point::operator>(const Point & p) const {
    Ctxt mu(*pubKey), ni(*pubKey);
    if(p.id == this->id) { //fixme
        pubKey->Encrypt(mu, ZZX(1&1)); //fixme
        return mu;
    }
    vector<Vec<Ctxt> > p1dec = eCoordinates, p2dec = p.eCoordinates;
    vector<long> slotsMin, slotsMax, slotsMu, slotsNi;
    Vec<Ctxt> eMax, eMin, enca = p1dec[DIM - 1], encb = p2dec[DIM - 1];
    compareTwoNumbers(mu, ni, CtPtrs_VecCt(enca), CtPtrs_VecCt(encb), &unpackSlotEncoding);
//    mu.nxorConstant(DoubleCRT(ZZX(p.id == this->id),*(keysServer->context),mu.getPrimeSet()));
    return mu;

//    return mu && !ni; // todo check results
    //! Convenience method: XOR and nXOR with arbitrary plaintext space:
    //! a xor b = a+b-2ab = a + (1-2a)*b,
    //! a nxor b = 1-a-b+2ab = (b-1)(2a-1)+a
//    void xorConstant(const DoubleCRT& poly, double size=-1.0)

}

Point Point::operator-(const Point & p) const { //fixme
    vector<Vec<Ctxt>> sum;
    for(size_t i = 0; i < this->eCoordinates.size(); ++i) {
        Vec<Ctxt> coor1 = (*this)[i], coor2 = p[i], eSum;
        CtPtrs_VecCt eep(eSum);
//        int nBits = (outSize>0 && outSize<2*BIT_SIZE)? outSize : (2*BIT_SIZE);
        //fixme - high potensial for BUG, sum.bitsize can be bigger than BIT_SIZE
//        addTwoNumbers(eep, CtPtrs_VecCt(coor1), CtPtrs_VecCt(coor2), BIT_SIZE, &unpackSlotEncoding);
        addTwoNumbers(eep, CtPtrs_VecCt(coor1), CtPtrs_VecCt(coor2));//, BIT_SIZE, &unpackSlotEncoding);
        sum.push_back(eSum);
        /** for DBG #1  - todo - remove
        vector<long> slots;
        decryptBinaryNums(slots, eep, *(keysServer->secKey), *(keysServer->pubKey->getContext().ea));
//        cout << "score! " << slots[0] << endl;
         END for DBG #1**/
    }
    Point sumPoint = Point(sum, pubKey);
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
/*//    new coor-vec
    vector<Vec<Ctxt>> pCoordinates = p.eCoordinates;
//    for every coor i
    for(int i = 0; i < DIM; ++i) {//(EncNumber c : eCoordinates) {
//      new coor
        Vec<Ctxt> newCoor;
//      for every bit j
        for(int j = 0; j < BIT_SIZE; ++j) {
//          b = this[i][j] - p[i][j]
            pCoordinates[i][j], eCoordinates[i][j].negate();
//          push new   bit   into new   coor
        }
//      push new   coor   into new   coor-vec
    }
//    create & return new point
    Point subPoint = Point(pubKey, pCoordinates);
    cout << "subPoint.decrypt: " << subPoint.decrypt(*keysServer) << endl;
    return subPoint;
//    return Point(pubKey, pCoordinates); //TODO should return the point with the encCoor*/
}

PointExtended::PointExtended(const vector<long> & coordinates, FHEPubKey * pubKey)
        : Point(vector<Vec<Ctxt> >(), pubKey), coordinates(coordinates) {
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
PointExtended::PointExtended(const vector<long> & coordinates, KeysServer * keysServer)
        : Point(vector<Vec<Ctxt> >(), keysServer), coordinates(coordinates) {
    for(long c : coordinates) {
        EncNumber encVal;
        Ctxt mu(*pubKey);
        resize(encVal, BIT_SIZE, mu);
        for(long i = 0; i < BIT_SIZE; i++) {
            pubKey->Encrypt(encVal[i], ZZX((c >> i)&1)); ////    <----   THE PROBLEM was HERE
        }
        eCoordinates.push_back(encVal);
    }
}

//PointExtended::PointExtended(KeysServer * keysServer, vector<long> & coordinates)
//        : Point(keysServer, vector<Vec<Ctxt> >()), coordinates(coordinates) {
//    for(long c : coordinates) {
//        NTL::Vec<Ctxt> encVal;
//        FHEPubKey * pubKey = (FHEPubKey *) keysServer->pubKey;
//        Ctxt mu(*pubKey);
//        resize(encVal, BIT_SIZE, mu);
//        for(long i = 0; i < BIT_SIZE; i++) {
//            pubKey->Encrypt(encVal[i], ZZX((c >> i)&1)); ////    <----   THE PROBLEM was HERE
//        }
//        eCoordinates.push_back(encVal);
//    }
//}

/** for DBG **/

void Point::print(ostream & os) const {
    cout << " ep(" << eCoordinates[0]._vec__rep;
    for(size_t i = 1; i < eCoordinates.size(); ++i) cout << ", " << eCoordinates[i]._vec__rep;
    cout << ") ";
}

std::ostream & operator<<(std::ostream & os, const Point & p) {
    p.print(os);
}

DecryptedPoint Point::decrypt(KeysServer & keysServ) const {
    DecryptedPoint dp;
    for(EncNumber c : eCoordinates) {
        CtPtrs_VecCt eep(c);
        vector<long> slots;
        //todo replace with keysServ.decrypt - secKey should not be public
//        decryptBinaryNums(slots, eep, *(keysServer->secKey), *(pubKey->getContext().ea)); // <---- throes exception - KeysServer of this is NULL!! why??
//        dp.push_back(slots[0]);
        long decC = keysServ.decrypt(c);
        dp.push_back(decC);
    }
//    cout << "The dec Point is: ";
//    cout << dp << endl;
    return dp;
}

long Point::decryptNumber(KeysServer & keysServ, EncNumber c) const {
//    DecryptedPoint dp;
//    for(EncNumber c : eCoordinates) {
    CtPtrs_VecCt eep(c);
    vector<long> slots;
    //todo replace with keysServ.decrypt - secKey should not be public
    decryptBinaryNums(slots, eep, *(keysServ.secKey), *(pubKey->getContext().ea));
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

long PointExtended::dist(DecryptedPoint & point) {
    long d = 0;
//    cout << "d0: " << d << endl;
//    cout << "coordinates: " << coordinates << endl;
    for(int i = 0; i < DIM; ++i) {
        d += pow(coordinates[i] - point[i], 2);
//        cout << "--" << i << "--  d: " << d << endl;
    }
//    cout << "d: " << d << endl;
    return d;  // notice this returns the SQUARE of the distance
}

EncNumber PointExtended::getDistanceFromClosestPoint(vector<DecryptedPoint> points) {
//    cout << "getDistanceFromClosestPoint means: " << points << endl;
    long min = dist(points[0]), currDist;
    DecryptedPoint closest = points[0]; //for dbg
//    cout << "min: " << min <<  "    closest: " << closest << endl;
    for(DecryptedPoint point : points) {
        currDist = dist(point);
        if(currDist < min) {
            min = currDist;
            closest = point; //for dbg
        }
    }
    if(dbg) cout << "dist from " << coordinates << " to " << closest << " is: " << min << endl;
    EncNumber minDist;
    Ctxt mu(*pubKey);
    resize(minDist, BIT_SIZE, mu);
    for(long i = 0; i < BIT_SIZE; i++) {
        pubKey->Encrypt(minDist[i], ZZX((min >> i)&1));
    }
    return minDist;
}

/*
 * Deprecated
 * */
/*
//this is not a real cmp between points - op< is used in std::map
Ctxt Point::operator<(const Point & p) const {


}
*/