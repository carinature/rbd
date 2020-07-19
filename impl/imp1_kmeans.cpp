#include <iostream>
#include <algorithm>
#include <cmath>       /* pow */


#include "yonis/run1meancore.h"
#include "imp1_kmeans.h"
//#include "aux.h"

using namespace std;

vector<vector<Point> >
getLeftoverPoints(
        const vector<PointExtended> & clients,
        const vector<DecryptedPoint> & means,
        KeysServer & keysServer);


using Cell = vector<Point>;
using Points = vector<Point>;


int main(int argc, char * argv[]) {
  auto * ks = new KeysServer();
  
//  vector<PointExtended> clients = getEncryptedPointsFromFile(*ks);
  
  encryptedKmeans();
  
  return 0;
}

void encryptedKmeans() {
    
  cout << " --------- encryptedKmeans --------- " << endl;
  auto t1 = std::chrono::high_resolution_clock::now();
  
  auto * ks = new KeysServer();  //   in the future should be FHEPubKey * pubKey = ks->pubKey;
  vector<PointExtended> clients = getEncryptedPointsFromFile(*ks);
  if (clients.empty()) return; // end of recursion
  
  vector<Point> points(clients.begin(), clients.end());
  
  vector<DecryptedPoint> means = calculateMeans(getCells(points, *ks), *ks);
  cout << "size of means: " << means.size() << ": " << means << endl;
  
  vector<vector<Point> > pointsSplited = getLeftoverPoints(clients, means, *ks);
  vector<Point> chosen = pointsSplited[0];
  vector<Point> & leftover = pointsSplited[1];
  
  cout << "           OK" << endl;
  auto t2 = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
  cout << "\n--- duration: " << duration; // << endl;
  cout << "\n--- #points: " << NUM_POINTS << " EPSILON = " << EPSILON << endl;
  
  
/**********   integration to the project    ***********/
  decAndWriteToFile(leftover, "points", *ks);
  
  vector<DecryptedPoint> chosenForCorset = getPointsFromFile("io/chosen");
  vector<vector<double> > dps;
  for (const DecryptedPoint & point : chosenForCorset) {
    vector<double> dpd;
    for (long coor : point) dpd.push_back(coor / CONVERSION_FACTOR);
//        dps.push_back(point.decrypt(*ks));
    dps.push_back(dpd);
//        if (0==dp[0] && 0==dp[1]) ++count;
//        cout << " point: " << point << endl;
  }
//    cout << "count: "<< count<< endl;
  
  decAndWriteToFile(leftover, "points", *ks);

  runCoreset(dps, NUM_POINTS, DIM, EPSILON);
  
  encryptedKmeans();

  return;
}


