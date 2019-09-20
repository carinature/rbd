#!/usr/bin/env python3

from numpy import *
from random import *
from sklearn.cluster import KMeans
from matplotlib import pyplot as plt
import sys

# Cluster data structure consisting of a list of points and a center/mean, 
# 	which might not be up to date. Use cluster.updateCenter() for that purpose.
# 	(for efficiency, the cluster does not update the center every time a point
# 	is added. In a general case should be accurate enough)
class Cluster:
	center = 0
	pointsList = []
	# numOfPoints = 0
	def __init__(self, center, pointsList): # todo maybe no need for empty C'tor and thus 'self' arg
		self.center = center
		self.pointsList = pointsList
	def __repr__(self):
		return "Cluster()\t == center: " + str(self.center) 			\
		 + " == \n\t\tnumOfPoints: --" + str(len(self.pointsList)) 		\
		 + "-- \n\t\tpoints: " + str(self.pointsList)
	def __str__(self):
		return "Cluster()\t == center: " + str(self.center) 			\
		 + " ==\n\t\tnumOfPoints: --" + str(len(self.pointsList)) 		\
		 + "-- \n\t\tpoints: " + str(self.pointsList)

	def updateCenter(self): # todo - consider using median instead of mean. in that case need to sort by Y as well as X (in 2D)
		sumPoints = [0,0]
		for p in self.pointsList:
			sumPoints[0]+=p[0]
			sumPoints[1]+=p[1]
		self.center = (round(sumPoints[0]/len(self.pointsList),2), 		\
		               round(sumPoints[1]/len(self.pointsList),2))
		return 

	def addPoint(self,point):
		self.pointsList.append(point)
		# numOfPoints+=1
		return

	def removePoint(self,point):
		self.pointsList.pop(self.pointsList.index(point))
		# numOfPoints-=1
		return

# ========================================================================
#	Here are the 3 steps of k-means standard algorithm, 
#	according to wikipedia - Initialization, Assignment step, Update step
# 	combined in the getRepresent() function
# ========================================================================

	# Initialize the centers' list; implements the Forgy Method
	# The Forgy method randomly chooses k observations from the dataset 
	#	and uses these as the initial means.
	# --recieves:	listOrg - (List) dataset
	#				k - (int) number of representatives/clusters
	# --returns: (List) k randomly chosen representatives
def initial(listOrg, k):
	print("Init - Randomly choose the first k representatives")
	temp = listOrg[:]
	repList = [temp.pop(temp.index(choice(temp))) for i in range(k)]
	repList.sort(key=lambda tup: tup[0]) # todo maybe don't need 'key=lambda tup: tup[1]'
	return repList

	# Assign each observation to the cluster 
	# 	whose mean has the least squared Euclidean distance
	# --recieve:	listOrg - (List) dataset
	# 				repList - (List) current cluster "centers"/ new reps 
	# --return:	(Dictionary) all the new clusters maped to their current centers/reps 
def assign(listOrg, repList):
	print("Assign - Cluster the points to their nearest representative")
	clusters = {}
	for rep in repList:
		clusters[rep] = Cluster(rep, [])
	for point in listOrg:
		dist = sys.maxsize-1
		rep = None
		for r in repList:
			d = distance(point,r)
			if dist>d :
				dist,rep = d,r
		clusters[rep].addPoint(point) 
	return clusters

	# Calculate the [Euclidean distance] between 2 points.
	# --recieve:	p1, p2 - (Tuple) points
	#				d - (int) dimantion [optional]
def distance(p1,p2,d=2):
	sqrSum = 0
	for i in range(d):
		sqrSum += (p1[i]-p2[i])**2
	return round(sqrt(sqrSum), 2)

	# Calculate and Update the new means (centroids) in the new clusters. 
	# --recieve:	clusters - (Dictionary) all the clusters
	# --return:		(List) all the new centers/reps
