

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
    vector<Point> ePoints = getEncryptedPointsFromFile(*ks);
    
    const string & filename = "points_test";
    writeToFile(ePoints, filename, *ks);
    
    vector<DecryptedPoint> points_test = getPointsFromFile(filename);
    
    for(int i = 0; i < points.size(); ++i) {
//        cout << "-- i --: "  << i++ << endl;
        vector<long> & vector = points[i];
//        cout << "11 " << vector <<  endl;
        std::vector<long> & vector1 = points_test[i];
//        cout << "11 " << vector1 << endl;
        std::vector<long> vector2 = ePoints[i].decrypt(*ks);
//        cout << "11 " << vector2 << endl;
//        assert(vector == vector1);
    }
    cout << "           OK" << endl;
}
void aux_test::getEncryptedPointsFromFileTest() {
    cout << " --------- test_get_enc_point_from_file --------- " << endl;
    KeysServer * ks = new KeysServer();
    FHEPubKey * pubKey = ks->pubKey;
    
    vector<DecryptedPoint> points = getPointsFromFile();
    vector<Point> ePoints = getEncryptedPointsFromFile(*ks);
    
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


