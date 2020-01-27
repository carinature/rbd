//
// Created by rbd on 13.1.2020.
//

#include "aux.h"


Point getAvgPointFromClient(Point point, int amount) {
    try {
//        tag(point);
//        tag(amount);
//        tag(point/amount);
        return point/amount;
    }
    catch(...){
        return Point::zeroPoint();
    }
}

Bit cmp(const Point& a, const Point& b) {
    //todo cmp should return bit
    //todo override Point's operator<
    //todo use helib cmp
    //todo consider <= over < (the later neve allows any points) and how to implement it with helib
    return a >= b;
//    return a[1] <= b[1];
}

void tag(){
    cout << "---------" << endl;
}

void tag(int i){
    cout << "----"<<i<<"----" << endl;
}

void tag(string i){
    cout << "----"<<i<<"----" << endl;
}

void tag(Point i) {
    cout << "----"<<i<<"----" << endl;
}
