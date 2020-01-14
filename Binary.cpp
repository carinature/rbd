//
// Created by rbd on 24.12.2019.
//
#include <iostream>
#include "Binary.h"
#include "Bit.h"

using namespace std;

Binary::Binary(){
//    cout << "Binary empty: "<< endl;
}

Binary::Binary(double value): value(value){
//        cout << "Binary value: " << this->value << endl;
}



//// -------------------------- for DBG --------------------------
//int getvalue(){
//    return this->value;
//}
double Binary::decryptVal() const {
    return this->value;
}


Binary operator+(const Binary &b1, const Binary &b2) {
    return {b1.decryptVal() + b2.decryptVal()};
}

std::ostream& operator<< (std::ostream& os, const Binary& b) {
    os << b.decryptVal();
    return os;
}