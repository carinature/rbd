//
// Created by rbd on 24.12.2019.
//

#ifndef TRY_IMP1_KMEANS_H
#define TRY_IMP1_KMEANS_H


#include "aux.h"
//#include "Skeys.h"

vector<Point> getEncryptedKMeans(vector<Point> points, Skeys &sk);
vector<vector<Point> > getLeftoverPoints(const vector<Point> &points, const vector<Point> &means, Skeys &sk);
Binary calculateThreshold(vector<Binary> distances, int amount);


#endif //TRY_IMP1_KMEANS_H
