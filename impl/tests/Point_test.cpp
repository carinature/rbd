

#include "Point_test.h"
#include "../KeysServer.h"
#include "../aux.h"
#include <iostream>
#include <cassert>

using namespace std;

////consider moving to prop file
//bool dbg = true;  // use KeysServer to init Point
////bool dbg = false;  // use PubKey to init Point

void Point_test::test_ctor() {  // todo - add some end cases
    cout << " --------- test_ctor --------- " << endl;
    KeysServer * ks = new KeysServer();
    FHEPubKey * pubKey = ks->pubKey;
    vector<long> v1 = {1, 2l};
    vector<long> v2 = {2, 3l};
    vector<long> v3 = {3, 4l};

//    PointExtended pe1(ks, v1);
//    PointExtended pe2(ks, v2);
//    PointExtended pe3(ks, v3);
    PointExtended pe1(v1, pubKey);
    PointExtended pe2(v2, pubKey);
    PointExtended pe3(v3, pubKey);
    pe1.eCoordinates;
//    cout << "pe1: " << pe1 << endl;
//    cout << "pe2: " << pe2 << endl;
//    cout << "pe3: " << pe3 << endl;
    
    Point pp = pe1;
    PointExtended ppe = pe1;
    pp.eCoordinates;
//    cout << "pp: " << pp << endl;
//    cout << "ppe: " << ppe << endl;
    
    Point p1(pe1.eCoordinates, pubKey);
    Point p2(pe2.eCoordinates, pubKey);
    Point p3(pe3.eCoordinates, pubKey);
//    Point p1(ks, pe1.eCoordinates);
//    Point p2(ks, pe2.eCoordinates);
//    Point p3(ks, pe3.eCoordinates);
////    cout << "p1: " << p1 << endl;
////    cout << "p2: " << p2 << endl;
////    cout << "p3: " << p3 << endl;
    
    cout << "           OK" << endl;
    delete (ks);
    
    
}

void Point_test::test_decrypt() {
    cout << " --------- test_decrypt --------- " << endl;
    KeysServer * ks = new KeysServer();
    FHEPubKey * pubKey = ks->pubKey;
    vector<long> v1 = {1, 2l};
    vector<long> v2 = {2, 3l};
    vector<long> v3 = {3, 4l};
    vector<long> v4 = {13, 11l};
    
    PointExtended pe1(v1, pubKey);
    PointExtended pe2(v2, pubKey);
    PointExtended pe3(v3, pubKey);
    PointExtended pe4(v4, pubKey);
    if(dbg) {
         pe1 = PointExtended(v1, ks);
         pe2 = PointExtended(v2, ks);
         pe3 = PointExtended(v3, ks);
         pe4 = PointExtended(v4, ks);
    }
    
//    Point p1(ks, pe1.eCoordinates);
//    Point p2(ks, pe2.eCoordinates);
//    Point p3(ks, pe3.eCoordinates);
//    Point p4(ks, pe4.eCoordinates);
    
    Point p1 = pe1;
    Point p2 = pe2;
    Point p3 = pe3;
    Point p4 = pe4;
    
    vector<long> dp1 = p1.decrypt(*ks);
    vector<long> dp2 = p2.decrypt(*ks);
    vector<long> dp3 = p3.decrypt(*ks);
    vector<long> dp4 = p4.decrypt(*ks);
    
    assert(dp1 == v1);
    assert(dp2 == v2);
    assert(dp3 == v3);
    assert(dp4 == v4);
    
    cout << "           OK" << endl;
    delete (ks);
}

