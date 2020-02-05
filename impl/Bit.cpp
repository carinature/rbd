//
// Created by rbd on 24.12.2019.
//

#include <iostream>
#include "Bit.h"

using namespace std;

Bit::Bit(bool value): value(value){
//    cout << "value: " << value << endl;
//    cout << "this -> value: " << this->value << endl;
}

Bit::Bit(short value){
    this->value = value == 1 ;
//    cout << "value: " << value << endl;
//    cout << "this -> value: " << this->value << endl;
}