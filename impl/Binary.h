//
// Created by rbd on 24.12.2019.
//

#ifndef TRY_BINARY_H
#define TRY_BINARY_H

//#include <iostream>

#include <helib/FHE.h>
//#include <helib/EncryptedArray.h>
//#include <NTL/vector.h>
//#include <helib/Ctxt.h>
//#include <NTL/BasicThreadPool.h>

//#include "Bit.h" // todo add
using Bit = bool; // todo remove
//using Bit = Ctxt; // todo remove

//using namespace std;

//using Binary = NTL::Vec<Ctxt>;
class Binary {
    NTL::Vec<Ctxt> encValue;

protected:
    double value{};

public:

    Binary();
//    Binary(int);
//    Binary(long);
    explicit Binary(double);
//    Binary(NTL::Vec<Ctxt> , double);

    friend Binary operator + (const Binary& b1, const Binary& b2);
    friend Binary operator += (Binary &b1, const Binary &b2);
    friend Binary operator += (Binary &b1, const Bit &b2);
    friend Binary operator - (const Binary& b1, const Binary& b2);
    friend Binary operator -= (Binary &b1, const Binary &b2);
    friend Binary operator * (const Binary& b1, const Bit& b2);
    friend Binary operator *= (Binary& b1, const Bit& b2);
    friend Binary operator / (const Binary& b1, const Binary& b2);
    friend Bit operator >= (const Binary &b1, const Binary &b2);
    friend std::ostream& operator << (std::ostream& os, const Binary& b);

//// -------------------------- for DBG --------------------------
    double getDecValue() const;
    void setValue(double newValue);
//    void setValue(NTL::Vec<Ctxt> encValue, double value);

    friend class Binary_test;


};


#endif //TRY_BINARY_H
