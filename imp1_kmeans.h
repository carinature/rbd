//
// Created by rbd on 24.12.2019.
//

#ifndef TRY_IMP1_KMEANS_H
#define TRY_IMP1_KMEANS_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <math.h>       /* pow */
#include <cassert>
#include <cmath>
#include <helib/FHE.h>
#include <helib/EncryptedArray.h>
#include <helib/intraSlot.h>
#include <helib/binaryCompare.h>
#include <helib/ArgMap.h>
#include <NTL/BasicThreadPool.h>
#include "Point.h"
//#include "Bit.h" // todo add
#include "Binary.h"
#include "aux.h"

vector<vector<double> > getPointsFromFile();
bool cmp(vector<double> a, vector<double>);
vector<vector<double> > getReps(vector<vector<double> >);
vector<Point> getEncryptedPointsFromFile();


bool cmp(const Point& a, const Point& b);



#endif //TRY_IMP1_KMEANS_H