void Point_test::test_sum() {
    cout << " --------- test_sum --------- " << endl;
    KeysServer * ks = new KeysServer();
    FHEPubKey * pubKey = ks->pubKey;
    vector<long> v0 = {0, 0};
    vector<long> v1 = {1, 2l};
    vector<long> v2 = {2, 3l};
    vector<long> v3 = {3, 4l};
    vector<long> v4 = {13, 11l};
    
    
    Point p0 = PointExtended(v0, pubKey);
    Point p1 = PointExtended(v1, pubKey);
    Point p2 = PointExtended(v2, pubKey);
    Point p3 = PointExtended(v3, pubKey);
    Point p4 = PointExtended(v4, pubKey);
    if(dbg) {
        p0 = PointExtended(v0, ks);
        p1 = PointExtended(v1, ks);
        p2 = PointExtended(v2, ks);
        p3 = PointExtended(v3, ks);
        p4 = PointExtended(v4, ks);
    }

//    PointExtended sume1 = pe1 + pe2;
//    PointExtended sume2 = pe1 + pe3;
//    PointExtended sume3 = pe1 + pe1;
    
    Point sum1 = p1 + p2;
    Point sum2 = p1 + p3;
    Point sum3 = p1 + p1;

//    DecryptedPoint decSum1  = ks->decryptPointByCA(sum1);
//    DecryptedPoint decSum2  = ks->decryptPointByCA(sum2);
    DecryptedPoint decSum1 = sum1.decrypt(*ks);
    DecryptedPoint decSum2 = sum2.decrypt(*ks);
    DecryptedPoint decSum3 = sum3.decrypt(*ks);
    
    DecryptedPoint orgSum1 = v1 + v2;
    DecryptedPoint orgSum2 = v1 + v3;
    DecryptedPoint orgSum3 = v1 + v1;
    assert(orgSum1 == decSum1);
    assert(orgSum2 == decSum2);
    assert(orgSum3 == decSum3);

//    cout << "sum1: " << sum1 << endl;
//    cout << "sum2: " << sum2 << endl;
//    cout << "sum3: " << sum3 << endl;
//    cout << "decSum1: " << decSum1 << endl;
//    cout << "decSum2: " << decSum2 << endl;
//    cout << "decSum3: " << decSum3 << endl;
    cout << "           OK" << endl;
    delete (ks);
}
void Point_test::test_subt() {
    cout << " --------- test_subt --------- " << endl;
    KeysServer * ks = new KeysServer();
    FHEPubKey * pubKey = ks->pubKey;
    vector<long> v0 = {0, 0};
    vector<long> v1 = {1, 2l};
    vector<long> v2 = {2, 3l};
    vector<long> v3 = {3, 4l};
    vector<long> v4 = {13, 11l};
    
    
    Point p0 = PointExtended(v0, pubKey);
    Point p1 = PointExtended(v1, pubKey);
    Point p2 = PointExtended(v2, pubKey);
    Point p3 = PointExtended(v3, pubKey);
    Point p4 = PointExtended(v4, pubKey);
    if(dbg) {
        p0 = PointExtended(v0, ks);
        p1 = PointExtended(v1, ks);
        p2 = PointExtended(v2, ks);
        p3 = PointExtended(v3, ks);
        p4 = PointExtended(v4, ks);
    }

//    PointExtended subte1 = pe1 + pe2;
//    PointExtended subte2 = pe1 + pe3;
//    PointExtended subte3 = pe1 + pe1;
    
    Point subt1 = p1 - p1;
    Point subt2 = p2 - p1;
    Point subt3 = p3 - p1;

//    DecryptedPoint decsubt1  = ks->decryptPointByCA(subt1);
//    DecryptedPoint decsubt2  = ks->decryptPointByCA(subt2);
    DecryptedPoint decsubt1 = subt1.decrypt(*ks);
    DecryptedPoint decsubt2 = subt2.decrypt(*ks);
    DecryptedPoint decsubt3 = subt3.decrypt(*ks);
    
    DecryptedPoint orgsubt1 = v1 - v1;
    DecryptedPoint orgsubt2 = v2 - v1;
    DecryptedPoint orgsubt3 = v3 - v1;
    assert(orgsubt1 == decsubt1);
    assert(orgsubt2 == decsubt2);
    assert(orgsubt3 == decsubt3);

//    cout << "subt1: " << subt1 << endl;
//    cout << "subt2: " << subt2 << endl;
//    cout << "subt3: " << subt3 << endl;
//    cout << "decsubt1: " << decsubt1 << endl;
//    cout << "decsubt2: " << decsubt2 << endl;
//    cout << "decsubt3: " << decsubt3 << endl;
    cout << "           OK" << endl;
    delete (ks);
}

