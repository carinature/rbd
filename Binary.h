//
// Created by rbd on 24.12.2019.
//

#ifndef TRY_BINARY_H
#define TRY_BINARY_H


class Binary {
    double value;

public:
    Binary();
//    Binary(int);
//    Binary(long);
    Binary(double);

//    basic_string<_CharT, _Traits, _Alloc> std::operator+(_CharT __lhs,
//


//// -------------------------- for DBG --------------------------
//    int getvalue();
    double decryptVal() const;
    friend Binary operator+ (const Binary& b1, const Binary& b2);
    friend std::ostream& operator<<(std::ostream& os, const Binary& b);

};


#endif //TRY_BINARY_H
