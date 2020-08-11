#!/usr/bin/env python3

import sys
from random import *

from matplotlib import pyplot as plt
from numpy import *
from sklearn.cluster import KMeans


# Cluster data structure consisting of a list of points and a center/mean,
# 	which might not be up to date. Use cluster.updateCenter() for that purpose.
# 	(for efficiency, the cluster does not update the center every time a point
# 	is added. In a general case should be accurate enough)
class Cluster:
    center = 0
    pointsList = []

    # numOfPoints = 0
    def __init__(self, center, pointsList):  # todo maybe no need for empty C'tor and thus 'self' arg
        self.center = center
        self.pointsList = pointsList

    def __repr__(self):
        return "Cluster()\t == center: " + str(self.center) \
               + " == \n\t\tnumOfPoints: --" + str(len(self.pointsList)) \
               + "-- \n\t\tpoints: " + str(self.pointsList)

    def __str__(self):
        return "Cluster()\t == center: " + str(self.center) \
               + " ==\n\t\tnumOfPoints: --" + str(len(self.pointsList)) \
               + "-- \n\t\tpoints: " + str(self.pointsList)

    # Caculates and updates the center of the current cluster
    def updateCenter(
            self):  # todo - consider using median instead of mean. in that case need to sort by Y as well as X (in 2D)
        sumPoints = [0, 0]
        for p in self.pointsList:
            sumPoints[0] += p[0]
            sumPoints[1] += p[1]
        self.center = (round(sumPoints[0] / len(self.pointsList), 2), \
                       round(sumPoints[1] / len(self.pointsList), 2))
        return self.center

    def addPoint(self, point):
        self.pointsList.append(point)
        # numOfPoints+=1
        return

    def removePoint(self, point):
        self.pointsList.pop(self.pointsList.index(point))
        # numOfPoints-=1
        return


# ------------------------------------------------------------------------
#	The following functions - createList, createRandSegment, isPointInSeg, test - 
# 	are mainly for testing purposes 
# ------------------------------------------------------------------------

def createList(listSize, d=2):  # todo remove; temporary list creation
    listOrg = []
    for i in range(listSize):
        tempTup = [round(10 * random(), 2) for i in range(d)]
        listOrg.append(tempTup)
    listOrg.sort(key=lambda tup: tup[0])
    return listOrg


# 	# choose a random segment/rectangle/cube...
# def createRandSegment(listOrg, d=2): # todo - make a rectangle
# 	lowBound = [listOrg[0][i] for i in range(d)]
# 	upBound = [listOrg[-1][i] for i in range(d)]
# 	segment = [add(round(distance(upBound,lowBound,d)*random(),2), lowBound) \
# 	for i in range(d)]

# 	# todo - find a rectangle
# 	return [(round(min(segment[0]),2), round(min(segment[1]), 2)), 		\
# 	(round(max(segment[0]),2), round(max(segment[1]), 2))]

# def isPointInSeg(segment, p): # todo - generalize for d dimantions
# 	lowBound, upBound = segment[0], segment[1]
# 	if (p[0] > lowBound[0] and p[0] < upBound[0] and 		\
# 	    p[1] > lowBound[1] and p[1] < upBound[1]) : 
# 		# print("TRUE")
# 		return True
# 	else : 
# 		return False

# def test(listOrg, repList, segment, eps):
# 	orgCovered = list(filter(lambda p: isPointInSeg(segment, p), listOrg)) 
# 	# print("	OrgCovered: ", orgCovered)
# 	# print("	OrgCovered length: ", len(orgCovered))
# 	print(repList)
# 	print("@@@@@@@@@@@@@@@@@@@: " )
# 	repCovered = list(filter(lambda p: isPointInSeg(segment, p), repList))
# 	print("	RepCovered: ", repCovered if repCovered is not None else [])
# 	print("	RepCovered length: ", len(repCovered))


# ------------------------------------------------------------------------
#	Here are the 3 steps of k-means standard algorithm, 
#	according to wikipedia - Initialization, Assignment step, Update step
# 	combined in the getRepresent() function
# ------------------------------------------------------------------------

# Initialize the centers' list; implements the Forgy Method
# The Forgy method randomly chooses k observations from the dataset
#	and uses these as the initial means.
# --recieves:	listOrg - (List) dataset
#				k - (int) number of representatives/clusters
# --returns: 	(List) k randomly chosen representatives
def initial(listOrg, k):
    temp = listOrg[:]
    # print("====",temp)
    repList = [temp.pop(temp.index(choice(temp))) for i in range(k)]
    repList.sort(key=lambda tup: tup[0])  # todo maybe don't need 'key=lambda tup: tup[1]'
    return repList


# Assign each observation to the cluster
# 	whose mean has the least squared Euclidean distance
# --recieve:	listOrg - (List) dataset
# 				repList - (List) current cluster "centers"/ new reps
# --return:		(Dictionary) all the new clusters maped to their current centers/reps
def assign(listOrg, repList):
    clusters = {}
    for rep in repList:
        clusters[tuple(rep)] = Cluster(rep, [])
    for point in listOrg:
        dist = sys.maxsize - 1
        rep = None
        for r in repList:
            d = distance(point, r)
            if dist > d:
                dist, rep = d, r
        clusters[tuple(rep)].addPoint(point)
    return clusters


