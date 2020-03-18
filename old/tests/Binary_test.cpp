
/*
#include "Binary_test.h"
#include "../Skeys.h"
#include <iostream>
//#include <vector>

using namespace std;

void Binary_test::mini_test() {
    cout << endl;
    cout << "==  Skeys_test  ==" << endl;
    cout << "--  mini test  --" << endl;
    cout << endl;
    
    Skeys                sk(0, NULL);
    Binary b0(0);
    Binary b1(1);
    Binary b2(2);
    Binary b34(3.4);

//    const vector<Binary> coordinates{b0, b1};

//    const DecryptedPoint &decPoint = sk.decryptPointByCA(p);
    NTL::Vec<Ctxt> encValue0 = sk.encryptDouble(0);
    NTL::Vec<Ctxt> encValue1 = sk.encryptDouble(1);
    NTL::Vec<Ctxt> encValue2 = sk.encryptDouble(2);
    NTL::Vec<Ctxt> encValue3 = sk.encryptDouble(3);
    
    double value0  = b0.getDecValue();
    double value1  = b1.getDecValue();
    double value2  = b2.getDecValue();
    double value34 = b34.getDecValue();
    
    cout << "value0: " << value0 << endl;
    cout << "value1: " << value1 << endl;
    cout << "value2: " << value2 << endl;
    cout << "value34: " << value34 << endl;
}
 */