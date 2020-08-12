#include <iostream>
#include <algorithm>
#include <cmath>       /* pow */


#include "yonis/run1meancore.h"
#include "imp1_kmeans.h"
//#include "aux.h"

using namespace std;

using Cell = vector<Point>;
using Points = vector<Point>;

vector<vector<Point> >
getLeftoverPoints(const vector<PointExtended> &clients, const vector<DecryptedPoint> &means, KeysServer &keysServer);

int main(int argc, char *argv[]) {
    cout << " --------- MAIN --------- " << endl;
    auto t1 = std::chrono::high_resolution_clock::now();
    auto *ks = new KeysServer();  //   in the future should be FHEPubKey * pubKey = ks->pubKey;
    encryptedKmeans(); //recursive
    cout << "           OK" << endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
    cout << "\n--- duration of MAIN: " << duration; // << endl;
    return 0;
}

/*recursive.
 * receives a set of data points and converts into a reduced set of data points,
 * each point has a value and weight representing a number of points.
 * corset alg is used after the points are devided into small "cells" (a group of close points) */
void encryptedKmeans() {

    cout << " --------- encryptedKmeans --------- " << endl;
    auto t1 = std::chrono::high_resolution_clock::now();
    auto *ks = new KeysServer();  //   in the future should be FHEPubKey * pubKey = ks->pubKey;
    vector<PointExtended> clients = getEncryptedPointsFromFile(*ks); // retrive the points
    if (clients.empty()) return; // end of recursion   // <--------
    vector<Point> points(clients.begin(), clients.end()); // make a copy of the points
    const vector<tuple<vector<Point>, EncNumber>> &cellTuples = getCells(points, *ks);  // <--------
    if (cellTuples.empty()) return;  // <--------
    vector<DecryptedPoint> means =   // <--------
            calculateMeans(cellTuples,*ks); // convert into eps-net and return a list of the cell means  // <--------
    vector<vector<Point> > pointsSplited = getLeftoverPoints(
            clients, means, *ks); // split data into "close" and "leftovre" points
    vector<Point> chosen = pointsSplited[0]; //choose the closest points to the means for the next step of the alg
    vector<Point> &leftover = pointsSplited[1]; // remove and save the furthest points of the set for the next itereation

    // for DBG
    cout << "           OK" << endl;
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
    cout << "\n--- duration: " << duration << endl;

    /**********   integration to coreset alg    ***********/
    decAndWriteToFile(chosen, chosen_file, *ks);  // <-------------
    vector<DecryptedPoint> chosenForCorset = getPointsFromFile(chosen_file);
    if (chosenForCorset.empty())
        return;
    vector<vector<double> > dps;
    for (const DecryptedPoint &point : chosenForCorset) {
        vector<double> dpd;
        for (long coor : point) dpd.push_back(coor / CONVERSION_FACTOR);
        dps.push_back(dpd); //        if (0==dp[0] && 0==dp[1]) ++count;  // for DBG
    }
    cout << "count: " << dps.size() << endl;

    cout << "********   integration to coreset alg    **********" << endl;
    runCoreset(dps, dps.size(), DIM, EPSILON);  // <-------------
    decAndWriteToFile(leftover, points_file, *ks);  // <-------------
    encryptedKmeans();
}

/*getCells
 * receives a vector of points and breaks it into a list of cells (group of close points)
 * points - vetor of points / clients
 * keysServer - a link to the keyServer which holds the public keys. in future - publicKey.
 * returns - cellTuples - a list of tuples containg cells (groups of close points) and their corresponding size*/
