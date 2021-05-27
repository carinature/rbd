

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
void
decAndWriteToFile( const vector<Point> & points, const string & filename, KeysServer & keysServer ) {  // <----------
    vector<DecryptedPoint> decPoints;
    decPoints.reserve( points.size() );
    for ( const Point & p : points ) decPoints.push_back( p.decrypt( keysServer ) );
    ofstream outputFileStream( filename );
    stringstream ss;
    long sum;
    for ( const DecryptedPoint & p : decPoints ) {
        sum = 0;
        ss.str( std::string() );
        for ( long coor : p ) {
            sum += coor;
            ss << coor / CONVERSION_FACTOR << " ";
        }
        if ( 0 < sum ) {
            outputFileStream << ss.rdbuf() << endl;
        }
    }
//    outputFileStream.flush();
    outputFileStream.close();
}

void
writeToFile( const vector<DecryptedPoint> & points, const string & filename ) {  // fixme PubKey instead of KeysServ
    ofstream outputFileStream( filename );
    for ( const DecryptedPoint & p : points ) {
        for ( long coor : p ) outputFileStream << (double) ( coor / CONVERSION_FACTOR ) << " ";
        outputFileStream << '\n';
    }
    outputFileStream.close();
}

void writeToFile( const vector<Point> & points, const string & filename ) {  // fixme PubKey instead of KeysServ
    cout << "write enc points to file" << endl;
    ofstream outputFileStream( filename );
    for ( const Point & p : points ) {
        for ( EncNumber coor : p.eCoordinates ) outputFileStream << coor << "\n\n---\n\n";
        outputFileStream << '\n';
    }
    outputFileStream.close();
}

/** Retrieve points from a specified file. Points are palintext.
 *      If the file is not specified uses default file 'points' **/
vector<DecryptedPoint> getPointsFromFile( const string & filename ) {
    ifstream inputFileStream( filename );
    string line;
    string pa[DIM];
    vector<DecryptedPoint> points;
    while ( getline( inputFileStream, line ) ) {
        istringstream lineStream( line );
        DecryptedPoint point;
        for ( auto & coor : pa ) {
            getline( lineStream, coor, ' ' );
//            cout <<" CONVERSION_FACTOR * stod(coor) " << CONVERSION_FACTOR * stod(coor) << endl;
//            cout <<" stod(coor) " << stod(coor) << endl;
//            cout <<" (coor) " << coor << endl;
            point.push_back( CONVERSION_FACTOR * stod( coor ) ); // conversion from double to long
        }
        points.push_back( point );
    }
    return points;
}

vector<PointExtended> getEncryptedPointsFromFile( KeysServer & keysServer ) { // fixme PubKey instead of KeysServ
    vector<DecryptedPoint> points = getPointsFromFile();
    vector<PointExtended> encPoints;
    for ( DecryptedPoint p : points ) {
        vector<long> coorVec; // = vector<Binary>();
        for ( int coor = 0; coor < DIM; ++coor ) {
            coorVec.emplace_back( p[coor] );
        }
        PointExtended point = PointExtended( coorVec, & keysServer );
//        PointExtended point = PointExtended(keysServer.pubKey, coorVec);
        encPoints.emplace_back( point );
    }
    return encPoints;
}

Bit cmp( const Point & a, const Point & b ) {
    //todo consider <= over < (the later neve allows any points) and how to implement it with helib
//    return a >= b;
    return a > b;
//    return a[1] <= b[1];
}

map<Point, map<Point, vector<Bit>, cmpPoints>, cmpPoints>
createCmpDict( const vector<Point> & randomPoints, const vector<Point> & stripPoints ) {
    map<Point, map<Point, vector<Bit>, cmpPoints>, cmpPoints> cmpDict;
    for ( const Point & p : randomPoints ) {
        map<Point, vector<Bit>, cmpPoints> cmpDictMini; //, cmpDictMini2;
        for ( const Point & pp : stripPoints ) {
            cmpDict[p][pp].push_back( p > pp ); // used to be cmpDictMini[pp].push_back(p > pp); and worked
            cmpDict[pp][p].push_back( pp > p );
        }
    }
    return cmpDict;
}

EncNumber sumEncNumVec( Vec<EncNumber> & vec, long factor ) {
    EncNumber sum;
    const CtPtrs & product = CtPtrs_VecCt( sum ); //, & productY = CtPtrs_VecCt(sumY);
    CtPtrMat_VecCt nums( vec ); // Wrapper around numbers
//        int nBits = (outSize>0 && outSize<2*BIT_SIZE)? outSize : (2*BIT_SIZE);
    //fixme - potential for BUG, sum.bitsize can be bigger than BIT_SIZE
    // <--- notice "findQ(x,y) not found" is printed here in addManyNum
    addManyNumbers( (CtPtrs &) product, nums, BIT_SIZE * factor, & unpackSlotEncoding );
//    cout << "sumEncNumVec sum: " << keysum << endl;
    return sum;
}

chrono::time_point<chrono::system_clock> NowTime() {
    return std::chrono::high_resolution_clock::now();
}

void printDuration( const chrono::time_point<chrono::system_clock> & t1, const string & funcName ) {
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>( t2 - t1 ).count();
    cout << "\'" << funcName << "\' Finished in " << duration << " seconds." << endl;
    fcout << "\'" << funcName << "\' Finished in " << duration << " seconds." << endl;
}

/**
 * Logger Class
 * */
Logger::Logger( LogLevel level ) {
    this->level = level;
    auto timenow = chrono::system_clock::to_time_t( chrono::system_clock::now() );
    for ( int l = level; l <= log_fatal; ++l ) this->logs[l] << ctime( & timenow );
}

Logger::~Logger() {
    for ( int l = this->level; l <= log_fatal; ++l ) {
        cout << " \n---------- " << levelToString( LogLevel( l ) ) << " ---------- " << endl;
        if ( log_error <= l ) {
            cerr << this->logs[l].str();
            cout << this->logs[l].str();
        }
        else cout << this->logs[l].str();
    }
}

void Logger::log( LogLevel msgLevel, const string & msg ) {
    for ( int l = this->level; l <= msgLevel; ++l ) {
        this->logs[l] << "- " << msg << endl;
        if ( log_error <= msgLevel ) cerr << msg << endl;
    }
}

void Logger::print_log( LogLevel msgLevel , bool all ) {
    if ( all ) {
        for ( int l = max( this->level, msgLevel ); l <= log_fatal; ++l ) {
            cout << "    --- " << levelToString( LogLevel( l ) ) << " --- " << endl << this->logs[l].str() << endl;
            if ( log_error <= msgLevel ) cerr << this->logs[l].str();

        }
    } else {
        cout << "    --- " << levelToString(msgLevel) << " --- " << endl << this->logs[msgLevel].str() << endl;
        if ( log_error <= msgLevel ) cerr << this->logs[msgLevel].str();
    }
}

string Logger::levelToString( LogLevel level ) {
    switch ( level ) {
        case log_trace:
            return "log_trace";
        case log_debug:
            return "log_debug";
        case log_info:
            return "log_info";
        case log_warning:
            return "log_warning";
        case log_error:
            return "log_error";
        case log_fatal:
            return "log_fatal";
    }
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

