//
// Created by rbd on 24.12.2019.
//
#include <iostream>
#include <utility>
#include <vector>
#include "Point.h"

#include "Bit.h"

#include "aux.h"
#include "Skeys.h"

//using namespace std;

//  TODO
//    consider using std::array for 'coordinates' (template < class T, size_t N > class array;)

//Point::Point()= default;

//Point::Point(Skeys &sk, const vector<Binary>& coordinates) :  sk(sk) {
Point::Point(Skeys &sk, vector<Binary> coordinates) : coordinates(std::move(coordinates)), sk(sk) {
/*    cout << "Point(Skeys &sk, const vector<Binary>& coordinates) \n" << endl;
//    coordinates(std::move(coordinates));

    this->coordinates = coordinates; //for DBG todo remove
    for (Binary c : coordinates){
        NTL::Vec<Ctxt> encVal;
//        FHEPubKey pubKey = *(sk.getPubKey());
        FHEPubKey * pubKey = sk.getPubKey();
        cout << "sk.getPubKey()" << endl;
//        Ctxt mu(pubKey);
        Ctxt mu(*pubKey);
        resize(encVal, BIT_SIZE, mu);
        cout << "resize(encVal, BIT_SIZE, mu)" << endl;
        for(long i = 0; i <= BIT_SIZE; i++) {
            if(i < BIT_SIZE) {
                cout << "before pubKey->Encrypt" << endl;
//                (pubKey).Encrypt(encVal[i], ZZX((c >> i)&1)); ////    <----   THE PROBLEM IS HERE
//                (*pubKey).Encrypt(encVal[i], ZZX((c >> i)&1)); ////    <----   THE PROBLEM IS HERE
                pubKey->Encrypt(encVal[i], ZZX((c >> i)&1)); ////    <----   THE PROBLEM IS HERE
                cout << "AFTER pubKey->Encrypt" << endl;
            }
            cout << "333444" << endl;
    
            encCoordinates.push_back(encVal);
        }
        cout << "4444" << endl;
    }
    
    cout << "------------" << endl;
    Vec<Ctxt> ed = sk.encryptDouble(coordinates[0]);
    cout << "---- ed ---" << endl;
    cout << ed << endl;
    cout << "------------" << endl;*/
}

Point::Point( const Point &p) : coordinates(p.getCoordinates()), sk(p.sk) {
//    cout << "Point::Point(Point &p)" << endl;
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

std::vector<Binary> Point::getCoordinates() const { return coordinates; }

std::ostream& operator << (std::ostream& os, const Point& p){
    os << " ep(" << p[0];
    for (size_t i=1; i<p.coordinates.size(); ++i) os << ", " << p[i];
    os << ") ";
    return os;
}

Point operator * (const Point & p1, const Bit & b) {
//todo consider std::transform(myv1.begin(), myv1.end(), myv1.begin(),std::bind1st(std::multiplies<T>(), 3));
    vector<Binary> temp;
    for (Binary & coor : p1.getCoordinates()) temp.push_back(coor * b);
    Point pointOrNot = Point(p1.sk, temp);
    return pointOrNot;
}

Point operator + (Point & p1, Point & p2){
    std::vector<Binary> temp;
    for (size_t i=0; i < p1.coordinates.size(); ++i) temp.push_back(p1[i] + p2[i]);
    return Point(p1.sk, temp);
}

Point operator += (Point & p1, Point & p2){
    std::vector<Binary> temp;
    for (size_t i=0; i < p1.coordinates.size(); ++i) p1[i] += p2[i];
    return Point(p1.sk, temp);
}

Point operator-(const Point &p1, const Point &p2) {
    std::vector<Binary> temp;
    for (size_t i=0; i < p1.coordinates.size(); ++i) temp.push_back(p1[i] - p2[i]);
    return Point(p1.sk, temp);
}

Point operator-=(Point &p1, const Point &p2) {
    std::vector<Binary> temp;
    for (size_t i=0; i < p1.coordinates.size(); ++i) p1[i] -= p2[i];
    return Point(p1.sk, temp);
}

Point operator / (const Point &p1, const Binary &b) {
    //todo consider std::transform(myv1.begin(), myv1.end(), myv1.begin(),std::bind1st(std::multiplies<T>(), 3));
    vector<Binary> coorVector;
    for (const Binary &coor : p1.getCoordinates()){
        coorVector.push_back(coor / b);
    }
    Point avgPoint = Point(p1.sk, coorVector);
    return avgPoint;
}

Bit operator >= (const Point &p1, const Point &p2) { return p1[DIM-1] >= p2[DIM-1]; }

Point & Point::operator = (const Point &p1) {
    sk = p1.sk;
    coordinates = p1.getCoordinates();
    return *this;
}




