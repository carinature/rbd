


#include "imp1_kmeans_test.h"
//#include "../KeysServer.h"
//#include "../Point.h"
//#include "../aux.h"
#include "../imp1_kmeans.h"
#include <iostream>
#include <cassert>


void imp1_kmeans_test::getEncryptedKMeansTest() {
    cout << " --------- get_Encrypted_KMeans_test --------- " << endl;
    KeysServer * ks = new KeysServer();
    FHEPubKey * pubKey = ks->pubKey;
    
    vector<Point> points = getEncryptedPointsFromFile(*ks);
//    cout << points << endl;
    cout << " number of points: "<< points.size() << " ====" << endl;
    auto t1 = std::chrono::high_resolution_clock::now();
    vector<DecryptedPoint> means = getEncryptedKMeans(points, *ks);
//    vector<Point> encMeans = getEncryptedKMeans(points, *ks);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
    std::cout << "\n--- duration: " << duration << endl;

//    for(int i = 0; i < points.size(); ++i) assert(i==0);
//    decWriteToFile(means, "means_test", *ks);
    decWriteToFile(means, "io/means_test", *ks);
    
    cout << "           OK" << endl;
}

void imp1_kmeans_test::getLeftoverPointsTest() {
    cout << " --------- test_write_To_File --------- " << endl;
    
}

void imp1_kmeans_test::calculateThreshold() {

}

void imp1_kmeans_test::run_all() {
    getEncryptedKMeansTest();
    getLeftoverPointsTest();
    calculateThreshold();
}
