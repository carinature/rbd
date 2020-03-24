

//#include <unistd.h>
//#include <numeric>
//#include <NTL/BasicThreadPool.h>
//#include <helib/ArgMap.h>
//#include <helib/binaryCompare.h>
//#include <helib/binaryArith.h>
//#include <helib/intraSlot.h>
//#include <helib/EncryptedArray.h>
//#include <helib/FHE.h>
//#include <cassert>
//#include <string>
//#include <iostream>
//#include <cmath>
//#include <algorithm>
#include "aux.h"
#include "properties.h"

#include <iostream>


using namespace std;

/** Writing points to a specified.
 *      The points are first decrypted and returned into a double form (from long) **/
//*  todo consider changing KeysServer to PubKey
void writeToFile(const vector<Point> & vec, const string & filename,
                 KeysServer & keysServer) {// fixme PubKey instead of KeysServ
    vector<DecryptedPoint> points; // = sk.decryptPointsByCA(vec);
    for(Point p : vec) points.push_back(p.decrypt(keysServer));
//    cout << "=%%%%%%%%%%%%%%%%%%%%%%%= in write points: " << points << endl;
    ofstream outputFileStream(filename);
    for(const DecryptedPoint & p : points) {
        for(double coor : p) {
//            cout << "in write coor: " << coor << endl;
            outputFileStream << coor / FACTOR << " ";
        }
        outputFileStream << '\n';
//        outputFileStream.flush();
    }
    outputFileStream.close();
}

/** Retrieve points from a specified file. Points are palintext.
 *      If the file is not specified uses default file 'points' **/
vector<DecryptedPoint> getPointsFromFile(const string & filename) {
    ifstream inputFileStream(filename);
    string line; //, pa0, pa1;
    string pa[DIM];
//    int count;  //  #points
//    inputFileStream >> count;
//    inputFileStream.ignore(1, '\n');
    
    vector<DecryptedPoint> points;
//    for (int i = 0; i < count; ++i) {
//    getline(inputFileStream, line);
    while(getline(inputFileStream, line)) {
        istringstream lineStream(line);
        DecryptedPoint point;
        for(auto & coor : pa) {
//            getline(lineStream, coor, ',');
            getline(lineStream, coor, ' ');
//            point.push_back(stod(coor));
//            cout <<" FACTOR * stod(coor) " << FACTOR * stod(coor) << endl;
//            cout <<" stod(coor) " << stod(coor) << endl;
//            cout <<" (coor) " << coor << endl;
            point.push_back(FACTOR * stod(coor)); // conversion from double to long
        }
        points.push_back(point);
    }
    return points;
}

vector<Point> getEncryptedPointsFromFile(KeysServer & keysServer) { // fixme PubKey instead of KeysServ
    vector<DecryptedPoint> points = getPointsFromFile();
    vector<Point> encPoints;
    for(DecryptedPoint p : points) {
        vector<long> coorVec; // = vector<Binary>();
        for(int coor = 0; coor < DIM; ++coor) {
            coorVec.emplace_back(p[coor]);
        }
//        PointExtended point = PointExtended(keysServer.pubKey, coorVec);
        Point point = PointExtended(keysServer.pubKey, coorVec);
        encPoints.emplace_back(point);
    }
    return encPoints;
}

Bit cmp(const Point & a, const Point & b) {
    //todo cmp should return bit
    //todo override Point's operator<
    //todo use helib cmp
    //todo consider <= over < (the later neve allows any points) and how to implement it with helib
//    return a >= b;
    return a > b;
//    return a[1] <= b[1];
}

map<Point, map<Point, vector<Bit>, cmpPoints>, cmpPoints> createCmpDict(const vector<Point> & randomPoints) {
    map<Point, map<Point, vector<Bit>, cmpPoints>, cmpPoints> cmpDict;
    for(const Point & p : randomPoints) {
        map<Point, vector<Bit>, cmpPoints> cmpDictMini;
        for(const Point & pp : randomPoints) {
            cmpDictMini[pp].push_back(p > pp);
        }
        cmpDict[p] = cmpDictMini;
    }
    return cmpDict;
}

///** aux **/

//DecryptedPoint operator/(DecryptedPoint p, int factor) {
//    if(0 == factor) return p;
//    DecryptedPoint dp;
//    for(long i : p) {
//        double x = double(double(i) / double(factor));
//        dp.push_back(x);
////        cout << x << ",";
//    }
////    cout << endl;
//    cout << "p: " << p << endl;
//    cout << "dp: " << dp << endl;
//    return dp;
//}
//



/*
 * Candidates for a MOVE
 */
//Binary encryptDouble(double d) {
//    return Binary(d);
//}
//
//vector<Binary> encryptVec(const vector<double> & vector) {
//    std::vector<Binary> encVec;
//    encVec.reserve(vector.size());
//    for(double d : vector) encVec.push_back(encryptDouble(d));
//    return encVec;
//}

