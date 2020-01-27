//
// Created by rbd on 24.12.2019.
//
#include <iostream>
#include <vector>
#include "Point.h"
#include "Bit.h"
#include "aux.h"

//using namespace std;

//  TODO
//    consider using std::array for 'coordinates' (template < class T, size_t N > class array;)

Point::Point(){
}

Point Point::zeroPoint(){
    const vector<Binary> &zeroVector = vector<Binary>(DIM, 0);
    return Point(zeroVector);
}

Point::Point(std::vector<Binary> coordinates) : coordinates(coordinates) {}

//Binary Point::getCoordinate(int i) const {
//    return coordinates[i];
//}
//std::vector<Binary> Point::getCoordinates() const {
//    return coordinates;
//}

std::vector<Binary> Point::getCoordinates() const { return coordinates; }

std::ostream& operator << (std::ostream& os, const Point& p){
    os << " ep(" << p[0];
    for (size_t i=1; i<p.coordinates.size(); ++i) os << ", " << p[i];
    os << ") ";
    return os;
}

Point operator * (const Point & p, const Bit & b) {
//todo consider std::transform(myv1.begin(), myv1.end(), myv1.begin(),std::bind1st(std::multiplies<T>(), 3));
    vector<Binary> temp;
    for (Binary & coor : p.getCoordinates()) temp.push_back(coor * b);
    Point pointOrNot = Point(temp);
    return pointOrNot;
}

Point operator + (Point & p1, Point & p2){
    std::vector<Binary> temp;
    for (size_t i=0; i < p1.coordinates.size(); ++i) temp.push_back(p1[i] + p2[i]);
    return Point(temp);
}

Point operator / (const Point &p, const int &i) {
    //todo consider std::transform(myv1.begin(), myv1.end(), myv1.begin(),std::bind1st(std::multiplies<T>(), 3));
    vector<Binary> coorVector;
    for (Binary & coor : p.getCoordinates()){
        coorVector.push_back(coor / i);
    }
    Point avgPoint = Point(coorVector);
    return avgPoint;
}

Bit operator >= (const Point &p1, const Point &p2) { return p1[DIM-1] >= p2[DIM-1]; }

