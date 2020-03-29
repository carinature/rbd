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

vector<DecryptedPoint> getEncryptedKMeans(vector<Point> points, KeysServer & keysServer) {
//vector<Point> getEncryptedKMeans(vector<Point> points, KeysServer & keysServer) {
    cout << "getEncryptedKMeans" << endl;
    vector<Point> randPoints, leftover, chosen;
    vector<DecryptedPoint> means;
    int numOfStrips = (int) (1 / EPSILON);/*    int numOfCells = (int) (1 / pow(EPSILON, 2));
    cout << "\nEPSILON: " << EPSILON << endl;
    cout << "numOfStrips: " << numOfStrips << endl;
    cout << "numOfCells: " << numOfCells << endl;*/
    //for every strip do
    for(int i = 0; i < numOfStrips; ++i) {
        ////    Fisher–Yates shuffle for choosing k random points
        int stripSize = (int) (points.size() * EPSILON), k = numOfStrips; //, left = stripSize;
        vector<Point> currStrip(points.begin() + i * stripSize, points.begin() + ((i + 1) * stripSize));
        vector<Point> copy(currStrip);
        auto begin = copy.begin(); //todo note the loss of points in tail (need to correct)
        while(k--) {
            auto r = begin;
            advance(r, random() % stripSize); //TODO this line crushes the program with small(<=>20) #points in file
//            swap(*begin, *r);
            swap(begin, r);
            ++begin;
            --stripSize;
        }
        vector<Point> random(copy.begin(), copy.begin() + numOfStrips);
        randPoints.insert(randPoints.end(), random.begin(), random.end());
        auto t1 = std::chrono::high_resolution_clock::now();
        map<Point, map<Point, vector<Bit>, cmpPoints>, cmpPoints> cmp = createCmpDict(random, currStrip);
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
        cout << "\ncreating the dict took: " << duration << " seconds.\n" << endl;
        vector<DecryptedPoint> stripMeans; //        vector<Point> stripMeans;
        ////    CUT INTO CELLS
        //for each rand point (cell)
        int rp = 0;  //DBG
        for(const Point & R : random) {
            cout << "The random point is : " << R.decrypt(keysServer) << endl;
            ////  find the points for each cell
            vector<Point> currCell;
            Point sum = R;
            EncNumber size, size2, sizeDBG;
            Vec<EncNumber> sizeDBGvec;
//            vector<Bit> sizeDBGvec;
            CtPtrs_VecCt eep(size);  //size.append(keysServer.randomBit()); //init size
            //  for each point in the strip
            int j = 0;  //DBG
            for(const Point & p : currStrip) {
                //        is above other stripMeans
                vector<Bit> cmpIsAbove;
                cout << "strip:" << i << ", rp: " << rp << ", p: " << j++ << " ----- "; // << endl;   //DBG
                for(const Point & r2 : random) {
//                    Bit r2isSmaller =       cmp(p,r2);
//                    r2isSmaller.multiplyBy( cmp(R,r2));  // r2isSmaller = (p > r2) * (R > r2)
//                    Bit r2isBigger =        cmp(r2,R);  // r2isBigger = (r2 > R)

//                    cout << "00-0-0" <<endl;
//                    auto r2isSmaller1 = cmp;
//                    auto r2isSmaller2 = cmp[p];
//                    auto r2isSmaller3 = cmp[p][r2];
//                    cout << "3-3-3" <<endl;
//                    cout << r2isSmaller3 << endl;
//                    cout << "4$$4" <<endl;
                    
                    Bit r2isSmaller = cmp[p][r2][0];
                    r2isSmaller.multiplyBy(cmp[R][r2][0]);  // r2isSmaller = (p > r2) * (R > r2)
                    Bit r2isBigger = cmp[r2][R][0];  // r2isBigger = (r2 > R)
//                    Bit r2isBigger = cmp(r2, p) * cmp[r2][R]; //this worked but was redundant

//                    cout << "******" <<endl;
//                  Bit isAboveR2 = r2isSmaller + r2isBigger - r2isSmaller * r2isBigger
                    Bit isAboveR2 = r2isSmaller;
                    isAboveR2.multiplyBy(r2isBigger);
                    isAboveR2.negate();
                    isAboveR2 += r2isSmaller;
                    isAboveR2 += r2isBigger;
                    cmpIsAbove.push_back(
                            isAboveR2);  // currently the vector is for DBG purposes - cmpIsAbove *= isAboveR2;
                }
                Bit isAboveOtherReps = cmpIsAbove[0];
                for(const Ctxt & b : cmpIsAbove) isAboveOtherReps.multiplyBy(b);  //accumulate prod of cmpIsAbove
                Bit isInCell = cmp[R][p][0];
                isInCell.multiplyBy(isAboveOtherReps);   //  is in  = is under random * is over other randoms
                Point isPoint = p * isInCell;
                currCell.push_back(isPoint); //todo do i need this for something? remove
                sum = sum + isPoint;
//        int nBits = (outSize>0 && outSize<2*BIT_SIZE)? outSize : (2*BIT_SIZE);
                //fixme - potential for BUG, sum.bitsize can be bigger than BIT_SIZE
                EncNumber isInCellNum;
                isInCellNum.append(isInCell);
                if(dbg) {
                    cout << "   cmpIsAbove: [ ";
                    for(const Ctxt & b : cmpIsAbove) {
                        EncNumber bb;
                        bb.append(b);
                        cout << keysServer.decrypt(bb) << " ";
                    }
                    cout << "] ";
                    EncNumber bb;
                    bb.append(cmp[R][p][0]);
                    cout << "   R > p: [" << keysServer.decrypt(bb) << "] ";
                    cout << "   isInCellNums: " << keysServer.decrypt(isInCellNum) << " ";
//                    cout << "   size before addNums: " << keysServer.decrypt(size) << " ";
                }
                sizeDBGvec.append(isInCellNum);
//                sizeDBG.append(isInCell);
//                addTwoNumbers(eep, CtPtrs_VecCt(size), CtPtrs_VecCt(isInCellNum));//, BIT_SIZE, &unpackSlotEncoding);
                if(dbg) {
//                    cout << "   size after addNums: " << keysServer.decrypt(size) << " ";
//                    cout << "   sizeDBG after addNums: " << keysServer.decrypt(sizeDBG) << " ";
                    cout << "   isPoint: " << isPoint.decrypt(keysServer) << " ";
                    cout << "   sum: " << sum.decrypt(keysServer) << " \n";
                }
            }
            // sum counter vector (#points_inCell)
            CtPtrMat_VecCt nums(sizeDBGvec); // Wrapper around numbers
            const CtPtrs & product = CtPtrs_VecCt(size2);
            addManyNumbers((CtPtrs &) product, nums, BIT_SIZE, &unpackSlotEncoding); // <--- "findQ(5,3) not found" is printed here
//            cout << "   size: " << keysServer.decrypt(size2) << " ";

//            sum = sum - R; //todo consider removing line for efficiency OR fixme
            DecryptedPoint mean = keysServer.calculateAvgPoint(sum, size2);
//            Point mean = keysServer.calculateAvgPoint(sum, size);
            cout << "mean: " << mean << endl;
            stripMeans.push_back(mean);
            ++rp;
        }
        means.insert(means.begin(), stripMeans.begin(), stripMeans.end());
        cout << "stripMeans: " << stripMeans << endl;
//        cout << "means: " << means << endl;
    }
//    cout << "\n means list of size " << means.size() << " " << means << endl << endl;
//    cout << "\n chosen list of size " << chosen.size() << " " << chosen << endl << endl;
//    cout << "\n leftover list of size " << leftover.size() << " " << leftover << endl << endl;
    if(dbg) writeToFile(randPoints, "io/random_means", keysServer);
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
