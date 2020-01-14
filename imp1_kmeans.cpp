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

NTL_CLIENT


//#include "HEcmp.h"
//#include "Contextor.cpp"
#include "imp1_kmeans.h"
#include "Bit.h"
#include "Binary.h"
#include "Point.h"
#include "aux.h"

using namespace std;
//using String = std::string;

//  this has been moved to
//#define DIM 1
//#define EPSILON 0.25

static std::vector<zzX> unpackSlotEncoding; // a global variable

Point vecToPoint(vector<double> &coorVec);

int main(int argc, char *argv[]){
    cout << "CommonCommonCommonCommon!" << endl;
    vector<vector<double> > points, representatives;
    std::vector<double> xVec, yVec;

//    Bit bit1(true);
//    Bit bit0((short)0);
    vector<Binary> bin0 = {Binary(0)};
    vector<Binary> bin1 = {Binary(1)};
    Point p0(bin0);
    Point p1(bin1);
    cout << bin0 << bin1 << p0 << p1 << endl;

    points = getPointsFromFile();
    representatives = getReps(points);

    vector<Point> encPoints, encReps;
//    for (int i=0; i<points.size(); ++i){
    for (vector<double> p : points){
        vector<Binary> temp; // = vector<Binary>();
        for (int coor=0; coor<DIM; ++coor) {
            temp.emplace_back(p[coor]);
        }
        encPoints.emplace_back(temp);
    }
    cout << encPoints << endl;
//    encReps = getEncReps(points);


//    cout << " ====== points (for loop) ====== " << endl;
//    for (vector<double> point : points) cout << point << endl;
//    cout << " ====== representatives (for loop) ====== " << endl;
//    for (vector<double> rep : representatives) cout << rep << endl;


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
//        cout << "stripSize: " << points.size() * EPSILON << endl;
//        cout << "stripSize: " << stripSize << endl;
//        int stripSize = points.size()*numOfStrips;
        vector<vector<double> > currStrip(points.begin()+i * stripSize, points.begin()+((i + 1) * stripSize));
        cout << "currStrip - " << currStrip << endl;
        for (int j = 0; j < numOfStrips; ++j) {  //todo note the loss of points in tail (need to correct)
//            int cellSize = points.size()/numOfCells;
            int cellSize = currStrip.size() * EPSILON;
//            int cellSize = points.size() * pow(EPSILON, 2);
//            cout << "cellSize; " << cellSize << endl;
//            cout << "j: " << endl;

            vector<vector<double> > currCell(currStrip.begin() + j * cellSize, currStrip.begin()+((j + 1) * cellSize));
//            cout << "currCell - " << currCell;

            //  #TODO - replace this part with your alg
            nth_element(currCell.begin(), currCell.begin() + currCell.size()/ 2, currCell.end(), cmp);


            representatives.push_back(currCell[currCell.size() / 2]);
            cout << ".  The median is " << currCell[currCell.size()/ 2] << '\n';
        }
    }
    cout << "\n # representatives:  " << representatives.size() << endl;
    cout << "The representatives are:  " << representatives << endl << endl;
    return representatives;
}

bool cmp(vector<double> a, vector<double> b){
    return a[DIM-1]>b[DIM-1];
}
