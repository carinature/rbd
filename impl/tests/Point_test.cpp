//
// Created by rbd on 15.2.2020.
//

#include "Point_test.h"
#include "../KeysServer.h"
#include "../aux.h"
#include <iostream>
#include <cassert>

using namespace std;


void Point_test::test_ctor() {
    cout << " --------- test_ctor --------- " << endl;
    KeysServer * ks = new KeysServer();
    vector<long> v1 = {1, 2l};
    vector<long> v2 = {2, 3l};
    vector<long> v3 = {3, 4l};
    
    PointExtended pe1(ks, v1);
    PointExtended pe2(ks, v2);
    PointExtended pe3(ks, v3);
    pe1.eCoordinates;
    cout << "pe1: " << pe1 << endl;
    cout << "pe2: " << pe2 << endl;
    cout << "pe3: " << pe3 << endl;
    
    Point pp = pe1;
    PointExtended ppe = pe1;
    pp.eCoordinates;
    cout << "pp: " << pp << endl;
    cout << "ppe: " << ppe << endl;
    
    Point p1(ks, pe1.eCoordinates);
    Point p2(ks, pe2.eCoordinates);
    Point p3(ks, pe3.eCoordinates);
    cout << "p1: " << p1 << endl;
    cout << "p2: " << p2 << endl;
    cout << "p3: " << p3 << endl;
    
    Point kaka = p1 + p2;
    cout << "kaka: " << kaka << endl;
    delete (ks);
}
/*

void Point_test::point_test_dtor() {
    cout << " --------- point_test_dtor --------- "   << endl;
}


void Point_test::point_test_decrypt() {
    cout << " --------- point_test_decrypt --------- "   << endl;
    Skeys * sk    = new Skeys();
    vector<long> v1 = {1, 2l};
    vector<long> v2 = {2, 3l};
    vector<long> v3 = {3, 4l};
    vector<long> v4 = {13, 11l};
    Point p1(sk, v1);
    Point p2(sk, v2);
    Point p3(sk, v3);
    Point p4(sk, v4);
    vector<long> dp1 = sk->decryptPointByCA(p1);
    vector<long> dp2 = sk->decryptPointByCA(p2);
    vector<long> dp3 = sk->decryptPointByCA(p3);
    vector<long> dp4 = sk->decryptPointByCA(p4);
    
    assert(dp1 == v1);
    assert(dp2 == v2);
    assert(dp3 == v3);
    assert(dp4 == v4);
    
    cout << "and the winner is: " << dp4 << endl;
    delete(sk);
}


void Point_test::point_test_sum() {
    cout << " --------- point_test_sum --------- "   << endl;
    Skeys * sk    = new Skeys();
    DecryptedPoint v1 = {1, 2l};
    DecryptedPoint v2 = {2, 3l};
//    vector<long> v1 = {1, 2l};
//    vector<long> v2 = {2, 3l};
    vector<long> v3 = {3, 4l};
    Point p1(sk, v1);
    Point p2(sk, v2);
    Point p3(sk, v3);
    
    Point          sum1 = p1 + p2;
    Point          sum2 = p1 + p3;
    
    DecryptedPoint decSum1  = sk->decryptPointByCA(sum1);
    DecryptedPoint decSum2  = sk->decryptPointByCA(sum2);
    cout << "sum1: " << sum1 << endl;
    cout << "sum2: " << sum2 << endl;
    cout << "decSum1: " << decSum1 << endl;
    cout << "decSum2: " << decSum2 << endl;

    //    DecryptedPoint orgSum1 = v1 + v2;
//    assert(orgSum1 == decSum1);
//    assert(dp2 == v2);
//    assert(dp3 == v3);
//    assert(dp4 == v4);
    
    delete(sk);
}

void Point_test::point_test_g_cmp() {
    cout << " --------- point_test_g_cmp --------- "   << endl;
    Skeys * sk    = new Skeys();
    vector<long> v1 = {1, 2l};
    vector<long> v2 = {2, 3l};
    vector<long> v3 = {3, 4l};
    Point p1(sk, v1);
    Point p2(sk, v2);
    Point p3(sk, v2);
    Point p4(sk, v3);
    
    assert((p1 > p2) == 0);
    assert((p2 > p1));
    assert((p2 > p3) == 0);
    assert((p3 > p2) == 0);
    delete(sk);
}


void Point_test::point_test_ge_cmp() {
    cout << " --------- point_test_ge_cmp --------- "   << endl;
    auto sk    = new Skeys();
    Point p1(sk, {1,2l});
    Point p2(sk, {2,3});
    Point p3(sk, {2,3});
    cout << "before" << endl;
//    assert((p1 >= p2) == 0);
//    assert((p2 >= p1));
//    assert((p2 >= p3)); //TODO
//    assert((p3 >= p2)); //TODO
    delete(sk);
}

void Point_test::mini_test() {
    cout << " --------- mini_test --------- "   << endl;
    Skeys * sk = new Skeys();
    cout << "getEncryptedPointsFromFile" << endl;
    getEncryptedPointsFromFile(*sk);
    
    vector<long> v1 = {1, 2l};
    vector<long> v2 = {2, 3l};
    vector<long> v3 = {3, 3l};
    Point p1(sk, v1);
    Point p2(sk, v2);
    Point p3(sk, v3);
}


*/
