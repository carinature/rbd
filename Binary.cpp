//
// Created by rbd on 24.12.2019.
//
#include <iostream>
#include "Binary.h"

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

Binary operator + (const Binary &b1, const Binary &b2) {
    return {b1.decryptVal() + b2.decryptVal()};
}

std::ostream& operator << (std::ostream& os, const Binary& b) {
    os << b.decryptVal();
    return os;
}

Binary operator * (const Binary &b1, const Bit &b2) {
    return b1.decryptVal() * b2;
}

Bit operator >= (const Binary &b1, const Binary &b2) {
    return b1.decryptVal() >= b2.decryptVal();
}

Binary operator/(const Binary &b1, const int b2) {
    //TODO note that b2 may be euqal ZERO
    return b1.decryptVal() / b2;
}