vector<tuple<vector<Point>, EncNumber> > getCells(vector<Point> points, KeysServer & keysServer) {
  cout << "getCells" << endl;
  fcout << "getCells" << endl;
  vector<Point> randPoints, leftover, chosen;
  vector<DecryptedPoint> means, badRands;
  
  srand(time(NULL));  // for extra randomness //fixme - why is this not random!?!?!
  cout << "TIME: " << time(NULL) << endl;
  
  vector<tuple<vector<Point>, EncNumber> > cellTuples;
  int numOfStrips = (int) (1 / EPSILON);
  
  for (int i = 0; i < numOfStrips; ++i) {
    //    Fisher–Yates shuffle for choosing k random points
    int stripSize = (int) (points.size() * EPSILON), k = numOfStrips;
    vector<Point> currStrip(points.begin() + i * stripSize, points.begin() + ((i + 1) * stripSize));
    vector<Point> copy(currStrip); //todo note the loss of points in tail (need to correct)
    auto begin = copy.begin();
    while (k--) {
      auto r = begin;
      advance(r, rand() % stripSize); //this line crushes the program with small(<20) #points in file
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
    fcout << "\ncreating the dict took: " << duration << " seconds.\n" << endl;
    
    ////    CUT INTO CELLS
    int rp = 0;  //DBG
    for (const Point & R : random) {        //for each rand point (cell)
      ////  find the points for each cell
      cout << "The random point is : " << R.decrypt(keysServer) << endl;
      fcout << "\nThe random point is : " << R.decrypt(keysServer) << endl;
      Cell currCell;
//            Point sum = R;
      Vec<EncNumber> sizeVec;  // vector<Bit> sizeVec;
      //  for each point in the strip
      int j = 0;  //DBG
      
      for (const Point & p : currStrip) {
        //        is above other stripMeans
        vector<Bit> cmpIsAbove;
        cout << "strip:" << i << ", rp: " << rp << ", p: " << j++ << " ----- "; // << endl;   //DBG
        fcout << "strip:" << i << ", rp: " << rp << ", p: " << j++ << " ----- "; // << endl;   //DBG
        for (const Point & r2 : random) {
          Bit r2IsSmaller = cmp[p][r2][0];
          r2IsSmaller.multiplyBy(cmp[R][r2][0]);  // r2IsSmaller = (p > r2) * (R > r2)
          Bit r2IsBigger = cmp[r2][R][0];  // r2IsBigger = (r2 > R)
          // Bit r2IsBigger = cmp(r2, p) * cmp[r2][R]; //this worked but was redundant
          
          Bit isAboveR2 = r2IsSmaller; // Bit isAboveR2 = r2IsSmaller + r2IsBigger - r2IsSmaller * r2IsBigger
          isAboveR2.multiplyBy(r2IsBigger);
          isAboveR2.negate();
          isAboveR2 += r2IsSmaller;
          isAboveR2 += r2IsBigger;
          cmpIsAbove.push_back(
                  isAboveR2);  // currently the vector is for DBG purposes - cmpIsAbove *= isAboveR2;
        }
        Bit isAboveOtherReps = cmpIsAbove[0];
        for (const Ctxt & b : cmpIsAbove) isAboveOtherReps.multiplyBy(b);  //accumulate prod of cmpIsAbove
        Bit isInCell = cmp[R][p][0];
        isInCell.multiplyBy(isAboveOtherReps);   //  is in  = is under random * is over other randoms
        Point isPoint = p * isInCell;
        currCell.push_back(isPoint); // todo consider  might be more efficient to summ all the points together
        
        EncNumber isInCellNum;
        isInCellNum.append(isInCell);
        if (DBG) {
          cout << "   cmpIsAbove: [ ";
          fcout << "   cmpIsAbove: [ ";
          for (const Ctxt & b : cmpIsAbove) {
            EncNumber bb;
            bb.append(b);
            cout << keysServer.decrypt(bb) << " ";
            fcout << keysServer.decrypt(bb) << " ";
          }
          cout << "] ";
          fcout << "] ";
          EncNumber bb;
          bb.append(cmp[R][p][0]);
          cout << "   R > p: [" << keysServer.decrypt(bb) << "] ";
          cout << "   isInCellNums: " << keysServer.decrypt(isInCellNum) << " ";
          fcout << "   R > p: [" << keysServer.decrypt(bb) << "] ";
          fcout << "   isInCellNums: " << keysServer.decrypt(isInCellNum) << " ";
        }
        sizeVec.append(isInCellNum);  // sizeDBG.append(isInCell);
//                addTwoNumbers(eep, CtPtrs_VecCt(size), CtPtrs_VecCt(isInCellNum));//, BIT_SIZE, &unpackSlotEncoding);
        if (DBG) {
//                    cout << "   size after addNums: " << keysServer.decrypt(size) << " ";
          cout << "   isPoint: " << isPoint.decrypt(keysServer) << " \n";
          fcout << "   isPoint: " << isPoint.decrypt(keysServer) << " \n";
//                    cout << "   sum: " << sum.decrypt(keysServer) << " \n";
        }
      }
      // sum counter vector (#points_inCell)
      EncNumber size = sumEncNumVec(sizeVec);//, 1);
      if (0 == keysServer.decrypt(size)) {
        cout << "\n \"special\" random point: " << R.decrypt(keysServer) << endl;
        fcout << "\n \"special\" random point: " << R.decrypt(keysServer) << endl;
        badRands.push_back(R.decrypt(keysServer));
      }
//            const CtPtrs & product = CtPtrs_VecCt(size);
//            CtPtrMat_VecCt nums(sizeVec); // Wrapper around numbers
//            addManyNumbers((CtPtrs &) product, nums, BIT_SIZE,  &unpackSlotEncoding); // <--- "findQ(5,3) not found" is printed here
      cellTuples.emplace_back(currCell, size);
      ++rp;
    }
  }
  if (DBG) decAndWriteToFile(randPoints, "io/random_means", keysServer);
  writeToFile(badRands, "io/bad_rands");//, keysServer);
  cout << "getCells cell size" << cellTuples.size() << endl;
  fcout << "getCells cell size" << cellTuples.size() << endl;
  
  return cellTuples;
}

vector<DecryptedPoint> calculateMeans(const vector<tuple<Cell, EncNumber> > & cellTuples, KeysServer & keysServer) {
  vector<DecryptedPoint> means;
  cout << "calculateMeans cell size" << cellTuples.size() << endl;
  fcout << "calculateMeans cell size" << cellTuples.size() << endl;
  int i = 0, j = 0; // for DBG
  for (const tuple<Cell, EncNumber> & cell : cellTuples) {
    ++j;
    try {
      ++i;
      means.push_back(calculateCellMean(cell, keysServer));
    } catch (const exception & exc) {
      cout << "exception: " << exc.what() << endl;
      cout << "SEGFAULT!!!    in cell #" << i << " or #" << j << endl;
      fcout << "exception: " << exc.what() << endl;
      fcout << "SEGFAULT!!!    in cell #" << i << " or #" << j << endl;
    }
  }
  if (DBG) writeToFile(means, "io/means");
  return means;
}

DecryptedPoint calculateCellMean(tuple<Cell, EncNumber> cellTuple, KeysServer & keysServer) {
  Cell points = get<0>(cellTuple);
  Vec<Ctxt> size = get<1>(cellTuple);
  cout << "calculateCellMean points size " << points.size() << endl;
  fcout << "calculateCellMean points size " << points.size() << endl;
  Vec<EncNumber> vec[DIM];// arr of vectors - cell per coordinate - vector of encrypted numbers
  // for every point p  // put p.x into vec.X // put p.y into vec.Y
  for (Point p : points) for (int i = 0; i < DIM; ++i) vec[i].append(p[i]);
  // sum each of the vectors
  EncNumber sum[DIM];
//    for (int j = 0; j < DIM; ++j) sum[j] = sumEncNumVec(vec[j], NUM_POINTS / pow(EPSILON, 2));  // waaayyy too much
//    for (int j = 0; j < DIM; ++j) sum[j] = sumEncNumVec(vec[j], NUM_POINTS);
  for (int j = 0; j < DIM; ++j) sum[j] = sumEncNumVec(vec[j], NUM_POINTS * pow(EPSILON, 2));
//    for (int j = 0; j < DIM; ++j) sum[j] = sumEncNumVec(vec[j], 1l / pow(EPSILON, 2));
//    for (int j = 0; j < DIM; ++j) sum[j] = sumEncNumVec(vec[j]);
  Point sumPoint = Point(vector<EncNumber>(sum, sum + DIM), &keysServer);
  DecryptedPoint avgPoint = keysServer.calculateAvgPoint(sumPoint, size);
  cout << "avgPoint: " << avgPoint << endl;
  fcout << "avgPoint: " << avgPoint << endl;
  return avgPoint;
}

Vec<EncNumber> getDistances(const vector<PointExtended> & clients, const vector<DecryptedPoint> & means) {
//    cout << "getDistances means: " << means << endl;
  Vec<EncNumber> distances;
  for (PointExtended client : clients) {
    const EncNumber & dist = client.getDistanceFromClosestPoint(means);
    distances.append(dist);
  }
  return distances;
}

EncNumber calculateThreshold(Vec<EncNumber> distances, KeysServer & keysServer) {//}, int amount) {
  EncNumber eThreshold;
  FHEPubKey & pubKey = *keysServer.pubKey;
  
  try {
//        EncNumber distSum = sumEncNumVec(distances, NUM_POINTS / pow(EPSILON, 2));
//        EncNumber distSum = sumEncNumVec(distances, NUM_POINTS);
    EncNumber distSum = sumEncNumVec(distances, NUM_POINTS * pow(EPSILON, 2));
//        EncNumber distSum = sumEncNumVec(distances, 1l / pow(EPSILON, 2));
//        EncNumber distSum = sumEncNumVec(distances);

//        const CtPtrs & product = CtPtrs_VecCt(distSum);
//        Vec<EncNumber> distanceVec;
//        for(const EncNumber & d : distances) distanceVec.append(d);
//        CtPtrMat_VecCt nums(distanceVec); // Wrapper around numbers
//        addManyNumbers((CtPtrs &) product, nums, BIT_SIZE,
//                       &unpackSlotEncoding); // <--- "findQ(5,3) not found" is printed here
    
    long size = distances.length();
    cout << "distSum: " << keysServer.decrypt(distSum) << " size: " << size << endl;
    fcout << "distSum: " << keysServer.decrypt(distSum) << " size: " << size << endl;
    long threshold = keysServer.calculateAvg(distSum, size);
    Ctxt mu(pubKey);
    resize(eThreshold, BIT_SIZE, mu);
    for (long i = 0; i < BIT_SIZE; i++)
      pubKey.Encrypt(
              eThreshold[i], ZZX((threshold >> i)&
                                 1)); ////    <----   THE PROBLEM was HERE
    
    return eThreshold; //todo check if division by a decrypted number is allowed in helib
  }
  catch (const exception & exc) {
    cout << "exception: " << exc.what() << endl;
    fcout << "exception: " << exc.what() << endl;
    pubKey.Encrypt(eThreshold[0], ZZX((0 >> 0)&1)); ////    <----   THE PROBLEM was HERE
    return eThreshold; //todo
  }
}

vector<vector<Point> > getLeftoverPoints(
        const vector<PointExtended> & clients,
        const vector<DecryptedPoint> & means,
        KeysServer & keysServer) {
  Vec<EncNumber> distances = getDistances(clients, means); //todo recieve as param
//    vector<long> decDistances;
//    for (const EncNumber & d : distances) decDistances.push_back(keysServer.decrypt(d));
  
  EncNumber threshold = calculateThreshold(distances, keysServer);
  cout << "The threshold is: " << keysServer.decrypt(threshold) << endl;
  fcout << "The threshold is: " << keysServer.decrypt(threshold) << endl;
  
  vector<Point> chosen, leftover;
  for (long j = 0; j < clients.size(); ++j) {
    Ctxt mu(*keysServer.pubKey), ni(*keysServer.pubKey);
    compareTwoNumbers(mu, ni, CtPtrs_VecCt(threshold), CtPtrs_VecCt(distances[j]), &unpackSlotEncoding);
    
    // chosen += mu*client
    const Point & isPoint = clients[j] * mu;
    cout << "isPoint: " << isPoint.decrypt(keysServer); //<<enl;
    fcout << "isPoint: " << isPoint.decrypt(keysServer); //<<enl;
    chosen.push_back(isPoint);
    // leftover  += ni*client
    const Point & isNotPoint = clients[j] * ni;
    cout << "   isNotPoint: " << isNotPoint.decrypt(keysServer) << endl;
    fcout << "   isNotPoint: " << isNotPoint.decrypt(keysServer) << endl;
    leftover.push_back(isNotPoint);

//        if (threshold >= decDistances[j]) chosen.push_back(clients[j]);
//        else leftover.push_back(clients[j]);
  }
  vector<vector<Point> > retVec;
  retVec.push_back(chosen);
  retVec.push_back(leftover);
  if (DBG) {
    //    cout << "distances of size: " << distances.size() << " " << distances << endl;
    cout << "chosen of size: " << chosen.size() << " " << chosen << endl;
    cout << "leftover of size: " << leftover.size() << " " << leftover << endl;
    //  fcout << "distances of size: " << distances.size() << " " << distances << endl;
    fcout << "chosen of size: " << chosen.size() << " " << chosen << endl;
    fcout << "leftover of size: " << leftover.size() << " " << leftover << endl;
    decAndWriteToFile(chosen, "io/chosen", keysServer);
    decAndWriteToFile(leftover, "io/leftover", keysServer);
  }
//    return leftover;
  return retVec;
}


/*
aux
 */



/*
 * Deprecated
 * */
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
  for (int i = 0; i < numOfStrips; ++i) {
    ////    Fisher–Yates shuffle for choosing k random points
    int stripSize = (int) (points.size() * EPSILON), k = numOfStrips; //, left = stripSize;
    vector<Point> currStrip(points.begin() + i * stripSize, points.begin() + ((i + 1) * stripSize));
    vector<Point> copy(currStrip);
    auto begin = copy.begin(); //todo note the loss of points in tail (need to correct)
    while (k--) {
      auto r = begin;
      advance(r, random() % stripSize); //notice this line crushes the program with small(<=>20) #points in file
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
    for (const Point & R : random) {
      cout << "The random point is : " << R.decrypt(keysServer) << endl;
      ////  find the points for each cell
      vector<Point> currCell;
      Point sum = R;
      Vec<EncNumber> sizeVec;  // vector<Bit> sizeVec;
      //  for each point in the strip
      int j = 0;  //DBG
      for (const Point & p : currStrip) {
        //        is above other stripMeans
        vector<Bit> cmpIsAbove;
        cout << "strip:" << i << ", rp: " << rp << ", p: " << j++ << " ----- "; // << endl;   //DBG
        for (const Point & r2 : random) {
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
        for (const Ctxt & b : cmpIsAbove) isAboveOtherReps.multiplyBy(b);  //accumulate prod of cmpIsAbove
        Bit isInCell = cmp[R][p][0];
        isInCell.multiplyBy(isAboveOtherReps);   //  is in  = is under random * is over other randoms
        Point isPoint = p * isInCell;
        currCell.push_back(isPoint); // todo consider  might be more efficient to summ all the points together
        sum = sum + isPoint;  // todo convert and use addManyNumbres instead
        EncNumber isInCellNum;
        isInCellNum.append(isInCell);
        if (DBG) {
          cout << "   cmpIsAbove: [ ";
          for (const Ctxt & b : cmpIsAbove) {
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
        if (DBG) {
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
//        int nBits = (outSize>0 && outSize<2*BIT_SIZE)? outSize : (2*BIT_SIZE);
      //fixme - potential for BUG, sum.bitsize can be bigger than BIT_SIZE
      addManyNumbers((CtPtrs &) product, nums, BIT_SIZE,
                     &unpackSlotEncoding); // <--- "findQ(x,y) not found" is printed here
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
  if (DBG) decAndWriteToFile(randPoints, "io/random_means", keysServer);
  writeToFile(means, "io/means_test");
  return means;
}

