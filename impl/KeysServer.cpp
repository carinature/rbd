

#include "KeysServer.h"
#include "Point.h"

//helib
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
#include <utility>
#include <vector>

KeysServer::KeysServer() {
    ArgMap amap;
//    long   prm = 0;
    long prm = 4;
    amap.arg("prm", prm, "parameter size (0-tiny,...,4-huge)");
    long bitSize = 5;
    amap.arg("bitSize", bitSize, "bitSize of input integers (<=32)");
    long nTests = 3;
//    long nTests = 4;
    amap.arg("nTests", nTests, "number of tests to run");
    bool bootstrap = true; //    fixme in original test it's : bool bootstrap = false;
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
    long * vals = mValues[0];
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
    auto * fheContext = new FHEcontext(m, p, /*r=*/1, gens, ords);
//    const FHEcontext * constContext = new FHEcontext(m, p, /*r=*/1, gens, ords);
    buildModChain(*fheContext, L, c,/*willBeBootstrappable=*/bootstrap);
    if(bootstrap) {
        fheContext->makeBootstrappable(mvec, 0);
    }
    buildUnpackSlotEncoding(unpackSlotEncoding, *fheContext->ea);
    if(verbose) {
        cout << " done.\n";
        fheContext->zMStar.printout();
        cout << " L=" << L << ", B=" << B << endl;
        cout << "\ncomputing key-dependent tables..." << std::flush;
    }
    
    FHESecKey fheSecKey(*fheContext);
    fheSecKey.GenSecKey();
    addSome1DMatrices(fheSecKey); // compute key-switching matrices
    addFrbMatrices(fheSecKey);
    if(bootstrap) fheSecKey.genRecryptData();
    if(verbose) cout << " done\n";
    
    activeContext = fheContext; // make things a little easier sometimes
    this->context = fheContext;
//    this->pubKey = new FHESecKey(fheSecKey);  // todo notice that pubKey is a ptr and not value, so the trick from max's lecture might not work
    this->secKey = new FHESecKey(fheSecKey);  // FHESecKey inherits from c
//    this->pubKey = (FHESecKey *) this->secKey;  // todo notice that pubKey is a ptr and not value, so the trick from max's lecture might not work
    this->pubKey = (FHEPubKey *) this->secKey;  // todo notice that pubKey is a ptr and not value, so the trick from max's lecture might not work
//    this->pubKey = new FHEPubKey(*fheContext);  // todo notice that pubKey is a ptr and not value, so the trick from max's lecture might not work
}

KeysServer::~KeysServer() {
    delete secKey; //fixme why you complain?
//    cout << "delete secKey OK" <<endl;
    delete context;
//    cout << "delete context OK" <<endl;
}

Ctxt KeysServer::randomBit() {
    long c = rand();  // fixme should be random
    Ctxt mu(*pubKey);
    pubKey->Encrypt(mu, ZZX(c&1)); ////    <----   THE PROBLEM was HERE
    return mu;
}

//DecryptedPoint KeysServer::calculateAvgPoint(const Point& r, const Point& p, EncNumber size) { //todo so you can subtract - for better accuracy
DecryptedPoint KeysServer::calculateAvgPoint(const Point & p, EncNumber size) {
    cout << "   --- calculateAvgPoint ---   ";
    const long amount = decrypt(std::move(size)) + 1;
    vector<long> coorVector;
    for(const EncNumber & coor : p.eCoordinates) {
        coorVector.push_back(decrypt(coor) / amount);
    }
    const vector<long> sumVec = std::move(coorVector); //todo remove
//    Point avgPoint = PointExtended(this, sumVec);
    cout << "decrypted mean: " << sumVec; // << endl;
    cout << "   decrypted size: " << amount << endl << endl;
    return sumVec;
    
}
//
//Point KeysServer::calculateAvgPoint(Point p, EncNumber size) {
//    cout << "   - calculateAvgPoint -   ";
//    long amount = decrypt(std::move(size)) + 1;
//    vector<long> coorVector;
//    for(const EncNumber & coor : p.eCoordinates) {
//        coorVector.push_back(decrypt(coor) / amount);
//    }
//    const vector<long> constVec = std::move(coorVector);
//    Point avgPoint = PointExtended(this, constVec);
//    return avgPoint;
//
//}


long KeysServer::decrypt(EncNumber n) {
//    cout << "decrypt in KS" << endl;
    CtPtrs_VecCt eep(n);
    vector<long> slots;
    //todo replace with keysServer.decrypt - secKey should not be public
    decryptBinaryNums(slots, eep, *(secKey), *(pubKey->getContext().ea));
    return slots[0];
}
