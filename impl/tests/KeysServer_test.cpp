//
// Created by rbd on 15.3.2020.
//

#include "KeysServer_test.h"
#include "../KeysServer.h"
#include "../properties.h"
#include <iostream>
#include <cassert>
using namespace std;

void KeysServer_test::test_ctor() {
    cout << " -------- test_ctor -------- "   << endl;
    KeysServer ks = KeysServer();
    ks.pubKey;
//    delete(sk);
}