void Point_test::test_sum_ass() {
    cout << " --------- test_sum_ass --------- " << endl;
    KeysServer * ks = new KeysServer();
    FHEPubKey * pubKey = ks->pubKey;
    vector<long> v0 = {0, 0};
    vector<long> v1 = {1, 2l};
    vector<long> v2 = {2, 3l};
    vector<long> v3 = {3, 4l};
    vector<long> v4 = {13, 11l};
    
    
    Point p0 = PointExtended(v0, pubKey);
    Point p1 = PointExtended(v1, pubKey);
    Point p2 = PointExtended(v2, pubKey);
    Point p3 = PointExtended(v3, pubKey);
    Point p4 = PointExtended(v4, pubKey);
    if(dbg) {
        p0 = PointExtended(v0, ks);
        p1 = PointExtended(v1, ks);
        p2 = PointExtended(v2, ks);
        p3 = PointExtended(v3, ks);
        p4 = PointExtended(v4, ks);
    }

//    PointExtended sume1 = pe1 + pe2;
//    PointExtended sume2 = pe1 + pe3;
//    PointExtended sume3 = pe1 + pe1;
    
//    p2+=p1;
//    p3+=p1;
//    p4+=p1;

//    DecryptedPoint decSum1  = ks->decryptPointByCA(sum1);
//    DecryptedPoint decSum2  = ks->decryptPointByCA(sum2);
    DecryptedPoint decSum1 = p2.decrypt(*ks);
    DecryptedPoint decSum2 = p3.decrypt(*ks);
    DecryptedPoint decSum3 = p4.decrypt(*ks);
    
    DecryptedPoint orgSum1 = v1 + v2;
    DecryptedPoint orgSum2 = v1 + v3;
    DecryptedPoint orgSum3 = v1 + v4;
    cout <<"decSum1 " <<decSum1 << endl;
    cout << "orgSum1 "<< orgSum1<< endl;
    assert(orgSum1 == decSum1);
    assert(orgSum2 == decSum2);
    assert(orgSum3 == decSum3);

//    cout << "sum1: " << sum1 << endl;
//    cout << "sum2: " << sum2 << endl;
//    cout << "sum3: " << sum3 << endl;
//    cout << "decSum1: " << decSum1 << endl;
//    cout << "decSum2: " << decSum2 << endl;
//    cout << "decSum3: " << decSum3 << endl;
    cout << "           OK" << endl;
    delete (ks);
}