def update(clusters): 		
	print("Update - Recalculate new (better) representatives")
	for rep,c in clusters.items():
		c.updateCenter()
	repList = [c.center for c in clusters.values()] # todo - consider deleting the clusters
	return repList

	# Implentation of the standard alg, which uses an iterative refinement technique.
	# --recieves:	listOrg - (List) dataset
	#				k - (int) number of representatives/clusters
	# --returns: (List) k chosen representatives
def getRepresent(listOrg, k):
	repList = initial(listOrg, k)
	print("repList - INIT: ", repList)
	orgList	= []
	i = 0
	# rinse and repeat until there is no more place for refinement
	while orgList != repList:
		i+=1
		clusters = assign(listOrg, repList)
		for rep,c in clusters.items():
			print(c)	
		orgList = repList
		repList = update(clusters)
	return repList


def withoutSklearn(pointList, k):
	repsList = getRepresent(pointList, k)
	print("Original Point List: ", pointList)
	print("Representatives List: ", repsList)


def withSklearn(pointList, k):
	f1 = [p[0] for p in pointList]
	f2 = [p[1] for p in pointList]
	X = array(list(zip(f1, f2)))
	# 
	plt.scatter(f1, f2, c='black', s=7)
	# 
	# Number of clusters
	kmeans = KMeans(n_clusters=k)
	# Fitting the input data
	kmeans = kmeans.fit(X)
	# Getting the cluster labels
	labels = kmeans.predict(X)
	# Centroid values
	centroids = kmeans.cluster_centers_
	centroids.sort()
	print(centroids)
	print(len(centroids))

	# repsList = getRepresent(pointList, k)
	# print("Original Point List: ", pointList)
	# print("Representatives List: ", repsList)


# ========================================================================
#	The following functions - createList, getRandSegment, isPointInSeg, test - 
# 	are mainly for testing purposes 
# ========================================================================

def createList(listSize, d=2): # todo remove; temporary list creation
	listOrg = []
	for i in range(listSize):
		tempTup = ()
		for j in range(d):
			tempTup = tempTup + (round(10*random(),2), )
		listOrg.append(tempTup)
	listOrg.sort(key=lambda tup: tup[0])
	return listOrg
	
	# choose a random segment/rectangle/cube...
def getRandSegment(listOrg, d=2): # todo - make a rectangle
	lowBound = ()
	for i in range(d):
		lowBound = lowBound + (listOrg[0][i], )
	upBound = ()
	for i in range(d):
		upBound = upBound + (listOrg[len(listOrg)-1][i], )
	segment = ()
	for i in range(d):
		segment = segment + 		\
			(add(round(distance(upBound,lowBound,d)*random(),2), lowBound), )
	# todo - find a rectangle
	return [(round(min(segment[0]),2), round(min(segment[1]), 2)), 		\
	(round(max(segment[0]),2), round(max(segment[1]), 2))]

def isPointInSeg(segment, p): # todo - generalize for d dimantions
	lowBound, upBound = segment[0], segment[1]
	if (p[0] > lowBound[0] and p[0] < upBound[0] and 		\
	    p[1] > lowBound[1] and p[1] < upBound[1]) : 
		# print("TRUE")
		return True
	else : 
		return False

def test(listOrg, repList, segment, eps):
	orgCovered = list(filter(lambda p: isPointInSeg(segment, p), listOrg)) 
	print("	OrgCovered: ", orgCovered)
	print("	OrgCovered length: ", len(orgCovered))
	repCovered = list(filter(lambda p: isPointInSeg(segment, p), repList))
	print("	RepCovered: ", repCovered if repCovered is not None else [])
	print("	RepCovered length: ", len(repCovered))



if __name__ == "__main__":
    # configure the params
	eps = 1/3
	listSize = 45
	k = int(listSize*eps) # number of clusters/repesentatives
	d = 2
	pointList = createList(listSize, d)
	repsList1 = withoutSklearn(pointList, k)
	repsList2 = withSklearn(pointList, k)

	# segment = getRandSegment(listOrg)
	# print('	Segment: ', segment)
	# test(pointList, repsList1, segment, eps)
	# test(pointList, repsList1, segment, eps)
