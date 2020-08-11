

#include <vector>

#ifndef TRY_AUX_TEST_H
#define TRY_AUX_TEST_H


class aux_test {
    
    static void createCmpDictTest();
    static void getEncryptedPointsFromFileTest();

public:
    static void runAll();
    
    static void writeToFileTest();
};

//void getEncryptedPointsFromFileTest();
//void writeToFileTest();

#endif //TRY_AUX_TEST_H

/** aux **/
bool operator==(std::vector<long> a, std::vector<long> b);
