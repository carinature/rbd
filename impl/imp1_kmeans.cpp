#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>       /* pow */

//helib
//#include <helib/FHE.h>
//#include <helib/EncryptedArray.h>
//#include <helib/intraSlot.h>
//#include <helib/EncNumberArith.h>
//#include <helib/EncNumberCompare.h>
//#include <helib/ArgMap.h>
//#include <NTL/BasicThreadPool.h>
//#include <numeric>
//#include <unistd.h>

//NTL_CLIENT
//static std::vector<zzX> unpackSlotEncoding; // a global variable

#include "imp1_kmeans.h"
//#include "aux.h"

Point calculateAvgPointByCA(Point point, Bit ctxt);

using namespace std;

/*

int main(int argc, char * argv[]) {
    
    KeysServer * keysServer = new KeysServer();
    vector<Point> encPoints = getEncryptedPointsFromFile(*keysServer);
    cout << "encPoints : " << encPoints << " END encPoints" << endl;
    vector<Point> encMeans = getEncryptedKMeans(encPoints, *keysServer);
//    vector<vector<Point> > leftoverPoints = getLeftoverPoints(encPoints, encMeans, *keysServer); //TODO SEGFAULT
//    writeToFile(encMeans, "means", *keysServer);
//    writeToFile(leftoverPoints[0], "chosen", *keysServer);
//    writeToFile(leftoverPoints[1], "leftover", *keysServer);
}
*/

vector<Point> getEncryptedKMeans(vector<Point> points, KeysServer & keysServer) {
    vector<Point> means, leftover, chosen;
    int numOfStrips = (int) (1 / EPSILON);
/*    int numOfCells = (int) (1 / pow(EPSILON, 2));
    cout << "\nEPSILON: " << EPSILON << endl;
    cout << "numOfStrips: " << numOfStrips << endl;
    cout << "numOfCells: " << numOfCells << endl;*/
    //for every strip do
    for(int i = 0; i < numOfStrips; ++i) {
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
        while(k--) {
            auto r = begin;
            advance(r, random() %
                       stripSize); //TODO this line crushes the program with small number of points from "points"
//            swap(*begin, *r);
            swap(begin, r);
            ++begin;
            --stripSize;
        }
        vector<Point> random(copy.begin(), copy.begin() + numOfStrips);
//        cout << "random points: " << random << endl << endl;
        map<Point, map<Point, vector<Bit>, cmpPoints>, cmpPoints> cmp = createCmpDict(random);
        
        ////    CUT INTO CELLS
        vector<Point> stripMeans;
        //todo create cmp-dict
        //for each rand point
        int rp = 0;
        for(const Point & R : random) {
            ////  find the points for each cell
            vector<Point> currCell;
            Point sum = currStrip[0];
            EncNumber size; size.append(keysServer.randomBit()); //init size
//            Bit size = keysServer.randomBit(); //= 0;  // fixme either a security issue or precision
            //  for each point in the strip
            int j = 0;
            for(const Point & p : currStrip) {
                //        is above other stripMeans
                vector<Bit> cmpIsAbove;
                cout << "strip:" << i << "    rp: " << rp << "    p: " << j++ << endl;
                for(const Point & r2 : random) {
                    Bit r2isSmaller = cmp[p][r2][0];
                    r2isSmaller.multiplyBy(cmp[R][r2][0]);  // r2isSmaller = (p > r2) * (R > r2)
                    
                    Bit r2isBigger = cmp[r2][R][0];  // r2isBigger = (r2 > R)
//                    Bit r2isBigger = cmp(r2, p) * cmp[r2][R]; //this worked but was redundant
//
//                  Bit isAboveR2 = r2isSmaller + r2isBigger - r2isSmaller * r2isBigger
                    Bit isAboveR2 = r2isSmaller;
                    isAboveR2.multiplyBy(r2isBigger);
                    isAboveR2.negate();
                    isAboveR2 += r2isSmaller;
                    isAboveR2 += r2isBigger;
//                    cmpIsAbove.push_back(isAboveR2);  // currently the vector is for DBG purposes
                    cmpIsAbove.push_back(isAboveR2);  // currently the vector is for DBG purposes
                    //todo instead of vector and then "accumulate" just accumulate in cmpIsAbove*=tempcmp
                    //               cmpIsAbove *= isAboveR2;
                }
                cout << "lalallaalalalal" <<endl;
    
                //                Bit isAboveOtherReps = accumulate(cmpIsAbove.begin(), cmpIsAbove.end(), 1, multiplies<Bit>());
                Bit isAboveOtherReps = cmpIsAbove[0];
                cout << "2222" <<endl;
                for(const Ctxt & b : cmpIsAbove) isAboveOtherReps *= b;  //accumulate prod of cmpIsAbove
                cout << "333" <<endl;
                Bit isInCell = cmp[R][p][0];
                EncNumber temp; temp.append(isInCell);
                cout << keysServer.decrypt(temp) << endl;
                cout << "44" <<endl;
//                isInCell.multiplyBy(isAboveOtherReps);   //  is in  = is under random * is over other randoms
                isInCell*=(isAboveOtherReps);   //  is in  = is under random * is over other randoms
//                temp.append(isInCell);
//                cout << keysServer.decrypt(temp) << endl;
                cout << "5" <<endl;
                Point isPoint = p * isInCell;
                cout << "6" <<endl;
                currCell.push_back(isPoint);
                cout << "The problem is here" <<endl;
                cout << sum.decrypt(keysServer) << endl;
                cout << isPoint.decrypt(keysServer) << endl;
                
                sum = sum + isPoint; // <--the problem is here
                cout << "7" <<endl;
//        int nBits = (outSize>0 && outSize<2*BIT_SIZE)? outSize : (2*BIT_SIZE);
                //fixme - high potensial for BUG, sum.bitsize can be bigger than BIT_SIZE
                EncNumber isInCellNum; isInCellNum.append(isInCell);
                cout << "8" <<endl;
                CtPtrs_VecCt eep(size);
                cout << "9" <<endl;
                addTwoNumbers(eep, CtPtrs_VecCt(size), CtPtrs_VecCt(isInCellNum), BIT_SIZE, &unpackSlotEncoding);
                cout << "2222" <<endl;
            }
            
            if(dbg) {
//                sum = sum - currStrip[0]; //todo consider removing line for efficiency OR fixme
//            cout << "current Cell of size " << currCell.size() << " " << currCell << endl;
//                Point mean = *keysServer.calculateAvgPointByCA(sum, size);
            
            }
            
            Point mean = keysServer.calculateAvgPoint(sum, size);
//            cout << "mean: " << mean << endl;
//            stripMeans.push_back(mean);
            stripMeans.push_back(mean);
            ++rp;
        }
        means.insert(means.begin(), stripMeans.begin(), stripMeans.end());

//        cout << "stripMeans: " << stripMeans << endl;
    
    }
//    cout << "\n means list of size " << means.size() << " " << means << endl << endl;
//    cout << "\n chosen list of size " << chosen.size() << " " << chosen << endl << endl;
//    cout << "\n leftover list of size " << leftover.size() << " " << leftover << endl << endl;
    return means;
}


