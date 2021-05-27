#!/usr/bin/env python3
from random import *

DIM = 2  # dimensions
NUM_POINTS = 0
RANGE_LIM = 0
Bottom_LIM = 0

points_file = None
points_copy_file = None

with open('impl/properties.h') as f:
    for line in f.readlines():
        if '#define' in line and not ('TRY_PROPERTIES_H' in line or 'CONVERSION_FACTOR' in line):
            key = line.split(' ')[1].strip('\'')
            value = line.split(' ')[2].strip(' \n\'') if 'file' in line else float(line.split(' ')[2])
            exec(f'{key} = {value}')


pointsList = []

# f = open(points_file, "w")

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

def rand(bottom_lim=Bottom_LIM, range_lim=RANGE_LIM, round_lim=2):
    return round(range_lim * random() + bottom_lim, round_lim)


def generate_points(n:int=NUM_POINTS, d:int=DIM):
    points = [tuple(rand() for _ in range(int(d))) for _ in range(int(n))]
    points.sort()
    return points


def create_points_file(points):
    with open(points_file, 'w') as f:
        # f.write('%d\n' % len(points)  # for c++ compatibility
        for point in points:
            f.write(f' '.join(map(str, point)))
            f.write('\n')
    # for integration with yonis code
    from shutil import copyfile
    copyfile(points_file, points_copy_file)


create_points_file(generate_points())