void Point_test::test_mult() {
    cout << " --------- test_mult --------- " << endl;
    KeysServer * ks = new KeysServer();
    FHEPubKey * pubKey = ks->pubKey;
    vector<long> v0 = {0, 0};
    vector<long> v1 = {1, 2l};
    vector<long> v2 = {2, 3l};
    vector<long> v3 = {3, 4l};
    vector<long> v4 = {13, 11l};
    
    Point p0 = PointExtended(v0, pubKey);
    Point p1 = PointExtended(v1, pubKey);
    Point p2 = PointExtended(v2, pubKey);
    Point p3 = PointExtended(v3, pubKey);
    Point p4 = PointExtended(v4, pubKey);
    if(dbg) {
        p0 = PointExtended(v0, ks);
        p1 = PointExtended(v1, ks);
        p2 = PointExtended(v2, ks);
        p3 = PointExtended(v3, ks);
        p4 = PointExtended(v4, ks);
    }
    
//    Vec<Ctxt> b0 = p0[0];
//    Vec<Ctxt> b1 = p1[0];
    Ctxt b0 = p0[0][0];
    Ctxt b0test = p0[0][0];
    Ctxt b1 = p1[0][0];

//    PointExtended producte1 = pe1 + pe2;
//    PointExtended producte2 = pe1 + pe3;
//    PointExtended producte3 = pe1 + pe1;

    b0test.multiplyBy(b1);
    EncNumber bla; bla.append(b0test); // = EncNumber();
    cout << "00000000  -  " << ks->decrypt(bla) << endl;
    
    Point product1 = p1 * b0;
    Point product2 = p2 * b0;
    Point product3 = p3 * b0;
    Point product11 = p1 * b1;
    Point product21 = p2 * b1;
    Point product31 = p3 * b1;

//    DecryptedPoint decproduct1  = ks->decryptPointByCA(product1);
//    DecryptedPoint decproduct2  = ks->decryptPointByCA(product2);
    DecryptedPoint decproduct1 = product1.decrypt(*ks);
    DecryptedPoint decproduct2 = product2.decrypt(*ks);
    DecryptedPoint decproduct3 = product3.decrypt(*ks);
    DecryptedPoint decproduct11 = product11.decrypt(*ks);
    DecryptedPoint decproduct21 = product21.decrypt(*ks);
    DecryptedPoint decproduct31 = product31.decrypt(*ks);
    
    DecryptedPoint orgproduct1 = v1 * 0;
    DecryptedPoint orgproduct2 = v2 * 0;
    DecryptedPoint orgproduct3 = v3 * 0;
    DecryptedPoint orgproduct11 = v1 * 1;
    DecryptedPoint orgproduct21 = v2 * 1;
    DecryptedPoint orgproduct31 = v3 * 1;
    
    cout << "orgproduct1: " << orgproduct1 << endl;
    cout << "decproduct1: " << decproduct1 << endl;
    
    assert(orgproduct1 == decproduct1);
    assert(orgproduct2 == decproduct2);
    assert(orgproduct3 == decproduct3);
    
    assert(orgproduct11 == p1.decrypt(*ks)); //check if point wasn't changed
    
    assert(orgproduct11 == decproduct11);
    assert(orgproduct21 == decproduct21);
    assert(orgproduct31 == decproduct31);

//    cout << "product1: " << product1 << endl;
//    cout << "product2: " << product2 << endl;
//    cout << "product3: " << product3 << endl;
//    cout << "decproduct1: " << decproduct1 << endl;
//    cout << "decproduct2: " << decproduct2 << endl;
//    cout << "decproduct3: " << decproduct3 << endl;
//    cout << "decproduct11: " << decproduct11 << endl;
//    cout << "decproduct21: " << decproduct21 << endl;
//    cout << "decproduct31: " << decproduct31 << endl;
    cout << "           OK" << endl;
    delete (ks);
}

