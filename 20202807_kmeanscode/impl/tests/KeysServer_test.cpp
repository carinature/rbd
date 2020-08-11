

#include "KeysServer_test.h"
#include "../KeysServer.h"
#include "../properties.h"
#include <iostream>
#include <cassert>

using namespace std;

void KeysServer_test::test_ctor() {
    cout << " -------- test_ctor -------- " << endl;
    KeysServer ks = KeysServer();
    ks.pubKey;
    cout << "           OK" << endl;
//  delete ks;
}

void KeysServer_test::test_dtor() {
    cout << " -------- test_dtor -------- " << endl;
    KeysServer ks = KeysServer();
    ks.pubKey;
    cout << "           OK" << endl;
//  delete ks;
}

void KeysServer_test::run_all() {
    test_ctor();
    test_dtor();
}
