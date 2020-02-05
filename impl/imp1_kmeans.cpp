//
// Created by rbd on 24.12.2019.
//
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>       /* pow */

//helib
#include <cassert>
#include <cmath>
#include <helib/FHE.h>
#include <helib/EncryptedArray.h>
#include <helib/intraSlot.h>
#include <helib/binaryArith.h>
#include <helib/binaryCompare.h>
#include <helib/ArgMap.h>

#include <NTL/BasicThreadPool.h>
#include <numeric>
#include <unistd.h>

NTL_CLIENT
static std::vector<zzX> unpackSlotEncoding; // a global variable


#include "imp1_kmeans.h"
#include "aux.h"

//#include "Bit.h"
using Bit = bool;//todo remove

using namespace std;

int main(int argc, char *argv[]) {
/*
    Bit bit1(true);
    Bit bit0((short)0);
    cout << bit0 << endl;
    cout << bit1 << endl;
    vector<Binary> bin0 = {Binary(0)};
    vector<Binary> bin1 = {Binary(1)};
    vector<Binary> bin2 = {Binary(2)};
    Point p0(bin0);
    Point p1(bin1);
    Point p3({Binary(3), Binary(2), Binary(0)});
//    cout << bin0 << bin1 << p0 << p1 << endl;
    cout << p1*bit1 << endl;
    cout << p3*bit1 << endl;
    cout << p3*bit0 << endl;

//    cout << " ====== points (for loop) ====== " << endl;
//    for (vector<double> point : points) cout << point << endl;
//    cout << " ====== representatives (for loop) ====== " << endl;
//    for (vector<double> rep : representatives) cout << rep << endl;

    cout << true * true << endl;
    cout << true + true << endl;
    cout << true * false << endl;
    cout << true + false << endl;

    //    vector<Binary> bin0 = {Binary(0)};
//    vector<Binary> bin1 = {Binary(1)};
//    Point p0(bin0);
//    Point p1(bin1);
//    cout << points << endl;
 */
    vector<Point> encPoints = getEncryptedPointsFromFile();
    vector<Point> encMeans = getEncryptedKMeans(encPoints);
    vector<vector<Point> > leftoverPoints = getLeftoverPoints(encPoints, encMeans); //TODO SEGFAUT
    writeToFile(encMeans, "means");
    writeToFile(leftoverPoints[0], "chosen");
    writeToFile(leftoverPoints[1], "leftover");
}

vector<Point> getEncryptedKMeans(vector<Point> points) {
    vector<Point> means, leftover, chosen;
    int numOfStrips = (int) (1 / EPSILON);
    int numOfCells = (int) (1 / pow(EPSILON, 2));
    cout << "\nEPSILON: " << EPSILON << endl;
    cout << "numOfStrips: " << numOfStrips << endl;
    cout << "numOfCells: " << numOfCells << endl;
    tag();
    //for every strip do
    for (int i = 0; i < numOfStrips; ++i) {
        //todo note the loss of points in tail (need to correct)
        ////    Fisher–Yates shuffle for choosing k random points
        int stripSize = (int) (points.size() * EPSILON), k = numOfStrips; //, left = stripSize;
        vector<Point> currStrip(points.begin() + i * stripSize,
                                points.begin() + ((i + 1) * stripSize));
        vector<Point> copy(currStrip);
        cout << "currStrip" << endl;
        cout << currStrip << endl;
        cout << "\n\ncurrent Strip of size " << currStrip.size() << currStrip << endl;
        auto begin = copy.begin();
        while (k--) {
            auto r = begin;
            advance(r, random() % stripSize); //TODO this line crushes the program with small number of points from "points"
            swap(*begin, *r);
            ++begin;
            --stripSize;
        }
        vector<Point> random(copy.begin(), copy.begin() + numOfStrips);
        cout << "random points: " << random << endl << endl;

        ////    CUT INTO CELLS
        vector<Point> stripMeans;
        //todo create cmp-dict
        //for each rand point
        for (const Point &R : random) {
            ////  find the points for each cell
            vector<Point> currCell;
            Point sum = currStrip[0];
            Binary amount = 0;
            //  for each point in the strip
            for (const Point &p : currStrip) {
                //        is above other stripMeans
                vector<Bit> cmpIsAbove;
                for (const Point &r2 : random) {
                    Bit r2smaller = cmp(p, r2) * cmp(R, r2);
                    Bit r2bigger = cmp(r2, p) * cmp(r2, R);
                    Bit isAboveR2 = r2smaller + r2bigger - r2smaller * r2bigger;
                    cmpIsAbove.push_back(isAboveR2);  // curently the vector is for DBG porpuses
                    //todo instead of vector and then "accumulate" just accumulate in cmpIsAbove*=tecmpcmp
                    //               cmpIsAbove *= isAboveR2;  overload op *= in Bit

                }
                Bit isAboveOtherReps = accumulate(cmpIsAbove.begin(), cmpIsAbove.end(), 1, multiplies<Bit>());
                Bit isInCell = cmp(R, p) * isAboveOtherReps;   //  is in  = is under random * is over other randoms
                Point isPoint = p * isInCell;
                currCell.push_back(isPoint);
                sum += isPoint;
                amount += isInCell;
            }
            sum = sum - currStrip[0];
            cout << "current Cell of size " << currCell.size() << " " << currCell << endl;
            Point mean = SKeys::calculateAvgPointByCA(sum, amount);
            cout << "mean: " << mean << endl;
            stripMeans.push_back(mean);
        }
        means.insert(means.begin(), stripMeans.begin(), stripMeans.end());
        cout << "stripMeans: " << stripMeans << endl;
        tag();

    }
    cout << "\nmeans list of size " << means.size() << " " << means << endl << endl;
//    cout << "\nchosen list of size " << chosen.size() << " " << chosen << endl << endl;
//    cout << "\nleftover list of size " << leftover.size() << " " << leftover << endl << endl;
    return means;
}

vector<vector<Point> > getLeftoverPoints(const vector<Point> &points, const vector<Point> &means) {
    vector<DecryptedPoint> decMeans = SKeys::decryptPointsByCA(means); //todo ideally this should be done by the point "itself" for every point (every "point" gets number of means and pickes the closest one)
    vector<Binary> distances = getDistFromClosestMeanByClient(decMeans, points);
    Binary threshold = SKeys::calculateThresholdByCA(distances, distances.size());
    vector<Point> chosen, leftover;
    for (unsigned long j = 0; j < points.size(); ++j) { //
        if (threshold >= distances[j]) {
            chosen.push_back(points[j]);
        } else {
            leftover.push_back(points[j]);
        }
    }
    vector<vector<Point> > retVec;
    retVec.push_back(chosen);
    retVec.push_back(leftover);
    cout << "distances of size: " << distances.size() << " " << distances << endl;
    cout << "chosen of size: " << chosen.size() << " " << chosen << endl;
    cout << "leftover of size: " << leftover.size() << " " << leftover << endl;
    tag();
//    return leftover;
    return retVec;
}

