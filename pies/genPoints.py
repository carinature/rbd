#!/usr/bin/env python3
from random import *
d = 2; #	#dimansions
numPoints = 11;
pointsList = []

f = open("/home/rbd/workspace/rbd/rbd_helib_with_remote_debugger/points","w")

for i in range(numPoints):
	# tempTup = [round(10*random(),2) for i in range(d)]
	tempTup = (round(10*random(),2), round(10*random(),2))
	pointsList.append(tempTup)
pointsList.sort(key=lambda tup: tup[0])
f.write('%d\n' %(numPoints))
for i in range(numPoints):
	f.write('%.2f , %.2f\n' % ( pointsList[i][0], pointsList[i][1]))

f.close()

# print(pointsList)
