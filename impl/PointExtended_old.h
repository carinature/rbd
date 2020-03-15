//
// Created by rbd on 15.3.2020.
//

#ifndef TRY_POINTEXTENDED_OLD_H
#define TRY_POINTEXTENDED_OLD_H

//mine
#include "properties.h"
#include "Point.h"
#include <vector>

using namespace std;

class PointExtended_old : public Point {
protected:
    vector<long> coordinates;
public:
    void print(ostream & os) const override;
    PointExtended_old(KeysServer * keysServer1, const vector<long>& eCoordinates1);
};


#endif //TRY_POINTEXTENDED_OLD_H
