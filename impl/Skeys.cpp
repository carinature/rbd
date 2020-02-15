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
//#include <string>
//#include <iostream>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>

#include "properties.h"

bool verbose = true;

//using BinaryNew =  Ctxt;

Skeys::Skeys(){
//Skeys::Skeys(int argc, char *argv[]){
//    cout << argc << endl;
//    cout << argv << endl;
//    cout << *argv << endl;
//    cout << **argv << endl;
    
    ArgMap amap;
//    long   prm                = 1;
    long   prm                = 0;
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
    
//    amap.parse(argc, argv); //todo find a way to remove this so that Skeys can be c'tores without argc, argv arguments
    assert(prm >= 0 && prm < 5);
    if(seed) NTL::SetSeed(ZZ(seed));
    if(nthreads > 1) NTL::SetNumThreads(nthreads);
    
    if(bitSize <= 0) bitSize = 5;
    else if(bitSize > 32) bitSize = 32;

//        long *vals = mValues[prm];
    long *vals = mValue;
    long p     = vals[0];
    //  long phim = vals[1];
    long m     = vals[2];
    
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
    FHEcontext context(m, p, /*r=*/1, gens, ords);
    buildModChain(context, L, c,/*willBeBootstrappable=*/bootstrap);
    if(bootstrap) {
        context.makeBootstrappable(mvec, /* t= */0);
    }
    buildUnpackSlotEncoding(unpackSlotEncoding, *context.ea);
    if(verbose) {
        cout << " done.\n";
        context.zMStar.printout();
        cout << " L=" << L << ", B=" << B << endl;
        cout << "\ncomputing key-dependent tables..." << std::flush;
    }
    FHESecKey secKey(context);
    secKey.GenSecKey();
    addSome1DMatrices(secKey); // compute key-switching matrices
    addFrbMatrices(secKey);
    if(bootstrap) secKey.genRecryptData();
    if(verbose) cout << " done\n";
    
    activeContext = &context; // make things a little easier sometimes
//    *(this->context) = context;
    this->context = &context;
    this->secKey = &secKey;  // FHESecKey inherits from FHEPubKey
    this->pubKey = &secKey;  // todo notice that pubKey is a ptr and not value, so the trick from max's lecture
    
//    *(this->secKey) = secKey;
}

DecryptedPoint Skeys::decryptPointByCA(const Point& p) {
    DecryptedPoint decrypted;
    for (const Binary& c : p.getCoordinates()) decrypted.push_back(c.getDecValue());// todo cahnge this - decrypting only done by ca
//    for (Binary c : p.getCoordinates()) decrypted.push_back(c);// todo cahnge this - decrypting only done by ca
//    for (Binary c : p.getCoordinates()) {
//        double decValue;
//        decrypted.push_back(decValue);// todo cahnge this - decrypting only done by ca
//    }
    return decrypted;
}

NTL::Vec<Ctxt> Skeys::encryptDouble(const double value) {
    NTL::Vec<Ctxt> encValue;
    long longVal = value;
    Ctxt mu(*secKey); //, ni(secKey);
    resize(encValue, BIT_SIZE, mu);
//    resize(encb, bitSize + 1, ni);
    for(long i = 0; i <= BIT_SIZE; i++) if(i < BIT_SIZE) secKey->Encrypt(encValue[i], ZZX((longVal >> i)&1));
    return encValue;
}

vector<DecryptedPoint> Skeys::decryptPointsByCA(const vector<Point> &reps) {
    vector<DecryptedPoint> decPoints;
    decPoints.reserve(reps.size());
    for (const Point &p : reps) decPoints.push_back(Skeys::decryptPointByCA(p));
    return decPoints;
}

Point Skeys::calculateAvgPointByCA(const Point &point, int amount) {
//    cout << "be qprepared!" << endl;
//    FHESecKey secKey(*(this->context));
//    this->secKey = &secKey;
    const EncryptedArray &ea = *(context->ea);
    try {
        return point / Binary(amount);
    }
    catch (...) {
        return Point(*this, {Binary(0), Binary(0)}); ///TODO
    }
}


FHEPubKey * Skeys::getPubKey() {
    return pubKey;
}


