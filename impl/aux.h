
#ifndef TRY_AUX_H
#define TRY_AUX_H

#include "Point.h"
#include "properties.h"
#include <map>

//Bit cmp(const Point & a, const Point & b) ;

void writeToFile(const vector<Point> & vec, const string & filename, KeysServer & keysServer);
void decWriteToFile(const vector<DecryptedPoint> & vec, const string & filename, KeysServer & keysServer);

vector<DecryptedPoint> getPointsFromFile(const string & filename = "points");

vector<Point> getEncryptedPointsFromFile(KeysServer & sk);
vector<long> encryptVec(const vector<double> & vector);

long encryptDouble(double d);

/** aux **/
DecryptedPoint operator/(DecryptedPoint p, int factor);

struct cmpPoints{
    bool operator()(const Point & a, const Point & b){
        return a.id>b.id;
    }
};

map<Point, map<Point, vector<Bit>, cmpPoints>, cmpPoints>
createCmpDict(const vector<Point> & randomPoints, const vector<Point> & stripPoints);

/*
 * Deprecated
 */
/*
using namespace std;
using Binary = long;

vector<long> getDistFromClosestMeanByClient(const vector<DecryptedPoint> &reps, const vector<Point> &pointsForDBG, Skeys &sk);

#define isNullPoint point == vector<double>(DIM, 0)

using DecryptedPoint =  vector<long>;

void tag();

template<typename T>
void tag(T t);

template<typename T>
void tag(vector<T> vector);

template<typename T>
void tag(vector<vector<T> > vector);
*/


#endif //TRY_AUX_H


