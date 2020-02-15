//
// Created by rbd on 15.2.2020.
//

#include "SKey_test.h"

void SKey_test::mini_test() {
    cout <<  endl;
    cout << "==  SKey_test  ==" << endl;
    cout << "--  mini test  --" << endl;
    cout <<  endl;
    
    Skeys          sk(0, NULL);
    vector<Binary> coordinates = comparePAlgebra();
    Point          &p          = Point(coordinates, sk);
    sk.decryptPointByCA(p);
}
