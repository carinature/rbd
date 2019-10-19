//
// Created by rbd on 15.10.2019.
//

#ifndef HELIB_SUPERBUILD_HECMP_H
#define HELIB_SUPERBUILD_HECMP_H

#include <iostream>
#include <NTL/BasicThreadPool.h>
NTL_CLIENT

#include <helib/FHE.h>
#include <helib/EncryptedArray.h>
#include <helib/binaryCompare.h>
#include <helib/intraSlot.h>

using namespace std;

//static long mValues[][15] = {
//// { p, phi(m),   m,   d, m1, m2, m3,    g1,   g2,   g3, ord1,ord2,ord3, B,c}
//        {  2,    48,   105, 12,  3, 35,  0,    71,    76,    0,   2,  2,   0, 25, 2},
//        {  2 ,  600,  1023, 10, 11, 93,  0,   838,   584,    0,  10,  6,   0, 25, 2},
//        {  2,  2304,  4641, 24,  7,  3,221,  3979,  3095, 3760,   6,  2,  -8, 25, 3},
//        {  2, 15004, 15709, 22, 23,683,  0,  4099, 13663,    0,  22, 31,   0, 25, 3},
//        {  2, 27000, 32767, 15, 31,  7, 151, 11628, 28087,25824, 30,  6, -10, 28, 4}
//};
bool bootstrap = false;

class HEcmp {
//    FHEcontext context;
//    std::vector<FHEcontext> conVec;
    FHESecKey secKey;

public:

//    HEcmp(FHESecKey secKey) : secKey(secKey.getContext()){
//        FHEcontext context = secKey.getContext();

    HEcmp(FHEcontext context) : secKey(context){
//        secKey.getContext();
        secKey.GenSecKey();
        addSome1DMatrices(secKey); // compute key-switching matrices
        addFrbMatrices(secKey);
        if (bootstrap) secKey.genRecryptData();
//        if (verbose) cout << " done\n";
        activeContext = &context; // make things a little easier sometimes

        cout << "NO Way!" << endl;
    }
    ~HEcmp() = default;




    Vec<Ctxt> encryptNumber(long pa, long bitSize) {
        cout << " ------1------- " << endl;
        const FHEcontext &context = secKey.getContext();
        cout << " ------2------- " << endl;
        const EncryptedArray &ea = *(context.ea);
        cout << " ------3------- " << endl;

        // Encrypt the individual bits
        NTL::Vec<Ctxt> eMax, eMin, enca, encb;
        vector<long> slotsMin, slotsMax, slotsMu, slotsNi;
        cout << " ------4------- " << endl;

        Ctxt mu(secKey), ni(secKey);
        cout << " ------5------- " << endl;
        resize(enca, bitSize, mu);
        cout << "size: " << enca[0] << endl;
        cout << "size: " << enca[1] << endl;
        cout << "size: " << enca[2] << endl;
        cout << "size: " << enca[3] << endl;
        cout << "size: " << enca[4] << endl;
        cout << "size: " << enca[5] << endl;
        cout << "size: " << enca[6] << endl;
        cout << "bla: " << ZZX((pa >> 0) & 1) << endl;
        cout << "bla: " << ZZX((pa >> 1) & 1) << endl;
        cout << "bla: " << ZZX((pa >> 2) & 1) << endl;
        cout << "bla: " << ZZX((pa >> 3) & 1) << endl;
        cout << "bla: " << ZZX((pa >> 4) & 1) << endl;
        cout << "bla: " << ZZX((pa >> 5) & 1) << endl;
        secKey.Encrypt(enca[0], ZZX((pa >> 0) & 1));
//        cout << "size: " << enca.size() << endl;
        cout << " ------6------- " << endl;
//        resize(encb, bitSize + 1, ni);
//        cout << " ------------- " << endl;

        for (long i = 0; i <= bitSize; ++i) {
            cout << " -----====------ " << endl;
            if (i < bitSize) secKey.Encrypt(enca[i], ZZX((pa >> i) & 1));
//            secKey.Encrypt(encb[i], ZZX((pb >> i) & 1));
            cout << " -------||||------ " << endl;
//            if (bootstrap) { // put them at a lower level
//                if (i < bitSize) enca[i].bringToSet(context.getCtxtPrimes(5));
////                encb[i].bringToSet(context.getCtxtPrimes(5));
//            }

        } // end of bitSize for
        return enca;
    }
/*
        //    const FHEcontext& context = secKey.getContext();
        const EncryptedArray &ea = *(context.ea);

//// this is for dbg mainly
////        cout << "pa (random num 1): " << pa << "\npb (random num 2): " << pb << endl;
//        long pMax = std::max(pa, pb);
//        long pMin = std::min(pa, pb);
//        bool pMu = pa > pb;
//        bool pNi = pa < pb;

//        for (long i=0; i<nTests; i++) {

            // Encrypt the individual bits
            NTL::Vec<Ctxt> eMax, eMin, enca, encb;
            vector<long> slotsMin, slotsMax, slotsMu, slotsNi;

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
            } // end of bitSize for


    bool cmp(long enca, long encb){

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

//        } // end of nTests for

        return slotsMu[0] > slotsNi[0];

        return false;
   }
*/


};


#endif //HELIB_SUPERBUILD_HECMP_H
