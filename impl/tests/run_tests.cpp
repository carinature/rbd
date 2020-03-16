//
// Created by rbd on 15.2.2020.
//

#include "run_tests.h"


int main(int argc, char * argv[]){
    
    cout << "==========  KeysServer_test  ==========" << endl;
    KeysServer_test::test_ctor();
//    KeysServer_test::test_dtor();
//    KeysServer_test::test_decrypt();
//    KeysServer_test::test_g_cmp();
//    KeysServer_test::mini_test();
//
    cout << "==========  Point_test ==========" << endl;
    Point_test::test_ctor();
    Point_test::test_decrypt();
    Point_test::test_sum();
    Point_test::test_mult();
    Point_test::test_g_cmp();
//    Point_test::test_ge_cmp();
//    Point_test::mini_test();

}