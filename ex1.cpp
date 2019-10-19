//
// Created by rbd on 15.10.2019.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "HEcmp.h"
#include "Contextor.cpp"

using namespace std;
//using String = std::string;

#define DIM 2

int main(){
    cout << "Common!" << endl;
//    HEcmp h =  HEcmp();

    ifstream inputFileStream("points");
    int count;  //  #points
    inputFileStream>>count;
    cout << "count: " << count << endl;
    inputFileStream.ignore(1, '\n');

    string line1, line2, pa0, pa1;
    double points[count][DIM];

    for(int i = 0; i < count; ++i) {
        getline(inputFileStream, line1);
        istringstream lineStream(line1);

        getline(lineStream, pa0, ',');
        getline(lineStream, pa1, ',');

        points[i][0] = stod(pa0);
        points[i][1] = stod(pa1);
//        cout << (h.cmp(a1,b1) ? "true" : "false") << endl;
    }
    for (int i=0; i<count; ++i){
            cout << "points[" << i << "][0]: " << points[i][0]; // << endl;
            cout << "   points[" << i << "][1]: " << points[i][1] << endl;
    }

//    FHEcontext context = ;
    HEcmp h = HEcmp(Contextor::getContextAndKey());
    h.encryptNumber(5, 7);

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