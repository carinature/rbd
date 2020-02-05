//
// Created by rbd on 24.12.2019.
//

#ifndef TRY_BINARY_H
#define TRY_BINARY_H

//#include "Bit.h" // todo add
using Bit = bool; // todo remove

class Binary {

protected:
public:
    void setValue(double newValue);

protected:
    double value{};
public:
    Binary();
//    Binary(int);
//    Binary(long);
    Binary(double);

    friend Binary operator + (const Binary& b1, const Binary& b2);
    friend Binary operator += (Binary &b1, const Binary &b2);
    friend Binary operator - (const Binary& b1, const Binary& b2);
    friend Binary operator -= (Binary &b1, const Binary &b2);
    friend Binary operator * (const Binary& b1, const Bit& b2);
    friend Binary operator / (const Binary& b1, const Binary& b2);
    friend Bit operator >= (const Binary &b1, const Binary &b2);
    friend std::ostream& operator << (std::ostream& os, const Binary& b);

//// -------------------------- for DBG --------------------------
    double getDecValue() const;


};


#endif //TRY_BINARY_H
