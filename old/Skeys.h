

#ifndef TRY_SKEYS_H
#define TRY_SKEYS_H

//#include "aux.h"
#include "Point_old.h"
#include "../impl/Point.h"

#include <NTL/BasicThreadPool.h>
NTL_CLIENT

#include <helib/FHE.h>
#include <helib/EncryptedArray.h>
#include <helib/binaryCompare.h>
#include <helib/intraSlot.h>

//static std::vector<zzX> unpackSlotEncoding; // a global variable

using DecryptedPoint =  vector<long>;  //  move to prop file

class Skeys {

    long mValue[15] =
// { p, phi(m),   m,   d, m1, m2, m3,    g1,   g2,   g3, ord1,ord2,ord3, B,c}
    {2, 48,    105,   12, 3,  35,  0,   71,    76,    0,     2,  2,  0,   25, 2};
/*
    {{2, 48,    105,   12, 3,  35,  0,   71,    76,    0,     2,  2,  0,   25, 2},
    {2, 600,   1023,  10, 11, 93,  0,   838,   584,   0,     10, 6,  0,   25, 2},
    {2, 2304,  4641,  24, 7,  3,   221, 3979,  3095,  3760,  6,  2,  -8,  25, 3},
    {2, 15004, 15709, 22, 23, 683, 0,   4099,  13663, 0,     22, 31, 0,   25, 3},
    {2, 27000, 32767, 15, 31, 7,   151, 11628, 28087, 25824, 30, 6,  -10, 28, 4}
}; */
    FHEcontext * context;
//    FHEcontext context;
    FHESecKey * secKey;
public:
    FHEPubKey * pubKey;
    Skeys();
    ~Skeys();
    Point calculateAvgPoint(const Point &point, const Vec<Ctxt>& amount);
    Vec<Ctxt> encryptLong(const long value);
    
    /** candidates for removal **/
    Point calculateAvgPointByCA(const Point &point, int amount);
    FHEPubKey * getPubKey();
    
//protected:
private:
    DecryptedPoint decryptPointByCA(const Point& p);
    vector<DecryptedPoint> decryptPointsByCA(const vector<Point> &reps);
    
    
  
    
    
    //// -------------------------- for DBG --------------------------
    
    
    
    
    
    long bin2int(vector<long> vec) {
//    cout << vec << endl;
        int     val = vec[0];
        for(int i   = 1; i < vec.size(); ++i) {
            val += pow(vec[i] * 2, i);
        }
//    cout << val << endl;
        return val;
    }
    long decryptVec(const Vec <Ctxt> &enca) {
        vector<long>   decaBit, vec;
        for(const Ctxt &a : enca) {
            context->ea->decrypt(a, *secKey, decaBit);
//        cout << decaBit.size() << "LAGOON " << decaBit << endl;
//            cout << decaBit[0];// << endl;
//        vec.insert(vec.begin(),decaBit[0]);
            vec.push_back(decaBit[0]);
        }
//        cout << endl;
        return bin2int(vec);
    }
};

#endif //TRY_SKEYS_H
