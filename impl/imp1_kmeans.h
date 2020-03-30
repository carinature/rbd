
#ifndef TRY_IMP1_KMEANS_H
#define TRY_IMP1_KMEANS_H


#include "aux.h"

//class Point;
//class KeysServer;

vector<DecryptedPoint> getEncryptedKMeans(vector<Point> points, KeysServer & keysServer);
vector<EncNumber> getDistances(const vector<PointExtended>& clients, const vector<DecryptedPoint>& means);
//EncNumber calculateThreshold(vector<EncNumber> distances, int amount);
EncNumber calculateThreshold(vector<EncNumber> distances, KeysServer & keysServer);
vector<vector<Point> > getLeftoverPoints(const vector<Point> &points, const vector<Point> &means, KeysServer & keysServer);



#endif //TRY_IMP1_KMEANS_H
