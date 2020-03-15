//
// Created by rbd on 15.2.2020.
//

#include "run_tests.h"


int main(int argc, char * argv[]){
//    Binary_test::skeys_mini_test();
    cout << "==========  Point_test ==========" << endl;
//    Point_test::point_test_ctor();
//    Point_test::point_test_dtor();
//    Point_test::point_test_decrypt();
    Point_test::point_test_sum();
//    Point_test::point_test_g_cmp();
//    Point_test::point_test_ge_cmp();
//    Point_test::skeys_mini_test();
    
    cout << "==========  Skeys_test  ==========" << endl;
    Skeys_test::skeys_test_ctor();
    Skeys_test::skeys_test_dtor();
    Skeys_test::skeys_test_decrypt();
//    Skeys_test::skeys_test_g_cmp();
//    Skeys_test::skeys_mini_test();
}