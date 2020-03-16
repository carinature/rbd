// Created by rbd on 24.12.2019.
#include <iostream>
#include <utility>
#include <vector>
#include "Point_old.h"

#include "../impl/aux.h"
#include "../impl/Skeys.h"

//Point::Point(Skeys &sk, const vector<Binary>& coordinates) : coordinates(coordinates), sk(&sk) {
//Point::Point(Skeys * sk, const vector<Binary>& coordinates){
Point::Point(Skeys *sk, const vector<Binary> &coordinates) : sk(sk), coordinates(coordinates) {
/*//    coordinates(std::move(coordinates));
//    this->coordinates = coordinates; //for DBG todo remove
//    cout << coordinates << endl;
//    this->sk = sk;*/
    for(Binary c : coordinates) {
        NTL::Vec<Ctxt> encVal;
        FHEPubKey *pubKey = (FHEPubKey *) sk->getPubKey();
        Ctxt mu(*pubKey);
        resize(encVal, BIT_SIZE, mu);
        for(long i = 0; i < BIT_SIZE; i++) {
            pubKey->Encrypt(encVal[i], ZZX((c >> i)&1)); ////    <----   THE PROBLEM was HERE
//                (*pubKey).Encrypt(encVal[i], ZZX((c >> i)&1));
        }
        encCoordinates.push_back(encVal);
    }
}

Point::Point(Skeys *sk, const vector<Vec<Ctxt> > &encCoordinates) : sk(sk), encCoordinates(encCoordinates) {
    // make sure encCoor are available / alive
    //todo :
    // decrypt and assign coor
//    for (auto ec : encCoordinates){
//        long dc = sk->decryptPointByCA(ec);
//        coordinates.push_back(dc)
    }


//Point::Point(const Point &p) : coordinates(p.getCoordinates()), sk(p.sk) {
//    cout << "Point::Point(Point &p)" << endl;
//}

//Point::~Point() {
////    cout << "Point::~Point()" << endl;
//}

//std::vector<Binary> Point::getCoordinates() const { return coordinates; }

std::ostream &operator<<(std::ostream &os, const Point &p) {
    os << " ep(" << p.getCoordinates()[0];
    for(size_t i = 1; i < p.coordinates.size(); ++i) os << ", " << p.getCoordinates()[i];
    os << ") ";
    return os;
}

//Point operator*(const Point &p1, const Bit &b) {
////todo consider std::transform(myv1.begin(), myv1.end(), myv1.begin(),std::bind1st(std::multiplies<T>(), 3));
//    vector<Binary> temp;
//    for(Binary &coor : p1.getCoordinates()) temp.push_back(coor * b);
////    Point pointOrNot = Point(*(p1.sk), temp);
//    Point pointOrNot = Point(p1.sk, temp);
//    return pointOrNot;
//}

//Point operator+(const Point &p1, const Point &p2) {
Point Point::operator+(const Point &p2) {
    vector<Vec<Ctxt>> sum;
    for(size_t i = 0; i < this->encCoordinates.size(); ++i) {
        Vec<Ctxt> sumVec = (*this)[i]; //todo check shallow or deep copy?
        const Vec<Ctxt> &p2vec = p2[i];
        for(int j = 0; j < sumVec.length(); ++j) {
            sumVec[j] += p2vec[j];
        }
        sum.push_back(sumVec);
    }
//    vector<Binary> decTemp;
//    for(int i = 0; i < this->getCoordinates().size(); ++i)
//        decTemp.push_back(this->getCoordinates()[i] + p2.coordinates[i]);
    Point sumPoint = Point(this->sk, sum);
//    sumPoint.encCoordinates = sum; //todo check shallow or deep copy
//    Point eSumPoint = Point(this->sk, sum);
//    return eSumPoint; //TODO should return the point with the encCoor
    return sumPoint; //TODO should return the point with the encCoor
}

Point operator+=(Point &p1, Point &p2) {
    std::vector<Binary> temp;
//    for (size_t i=0; i < p1.coordinates.size(); ++i) p1[i] += p2[i];
    return Point(p1.sk, temp);
//    return Point(*(p1.sk), temp);
}

Point operator-(const Point &p1, const Point &p2) {
    std::vector<Binary> temp;
//    for (size_t i=0; i < p1.coordinates.size(); ++i) temp.push_back(p1[i] - p2[i]);
//    return Point(*(p1.sk), temp);
    return Point(p1.sk, temp);
}

Point operator-=(Point &p1, const Point &p2) {
    std::vector<Binary> temp;
//    for (size_t i=0; i < p1.coordinates.size(); ++i) p1[i] -= p2[i];
//    return Point(*(p1.sk), temp);
    return Point(p1.sk, temp);
}

Point operator/(const Point &p1, const Binary &b) {
    //todo consider std::transform(myv1.begin(), myv1.end(), myv1.begin(),std::bind1st(std::multiplies<T>(), 3));
    vector<Binary> coorVector;
    for(const Binary &coor : p1.getCoordinates()) {
        coorVector.push_back(coor / b);
    }
    Point avgPoint = Point(p1.sk, coorVector);
//    Point avgPoint =  Point(*(p1.sk), coorVector);
    return avgPoint;
}

Bit operator>(const Point &p1, const Point &p2) {
    //TODO helib copmare
    vector<Binary> p1dec = p1.getCoordinates(), p2dec = p2.getCoordinates();
//    for (size_t    i      =0; i < p1.coordinates.size(); ++i) decTemp.push_back(p1dec[i] + p2coor[i]);
//    return p1dec[DIM-1] >= p2dec[DIM-1];
    
    FHEPubKey *pk = p1.sk->pubKey;
    Ctxt mu(*pk), ni(*pk);
//    Ctxt           mu(pk), ni(pk);
    vector<long> slotsMin, slotsMax, slotsMu, slotsNi;
    NTL::Vec<Ctxt> eMax, eMin, enca = p1[DIM - 1], encb = p2[DIM - 1];
    //cmp only
//        compareTwoNumbers(mu, ni, CtPtrs_VecCt(enca), CtPtrs_VecCt(encb), &unpackSlotEncoding);
    compareTwoNumbers(mu, ni, CtPtrs_VecCt(enca), CtPtrs_VecCt(encb), &unpackSlotEncoding);
    
    NTL::Vec<Ctxt> mumu;
    mumu.append(mu);
    NTL::Vec<Ctxt> nini;
    nini.append(ni);
//    const EncryptedArray &ea = *(p1.sk->pubKey->getContext().ea);
    int aa = p1.sk->decryptVec(mumu);
    int bb = p1.sk->decryptVec(nini);
//    cout << "mumu: " << aa << endl;
//    cout << "nini: " << bb << endl;
    return Bit(aa && !bb); // todo check results
    
}
//Bit operator >= (const Point &p1, const Point &p2) { TODO

Point &Point::operator=(const Point &p1) {
    sk = p1.sk;
    encCoordinates = p1.encCoordinates; //todo deep copy?
//    coordinates = p1.getCoordinates();
    return *this;
}



//Point Point::dummyPoint(Skeys &sk) {  // TODO sounds like bad practice - will the user/alg know the encrypted point equals zero?
//    const vector<Binary> zeroVector = vector<Binary>(DIM, 0);
////    auto zeroVector = new vector<Binary>(DIM, 0);
//    Point p = Point(sk, zeroVector);
//    return p;
//}

//Point::Point(Skeys &sk) : sk(sk){
//    cout << "Point::Point(Skeys &sk)" << endl;
//}

