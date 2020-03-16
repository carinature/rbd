// Created by rbd on 15.3.2020.

#ifndef TRY_KEYSSERVER_H
#define TRY_KEYSSERVER_H

// helib
#include <helib/FHE.h>
#include <helib/EncryptedArray.h>
#include <helib/binaryCompare.h>
#include <helib/intraSlot.h>
#include <NTL/BasicThreadPool.h>
NTL_CLIENT
static std::vector<zzX> unpackSlotEncoding; // a global variable

//mine
#include "properties.h"

class KeysServer {
public:
    /** params **/
    FHEcontext * context;
    FHESecKey * secKey; //todo move to private!!

protected:
    long mValues[8][15] = {
// { p, phi(m),   m,   d, m1, m2, m3,    g1,   g2,   g3, ord1,ord2,ord3, B,c}
            {  2,    48,   105, 12,   3, 35,  0,    71,    76,    0,   2,  2,   0, 25, 2}/*
            {  2 ,  600,  1023, 10,  11, 93,  0,   838,   584,    0,  10,  6,   0, 25, 2},
            {  2,  2304,  4641, 24,   7,  3,221,  3979,  3095, 3760,   6,  2,  -8, 25, 3},
            {  2,  5460,  8193, 26,8193,  0,  0,    46,     0,    0, 210,  0,   0, 25, 3},
            {  2,  8190,  8191, 13,8191,  0,  0,    39,     0,    0, 630,  0,   0, 25, 3},
            {  2, 10752, 11441, 48,  17,673,  0,  4712,  2024,    0,  16,-14,   0, 25, 3},
            {  2, 15004, 15709, 22,  23,683,  0,  4099, 13663,    0,  22, 31,   0, 25, 3},
            {  2, 27000, 32767, 15,  31,  7,151, 11628, 28087,25824,  30,  6, -10, 28, 4}
*/    };

public:
    FHEPubKey * pubKey;
//    FHEPubKey & pubKey;

/** methods **/
    KeysServer();
};

#endif //TRY_KEYSSERVER_H
