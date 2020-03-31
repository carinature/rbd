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
//    decAndWriteToFile(leftoverPoints[1], "leftover", *keysServer);
}
*/


vector<tuple<vector<Point>, EncNumber> > getCells(vector<Point> points, KeysServer & keysServer) {
    cout << "getCells" << endl;
    vector<Point> randPoints, leftover, chosen;
    vector<DecryptedPoint> means;
    vector<tuple<vector<Point>, EncNumber> > cellTuples;
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
//            Point sum = R;
            Vec<EncNumber> sizeVec;  // vector<Bit> sizeVec;
            //  for each point in the strip
            int j = 0;  //DBG
            
            for(const Point & p : currStrip) {

                //        is above other stripMeans
                vector<Bit> cmpIsAbove;
                cout << "strip:" << i << ", rp: " << rp << ", p: " << j++ << " ----- "; // << endl;   //DBG
                for(const Point & r2 : random) {
//                    cout << "here's the problem\n";
    
                    Bit r2isSmaller = cmp[p][r2][0];
//                    cout << "t'was the problem\n";
                    r2isSmaller.multiplyBy(cmp[R][r2][0]);  // r2isSmaller = (p > r2) * (R > r2)
                    Bit r2isBigger = cmp[r2][R][0];  // r2isBigger = (r2 > R)
//                    Bit r2isBigger = cmp(r2, p) * cmp[r2][R]; //this worked but was redundant

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
                currCell.push_back(isPoint); // todo consider  might be more efficient to summ all the points together

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
                sizeVec.append(isInCellNum);  // sizeDBG.append(isInCell);
//                addTwoNumbers(eep, CtPtrs_VecCt(size), CtPtrs_VecCt(isInCellNum));//, BIT_SIZE, &unpackSlotEncoding);
                if(dbg) {
//                    cout << "   size after addNums: " << keysServer.decrypt(size) << " ";
//                    cout << "   sizeDBG after addNums: " << keysServer.decrypt(sizeDBG) << " ";
                    cout << "   isPoint: " << isPoint.decrypt(keysServer) << " \n";
//                    cout << "   sum: " << sum.decrypt(keysServer) << " \n";
                }
            }
            // sum counter vector (#points_inCell)
            EncNumber size; //, size, sizeDBG;
            const CtPtrs & product = CtPtrs_VecCt(size);
            CtPtrMat_VecCt nums(sizeVec); // Wrapper around numbers
            addManyNumbers((CtPtrs &) product, nums, BIT_SIZE, &unpackSlotEncoding); // <--- "findQ(5,3) not found" is printed here
            cellTuples.emplace_back(currCell, size);
//            cout << "   size: " << keysServer.decrypt(size) << " ";
 
            ++rp;
        }
    }
    if(dbg) decAndWriteToFile(randPoints, "io/random_means", keysServer);
//    writeToFile(means, "io/means_test", keysServer);
    return cellTuples;
    
//
//
//                }
//            }
//        }
//    }
//    return vector<tuple<vector<Point>, EncNumber> >();
}

vector<DecryptedPoint>
calculateMeans(const vector<tuple<vector<Point>, EncNumber> > & cellTuples, KeysServer & keysServer) {
    cout << "calculateMeans " << endl;
//    cout << cellTuples << endl;
    vector<DecryptedPoint> means;
    for(tuple<vector<Point>, EncNumber> cell : cellTuples) {
        means.push_back(calculateCellMean(cell, keysServer));
    }
    return means;
}

DecryptedPoint calculateCellMean(tuple<vector<Point>, EncNumber> cellTuple, KeysServer & keysServer) {
    cout << "calculateCellMean: " << endl;
    vector<Point> points = get<0>(cellTuple);
    Vec<Ctxt> size = get<1>(cellTuple);
    
    // create 2 vectors - one for each coordinate - vector of encrypted numbers
    Vec<EncNumber > vecX, vecY, vec[DIM];
    // for every point p
    for (Point p : points){
//        //  put p.x into vecX
//        vecX.append(p[0]);
//        //  put p.y into vecY
//        vecY.append(p[1]);
        for(int i = 0; i < DIM; ++i) {
            vec[i].append(p[i]);
        }
    }
    // sum each of the vectors
    EncNumber sumX, sumY, sum[DIM];
//    const CtPtrs & productX = CtPtrs_VecCt(sumX), & productY = CtPtrs_VecCt(sumY);
//    CtPtrMat_VecCt numsX(vecX); // Wrapper around numbers
//    CtPtrMat_VecCt numsY(vecY); // Wrapper around numbers
//    addManyNumbers((CtPtrs &) productX, numsX, BIT_SIZE*NUM_POINTS/EPSILON, &unpackSlotEncoding); // <--- "findQ(5,3) not found" is printed here
//    addManyNumbers((CtPtrs &) productY, numsY, BIT_SIZE*NUM_POINTS/EPSILON, &unpackSlotEncoding); // <--- "findQ(5,3) not found" is printed here
    for(int j = 0; j < DIM; ++j) {
        const CtPtrs & product = CtPtrs_VecCt(sum[j]); //, & productY = CtPtrs_VecCt(sumY);
        CtPtrMat_VecCt nums(vec[j]); // Wrapper around numbers
//        addManyNumbers((CtPtrs &) product, nums, BIT_SIZE*NUM_POINTS/EPSILON, &unpackSlotEncoding); // <--- "findQ(5,3) not found" is printed here
        addManyNumbers((CtPtrs &) product, nums, BIT_SIZE/pow(EPSILON,2), &unpackSlotEncoding); // <--- "findQ(5,3) not found" is printed here
    
    }
//    Point sumPoint = Point({sumX, sumY}, &keysServer);
//    vector<EncNumber> sumVec(sum, sum+DIM);
    Point sumPoint2 = Point(vector<EncNumber>(sum, sum+DIM), &keysServer);
//    cout << "------- sumPoint" << sumPoint.decrypt(keysServer) << endl;
//    DecryptedPoint avgPoint = keysServer.calculateAvgPoint(sumPoint, size);
    DecryptedPoint avgPoint2 = keysServer.calculateAvgPoint(sumPoint2, size);
   return avgPoint2;
    
    
    
    
    
    
    
    
    
    
    
//    DecryptedPoint mean;
//    vector<Vec<EncNumber> > vectorOfCoorVecs; //(DIM); // a vector for each coordinate
//    vector<EncNumber> sums(DIM);
////    cout << "vectorOfCoorVecs: " << vectorOfCoorVecs << endl;
//    vector<Point> & cellPoints = get<0>(cellTuple);
//    cout << "cellPoints: " << cellPoints << endl;
//    for(Point point: cellPoints) {
//        // devide each point's coors into the appropriate vector of coors (x into 1st vec, y into 2dn etc.)
//        for(int i = 0; i < DIM; ++i) { //accumulate the coors of each DIM
//            const EncNumber & currCoor = point[i];
//            vectorOfCoorVecs[i].append(currCoor); //todo check with debug
//        }
//    }
//    for(int j = 0; j < DIM; ++j) { //sum the coors of each DIM
//        vector<EncNumber> currCoorVec;//        EncNumber sum;
//        EncNumber & vec = sums[j];
//        const CtPtrs & sum = CtPtrs_VecCt(
//                vec);//        Vec<EncNumber> distanceVec; for (const EncNumber & d : currCoorVec) distanceVec.append(d);
//        CtPtrMat_VecCt nums(vectorOfCoorVecs[j]); // Wrapper around numbers
//        addManyNumbers((CtPtrs &) sum, nums, BIT_SIZE,
//                       &unpackSlotEncoding); // <--- "findQ(5,3) not found" is printed here
//        Vec<Ctxt> & size = get<1>(cellTuple);
//        mean.push_back(keysServer.calculateAvg(vec, size));
////        mean.push_back(keysServer.calculateAvg(sum, size));
//    }
//    return mean;
}


