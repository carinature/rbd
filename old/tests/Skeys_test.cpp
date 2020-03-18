

#include "Skeys_test.h"

void Skeys_test::skeys_mini_test() {
    cout << " --------- skeys_mini_test --------- "   << endl;
    Skeys sk;

//    const DecryptedPoint &decPoint = sk.decryptPointByCA(p);
//    cout << "decPoint: "  << decPoint << endl;

//    Binary               b0(0l);
//    Binary               b1(1l);
//    Binary               b2(2);
//    Binary               b34(3.4);
//    const vector<Binary> coordinates{b0, b1};
//    Point p(sk, coordinates);

}

void Skeys_test::skeys_test_ctor() {
    cout << " --------- skeys_test_ctor --------- "   << endl;
    Skeys sk = Skeys();
    sk.pubKey;
//    delete(sk);
}

void Skeys_test::skeys_test_dtor() {
    cout << " --------- skeys_test_dtor --------- "   << endl;
    Skeys sk = Skeys();
//    cout << *sk.pubKey << endl;
//    delete(&sk);
//    try{
//        cout << *sk.pubKey << endl;
//    } catch(exception){
//        cout << "well done" << endl;
//    }
}

void Skeys_test::skeys_test_g_cmp() {
    cout << " --------- skeys_test_g_cmp --------- "   << endl;
    
}

void Skeys_test::skeys_test_decrypt() {
    Skeys * sk = new Skeys();
    Point p = Point(sk, {1,2});
    DecryptedPoint dp = sk->decryptPointByCA(p);
    cout << "Dec Point:" << dp << endl;
    delete(sk);
}
