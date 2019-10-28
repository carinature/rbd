//
// Created by rbd on 14.10.2019.
//

#include <cassert>
#include <cmath>

#include <helib/FHE.h>
#include <helib/EncryptedArray.h>

#include <helib/intraSlot.h>
//#include <helib/binaryArith.h>
#include <helib/binaryCompare.h>
#include <helib/ArgMap.h>

#include <NTL/BasicThreadPool.h>
NTL_CLIENT

static std::vector<zzX> unpackSlotEncoding; // a global variable

using namespace std;

int main(int argc, char * argv[]){
    cout << "\n     ----    This time it's gonna work   ----" << endl;

    bool verbose=true;

    static long mValues[][15] = {
// { p, phi(m),   m,   d, m1, m2, m3,    g1,   g2,   g3, ord1,ord2,ord3, B,c}
            {  2,    48,   105, 12,  3, 35,  0,    71,    76,    0,   2,  2,   0, 25, 2},
            {  2 ,  600,  1023, 10, 11, 93,  0,   838,   584,    0,  10,  6,   0, 25, 2},
            {  2,  2304,  4641, 24,  7,  3,221,  3979,  3095, 3760,   6,  2,  -8, 25, 3}
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
    amap.arg("verbose", verbose, "print more information");

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

    if (verbose) {
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
    if (verbose) {
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
    if (verbose) cout << " done\n";

    activeContext = &context; // make things a little easier sometimes

    for (long i=0; i<nTests; i++) {
//        testCompare(secKey, bitSize, bootstrap);
        //todo  <-- inserted tempCompare here

//    const FHEcontext& context = secKey.getContext();
        const EncryptedArray &ea = *(context.ea);

        // Choose two random n-bit integers
//        long pa = RandomBits_long(bitSize);
//        long pb = RandomBits_long(bitSize + 1);

        int intSize = pow(2, bitSize) - 1;
        long pa = rand() % intSize ; //RandomBits_long(bitSize + 1);
        long pb = rand() % intSize + 1 ; //RandomBits_long(bitSize + 1);
        cout << "pa (random num 1): " << pa << "\npb (random num 2): " << pb << endl;
        long pMax = std::max(pa, pb);
        long pMin = std::min(pa, pb);
        bool pMu = pa > pb;
        bool pNi = pa < pb;

        // Encrypt the individual bits
        NTL::Vec<Ctxt> eMax, eMin, enca, encb;

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
//#ifdef DEBUG_PRINTOUT
//        decryptAndPrint((cout<<" before comparison: "), encb[0], secKey, ea,0);
//#endif

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

        /*
        {
            CtPtrs_VecCt wMin(eMin), wMax(eMax); // A wrappers around output vectors

            //cmp with max and min
            compareTwoNumbers(wMax, wMin, mu, ni,
                              CtPtrs_VecCt(enca), CtPtrs_VecCt(encb),
                              &unpackSlotEncoding);
            decryptBinaryNums(slotsMax, wMax, secKey, ea);
            decryptBinaryNums(slotsMin, wMin, secKey, ea);
        } // get rid of the wrapper
        ea.decrypt(mu, secKey, slotsMu);
        ea.decrypt(ni, secKey, slotsNi);

        if (slotsMax[0] != pMax || slotsMin[0] != pMin
            || slotsMu[0] != pMu || slotsNi[0] != pNi) {
            cout << "BAD\n";
            if (verbose)
                cout << "Comparison (with min max) error: a=" << pa << ", b=" << pb
                     << ", but min=" << slotsMin[0] << ", max=" << slotsMax[0]
                     << ", mu=" << slotsMu[0] << ", ni=" << slotsNi[0] << endl;
            exit(0);
        } else if (verbose) {
            cout << "Comparison (with min max) succeeded: ";
            cout << '(' << pa << ',' << pb << ")=>(" << slotsMin[0] << ',' << slotsMax[0]
                 << "), mu=" << slotsMu[0] << ", ni=" << slotsNi[0] << endl;
        }

         */
    }

        //todo  <-- end of insertion

//    cout << "GOOD\n";
//
//    if (verbose) printAllTimers(cout);

    cout << "\nDone!" << endl;
    return 0;






}