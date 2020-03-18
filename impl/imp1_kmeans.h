
#ifndef TRY_IMP1_KMEANS_H
#define TRY_IMP1_KMEANS_H


#include "aux.h"

//class Point;
//class KeysServer;

vector<Point> getEncryptedKMeans(vector<Point> points, KeysServer & keysServer);
vector<vector<Point> > getLeftoverPoints(const vector<Point> &points, const vector<Point> &means, KeysServer & keysServer);
EncNumber calculateThreshold(vector<EncNumber> distances, int amount);


#endif //TRY_IMP1_KMEANS_H
