//
// Created by rbd on 24.12.2019.
//
//#include <iostream>
#include "Binary.h"

#include <utility>
#include "properties.h"

using namespace std;

//Binary::Binary(FHESecKey secKey, NTL::Vec<Ctxt> encValue) : value(0) {
////    cout << "Binary empty: "<< endl;
//    Ctxt mu(secKey), ni(secKey);
//    resize(encValue, BIT_SIZE, mu);
////    resize(encb, bitSize + 1, ni);
//
//}
Binary::Binary() : value(0) {
//    cout << "Binary empty: "<< endl;
}

Binary::Binary(double value): value(value) {
//        cout << "Binary value: " << this->value << endl;
//    encValue =
}
//Binary::Binary(NTL::Vec<Ctxt> encValue, double value) : encValue(std::move(encValue)), value(value) {
////        cout << "Binary value: " << this->value << endl;
////        cout << "encValue value: " << this->encValue << endl;
//}

void Binary::setValue(double newValue) {
    Binary::value = newValue;
}

//void Binary::setValue(NTL::Vec<Ctxt> newEncValue, double newValue)  {
//    Binary::value = newValue;
//}

//// -------------------------- for DBG --------------------------
//int getvalue(){
//    return this->value;
//}
double Binary::getDecValue() const {
    return this->value;
}

Binary operator + (const Binary &b1, const Binary &b2) {
    return Binary(b1.getDecValue() + b2.getDecValue());
}

Binary operator += (Binary &b1, const Binary &b2) {
    b1.setValue(b1.getDecValue() + b2.getDecValue());
    return b1;
}

Binary operator+=(Binary &b1, const Bit &b2) {
    b1.setValue(b1.getDecValue() + b2);
    return b1;
}

std::ostream& operator << (std::ostream& os, const Binary& b) {
    os << b.getDecValue();
    return os;
}

Binary operator * (const Binary &b1, const Bit &b2) {
    return Binary(b1.getDecValue() * b2);
}

Binary operator *= (Binary &b1, const Bit &b2) {
    b1.setValue(b1.getDecValue() * b2);
    return b1;
}

Bit operator >= (const Binary &b1, const Binary &b2) {
    return b1.getDecValue() >= b2.getDecValue();
}

Binary operator / (const Binary &b1, const Binary &b2) {
    //TODO note that b2 may be euqal ZERO
    return Binary(b1.getDecValue() / b2.getDecValue());
}

Binary operator-=(Binary &b1, const Binary &b2) {
    b1.setValue(b1.getDecValue()+b2.getDecValue());
    return b1;
}

Binary operator - (const Binary &b1, const Binary &b2) {
    return Binary(b1.getDecValue() - b2.getDecValue());
}


Binary operator /= (Binary &b1, const Binary &b2) {
    //TODO note that b2 may be euqal ZERO
    b1.setValue(b1.getDecValue() / b2.getDecValue());
    return b1;
}

