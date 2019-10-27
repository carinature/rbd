//
// Created by rbd on 15.10.2019.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "HEcmp.h"
#include "Contextor.cpp"

using namespace std;
//using String = std::string;

#define DIM 2


bool cmp(vector<double> a, vector<double> b){
    return a[DIM-1]>b[DIM-1];
}


int main(){
    cout << "CommonCommon!" << endl;
//    HEcmp h =  HEcmp();

    ifstream inputFileStream("points");
    int count;  //  #points
    inputFileStream>>count;
    cout << "count: " << count << endl;
    inputFileStream.ignore(1, '\n');

    string line1, line2, pa0, pa1;
//    double points[count][DIM];

    vector<vector<double> > points, points1;
    std::vector<double> xVec, yVec; //{5, 6, 4, 3, 2, 6, 7, 9, 3};;

    for(int i = 0; i < count; ++i) {
        getline(inputFileStream, line1);
        istringstream lineStream(line1);

        getline(lineStream, pa0, ',');
        getline(lineStream, pa1, ',');

        points.push_back({stod(pa0), stod(pa1)});
//        xVec.push_back(stod(pa0));
//        yVec.push_back(stod(pa1));
//        points[i][0] = stod(pa0);
//        points[i][1] = stod(pa1);
//        points1<double> temp{stod(pa0), stod(pa1)};
//        cout << (h.cmp(a1,b1) ? "true" : "false") << endl;
    }
//    points.push_back(xVec);
//    points.push_back(yVec);

    for (int i=0; i<count; ++i) {
        for (int j = 0; j < DIM; ++j) {
            cout << "points[" << i << "][0]: " << points[i][j] << "\t\t"; // << endl;
        }
        cout << endl;
    }

    nth_element(points.begin(), points.begin() + points.size()/ 2, points.end(), cmp);
    cout << "The median is " << points[points.size()/ 2] << '\n';

    for (int i=0; i<count; ++i) {
        for (int j = 0; j < DIM; ++j) {
            cout << "points[" << i << "][0]: " << points[i][j] << "\t\t"; // << endl;
        }
        cout << endl;
    }


//    *************************************************************************

//    FHEcontext context = ;

//    HEcmp h = HEcmp(Contextor::getContextAndKey());
//    cout << "lalalallalalalala" << endl;
//    h.encryptNumber(5, 7);





}














/*
 * insert all the points into array
 * cut the array into equal pieces
 * find median in each piece (which meansfinding the median of each piece)
 *      make an algorithm for randomly choosing the "median"
 *      make an algorithm for finding the median
 * ---->this is the way to choose the repesentatives
 *
 * note the points should be encrypted
 *
 */            ;