//
// Created by rbd on 18.10.2019.
//

#include <iostream>
#include <NTL/BasicThreadPool.h>
NTL_CLIENT

#include <helib/FHE.h>
#include <helib/EncryptedArray.h>
#include <helib/binaryCompare.h>
#include <helib/intraSlot.h>

using namespace std;


static long mValues[][15] = {
// { p, phi(m),   m,   d, m1, m2, m3,    g1,   g2,   g3, ord1,ord2,ord3, B,c}
        {2, 48,    105,   12, 3,  35,  0,   71,    76,    0,     2,  2,  0,   25, 2},
        {2, 600,   1023,  10, 11, 93,  0,   838,   584,   0,     10, 6,  0,   25, 2},
        {2, 2304,  4641,  24, 7,  3,   221, 3979,  3095,  3760,  6,  2,  -8,  25, 3},
        {2, 15004, 15709, 22, 23, 683, 0,   4099,  13663, 0,     22, 31, 0,   25, 3},
        {2, 27000, 32767, 15, 31, 7,   151, 11628, 28087, 25824, 30, 6,  -10, 28, 4}
};

class Contextor {

public:
    static FHEcontext getContextAndKey() {
//    static FHEcontext getContextAndKey() {
        long prm = 1;             // "prm", prm, "parameter size (0-tiny,...,4-huge)");
        long bitSize = 5;       // "bitSize", bitSize, "bitSize of input integers (<=32)");
        long nTests = 1;        // "nTests", nTests, "number of tests to run");
        long seed = 0;            // "seed", seed, "PRG seed");
        long nthreads = 1;        // "nthreads", nthreads, "number of threads");
        bool bootstrap = false; // "bootstrap", bootstrap, "test comparison with bootstrapping");
        bool verbose = true;      // "verbose", verbose, "print more information");

        std::vector<zzX> unpackSlotEncoding; // a global variable


        if (seed) NTL::SetSeed(ZZ(seed));
        if (nthreads > 1) NTL::SetNumThreads(nthreads);
        if (bitSize <= 0) bitSize = 5;
        else if (bitSize > 32) bitSize = 32;


        long *vals = mValues[prm];
        long p = vals[0];
        //  long phim = vals[1];
        long m = vals[2];

        NTL::Vec<long> mvec;
        append(mvec, vals[4]);
        if (vals[5] > 1) append(mvec, vals[5]);
        if (vals[6] > 1) append(mvec, vals[6]);
        std::vector<long> gens;
        gens.push_back(vals[7]);
        if (vals[8] > 1) gens.push_back(vals[8]);
        if (vals[9] > 1) gens.push_back(vals[9]);
        std::vector<long> ords;
        ords.push_back(vals[10]);
        if (abs(vals[11]) > 1) ords.push_back(vals[11]);
        if (abs(vals[12]) > 1) ords.push_back(vals[12]);
        long B = vals[13];
        long c = vals[14];
        // Compute the number of levels
        long L;
        if (bootstrap) L = 900; // that should be enough
        else L = 30 * (7 + NTL::NumBits(bitSize + 2));

        if (verbose) {
            cout << "input bitSize=" << bitSize
                 << ", running " << nTests << " tests for each function\n";
            if (nthreads > 1) cout << "  using " << NTL::AvailableThreads() << " threads\n";
            cout << "computing key-independent tables..." << std::flush;
        }
        FHEcontext context(m, p, /*r=*/1, gens, ords);
//        FHEcontext * tempcontext = new FHEcontext(m, p, /*r=*/1, gens, ords);
//        FHEcontext context = * tempcontext;
        buildModChain(context, L, c,/*willBeBootstrappable=*/bootstrap);
        if (bootstrap) {
            context.makeBootstrappable(mvec, /*t=*/0);
        }
        buildUnpackSlotEncoding(unpackSlotEncoding, *context.ea);
        if (verbose) {
            cout << " done.\n";
            context.zMStar.printout();
            cout << " L=" << L << ", B=" << B << endl;
            cout << "\ncomputing key-dependent tables..." << std::flush;
        }
        FHESecKey secKey(context);
        secKey.GenSecKey();
        addSome1DMatrices(secKey); // compute key-switching matrices
        addFrbMatrices(secKey);
        if (bootstrap) secKey.genRecryptData();
        if (verbose) cout << " done\n";

        activeContext = &context; // make things a little easier sometimes

//        return secKey;
        return context;


    }
};