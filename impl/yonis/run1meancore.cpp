//#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include "run1meancore.h"
//using namespace std;

//void toCSV(vector<vector<double>> P, int n, int d, string file);
//
//vector<vector<double>> & parseCSV(string file);

//inline vector<vector<double>> & runCoreset(vector<vector<double>> & P, int n, int d, double eps,
//        double alpha = 1, double delta = 0.1, bool isPrivate = true, int security = 1024){}
vector<vector<double>> & runCoreset(vector<vector<double> > & P, int n, int d, double eps,
        double alpha, double delta, bool isPrivate, int security) {
    toCSV(P, n, d, "points.csv");
//    string commandLS ="ls -l */*"; system(commandLS.c_str());
    string command =
            "/home/fares/.virtualenvs/kmeans/bin/python -m impl.yonis.simulator points.csv " + to_string(eps) + " " + to_string(alpha) + " " + to_string(delta) +
            " -s " + to_string(security) + (isPrivate ? "" : " -n") + " -f coreset.csv";
    system(command.c_str());
//    string commandl ="ls -l "; system(commandl.c_str());
//    string commandl2 ="pwd "; system(commandl2.c_str());
    return parseCSV("coreset.csv");
}

void toCSV(vector<vector<double>> P, int n, int d, string file) {
    ofstream fout;
    fout.open(file);
    for (int i = 0; i < n; i++) {
        int j;
        for (j = 0; j < d - 1; j++) fout << P[i][j] << ',';
        fout << P[i][j] << '\n';
    }
    fout.close();
}

// TODO fix bug
vector<vector<double>> & parseCSV(string file) {
    ifstream data;
    data.open(file);
    string line;
    vector<vector<double>> parsedCsv;
    
    while (getline(data, line)) {
        stringstream lineStream(line);
        string cell;
        vector<double> parsedRow;
        while (getline(lineStream, cell, ',')) {
            parsedRow.push_back(stod(cell));
        }
        parsedCsv.push_back(parsedRow);
    }
    data.close();
    return parsedCsv;
}

