//
// Created by rbd on 15.10.2019.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <assert.h>

#include "HEcmp.h"
#include "Contextor.cpp"


#include <helib/ArgMap.h>

using namespace std;
//using String = std::string;

#define DIM 2
#define EPS 2


bool cmp(vector<double> a, vector<double> b){
    return a[DIM-1]>b[DIM-1];
}


int main3(int argc, char *argv[]){
    cout << "CommonCommon!" << endl;

    ifstream inputFileStream("points");
    int count;  //  #points
    inputFileStream>>count;
    cout << "count: " << count << endl;
    inputFileStream.ignore(1, '\n');

    string line, pa0, pa1;
    vector<vector<double> > points;
    std::vector<double> xVec, yVec;

    for(int i = 0; i < count; ++i) {
        getline(inputFileStream, line);
        istringstream lineStream(line);

        getline(lineStream, pa0, ',');
        getline(lineStream, pa1, ',');

        points.push_back({stod(pa0), stod(pa1)});
    }

//    for (int i=0; i<count; ++i) {
//        for (int j = 0; j < DIM; ++j) {
//            cout << "points[" << i << "][0]: " << points[i][j] << "\t\t"; // << endl;
//        }
//        cout << endl;
//    }
    nth_element(points.begin(), points.begin() + points.size()/ 2, points.end(), cmp);
    cout << "The median is " << points[points.size()/ 2] << '\n';
//    for (int i=0; i<count; ++i) {
//        for (int j = 0; j < DIM; ++j) {
//            cout << "points[" << i << "][0]: " << points[i][j] << "\t\t"; // << endl;
//        }
//        cout << endl;
//    }




//    ****************************************************************************************
//    ****************************************************************************************
//    ****************************************************************************************
//    ****************************************************************************************
//    ****************************************************************************************
//    ****************************************************************************************
//    ****************************************************************************************
//    ****************************************************************************************
//    ****************************************************************************************
//    ****************************************************************************************
//    ****************************************************************************************
//    ****************************************************************************************
//    ****************************************************************************************
//    ****************************************************************************************

//    FHEcontext context = ;
//    HEcmp h = HEcmp(Contextor::getContextAndKey());
//    cout << "lalalallalalalala" << endl;
//    h.encryptNumber(5, 7);

    static std::vector<zzX> unpackSlotEncoding; // a global variable
    long mValues[][15] = {
// { p, phi(m),   m,   d, m1, m2, m3,    g1,   g2,   g3, ord1,ord2,ord3, B,c}
            {  2,    48,   105, 12,  3, 35,  0,    71,    76,    0,   2,  2,   0, 25, 2}
//            {  2 ,  600,  1023, 10, 11, 93,  0,   838,   584,    0,  10,  6,   0, 25, 2},
//            {  2,  2304,  4641, 24,  7,  3,221,  3979,  3095, 3760,   6,  2,  -8, 25, 3},
//            {  2, 15004, 15709, 22, 23,683,  0,  4099, 13663,    0,  22, 31,   0, 25, 3},
//            {  2, 27000, 32767, 15, 31,  7, 151, 11628, 28087,25824, 30,  6, -10, 28, 4}
    };


    ArgMap amap;
    long prm=1;
    amap.arg("prm", prm, "parameter size (0-tiny,...,4-huge)");
    long bitSize = 5;
    amap.arg("bitSize", bitSize, "bitSize of input integers (<=32)");
//    long nTests = 3;
    long nTests = 2;
    amap.arg("nTests", nTests, "number of tests to run");
    bool bootstrap = false;
    amap.arg("bootstrap", bootstrap, "test comparison with bootstrapping");
    long seed=0;
    amap.arg("seed", seed, "PRG seed");
    long nthreads=1;
    amap.arg("nthreads", nthreads, "number of threads");
    bool verbose = true;
    amap.arg("verbose", verbose = true, "print more information");
    amap.parse(argc, argv);
    assert(prm >= 0 && prm < 5);
    if (seed) NTL::SetSeed(ZZ(seed));
    if (nthreads>1) NTL::SetNumThreads(nthreads);

    if (bitSize<=0) bitSize=5;
    else if (bitSize>32) bitSize=32;

    long* vals = mValues[prm];
    long p = vals[0];
    //  long phim = vals[1];
    long m = vals[2];

    NTL::Vec<long> mvec;
    append(mvec, vals[4]);
    if (vals[5]>1) append(mvec, vals[5]);
    if (vals[6]>1) append(mvec, vals[6]);

    std::vector<long> gens;
    gens.push_back(vals[7]);
    if (vals[8]>1) gens.push_back(vals[8]);
    if (vals[9]>1) gens.push_back(vals[9]);

    std::vector<long> ords;
    ords.push_back(vals[10]);
    if (abs(vals[11])>1) ords.push_back(vals[11]);
    if (abs(vals[12])>1) ords.push_back(vals[12]);

    long B = vals[13];
    long c = vals[14];

    // Compute the number of levels
    long L;
    if (bootstrap) L = 900; // that should be enough
    else           L = 30*(7+ NTL::NumBits(bitSize+2));

    if (verbose = true) {
        cout <<"input bitSize="<<bitSize
             <<", running "<<nTests<<" tests for each function\n";
        if (nthreads>1) cout << "  using "<<NTL::AvailableThreads()<<" threads\n";
        cout << "computing key-independent tables..." << std::flush;
    }
    FHEcontext context(m, p, /*r=*/1, gens, ords);
    buildModChain(context, L, c,/*willBeBootstrappable=*/bootstrap);
    if (bootstrap) {
        context.makeBootstrappable(mvec, /*t=*/0);
    }
    buildUnpackSlotEncoding(unpackSlotEncoding, *context.ea);
    if (verbose = true) {
        cout << " done.\n";
        context.zMStar.printout();
        cout << " L="<<L<<", B="<<B<<endl;
        cout << "\ncomputing key-dependent tables..." << std::flush;
    }
    FHESecKey secKey(context);
    secKey.GenSecKey();
    addSome1DMatrices(secKey); // compute key-switching matrices
    addFrbMatrices(secKey);
    if (bootstrap) secKey.genRecryptData();
    if (verbose = true) cout << " done\n";

    activeContext = &context; // make things a little easier sometimes

//----------------------------------------------------------------------------------
//-------------------------     so far creating context and key --------------------
//----------------------------------------------------------------------------------




    for (long i=0; i<nTests; i++) {
//        testCompare(secKey, bitSize, bootstrap);
        //todo  <-- inserted tempCompare here

//    const FHEcontext& context = secKey.getContext();
        const EncryptedArray &ea = *(context.ea);

        // Choose two random n-bit integers
//        long pa = RandomBits_long(bitSize);
//        long pb = RandomBits_long(bitSize + 1);

        int intSize = pow(2, bitSize) - 1;
        long pa = rand() % intSize; //RandomBits_long(bitSize + 1);
        long pb = rand() % intSize + 1; //RandomBits_long(bitSize + 1);
        cout << "pa (random num 1): " << pa << "\npb (random num 2): " << pb << endl;
        long pMax = std::max(pa, pb);
        long pMin = std::min(pa, pb);
        bool pMu = pa > pb;
        bool pNi = pa < pb;

        // Encrypt the individual bits
        NTL::Vec<Ctxt> eMax, eMin, enca, encb;

// ---------------------------------- encrypting the numbers
        Ctxt mu(secKey), ni(secKey);
        resize(enca, bitSize, mu);
        resize(encb, bitSize + 1, ni);
        for (long i = 0; i <= bitSize; i++) {
            if (i < bitSize) secKey.Encrypt(enca[i], ZZX((pa >> i) & 1));
            secKey.Encrypt(encb[i], ZZX((pb >> i) & 1));
            if (bootstrap) { // put them at a lower level
                if (i < bitSize) enca[i].bringToSet(context.getCtxtPrimes(5));
                encb[i].bringToSet(context.getCtxtPrimes(5));
            }
        }

        vector<long> slotsMin, slotsMax, slotsMu, slotsNi;

        //cmp only
        compareTwoNumbers(mu, ni, CtPtrs_VecCt(enca), CtPtrs_VecCt(encb),
                          &unpackSlotEncoding);
        ea.decrypt(mu, secKey, slotsMu);
        ea.decrypt(ni, secKey, slotsNi);
        if (slotsMu[0] != pMu || slotsNi[0] != pNi) {
            cout << "BAD\n";
            if (verbose)
                cout << "Comparison (without min max) error: a=" << pa << ", b=" << pb
                     << ", mu=" << slotsMu[0] << ", ni=" << slotsNi[0] << endl;
            exit(0);
        } else if (verbose) {
            cout << "Comparison (without min max) succeeded: ";
            cout << '(' << pa << ',' << pb << ")=> mu=" << slotsMu[0] << ", ni=" << slotsNi[0] << endl;
        }
    }

    return 0;
}














/*
 * insert all the points into array
 * cut the array into equal pieces
 * find median in each piece (which meansfinding the median of each piece)
 *      make an algorithm for randomly choosing the "median"
 *      make an algorithm for finding the median
 * ---->this is the way to choose the repesentatives
 *
 * note the points should be encrypted
 *
 */