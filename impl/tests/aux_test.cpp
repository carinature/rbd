

#include "aux_test.h"
#include "../KeysServer.h"
//#include "../Point.h"
#include "../aux.h"
#include <iostream>
#include <cassert>

using namespace std;

void aux_test::runAll() {
    getEncryptedPointsFromFileTest();
    writeToFileTest();
//    mini_test();
}
void aux_test::writeToFileTest() {
    cout << " --------- test_write_To_File --------- " << endl;
    KeysServer * ks = new KeysServer();
    FHEPubKey * pubKey = ks->pubKey;
    
    vector<DecryptedPoint> points = getPointsFromFile();
    vector<PointExtended> point = getEncryptedPointsFromFile(*ks);
    vector<Point> ePoints(point.begin(), point.end());
    
    const string & filename = "io/points_test";
    writeToFile(ePoints, filename, *ks);
    vector<DecryptedPoint> points_test = getPointsFromFile(filename);
    
    for(int i = 0; i < points.size(); ++i) {
//        vector<long> & vector = points[i];
//        std::vector<long> & test_vector = points_test[i];
//        std::vector<long> vector2 = ePoints[i].decrypt(*ks);
//        if (vector!= test_vector) cout << "vector: " << vector << endl << "test_vector: " << test_vector << endl;
        assert(points[i] == points_test[i]); //consider subtracting and rounding to avoid bugs from the enc/dec of
        assert(ePoints[i].decrypt(*ks) == points_test[i]);
        assert(points[i] == ePoints[i].decrypt(*ks));
    }
    cout << "           OK" << endl << endl;
}
void aux_test::getEncryptedPointsFromFileTest() {
    cout << " --------- test_get_enc_point_from_file --------- " << endl;
    KeysServer * ks = new KeysServer();
    FHEPubKey * pubKey = ks->pubKey;
    
    vector<DecryptedPoint> points = getPointsFromFile();
    vector<PointExtended> point = getEncryptedPointsFromFile(*ks);
    vector<Point> ePoints(point.begin(), point.end());
    
    for(int i = 0; i < points.size(); ++i) assert(points[i] == ePoints[i].decrypt(*ks));
    cout << "           OK" << endl;
}

void aux_test::createCmpDictTest() {

}

/** aux **/
bool operator==(std::vector<long> a, std::vector<long> b) {
    for(int i = 0; i < a.size(); ++i) if(a[i] != b[i]) return false;
    return true;
}