vector<tuple<vector<Point>, EncNumber> > getCells(vector<Point> points, KeysServer &keysServer) {
//  if (DBG) cout << "getCells" << endl;
//  if (DBG) fcout << "getCells" << endl;
    vector<Point> randPoints, leftover, chosen;
    vector<DecryptedPoint> means, badRands;
    srand(time(NULL));  // for extra randomness
    if (DBG) cout << "TIME: " << time(NULL) << endl;

    vector<tuple<vector<Point>, EncNumber> > cellTuples;
    int numOfStrips = (int) (1 / EPSILON);
    int rp = 0, j = 0; // for dbg
    for (int i = 0; i < numOfStrips; ++i) {
        //    Fisher–Yates shuffle for choosing k random points
        int stripSize = (int) (points.size() * EPSILON), k = numOfStrips;
        if (k > stripSize) return vector<tuple<vector<Point>, EncNumber> >();  // <----------
        vector<Point> currStrip(points.begin() + i * stripSize, points.begin() + ((i + 1) * stripSize));
        vector<Point> copy(currStrip); //todo note the loss of points in tail (need to correct)
        auto begin = copy.begin();
        while (k--) {
            auto r = begin;
            advance(r, rand() % (stripSize)); //this line crushes the program with small(<20) #points in file
//      advance(r, rand() % (stripSize-1)); //experimental
            swap(begin, r);
            ++begin;
            --stripSize;
        }
        vector<Point> random(copy.begin(), copy.begin() + numOfStrips - 1);
//    vector<Point> random(copy.begin(), copy.begin() + numOfStrips - 1); // experimental
        randPoints.insert(randPoints.end(), random.begin(), random.end());
//    randPoints.insert(randPoints.end(), copy.end(), copy.end());  // <--- this is an experimental line, to cover the cases of top strip points

        auto t1 = std::chrono::high_resolution_clock::now();
        // create a dictionary for the results of the compare operation, for efficiency and consistency
        map<Point, map<Point, vector<Bit>, cmpPoints>, cmpPoints> cmp = createCmpDict(random, currStrip);
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
        if (DBG) {
            cout << "\ncreating the dict took: " << duration << " seconds.\n" << endl;
            fcout << "\ncreating the dict took: " << duration << " seconds.\n" << endl;
            rp = 0;
        }
        //    CUT INTO CELLS
        for (const Point &R : random) {        //for each randomly chosen point (meaning for each potential cell)
            //  matching each cell with it's points
            if (DBG) {
//        cout << "The random point is : " << R.decrypt(keysServer) << endl;
//        fcout << "\nThe random point is : " << R.decrypt(keysServer) << endl;
                j = 0;
            }
            Cell currCell; // current cell is empty
//            Point sum = R;
            Vec<EncNumber> sizeVec;  // vector<Bit> sizeVec; //curr sell size is zero
            //  for each point in the strip
            for (const Point &p : currStrip) { // for every point match it's corresponding
                //        is above other randomly chosen points in the strip
                vector<Bit> cmpIsAbove; // find out if the current point is above all the random points which are below the current one
                if (DBG) {
//          cout << "strip:" << i << ", rp: " << rp << ", p: " << j++ << " ----- "; // << endl;   //DBG
//          fcout << "strip:" << i << ", rp: " << rp << ", p: " << j++ << " ----- "; // << endl;   //DBG
                }
                for (const Point &r2 : random) {
                    // results in: Bit r2IsBigger = cmp(r2, p) * cmp[r2][R]; //this worked but was redundant
                    Bit r2IsSmaller = cmp[p][r2][0]; //r2IsSmaller = (p > r2)
                    r2IsSmaller.multiplyBy(cmp[R][r2][0]);  // r2IsSmaller = (p > r2) * (R > r2)
                    Bit r2IsBigger = cmp[r2][R][0];  // r2IsBigger = (r2 > R)

                    // results in: Bit isAboveR2 = r2IsSmaller + r2IsBigger - r2IsSmaller * r2IsBigger
                    Bit isAboveR2 = r2IsSmaller;
                    isAboveR2.multiplyBy(r2IsBigger);
                    isAboveR2.negate();
                    isAboveR2 += r2IsSmaller;
                    isAboveR2 += r2IsBigger;
                    cmpIsAbove.push_back(
                            isAboveR2);  // currently the vector is for DBG purposes - cmpIsAbove *= isAboveR2;
                }
                Bit isAboveOtherReps = cmpIsAbove[0];
                for (const Ctxt &b : cmpIsAbove) isAboveOtherReps.multiplyBy(b);  //accumulate prod of cmpIsAbove
                Bit isInCell = cmp[R][p][0];
                isInCell.multiplyBy(isAboveOtherReps);   //  is point in cell = is under random * is over other randoms
                Point isPoint =
                        p * isInCell;  // equals org point if point is included in the cell, and null(0) point otherwise
                currCell.push_back(isPoint); // more efficient to sum all the points together

                //count all the points in the cell
                EncNumber isInCellNum;
                isInCellNum.append(isInCell);
                if (DBG) {
//          cout << "   cmpIsAbove: [ ";
//          fcout << "   cmpIsAbove: [ ";
                    for (const Ctxt &b : cmpIsAbove) {
                        EncNumber bb;
                        bb.append(b);
//            cout << keysServer.decrypt(bb) << " ";
//            fcout << keysServer.decrypt(bb) << " ";
                    }
//          cout << "] ";
//          fcout << "] ";
                    EncNumber bb;
                    bb.append(cmp[R][p][0]);
//          cout << "   R > p: [" << keysServer.decrypt(bb) << "] ";
//          cout << "   isInCellNums: " << keysServer.decrypt(isInCellNum) << " ";
//          fcout << "   R > p: [" << keysServer.decrypt(bb) << "] ";
//          fcout << "   isInCellNums: " << keysServer.decrypt(isInCellNum) << " ";
                }
                sizeVec.append(
                        isInCellNum);  // more efficient to sum all the bits together, which we are doing in a later step
                if (DBG) {
                    cout << "   isPoint: " << isPoint.decrypt(keysServer) << " \n";
                    fcout << "   isPoint: " << isPoint.decrypt(keysServer) << " \n";
                }
            }
            EncNumber size = sumEncNumVec(sizeVec);//, 1);  // number of points in the cell
            if (0 == keysServer.decrypt(size)) {
                if (DBG) {
                    cout << "\n \"special\" random point: " << R.decrypt(keysServer) << endl;
                    fcout << "\n \"special\" random point: " << R.decrypt(keysServer) << endl;
                }
                badRands.push_back(R.decrypt(keysServer));
            }
            cellTuples.emplace_back(currCell, size);
            ++rp;
        }
    }
    if (DBG) {
        decAndWriteToFile(randPoints, rands_file, keysServer);
        writeToFile(badRands, bad_rands_file);//, keysServer);
//    cout << "getCells cell size" << cellTuples.size() << endl;
//    fcout << "getCells cell size" << cellTuples.size() << endl;
    }
    return cellTuples;
}

