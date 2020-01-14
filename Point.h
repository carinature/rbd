//
// Created by rbd on 24.12.2019.
//

#ifndef TRY_POINT_H
#define TRY_POINT_H

#include "Binary.h"

class Point {
//    Binary coordinate[];
//    Binary coordinates[];
//    Binary * coordinates;
//    Binary coordinates[DIM];
    std::vector<Binary> coordinates;

public:
    Point();
    Point(int);
    Point(std::vector<Binary>);

    Binary getCoordinate(int) const;
    std::vector<Binary> getCoordinates();

    friend Point operator + (Point &, Point &);
    friend std::ostream& operator<<(std::ostream& os, const Point& p);

};


#endif //TRY_POINT_H