/*vector<vector<Point> > getLeftoverPoints(const vector<Point> & points, const vector<Point> & means, KeysServer & keysServer) {
    vector<DecryptedPoint> decMeans = *keysServer.decryptPointsByCA(
            means); //todo ideally this should be done by the point "itself" for every point (every "point" gets number of means and picks the closest one)
    vector<EncNumber> distances = getDistFromClosestMeanByClient(decMeans, points, *keysServer);
    EncNumber threshold = calculateThreshold(distances, distances.size());
    vector<Point> chosen, leftover;
    for(unsigned long j = 0; j < points.size(); ++j) { //TODO this should not be an IF !!!
        if(threshold >= distances[j]) {
            chosen.push_back(points[j]);
        } else {
            leftover.push_back(points[j]);
        }
    }
    vector<vector<Point> > retVec;
    retVec.push_back(chosen);
    retVec.push_back(leftover);
*//*    cout << "distances of size: " << distances.size() << " " << distances << endl;
    cout << "chosen of size: " << chosen.size() << " " << chosen << endl;
    cout << "leftover of size: " << leftover.size() << " " << leftover << endl;*//*
//    return leftover;
    return retVec;
}

EncNumber calculateThreshold(vector<EncNumber> distances, int amount) {
    try {
        EncNumber distSum = distances[0];
        for(EncNumber d : distances) {
            distSum += d;
        }
        distSum -= distances[0];
        return distSum / EncNumber(amount); //todo check if division by a decrypted number is allowed in helib
    }
    catch(...) {
        return EncNumber(0); ///TODO
    }
}*/
