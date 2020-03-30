


#include "imp1_kmeans_test.h"
//#include "../KeysServer.h"
//#include "../Point.h"
//#include "../aux.h"
#include "../imp1_kmeans.h"
#include <iostream>
#include <cassert>


void imp1_kmeans_test::getEncryptedKMeansTest() {
    cout << " --------- get_Encrypted_KMeans_test --------- " << endl;
    auto t1 = std::chrono::high_resolution_clock::now();
    
    KeysServer * ks = new KeysServer();
    FHEPubKey * pubKey = ks->pubKey;
    vector<PointExtended> pointsEx = getEncryptedPointsFromFile(*ks);
    vector<Point> points(pointsEx.begin(), pointsEx.end());
    vector<DecryptedPoint> means = getEncryptedKMeans(points, *ks);

//    for(int i = 0; i < points.size(); ++i) assert(i==0);
    //todo check points manually and not visually
    
    cout << "           OK" << endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
    std::cout << "\n--- duration: " << duration << endl;
}

void imp1_kmeans_test::calculateThresholdTest() {
    cout << " --------- calculateThresholdTest --------- " << endl;
    auto t1 = std::chrono::high_resolution_clock::now();
    
    KeysServer * ks = new KeysServer();
    FHEPubKey * pubKey = ks->pubKey;
    vector<PointExtended> clients = getEncryptedPointsFromFile(*ks);
    vector<DecryptedPoint> means = getPointsFromFile("io/means");
    vector<EncNumber> distances = getDistances(clients, means);
    //todo test with the "real" distances
    //for every point in file
    //  find minimal dist:
    //  for every mean
    //      get dist
    //      check if minimal
    //  check against distances.decrypted (assert)

    cout << "           OK" << endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
    std::cout << "\n--- duration: " << duration << endl;
    
}

void imp1_kmeans_test::getLeftoverPointsTest() {
    cout << " --------- test_write_To_File --------- " << endl;
    
    cout << "           OK" << endl;
}

void imp1_kmeans_test::run_all() {
    getEncryptedKMeansTest();
    getLeftoverPointsTest();
    calculateThresholdTest();
}
