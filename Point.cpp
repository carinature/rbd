//
// Created by rbd on 24.12.2019.
//
#include <iostream>
#include <vector>
#include "Point.h"
#include "Bit.h"
#include "aux.h"

using namespace std;

//  TODO
//    consider using std::array for 'coordinates' (template < class T, size_t N > class array;)

Point::Point(){
}
//Point::Point(int numOfCoordinate){
//    coordinate = Binary(0);
//    coordinate = {}(Binary(0));
//    cout << "Point this -> coordinate: " << this->coordinate.decryptVal() << endl;
//}

//Point::Point(Binary coordinates[]){
//    for (int i=0; i<DIM; ++i){
//        this->coordinates[i] = coordinates[i];
//    }
////    cout << "Point this -> coordinate: " << this->coordinates[0].decryptVal() << endl;
//}

Point::Point(std::vector<Binary> coordinates){
    tag("Point");

    this->coordinates = coordinates;
    tag("end Point");
//    for (int i=0; i<DIM; ++i){
//        this->coordinates[i] = coordinates[i];
//    }
//    cout << "Point this -> coordinate: " << this->coordinates[0].decryptVal() << endl;
}

Binary Point::getCoordinate(int i) const {
    return coordinates[i];
}
std::vector<Binary> Point::getCoordinates() {
    return coordinates;
}

Point operator + (Point & p1, Point & p2){
    std::vector<Binary> temp;
    for (int i; i<p1.coordinates.size(); ++i) temp[i] = p1.getCoordinate(i)+p2.getCoordinate(i);
    return Point(temp);
}

std::ostream& operator<<(std::ostream& os, const Point& p){
    os << "(" << p.getCoordinate(0);
    for (int i; i<p.coordinates.size(); ++i) os << ", " << p.getCoordinate(i);
    os << ")";
    return os;
}

