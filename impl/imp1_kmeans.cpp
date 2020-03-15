//
// Created by rbd on 24.12.2019.
//
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>       /* pow */

//helib
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
//static std::vector<zzX> unpackSlotEncoding; // a global variable


#include "imp1_kmeans.h"
//#include "aux.h"

//#include "Bit.h"
using Bit = bool;//todo remove
//using Binary = long;//todo remove

using namespace std;

int main(int argc, char *argv[]) {

    Skeys         sk        = Skeys();
    vector<Point> encPoints = getEncryptedPointsFromFile(sk);
    cout << "encPoints : " << encPoints << " END encPoints" << endl;
//    vector<Point> encMeans = getEncryptedKMeans(encPoints, sk);
//    vector<vector<Point> > leftoverPoints = getLeftoverPoints(encPoints, encMeans, sk); //TODO SEGFAULT
//    writeToFile(encMeans, "means", sk);
//    writeToFile(leftoverPoints[0], "chosen", sk);
//    writeToFile(leftoverPoints[1], "leftover", sk);
}

vector<Point> getEncryptedKMeans(vector<Point> points, Skeys &sk) {
    vector<Point> means, leftover, chosen;
    int numOfStrips = (int) (1 / EPSILON);
/*    int numOfCells = (int) (1 / pow(EPSILON, 2));
    cout << "\nEPSILON: " << EPSILON << endl;
    cout << "numOfStrips: " << numOfStrips << endl;
    cout << "numOfCells: " << numOfCells << endl;*/
    //for every strip do
    for (int i = 0; i < numOfStrips; ++i) {
        //todo note the loss of points in tail (need to correct)
        ////    Fisher–Yates shuffle for choosing k random points
        int stripSize = (int) (points.size() * EPSILON), k = numOfStrips; //, left = stripSize;
        vector<Point> currStrip(points.begin() + i * stripSize,
                                points.begin() + ((i + 1) * stripSize));
        vector<Point> copy(currStrip);
        
/*        cout << "currStrip" << endl;
        cout << currStrip << endl;
        cout << "\n\n current Strip of size " << currStrip.size() << currStrip << endl;*/
        auto begin = copy.begin();
        while (k--) {
            auto r = begin;
            advance(r, random() % stripSize); //TODO this line crushes the program with small number of points from "points"
//            swap(*begin, *r);
            swap(begin, r);
            ++begin;
            --stripSize;
        }
        vector<Point> random(copy.begin(), copy.begin() + numOfStrips);
//        cout << "random points: " << random << endl << endl;

        ////    CUT INTO CELLS
        vector<Point> stripMeans;
        //todo create cmp-dict
        //for each rand point
        for (const Point &R : random) {
            ////  find the points for each cell
            vector<Point> currCell;
            Point sum = currStrip[0];
//            Binary amount = Binary(0);
            int amount = 0;
            //  for each point in the strip
            for (const Point &p : currStrip) {
                //        is above other stripMeans
                vector<Bit> cmpIsAbove;
                for (const Point &r2 : random) {
                    Bit r2smaller = cmp(p, r2) * cmp(R, r2);
                    Bit r2bigger = cmp(r2, p) * cmp(r2, R);
                    Bit isAboveR2 = r2smaller + r2bigger - r2smaller * r2bigger;
                    cmpIsAbove.push_back(isAboveR2);  // currently the vector is for DBG purposes
                    //todo instead of vector and then "accumulate" just accumulate in cmpIsAbove*=tempcmp
                    //               cmpIsAbove *= isAboveR2;  overload op *= in Bit

                }
                Bit isAboveOtherReps = accumulate(cmpIsAbove.begin(), cmpIsAbove.end(), 1, multiplies<Bit>());
                Bit isInCell = cmp(R, p) * isAboveOtherReps;   //  is in  = is under random * is over other randoms
                Point isPoint = p * isInCell;
                currCell.push_back(isPoint);
                sum += isPoint;
                amount += isInCell;
            }
            sum -= currStrip[0]; //todo consider removing line for efficiency
//            cout << "current Cell of size " << currCell.size() << " " << currCell << endl;
            Point mean = sk.calculateAvgPointByCA(sum, amount);
//            Point mean = Skeys::calculateAvgPointByCA(sum, amount);
//            cout << "mean: " << mean << endl;
            stripMeans.push_back(mean);
        }
        means.insert(means.begin(), stripMeans.begin(), stripMeans.end());
//        cout << "stripMeans: " << stripMeans << endl;
    }
//    cout << "\n means list of size " << means.size() << " " << means << endl << endl;
//    cout << "\n chosen list of size " << chosen.size() << " " << chosen << endl << endl;
//    cout << "\n leftover list of size " << leftover.size() << " " << leftover << endl << endl;
    return means;
}

vector<vector<Point> > getLeftoverPoints(const vector<Point> &points, const vector<Point> &means, Skeys &sk) {
    vector<DecryptedPoint> decMeans = sk.decryptPointsByCA(means); //todo ideally this should be done by the point "itself" for every point (every "point" gets number of means and picks the closest one)
    vector<Binary> distances = getDistFromClosestMeanByClient(decMeans, points, sk);
    Binary threshold = calculateThreshold(distances, distances.size());
    vector<Point> chosen, leftover;
    for (unsigned long j = 0; j < points.size(); ++j) { //TODO this should not be an IF !!!
        if (threshold >= distances[j]) {
            chosen.push_back(points[j]);
        } else {
            leftover.push_back(points[j]);
        }
    }
    vector<vector<Point> > retVec;
    retVec.push_back(chosen);
    retVec.push_back(leftover);
/*    cout << "distances of size: " << distances.size() << " " << distances << endl;
    cout << "chosen of size: " << chosen.size() << " " << chosen << endl;
    cout << "leftover of size: " << leftover.size() << " " << leftover << endl;*/
//    return leftover;
    return retVec;
}

Binary calculateThreshold(vector<Binary> distances, int amount) {
    try {
        Binary distSum = distances[0];
        for (Binary d : distances) {
            distSum += d;
        }
        distSum -= distances[0];
        return distSum / Binary(amount); //todo check if division by a decrypted number is allowed in helib
    }
    catch (...) {
        return Binary(0); ///TODO
    }
}
