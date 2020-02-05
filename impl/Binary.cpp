//
// Created by rbd on 24.12.2019.
//
#include <iostream>
#include "Binary.h"

using namespace std;

Binary::Binary() : value(0) {
//    cout << "Binary empty: "<< endl;
}

Binary::Binary(double value): value(value){
//        cout << "Binary value: " << this->value << endl;
}

void Binary::setValue(double newValue) {
    Binary::value = newValue;
}

//// -------------------------- for DBG --------------------------
//int getvalue(){
//    return this->value;
//}
double Binary::getDecValue() const {
    return this->value;
}

Binary operator + (const Binary &b1, const Binary &b2) {
    return {b1.getDecValue() + b2.getDecValue()};
}

Binary operator += (Binary &b1, const Binary &b2) {
    b1.setValue(b1.getDecValue() + b2.getDecValue());
    return b1;
}

std::ostream& operator << (std::ostream& os, const Binary& b) {
    os << b.getDecValue();
    return os;
}

Binary operator * (const Binary &b1, const Bit &b2) {
    return b1.getDecValue() * b2;
}

Bit operator >= (const Binary &b1, const Binary &b2) {
    return b1.getDecValue() >= b2.getDecValue();
}

Binary operator / (const Binary &b1, const Binary &b2) {
    //TODO note that b2 may be euqal ZERO
    return {b1.getDecValue() / b2.getDecValue()};
}

Binary operator-=(Binary &b1, const Binary &b2) {
    b1.setValue(b1.getDecValue()+b2.getDecValue());
    return b1;
}

Binary operator - (const Binary &b1, const Binary &b2) {
    return {b1.getDecValue() - b2.getDecValue()};
}

Binary operator /= (Binary &b1, const Binary &b2) {
    //TODO note that b2 may be euqal ZERO
    b1.setValue(b1.getDecValue() / b2.getDecValue());
    return b1;
}

