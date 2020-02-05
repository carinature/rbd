//
// Created by rbd on 31.1.2020.
//

#include "SKeys.h"

Point SKeys::calculateAvgPointByCA(const Point &point, Binary amount) {
    try {
        return point / amount;
    }
    catch (...) {
        return Point::zeroPoint(); ///TODO
    }
}

Binary SKeys::calculateThresholdByCA(vector<Binary> distances, int amount) {
    try {
        Binary distSum = distances[0];
        for (Binary d : distances) {
            distSum += d;
        }
        distSum -= distances[0];
        return distSum / Binary(amount); //todo check if division by a decrypted number is allowed in helib
    }
    catch (...) {
        return Point::zeroPoint()[0]; ///TODO
    }
}

DecryptedPoint SKeys::decryptPointByCA(const Point& p) {
    DecryptedPoint decrypted;
    for (Binary c : p.getCoordinates()) decrypted.push_back(c.getDecValue());
    return decrypted;
}

vector<DecryptedPoint> SKeys::decryptPointsByCA(const vector<Point> &reps) {
    vector<DecryptedPoint> decReps;  // decrypt decReps
    decReps.reserve(reps.size());
    for (const Point &p : reps) decReps.push_back(SKeys::decryptPointByCA(p));
    return decReps;
}

