//
// Created by rbd on 31.1.2020.
//

#include "Skeys.h"

#include <unistd.h>
#include <numeric>
#include <NTL/BasicThreadPool.h>
#include <helib/ArgMap.h>
#include <helib/binaryCompare.h>
#include <helib/binaryArith.h>
#include <helib/intraSlot.h>
#include <helib/EncryptedArray.h>
#include <helib/FHE.h>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>

#include "../impl/properties.h"

//bool verbose = false;

Skeys::Skeys() {
    ArgMap amap;
//    long   prm = 0;
    long prm = 4;
    amap.arg("prm", prm, "parameter size (0-tiny,...,4-huge)");
    long bitSize = 5;
    amap.arg("bitSize", bitSize, "bitSize of input integers (<=32)");
    long nTests = 3;
//    long nTests = 4;
    amap.arg("nTests", nTests, "number of tests to run");
    bool bootstrap = false;
    amap.arg("bootstrap", bootstrap, "test comparison with bootstrapping");
    long seed = 0;
    amap.arg("seed", seed, "PRG seed");
    long nthreads = 1;
    amap.arg("nthreads", nthreads, "number of threads");
    amap.arg("verbose", verbose, "print more information");
//    amap.parse(argc, argv); //don't remove
    assert(prm >= 0 && prm < 5);
    if(seed) NTL::SetSeed(ZZ(seed));
    if(nthreads > 1) NTL::SetNumThreads(nthreads);
    if(bitSize <= 0) bitSize = 5;
    else if(bitSize > 32) bitSize = 32;
    long * vals = mValue;
    long p = vals[0];
    //  long phim = vals[1];
    long m = vals[2];
    NTL::Vec<long> mvec;
    append(mvec, vals[4]);
    if(vals[5] > 1) append(mvec, vals[5]);
    if(vals[6] > 1) append(mvec, vals[6]);
    std::vector<long> gens;
    gens.push_back(vals[7]);
    if(vals[8] > 1) gens.push_back(vals[8]);
    if(vals[9] > 1) gens.push_back(vals[9]);
    std::vector<long> ords;
    ords.push_back(vals[10]);
    if(abs(vals[11]) > 1) ords.push_back(vals[11]);
    if(abs(vals[12]) > 1) ords.push_back(vals[12]);
    long B = vals[13];
    long c = vals[14];
    // Compute the number of levels
    long L;
    if(bootstrap) L = 900; // that should be enough
    else L = 30 * (7 + NTL::NumBits(bitSize + 2));
    if(verbose) {
        cout << "input bitSize=" << bitSize
             << ", running " << nTests << " tests for each function\n";
        if(nthreads > 1) cout << "  using " << NTL::AvailableThreads() << " threads\n";
        cout << "computing key-independent tables..." << std::flush;
    }
    FHEcontext * context = new FHEcontext(m, p, /*r=*/1, gens, ords);
    const FHEcontext * constContext = new FHEcontext(m, p, /*r=*/1, gens, ords);
    buildModChain(*context, L, c,/*willBeBootstrappable=*/bootstrap);
    if(bootstrap) {
        context->makeBootstrappable(mvec, /* t= */0);
    }
    buildUnpackSlotEncoding(unpackSlotEncoding, *context->ea);
    if(verbose) {
        cout << " done.\n";
        context->zMStar.printout();
        cout << " L=" << L << ", B=" << B << endl;
        cout << "\ncomputing key-dependent tables..." << std::flush;
    }
    
    FHESecKey secKey(*context);
    secKey.GenSecKey();
    addSome1DMatrices(secKey); // compute key-switching matrices
    addFrbMatrices(secKey);
    if(bootstrap) secKey.genRecryptData();
    if(verbose) cout << " done\n";
    
    activeContext = context; // make things a little easier sometimes
    this->context = context;
    this->pubKey = new FHESecKey(secKey);  //  notice that pubKey is a ptr and not value, so the trick from max's lecture might not work
//    this->secKey = new FHESecKey(secKey);  // FHESecKey inherits from FHEPubKey
    this->secKey = (FHESecKey *) this->pubKey;  //  notice that pubKey is a ptr and not value, so the trick from max's lecture might not work
}


Skeys::~Skeys() {
//    cout << "Skeys::~Skeys()" << endl;
    delete (context);
    delete (pubKey);
//     delete(secKey);
}

Vec<Ctxt> Skeys::encryptLong(const long value) {
    NTL::Vec<Ctxt> encValue;
    long longVal = value;
    Ctxt mu(*secKey); //, ni(secKey);
    resize(encValue, BIT_SIZE, mu);
//    resize(encb, bitSize + 1, ni);
    for(long i = 0; i <= BIT_SIZE; i++) if(i < BIT_SIZE) secKey->Encrypt(encValue[i], ZZX((longVal >> i)&1));
    return encValue;
}

DecryptedPoint Skeys::decryptPointByCA(const Point & p) {
    DecryptedPoint decrypted;
    FHESecKey * temp = this->secKey; // just to use something of skeys so it won't try to make it static
    for(const Vec<Ctxt> & c : p.encCoordinates)
        decrypted.push_back(decryptVec(c));// todo cahnge this - decrypting only done by ca
    return decrypted;
}

vector<DecryptedPoint> Skeys::decryptPointsByCA(const vector<Point> & reps) {
    vector<DecryptedPoint> decPoints;
    decPoints.reserve(reps.size());
    for(const Point & p : reps) decPoints.push_back(Skeys::decryptPointByCA(p));
    return decPoints;
}


FHEPubKey * Skeys::getPubKey() {
    return pubKey;
}

Point Skeys::calculateAvgPoint(const Point & point, const Vec<Ctxt>& amount) {
    vector<Vec<Ctxt> > vec = {amount, amount};
    return Point(*this, vec);
}

/** candidates for removal **/

Point Skeys::calculateAvgPointByCA(const Point & point, int amount) {
//    cout << "be qprepared!" << endl;
//    FHESecKey secKey(*(this->context));
//    this->secKey = &secKey;
    const EncryptedArray & ea = *(context->ea);
    try {
        return point / Binary(amount);
    }
    catch(...) {
        return Point(this, {Binary(0), Binary(0)}); ///TODO
//        return Point(*this, {Binary(0), Binary(0)});
    }
}