EncNumber calculateThreshold(vector<EncNumber> distances, KeysServer * keysServer) {//}, int amount) {
    try {
        EncNumber distSum;
        const CtPtrs & product = CtPtrs_VecCt(distSum);
        Vec<EncNumber> distanceVec;
        for(const EncNumber & d : distances) distanceVec.append(d);
        CtPtrMat_VecCt nums(distanceVec); // Wrapper around numbers
        addManyNumbers((CtPtrs &) product, nums, BIT_SIZE, &unpackSlotEncoding); // <--- "findQ(5,3) not found" is printed here
        return distances[0]; //fixme should return avg dist
//        vector<EncNumber> vec(DIM, distSum);
//        Point distPoint(keysServer, vec);
//        return  keysServer->calculateAvgPoint(distPoint, distances.size()); //todo check if division by a decrypted number is allowed in helib
    }
    catch(...) {
        return distances[0]; ///TODO
    }
}

vector<EncNumber> getDistances(const vector<PointExtended> & clients, const vector<DecryptedPoint> & means) {
//    cout << "getDistances means: " << means << endl;
    vector<EncNumber> distances;
    for(PointExtended client : clients) {
        const EncNumber & dist = client.getDistanceFromClosestPoint(means);
        distances.push_back(dist);
    }
    return distances;
}

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
            Vec<EncNumber> sizeVec;  // vector<Bit> sizeVec;
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
                currCell.push_back(
                        isPoint); // todo consider  might be more efficient to summ all the points together
                sum = sum + isPoint;  // todo convert and use addManyNumbres instead
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
                sizeVec.append(isInCellNum);  // sizeDBG.append(isInCell);
//                addTwoNumbers(eep, CtPtrs_VecCt(size), CtPtrs_VecCt(isInCellNum));//, BIT_SIZE, &unpackSlotEncoding);
                if(dbg) {
//                    cout << "   size after addNums: " << keysServer.decrypt(size) << " ";
//                    cout << "   sizeDBG after addNums: " << keysServer.decrypt(sizeDBG) << " ";
                    cout << "   isPoint: " << isPoint.decrypt(keysServer) << " ";
                    cout << "   sum: " << sum.decrypt(keysServer) << " \n";
                }
            }
            // sum counter vector (#points_inCell)
            EncNumber size; //, size, sizeDBG;
            const CtPtrs & product = CtPtrs_VecCt(size);
            CtPtrMat_VecCt nums(sizeVec); // Wrapper around numbers
            addManyNumbers((CtPtrs &) product, nums, BIT_SIZE,
                           &unpackSlotEncoding); // <--- "findQ(5,3) not found" is printed here
//            cout << "   size: " << keysServer.decrypt(size) << " ";

//            sum = sum - R; //todo consider removing line for efficiency OR fixme
            DecryptedPoint mean = keysServer.calculateAvgPoint(sum, size);
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
    if(dbg) decAndWriteToFile(randPoints, "io/random_means", keysServer);
    writeToFile(means, "io/means_test");
    return means;
}
/*vector<vector<Point> > getLeftoverPoints(const vector<Point> & points, const vector<Point> & means, KeysServer & keysServer) {
    vector<DecryptedPoint> decMeans = *keysServer.decryptPointsByCA(
            means); //todo ideally this should be done by the point "itself" for every point (every "point" gets number of means and picks the closest one)
    vector<EncNumber> distances = getDistFromClosestMeanByClient(decMeans, points, *keysServer);
    EncNumber threshold = calculateThresholdTest(distances, distances.size());
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

*/