void Point_test::test_g_cmp() {
    cout << " --------- point_test_g_cmp --------- " << endl;
    KeysServer * ks = new KeysServer();
    FHEPubKey * pubKey = ks->pubKey;
    vector<long> v0 = {0, 0};
    vector<long> v1 = {1, 2l};
    vector<long> v2 = {2, 3l};
    vector<long> v3 = {3, 4l};
    vector<long> v4 = {13, 11l};
    
    Point p0 = PointExtended(v0, pubKey);
    Point p1 = PointExtended(v1, pubKey);
    Point p2 = PointExtended(v2, pubKey);
    Point p3 = PointExtended(v3, pubKey);
    Point p4 = PointExtended(v4, pubKey);
    if(dbg) {
        p0 = PointExtended(v0, ks);
        p1 = PointExtended(v1, ks);
        p2 = PointExtended(v2, ks);
        p3 = PointExtended(v3, ks);
        p4 = PointExtended(v4, ks);
    }
    
    Ctxt cmp11 = p1 > p1;  // should return 0
    Ctxt cmp21 = p2 > p1;  // should return 1
    Ctxt cmp31 = p3 > p1;  // should return 1
    Ctxt cmp12 = p1 > p2;  // should return 0
    Ctxt cmp22 = p2 > p2;  // should return 0
    Ctxt cmp32 = p3 > p2;  // should return 1
    Ctxt cmp13 = p1 > p3;  // should return 0
    Ctxt cmp23 = p2 > p3;  // should return 0
    Ctxt cmp33 = p3 > p3;  // should return 0
    
    Point pointOrNot11 = p1 * cmp11;  // should return p0
    Point pointOrNot21 = p1 * cmp21;  // should return p1
    Point pointOrNot31 = p1 * cmp31;  // should return p1
    Point pointOrNot12 = p1 * cmp12;  // should return p0
    Point pointOrNot22 = p1 * cmp22;  // should return p0
    Point pointOrNot32 = p1 * cmp32;  // should return p1
    Point pointOrNot13 = p1 * cmp13;  // should return p0
    Point pointOrNot23 = p1 * cmp23;  // should return p0
    Point pointOrNot33 = p1 * cmp33;  // should return p0
    
    DecryptedPoint decproduct11 = pointOrNot11.decrypt(*ks);
    DecryptedPoint decproduct21 = pointOrNot21.decrypt(*ks);
    DecryptedPoint decproduct31 = pointOrNot31.decrypt(*ks);
    DecryptedPoint decproduct12 = pointOrNot12.decrypt(*ks);
    DecryptedPoint decproduct22 = pointOrNot22.decrypt(*ks);
    DecryptedPoint decproduct32 = pointOrNot32.decrypt(*ks);
    DecryptedPoint decproduct13 = pointOrNot13.decrypt(*ks);
    DecryptedPoint decproduct23 = pointOrNot23.decrypt(*ks);
    DecryptedPoint decproduct33 = pointOrNot33.decrypt(*ks);
    
    assert(decproduct11 == v0);
    assert(decproduct21 == v1);
    assert(decproduct31 == v1);
    assert(decproduct12 == v0);
    assert(decproduct22 == v0);
    assert(decproduct32 == v1);
    assert(decproduct13 == v0);
    assert(decproduct23 == v0);
    assert(decproduct33 == v0);

//    cout << "product1: " << product1 << endl;
//    cout << "product2: " << product2 << endl;
//    cout << "product3: " << product3 << endl;
//    cout << "decproduct1: " << decproduct1 << endl;
//    cout << "decproduct2: " << decproduct2 << endl;
//    cout << "decproduct3: " << decproduct3 << endl;
//    cout << "decproduct11: " << decproduct11 << endl;
//    cout << "decproduct21: " << decproduct21 << endl;
//    cout << "decproduct31: " << decproduct31 << endl;
    cout << "           OK" << endl;
    delete (ks);
}

/*void Point_test::mini_test() {
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
}*/

void Point_test::run_all() {
    test_ctor();
    test_mult();
    test_sum();
//    test_subt();
//    test_sum_ass();
    test_decrypt();
    test_g_cmp();
//    mini_test();
}

void Point_test::test_id() {
    cout << " --------- test_id --------- " << endl;
    KeysServer * ks = new KeysServer();
    FHEPubKey * pubKey = ks->pubKey;
    vector<long> v1 = {1, 2l};
    vector<long> v2 = {2, 3l};
    vector<long> v3 = {3, 4l};
    vector<long> v4 = {13, 11l};

        Point pe1 = PointExtended(v1, ks);
        Point pe2 = PointExtended(v2, ks);
        Point pe3 = PointExtended(v3, ks);
        Point pe4 = PointExtended(v4, ks);
        
        cout << pe1.id << endl;
        cout << pe2.id << endl;
        cout << pe3.id << endl;
        cout << pe4.id << endl;
}




/** aux **/
DecryptedPoint operator+(DecryptedPoint a, DecryptedPoint b) {
    DecryptedPoint sum;
    for(int i = 0; i < a.size(); ++i) {
        sum.push_back(a[i] + b[i]);
    }
    return sum;
}

DecryptedPoint operator*(DecryptedPoint a, int b) {
    DecryptedPoint prod;
    for(long i : a) {
        prod.push_back(i * b);
    }
    return prod;
}

DecryptedPoint operator-(DecryptedPoint a, DecryptedPoint b) {
    DecryptedPoint sum;
    for(int i = 0; i < a.size(); ++i) {
        sum.push_back(a[i] - b[i]);
    }
    return sum;
}