# Calculate the [Euclidean distance] between 2 points.
# --recieve:	p1, p2 - (Tuple) points
#				d - (int) dimantion [optional]
def distance(p1, p2, d=2):
    sqrSum = 0
    for i in range(d):
        sqrSum += (p1[i] - p2[i]) ** 2
    return round(sqrt(sqrSum), 2)


# Calculate and Update the new means (centroids) in the new clusters.
# --recieve:	clusters - (Dictionary) all the clusters
# --return:		(List) all the new centers/reps
def update(clusters):
    repList = [c.updateCenter() for c in clusters.values()]  # todo - consider deleting the clusters
    return repList


# ========================================================================
# ========================================================================
#	Here are the 3 different implementation of get "representatives"
# ========================================================================
# ========================================================================

# Implentation of the standard alg, which uses an iterative refinement technique.
# --recieves:	listOrg - (List) dataset
#				k - (int) number of representatives/clusters
# --returns: 	(List) k chosen representatives
def getRepresent(listOrg, k):
    repList = initial(listOrg, k)
    newList = []
    # rinse and repeat until there is no more place for refinement
    while newList != repList:
        clusters = assign(listOrg, repList)
        newList = repList
        repList = update(clusters)
    return repList


# Choosing reps using the standard k-means algorithm as presented by wikipedia.
# --recieves:	pointList - (List) dataset
#				k - (int) number of representatives/clusters
# --returns: 	(List) k chosen representatives
def withoutSklearn(pointList, k):
    repList = getRepresent(pointList, k)
    plt.scatter(*zip(*repList), c='red', s=6, label='Without Sklearn Lib')
    return repList


# Choosing reps using the standard k-means algorithm as presented by wikipedia.
# --recieves:	pointList - (List) dataset
#				k - (int) number of representatives/clusters
# --returns: 	(List) k chosen representatives
def withSklearn(pointList, k):
    f1 = [p[0] for p in pointList]
    f2 = [p[1] for p in pointList]
    arr = array(list(zip(f1, f2)))
    # Number of clusters
    kmeans = KMeans(n_clusters=k)
    # Fitting the input data
    kmeans = kmeans.fit(arr)
    # Getting the cluster labels
    labels = kmeans.predict(arr)
    # Centroid values
    repsList = kmeans.cluster_centers_
    plt.scatter(*zip(*repsList), c='pink', s=6, label='With Sklearn Lib')


# Implentation of of what I think is the problem given by Danny
# Choosing a representative from each section using the standard k-means alg.
#
# --recieves:	pointList - (List) dataset
#				eps -	(float) the margin of error. Also determens the
# 						number of sections into which the points are devided.
# --returns: 	(List) k chosen representatives
def brakeToSections(pointList, eps):
    n = len(pointList)
    stripSize = round(n * eps)
    cellSize = round(n * eps ** 2)
    sectionEnd = 0
    strips = []
    reps = []
    # cut into "strips"
    for i in range(round(1 // eps)):
        currStrip = pointList[i * stripSize:(i + 1) * stripSize - 1]
        strips.append(currStrip)
        sectionBegin = sectionEnd  # <--	for plot purposes only
        sectionEnd = strips[-1][-1][0]  # <--	for plot purposes only
        plt.axvline(x=sectionEnd, color='grey', linestyle='--', linewidth=1)  # <--	for plot purposes only
        strips[-1].sort(key=lambda x: x[1])
        cells = []
        # cut each strip into "cells"
        for j in range(round(1 // eps) + 1):
            currCell = strips[-1][j * cellSize:(j + 1) * cellSize - 1]
            if ([] != currCell):
                # choose a representative from each section by k-means
                reps.append(getRepresent(currCell, 1)[0])
                cells.append(currCell)
                plt.hlines(currCell[-1][1], sectionBegin, sectionEnd, \
                           colors='grey', linestyle='--', linewidth=1)  # <--	for plot purposes only
    plt.scatter(*zip(*reps), label='By Sections', c='black', s=10)  # <--	for plot purposes only


if __name__ == "__main__":
    # configure the params
    eps = 1 / 9
    n = 600
    # k = round(n*eps) # number of clusters/repesentatives
    d = 2
    pointList = createList(n, d)
    plt.scatter(*zip(*pointList), label='Input Points', s=3)

    # repList0 = withSklearn(pointList, round(n*eps))
    # repList1 = withoutSklearn(pointList, round(n*eps))
    repList2 = brakeToSections(pointList, eps)

    plt.title('Choosing representatives')
    plt.legend(loc='upper right')
    plt.show()

# segment = createRandSegment(pointList)
# print('	Segment: ', segment)
# test(pointList, repsList1, segment, eps)
# test(pointList, repsList2, segment, eps)

# plt.xlabel('thats the xlable')
# plt.ylabel('thats the ylable')
