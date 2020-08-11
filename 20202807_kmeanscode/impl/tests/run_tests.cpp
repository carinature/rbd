
#include "run_tests.h"


int main(int argc, char * argv[]) {
    
    time_t timer = time(NULL);
    cout << endl << std::ctime(&timer) << endl;
    
    cout << endl << "==========  KeysServer_test  ==========" << endl;
//    KeysServer_test::test_ctor();
//    KeysServer_test::test_dtor();
////    KeysServer_test::test_decrypt();
//    KeysServer_test::test_g_cmp();
//    KeysServer_test::mini_test();
//    KeysServer_test::run_all();
    
    cout << endl << "==========  Point_test ==========" << endl;
//    Point_test::test_ctor();
//    Point_test::test_id();
//    Point_test::test_decrypt();
//    Point_test::test_sum();
//    Point_test::test_mult();
//    Point_test::test_g_cmp();
//    Point_test::test_sum_ass();
//    Point_test::run_all();
    
    cout << endl << "==========  AUX_test ==========" << endl;
//    aux_test::getEncryptedPointsFromFileTest();
//    aux_test::writeToFileTest();
//    aux_test::createCmpDictTest();
//    aux_test::runAll();
    
    cout << endl << "==========  IMPL_test ==========" << endl;

//    imp1_kmeans_test::getEncryptedKMeansTestNew();
//    imp1_kmeans_test::calculateThresholdTest();
    imp1_kmeans_test::getLeftoverPointsTest();
//    imp1_kmeans_test::run_all();



    /*
 * Deprecated
 * */
    //    imp1_kmeans_test::getEncryptedKMeansTest();
    
}