/*calculateMeans
 * uses calculateCellMean for a list cell and their sizes to find for each cell its corresponding mean
 * cellTuples - a vector of tuples, each tuple contains a vector of points (belonging to a specific cell) and its size.
 * keysServer - a link to the keyServer which holds the public keys. in future - publicKey.
 * returns - means - a list of cell means
 * */
vector<DecryptedPoint> calculateMeans(const vector<tuple<Cell, EncNumber> > &cellTuples, KeysServer &keysServer) {
    if (cellTuples.empty()) return vector<DecryptedPoint>();  // <-------------
    vector<DecryptedPoint> means;
//  if (DBG) cout << "calculateMeans cell size" << cellTuples.size() << endl;
//  if (DBG) fcout << "calculateMeans cell size" << cellTuples.size() << endl;
    int i = 0, j = 0; // for DBG
    for (const tuple<Cell, EncNumber> &cell : cellTuples) {
        ++j;
        try {
            ++i;
            means.push_back(calculateCellMean(cell, keysServer));
        } catch (const exception &exc) {
            cout << "exception: " << exc.what() << endl;
            cout << "SEGFAULT!!!    in cell #" << i << " or #" << j << endl;
            fcout << "exception: " << exc.what() << endl;
            fcout << "SEGFAULT!!!    in cell #" << i << " or #" << j << endl;
        }
    }
    if (DBG) writeToFile(means, means_file);
    return means;
}

/*calculateCellMean
 * recieves group of points (cell) and returns their mean.
 * caculate the sum and then uses a server to make the final avg ccalculation (sum/size)
 * * (since there is no division in HElib and also to handle the case of vec size==0)
 * cellTuple - a tuple contains a vector of points (belonging to a specific cell) and its size.
 * keysServer - a link to the keyServer which holds the public keys. in future - publicKey.
 * return - avg point - a point equals in value the avg of all the points in the group
*/
DecryptedPoint calculateCellMean(tuple<Cell, EncNumber> cellTuple, KeysServer &keysServer) {
    Cell points = get<0>(cellTuple);
    Vec<Ctxt> size = get<1>(cellTuple);
    if (DBG) cout << "calculateCellMean points size " << points.size() << endl;
    if (DBG) fcout << "calculateCellMean points size " << points.size() << endl;
    Vec<EncNumber> vec[DIM];// array of vectors - cell per coordinate - vector of encrypted numbers

    // for every point p  // put p.x into vec.X // put p.y into vec.Y
    for (Point p : points) for (int i = 0; i < DIM; ++i) vec[i].append(p[i]);
    // sum each of the vectors
    EncNumber sum[DIM];
    for (int j = 0; j < DIM; ++j) sum[j] = sumEncNumVec(vec[j], NUM_POINTS * pow(EPSILON, 2));
    Point sumPoint = Point(vector<EncNumber>(sum, sum + DIM), &keysServer);
    //caculate the avg point using the server
    DecryptedPoint avgPoint = keysServer.calculateAvgPoint(sumPoint, size);
    if (DBG) cout << "avgPoint: " << avgPoint << endl;
    if (DBG) fcout << "avgPoint: " << avgPoint << endl;
    return avgPoint;
}

/*getDistances
 * receives a list of clients and returns the shortest distance between the client and it's closest mean
 * clients - a vector of points
 * means - the vector of cell means, decrypted since this data does not leak any information
 * returns - distances - a vector of shortest distances
 * */
Vec<EncNumber> getDistances(const vector<PointExtended> &clients, const vector<DecryptedPoint> &means) {
//    cout << "getDistances means: " << means << endl;
    Vec<EncNumber> distances;
    for (PointExtended client : clients) {
        const EncNumber &dist = client.getDistanceFromClosestPoint(means);
        distances.append(dist);
    }
    return distances;
}