//vector<Binary> getDistFromClosestMeanByClient
//        (const vector<DecryptedPoint> &reps, const vector<Point> &pointsForDBG, Skeys &sk) {
//    // init distance vector
//    vector<double> distance;
//    // each point ( from points for DBG)
//    for (const Point &p : pointsForDBG) {
//        DecryptedPoint point = sk.decryptPointByCA(p);
////        DecryptedPoint point = Skeys::decryptPointByCA(p);
//        if (isNullPoint) distance.push_back(-1);
//        else {
//            //  calculates the distance from the reps
//            double min = dist(point, reps[0]);
//            for (const DecryptedPoint& rep : reps) {
//                double currDist = dist(point, rep);
//                if (currDist < min) min = currDist;
//            }
//            //  chooses the smallest distance and adds to the distance vector
//            distance.push_back(min);
//        }
//    }
//    // return the distance vector
//    return encryptVec(distance);
//}


/*
 * Deprecated
 */
/*
// Calculates Euclidean distance
double dist(const DecryptedPoint & a, const DecryptedPoint & b) {
    //  for i : m.size  :  sqr += m[i]^2 - p[i]^2
    double sqr = 0;
    for(int i = 0; i < DIM; ++i) {
        sqr += pow(a[i] - b[i], 2);
    }
    return sqrt(sqr);
}

 vector<vector<double> > getReps(vector<vector<double> > points) {
    vector<vector<double> > representatives;
    int numOfStrips = 1 / EPSILON;
//    int numOfCells = 1 / pow(EPSILON,2);
    cout << EPSILON << endl;
    cout << numOfStrips << endl;
    for (int i = 0; i < numOfStrips; ++i) {  //todo note the loss of points in tail (need to correct)
        int stripSize = (int) (points.size() * EPSILON);
        vector<vector<double> > currStrip(points.begin() + i * stripSize, points.begin() + ((i + 1) * stripSize));
        cout << "currStrip - " << currStrip << endl;
        for (int j = 0; j < numOfStrips; ++j) {  //todo note the loss of points in tail (need to correct)
            int cellSize = (int) (currStrip.size() * EPSILON);
            vector<vector<double> > currCell(currStrip.begin() + j * cellSize,
                                             currStrip.begin() + ((j + 1) * cellSize));
            //  #TODO - replace this part with your alg
//            nth_element(currCell.begin(), currCell.begin() + currCell.size()/ 2, currCell.end(), cmp);
            representatives.push_back(currCell[currCell.size() / 2]);
            cout << ".  The median is " << currCell[currCell.size() / 2] << '\n';
        }
    }
    cout << "\n # representatives:  " << representatives.size() << endl;
    cout << "The representatives are:  " << representatives << endl << endl;
    return representatives;
}
vector<Point> getRepFromClient(const Point &encMean, const vector<Point> &encCell, Binary encCellSize) { //todo add #points in cell?
    cout << endl ;
    tag("getRepFromClient");
    cout << "encMean: " << encMean << endl << "encCell: ";
    tag(encCell);

    // decrypt encMean and encCell points
//    double cellSize = encCellSize.getDecValue();
    double sumDist = 0;
    DecryptedPoint mean;  // decrypt mean
    for (Binary c : encMean.getCoordinates()) mean.push_back(c.getDecValue());
    vector<DecryptedPoint> cell;  // decrypt cell
    for (const Point &p : encCell) {
        DecryptedPoint decrypted;
        for (Binary c : p.getCoordinates()) decrypted.push_back(c.getDecValue());
        cell.push_back(decrypted);
    }
    tag("decrypt encMean and encCell");
//    cout << "mean: "; tag(mean);
    cout << "cell: "; tag(cell);

    // calculate distances from encCell points to encMean
//    DecryptedPoint sum(mean.size(), 0); // for the threshold todo in don think this is really needed
    vector<double> distDict;
    for (const DecryptedPoint& point : cell) {
        tag("mean and point");
        tag(mean);
        tag(point);
        tag();
        if (isNullPoint) { // first check that point != (0,0)
            double distance = dist(mean, point);
            distDict.push_back(distance);
//            transform(sum.begin(), sum.end(), point.begin(), sum.begin(), plus<double>());  // sum += point
            sumDist += distance;
            cout << "distDict: "; tag(distDict);  // todo consider an actual dict and not an array/list/vector
//            cout << "sum: "; tag(sum);
        }
    }
//    tag("sum");
//    tag(sum);

    // choose the closes points, i.e. the encReps (the leftovers)
//    double threshold = sum / cellSize;  //todo not good - cell.size contains zeroes
//    transform(sum.begin(), sum.end(), sum.begin(), plus<double>());
//    cout << "mean: " << mean << endl;
//    cout << "threshold: " << threshold << endl;
//    for (double d : distDict) cout << d << endl;

    // encrypt the points
    vector<Point> encReps;
    for (const DecryptedPoint& p : cell) {
        vector<Binary> encBinVec;
        for (double c : p) encBinVec.emplace_back(c);
        encReps.emplace_back(encBinVec,encMean.sk);
    }
    cout << "encReps: " ;
    tag(encReps);
    return encReps;
}
*/
//

