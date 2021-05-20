
#include "imp1_kmeans_test.h"
//#include "../KeysServer.h"
//#include "../Point.h"
//#include "../aux.h"
#include "../imp1_kmeans.h"
#include <iostream>
#include <cassert>

vector<vector<Point> >
getLeftoverPoints(
        const vector<PointExtended> & clients,
        const vector<DecryptedPoint> & means,
        KeysServer & keysServer);


void imp1_kmeans_test::getEncryptedKMeansTestNew() {
    auto t1 = std::chrono::high_resolution_clock::now();
    cout << " --------- get_Encrypted_KMeans_test --------- " << endl;
    auto * ks = new KeysServer();
//    FHEPubKey * pubKey = ks->pubKey;
    vector<PointExtended> pointsEx = getEncryptedPointsFromFile(*ks);
    assert(pointsEx.size() == NUM_POINTS); //sanity check
    
    vector<Point> points(pointsEx.begin(), pointsEx.end());
    vector<DecryptedPoint> means = calculateMeans(getCells(points, *ks), *ks);
    writeToFile(means, means_file);
//    for(int i = 0; i < points.size(); ++i) assert(i==0);
    //todo check points manually and not visually

    printDuration( t1, "getEncryptedKMeansTestNew" );
}

void imp1_kmeans_test::calculateThresholdTest() {
    cout << " --------- calculateThresholdTest --------- " << endl;
    auto t1 = std::chrono::high_resolution_clock::now();
    
    auto * ks = new KeysServer();
//    FHEPubKey * pubKey = ks->pubKey;
    vector<PointExtended> clients = getEncryptedPointsFromFile(*ks);
    vector<DecryptedPoint> means = getPointsFromFile(means_file);
    Vec<EncNumber> distances = getDistances(clients, means);
    long threshold = ks->decrypt(calculateThreshold(distances, *ks));
    cout << "The threshold is: " << threshold << endl;
    //todo test with the "real" distances check against distances.decrypted (assert)

    printDuration( t1, "calculateThresholdTest" );
    
}

void imp1_kmeans_test::getLeftoverPointsTest() {
    cout << " --------- getLeftoverPointsTest --------- " << endl;
    auto t1 = std::chrono::high_resolution_clock::now();
    
    auto * ks = new KeysServer();
//    FHEPubKey * pubKey = ks->pubKey;
    vector<PointExtended> clients = getEncryptedPointsFromFile(*ks);
    assert(clients.size() == NUM_POINTS); //sanity check
    vector<Point> points(clients.begin(), clients.end());
    
//    vector<DecryptedPoint> means = getPointsFromFile(means_file); // requires reloading of cmake project
    vector<DecryptedPoint> means = calculateMeans(getCells(points, *ks), *ks);
    cout << "size of means: " << means.size() << ": " << means << endl;
    
//    Vec<EncNumber> distances = getDistances(clients, means);

//    EncNumber threshold = calculateThreshold(distances, *ks);// {//}, int amount) {
//    long threshold = calculateThreshold(distances, *ks);// {//}, int amount) {
//    cout << "The threshold is: " << threshold << endl;
    //todo test with the "real" distances
    //  check against distances.decrypted (assert)
    
    vector<vector<Point> > leftoverPointsDec = getLeftoverPoints(clients, means, *ks);
    vector<Point> chosen = leftoverPointsDec[0];

    printDuration( t1, "getLeftoverPointsTest" );
    cout << "\n--- #points: " << NUM_POINTS <<" EPSILON = " << EPSILON << endl;
}

void imp1_kmeans_test::run_all() {
    cout << " --------- test_write_To_File --------- " << endl;
    auto t1 = std::chrono::high_resolution_clock::now();
    
    std::ifstream f("text.txt");
    std::string line;
    long i;
    for (i = 0; std::getline(f, line); ++i) ;
    assert(NUM_POINTS == i);
    getEncryptedKMeansTest();
    getLeftoverPointsTest();
    calculateThresholdTest();

    printDuration( t1, "run_all" );

}


/*
 * aux
 * */
//vector<vector<Point> > getLeftoverPoints(
//        const vector<PointExtended> & clients, const vector<DecryptedPoint> & means, KeysServer & keysServer) {
//    Vec<EncNumber> distances = getDistances(clients, means);
//    vector<long> decDistances;
//    for (const EncNumber & d : distances) decDistances.push_back(keysServer.decrypt(d));
//
////    EncNumber threshold = calculateThreshold(distances, keysServer);
//    long threshold = calculateThreshold(distances, keysServer);
//    cout << "The threshold is: " << threshold << endl;
//
//    vector<Point> chosen, leftover;
//    for (unsigned long j = 0; j < clients.size(); ++j) {
////        if(threshold >= decDistances[j]) chosen.push_back(clients[j].decrypt(keysServer));
////        else leftover.push_back(clients[j].decrypt(keysServer));
//        if (threshold >= decDistances[j]) chosen.push_back(clients[j]);
//        else leftover.push_back(clients[j]);
//    }
//    vector<vector<Point> > retVec;
//    retVec.push_back(chosen);
//    retVec.push_back(leftover);
////    cout << "distances of size: " << distances.size() << " " << distances << endl;
//    cout << "chosen of size: " << chosen.size() << " " << chosen << endl;
//    cout << "leftover of size: " << leftover.size() << " " << leftover << endl;
//    if (DBG) decAndWriteToFile(chosen, chosen_file, keysServer);
////    return leftover;
//    return retVec;
//}

/*
 * Deprecated
 * */
//void imp1_kmeans_test::getEncryptedKMeansTest() {
//    cout << " --------- get_Encrypted_KMeans_test --------- " << endl;
//    auto t1 = std::chrono::high_resolution_clock::now();
//
//    auto * ks = new KeysServer();
//    FHEPubKey * pubKey = ks->pubKey;
//    vector<PointExtended> pointsEx = getEncryptedPointsFromFile(*ks);
//    vector<Point> points(pointsEx.begin(), pointsEx.end());
//    vector<DecryptedPoint> means = getEncryptedKMeans(points, *ks);
//
////    for(int i = 0; i < points.size(); ++i) assert(i==0);
//    //todo check points manually and not visually
//
//    printDuration( t1, "getEncryptedKMeansTest" );
//
//}
