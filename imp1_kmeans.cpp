//
// Created by rbd on 24.12.2019.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <math.h>       /* pow */


//helib
#include <cassert>
#include <cmath>

#include <helib/FHE.h>
#include <helib/EncryptedArray.h>

#include <helib/intraSlot.h>
//#include <helib/binaryArith.h>
#include <helib/binaryCompare.h>
#include <helib/ArgMap.h>

#include <NTL/BasicThreadPool.h>
#include <numeric>

NTL_CLIENT


//#include "HEcmp.h"
//#include "Contextor.cpp"
#include "imp1_kmeans.h"
//#include "Bit.h"
using Bit = bool;//todo remove
//#include "Binary.h"
//#include "Point.h"
#include "aux.h"

using namespace std;
//using String = std::string;

//  this has been moved to
//#define DIM 1
//#define EPSILON 0.25

static std::vector<zzX> unpackSlotEncoding; // a global variable

Point vecToPoint(vector<double> &coorVec);

vector<Point> getEncReps(vector<Point> vector);

Point getAvgPointFromClient(Point point, int amount);

int main(int argc, char *argv[]){
    cout << "CommonCommonCommonCommon!" << endl;
//    vector<vector<double> > points, representatives;
    std::vector<double> xVec, yVec;
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
    */
    vector<Point> encPoints = getEncryptedPointsFromFile();
    cout << encPoints << endl;
    vector<Point> encReps = getEncReps(encPoints);
    cout << encReps << endl;

}

vector<Point> getEncReps(vector<Point> points) {
/*//    vector<Binary> bin0 = {Binary(0)};
//    vector<Binary> bin1 = {Binary(1)};
//    Point p0(bin0);
//    Point p1(bin1);
//    cout << points << endl;*/
    vector<Point> representatives;
    int numOfStrips = (int)(1 / EPSILON);
    int numOfCells = (int)(1 / pow(EPSILON,2));
    cout << EPSILON << endl;
    cout << numOfStrips << endl;
    //for every strip do
    for (int i = 0; i < numOfStrips; ++i ) {    //todo note the loss of points in tail (need to correct)
        int stripSize = (int)(points.size()*EPSILON);
        vector<Point > currStrip(points.begin()+i * stripSize, points.begin()+((i + 1) * stripSize));
        cout << "currStrip of size " << currStrip.size() << currStrip << endl;
        cout << "stripSize " << stripSize << currStrip << endl;

        vector<Point> copy(currStrip);
        int k=numOfStrips; //, left = stripSize;
////    Fisher–Yates shuffle for choosing k random points
        auto begin = copy.begin();
        while(k--) {
            auto r = begin;
            advance(r, rand()%stripSize);
            swap(*begin, *r);
            ++begin;
            --stripSize;
        }
        vector<Point> random(copy.begin(), copy.begin()+numOfStrips);
        cout << "THE RANDOM : " << random << endl;
        vector<Point> reps;
        ////    CUT INTO CELLS
        //todo create cmp-dict
        //for each rand point
        for(const Point& R : random) {
            tag("Random");
            tag(R);
        ////  find the points for each cell
            vector<Point> currCell;
            Point sum = Point::zeroPoint();
            int amount = 0;
            //  for each point in the strip
            for (const Point& p : currStrip) {
                //        is above other reps
                //todo converto to cmp resault from bool to bit
                vector<bool> cmpIsAbove;
                for (Point r : random){
                    Bit r2smaller = cmp(p, r) * cmp(R, r);
                    int r2bigger = cmp(r, p) * cmp(r, R);
                    Bit tempcmp =  r2smaller + r2bigger - r2smaller*r2bigger;
                    cmpIsAbove.push_back(tempcmp);
                    //todo instead of vector and then "accumulate" just accumulate in cmpIsAbove*=tecmpcmp
                    //               cmpIsAbove = cmpIsAbove * tempcmp;
                }
                Bit isAboveOtherReps = accumulate(cmpIsAbove.begin(), cmpIsAbove.end(), 1, multiplies<Bit>());
                Bit isInCell = cmp(R, p) * isAboveOtherReps;   //  is in  = is under * is over
                Point isPoint = p * isInCell;
                currCell.push_back(isPoint);
                sum = sum + isPoint;
                amount += isInCell;
            }
            cout << "currCell of size "  << currCell.size() << " "  << currCell << endl;
            //      find rep for cell
            Point avgPoint = getAvgPointFromClient(sum, amount);
            cout << "avgPoint" << avgPoint << endl;
            reps.push_back(avgPoint);
        }
        representatives.insert(representatives.begin(), reps.begin(), reps.end());
    }
    cout << "\n # representatives:  " << representatives.size() << endl;
    cout << "The representatives are:  " << representatives << endl << endl;
    return representatives;
}


vector<Point> getEncryptedPointsFromFile(){
    vector<vector<double> > points = getPointsFromFile();

    vector<Point> encPoints;
//    for (int i=0; i<points.size(); ++i){
    for (vector<double> p : points){
        vector<Binary> coorVec; // = vector<Binary>();
        for (int coor=0; coor<DIM; ++coor) {
            coorVec.emplace_back(p[coor]);
        }
        encPoints.emplace_back(coorVec);
    }
//    cout << "===encPoints===" << endl;
//    cout << encPoints << endl;
    return encPoints;
}

vector<vector<double> > getPointsFromFile(){
    ifstream inputFileStream("points");
    string line, pa0, pa1;
    int count;  //  #points
    inputFileStream>>count;
    inputFileStream.ignore(1, '\n');

    vector<vector<double> > points;
    for(int i = 0; i < count; ++i) {
        getline(inputFileStream, line);
        istringstream lineStream(line);

        getline(lineStream, pa0, ',');
        getline(lineStream, pa1, ',');

        points.push_back({stod(pa0), stod(pa1)});
    }
    return points;
}

vector<vector<double> > getReps(vector<vector<double> > points) {
    vector<vector<double> > representatives;
    int numOfStrips = 1 / EPSILON;
//    int numOfCells = 1 / pow(EPSILON,2);
    cout << EPSILON << endl;
    cout << numOfStrips << endl;
    for (int i = 0; i < numOfStrips; ++i ) {  //todo note the loss of points in tail (need to correct)
        int stripSize = points.size()*EPSILON;
        vector<vector<double> > currStrip(points.begin()+i * stripSize, points.begin()+((i + 1) * stripSize));
        cout << "currStrip - " << currStrip << endl;
        for (int j = 0; j < numOfStrips; ++j) {  //todo note the loss of points in tail (need to correct)
            int cellSize = currStrip.size() * EPSILON;
            vector<vector<double> > currCell(currStrip.begin() + j * cellSize, currStrip.begin()+((j + 1) * cellSize));
            //  #TODO - replace this part with your alg
//            nth_element(currCell.begin(), currCell.begin() + currCell.size()/ 2, currCell.end(), cmp);
            representatives.push_back(currCell[currCell.size() / 2]);
            cout << ".  The median is " << currCell[currCell.size()/ 2] << '\n';
        }
    }
    cout << "\n # representatives:  " << representatives.size() << endl;
    cout << "The representatives are:  " << representatives << endl << endl;
    return representatives;
}

//bool cmp(vector<double> a, vector<double> b){
//    return a[DIM-1]>b[DIM-1];
//}
