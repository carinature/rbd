#!/usr/bin/env python3
from random import *

dim = 2  # dimensions
numPoints = 0
rangeLim = 0
bottomLim = 0
with open('../impl/properties.h') as f:
    datafile = f.readlines()
    for line in datafile:
        if 'NUM_POINTS' in line:
            numPoints = int(line.split(' ')[2])
        if 'DIM' in line:
            dim = int(line.split(' ')[2])
        if 'RANGE_LIM' in line:
            rangeLim = int(line.split(' ')[2])
        if 'Bottom_LIM' in line:
            bottomLim = int(line.split(' ')[2])

pointsList = []

# f = open("/home/rbd/workspace/rbd/rbd_helib_with_remote_debugger/io/points", "w")
file = "/home/rbd/workspace/rbd/rbd_helib_with_remote_debugger/io/points"
filecopy = "/home/rbd/workspace/rbd/rbd_helib_with_remote_debugger/io/points_copy"


# for i in range(numPoints):
# 	tempTup = [round(10*random(), 2) for _ in range(d)]
# 	# tempTup = (round(10*random(),2), )
# 	# for _ in range(d):
# 	# 	tempTup += (round(10*random(),2), )
# 	pointsList.append(tempTup)
# pointsList.sort(key=lambda tup: tup[0])
#
# # f.write('%d\n' %(numPoints))
#
# # for i in range(numPoints):
# # 	for j in range(d-1):
# # 		f.write('%.2f ' % ( pointsList[i][j]))
# # 	f.write('%.2f\n' % ( pointsList[i][d-1]))
#
# for point in pointsList:
# 	f.write(f' '.join(map(str, point)))
# 	f.write('\n')
#
# f.close()

# print(pointsList)

def rand(bottom_lim=bottomLim, range_lim=rangeLim, round_lim=2):
    return round(range_lim * random() + bottom_lim, round_lim)


def generate_points(n=numPoints, d=dim):
    points = [tuple(rand() for _ in range(d)) for _ in range(n)]
    points.sort()
    return points


def create_points_file(points):
    with open(file, 'w') as f:
        # f.write('%d\n' % len(points)  # for c++ compatibility
        for point in points:
            f.write(f' '.join(map(str, point)))
            f.write('\n')
    # for integration with yoins code
    from shutil import copyfile
    copyfile(file, filecopy)


create_points_file(generate_points())