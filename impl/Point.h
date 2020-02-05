//
// Created by rbd on 24.12.2019.
//

#ifndef TRY_POINT_H
#define TRY_POINT_H

#include "Binary.h"
#include <vector>

//#include "Bit.h" //todo add
using Bit = bool; //todo remove
using namespace std;

class Point {
    vector<Binary> coordinates;

public:
    Point();
//    Point(int);
    explicit Point(vector<Binary>);
    vector<Binary> getCoordinates() const;
    Binary& operator [] (int idx) { return coordinates[idx]; }
    const Binary& operator [] (int idx) const { return coordinates[idx]; }
    friend std::ostream& operator << (std::ostream& os, const Point& p);
    friend Point operator + (Point &, Point &);
    friend Point operator += (Point &, Point &);
    friend Point operator - (const Point &, const Point &);
    friend Point operator -= (Point &, const Point &);
    friend Point operator * (const Point &, const Bit &);
    friend Point operator / (const Point &p, const Binary &i);
    friend Bit operator >= (const Point &p1, const Point &p2);

    static Point dummyPoint();
    //TODO consider using the function below instead of getDistFromClosestMeanByClient in aux.h
    //friend double getDistFromClosestMean(vector<DecryptedPoint>); {
    //    return 0;
    //}

};


#endif //TRY_POINT_H
