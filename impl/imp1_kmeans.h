
#ifndef TRY_IMP1_KMEANS_H
#define TRY_IMP1_KMEANS_H


#include "aux.h"

//class Point;
//class KeysServer;

vector<tuple<vector<Point>, EncNumber> > getCells(vector<Point> points, KeysServer & keysServer);

DecryptedPoint calculateCellMean(tuple<vector<Point>, EncNumber> cellTuple, KeysServer & keysServer);

vector<DecryptedPoint>
calculateMeans(const vector<tuple<vector<Point>, EncNumber> > & cellTuples, KeysServer & keysServer);

vector<DecryptedPoint> getEncryptedKMeans(vector<Point> points, KeysServer & keysServer);

Vec<EncNumber> getDistances(const vector<PointExtended> & clients, const vector<DecryptedPoint> & means);

//EncNumber calculateThreshold(vector<EncNumber> distances, int amount);
EncNumber calculateThreshold(Vec<EncNumber> distances, KeysServer & keysServer);

vector<vector<Point> >
getLeftoverPoints(const vector<Point> & points, const vector<Point> & means, KeysServer & keysServer);

#endif //TRY_IMP1_KMEANS_H
