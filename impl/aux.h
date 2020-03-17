//
// Created by rbd on 13.1.2020.
//

#ifndef TRY_AUX_H
#define TRY_AUX_H

//#include <iostream>
//#include <string>
//#include <fstream>
//#include <sstream>
//#include <algorithm>
//#include <cassert>
//#include <cmath>       /* pow */
//#include <helib/FHE.h>
//#include <helib/EncryptedArray.h>
//#include <helib/intraSlot.h>
//#include <helib/binaryCompare.h>
//#include <helib/ArgMap.h>
//#include <NTL/BasicThreadPool.h>
#include "Point.h"
//#include "KeysServer.h"
//#include "Skeys.h"
#include "properties.h"

//using namespace std;
//using Binary = long;


//#define isNullPoint point == vector<double>(DIM, 0)
/*
using DecryptedPoint =  vector<long>;

void tag();

template<typename T>
void tag(T t);

template<typename T>
void tag(vector<T> vector);

template<typename T>
void tag(vector<vector<T> > vector);
*/

Bit cmp(const Point &a, const Point &b);

void writeToFile(const vector<Point> &vec, const string &filename, KeysServer & keysServer);

vector<DecryptedPoint> getPointsFromFile(const string & filename = "points");

vector<Point> getEncryptedPointsFromFile(KeysServer &sk);


vector<long> encryptVec(const vector<double>& vector);

long encryptDouble(double d);

/** aux **/
DecryptedPoint operator/(DecryptedPoint p, int factor);

/*
 * Candidates for a MOVE
 */
//TODO consider using function in point insted
//  double getDistFromClosestMean(vector<DecryptedPoint>)
//vector<long> getDistFromClosestMeanByClient(const vector<DecryptedPoint> &reps, const vector<Point> &pointsForDBG, Skeys &sk);


#endif //TRY_AUX_H


