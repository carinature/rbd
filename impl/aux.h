
#ifndef TRY_AUX_H
#define TRY_AUX_H

#include "Point.h"
#include "properties.h"
#include <map>

struct cmpPoints;

void decAndWriteToFile( const vector<Point> & points, const string & filename, KeysServer & keysServer );

void writeToFile( const vector<DecryptedPoint> & points, const string & filename );

void writeToFile( const vector<Point> & points, const string & filename );

//! retrieves the list of coordinates and converts to a list of points
vector<DecryptedPoint> getPointsFromFile( const string & filename = points_file );

vector<PointExtended> getEncryptedPointsFromFile( KeysServer & keysServer );

//! creates a dictionary of helib's compatr results.
//! the result for the query point1 > point2 is located at cmp[point1][point2] and return as Bit (Ctxt)
map<Point, map<Point, vector<Bit>, cmpPoints>, cmpPoints>
createCmpDict( const vector<Point> & randomPoints, const vector<Point> & stripPoints );
//Bit cmp(const Point & a, const Point & b) ;

EncNumber sumEncNumVec( Vec<EncNumber> & vec, long factor = 1 );


long encryptDouble( double d );

vector<long> encryptVec( const vector<double> & vector );


/***********  aux  ***********/
DecryptedPoint operator/( DecryptedPoint p, int factor );

struct cmpPoints {
    bool operator()( const Point & a, const Point & b ) {
        return a.id > b.id;
    }
};

chrono::time_point<chrono::system_clock> NowTime();

void printDuration( const chrono::time_point<chrono::system_clock> & t1, const string & funcName = "" );


/**
 * Logger Class
 * */
enum LogLevel {
    log_trace = 0,
    log_debug,
    log_info,
    log_warning,
    log_error,
    log_fatal,
};

class Logger {
private:
    LogLevel level = log_trace;
    ostringstream logs[log_fatal+1];

    static string levelToString( LogLevel level );

public:
    explicit Logger( LogLevel minLevel = log_trace );

    virtual ~Logger();

    void log( LogLevel msgLevel, const string & msg );
};



/*
 * Deprecated
 */
/*
using namespace std;
using Binary = long;

vector<long> getDistFromClosestMeanByClient(const vector<DecryptedPoint> &reps, const vector<Point> &pointsForDBG, Skeys &sk);

#define isNullPoint point == vector<double>(DIM, 0)

using DecryptedPoint =  vector<long>;

void tag();

template<typename T>
void tag(T t);

template<typename T>
void tag(vector<T> vector);

template<typename T>
void tag(vector<vector<T> > vector);
*/



#endif //TRY_AUX_H


