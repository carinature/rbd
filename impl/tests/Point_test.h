//
// Created by rbd on 15.2.2020.
//

#ifndef TRY_POINT_TEST_H
#define TRY_POINT_TEST_H

#include "../Point.h"

class Point_test {
public:
    static void test_ctor();
    static void test_mult();
    static void test_sum();
    static void test_sum_ass();
    static void test_decrypt();
    static void test_g_cmp();
//    static void test_ge_cmp();
//    static void mini_test();
    static void run_all();

};

/** aux **/
DecryptedPoint operator+(DecryptedPoint a, DecryptedPoint b);
DecryptedPoint operator*(DecryptedPoint a, int i) ;
#endif //TRY_POINT_TEST_H