/*calculateThreshold
 * receives a list of distances and returns the average distance,
 * to be the threshold by which "close" points will be chosen late ("close" - within the threshold radius)
 * distances - a vector of encrypted distances
 * keysServer - a link to the keyServer which holds the public keys. in future - publicKey.
 * returns - eThreshold - an encrypted number. should equal to sum(dist)/(#points)
 * */
EncNumber calculateThreshold(Vec<EncNumber> distances, KeysServer &keysServer) {//}, int amount) {
    EncNumber eThreshold;
    FHEPubKey &pubKey = *keysServer.pubKey;

    try {
        EncNumber distSum = sumEncNumVec(distances, NUM_POINTS * pow(EPSILON, 2));
        long size = distances.length();
        if (DBG) cout << "distSum: " << keysServer.decrypt(distSum) << " size: " << size << endl;
        if (DBG) fcout << "distSum: " << keysServer.decrypt(distSum) << " size: " << size << endl;
        long threshold = keysServer.calculateAvg(distSum, size);
        Ctxt mu(pubKey);
        resize(eThreshold, BIT_SIZE, mu);
        for (long i = 0; i < BIT_SIZE; i++)
            pubKey.Encrypt(eThreshold[i], ZZX((threshold >> i) & 1));

        return eThreshold; //todo check if division by a decrypted number is allowed in helib
    }
    catch (const exception &exc) {
        if (DBG) cout << "exception: " << exc.what() << endl;
        if (DBG) fcout << "exception: " << exc.what() << endl;
        pubKey.Encrypt(eThreshold[0], ZZX((0 >> 0) & 1));
        return eThreshold; //todo
    }
}

/*getLeftoverPoints
 * split the group of points into "close" and "far" points.
 * "close" - meaning within the threshold radius around any of the mean points.
 * clients - a vector of points
 * means - the vector of the means in the strip
 * keysServer - a link to the keyServer which holds the public keys. in future - publicKey.
 * returns - retVec - a tuple of 2 vector -
 *    a vector of points chosen for this iteration
 *    a vector of points to be used in the next iteration
 * */
vector<vector<Point> > getLeftoverPoints(
        const vector<PointExtended> &clients, const vector<DecryptedPoint> &means, KeysServer &keysServer) {
    Vec<EncNumber> distances = getDistances(clients, means); //todo recieve as param

    EncNumber threshold = calculateThreshold(distances, keysServer);
    cout << "The threshold is: " << keysServer.decrypt(threshold) << endl;
    fcout << "The threshold is: " << keysServer.decrypt(threshold) << endl;

    vector<Point> chosen, leftover;
    for (long j = 0; j < clients.size(); ++j) {
        //mu, ni are encrypted bits indicating if a>b and b>a, with respect
        Ctxt mu(*keysServer.pubKey), ni(*keysServer.pubKey);
        compareTwoNumbers(mu, ni, CtPtrs_VecCt(threshold), CtPtrs_VecCt(distances[j]), &unpackSlotEncoding);
//    mu==1 & ni==0 if current point within the threshold, and mu==0 & ni==1 otherwise

//        if (threshold >= decDistances[j]) chosen.push_back(clients[j]);
//        else leftover.push_back(clients[j]);
        const Point &isPoint = clients[j] * mu;// chosen += mu*client
        chosen.push_back(isPoint);
        const Point &isNotPoint = clients[j] * ni;    // leftover  += ni*client
        leftover.push_back(isNotPoint);
        if (DBG) {
            cout << "isPoint: " << isPoint.decrypt(keysServer);
            fcout << "isPoint: " << isPoint.decrypt(keysServer);
            cout << "   isNotPoint: " << isNotPoint.decrypt(keysServer) << endl;
            fcout << "   isNotPoint: " << isNotPoint.decrypt(keysServer) << endl;
        }
    }
    vector<vector<Point> > retVec;
    retVec.push_back(chosen);
    retVec.push_back(leftover);
    if (DBG) {
//    cout << "distances of size: " << distances.length() << " " << distances << endl;
//    cout << "chosen of size: " << chosen.size() << " " << chosen << endl;
//    cout << "leftover of size: " << leftover.size() << " " << leftover << endl;
//    fcout << "distances of size: " << distances.length() << " " << distances << endl;
//    fcout << "chosen of size: " << chosen.size() << " " << chosen << endl;
//    fcout << "leftover of size: " << leftover.size() << " " << leftover << endl;
        // todo make sure to not use the files as input (the points should be encrypted)
        decAndWriteToFile(chosen, chosen_file, keysServer);
        decAndWriteToFile(leftover, leftover_file, keysServer);
    }
    return retVec;
}
