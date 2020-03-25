#!/usr/bin/env python3

from random import *

from matplotlib import pyplot as plt
from numpy import *

from pies.quickSelect import median


# from pies.quick_select import median


# ------------------------------------------------------------------------
# 	The following functions - createList, create_rand_segment, isPointIn_seg, test - 
# 	are mainly for testing purposes 
# ------------------------------------------------------------------------

def createList(list_size, d=2):  # todo remove; temporary list creation
    list_org = []
    for i in range(list_size):
        # tempTup = [round(10 * random(), 2) for i in range(d)]
        tempTup = [round(100 * random(), 2) for i in range(d)]
        list_org.append(tempTup)
    list_org.sort(key=lambda tup: tup[0])
    return list_org


# ========================================================================
# ========================================================================
# Getting _reps - T_oD_o elaborate
# ========================================================================
# ========================================================================

# Implantation of the standard alg, which uses an iterative refinement technique.
# --receives:	list_org - (List) dataset
# 				k - (int) number of representatives/clusters
# --returns: 	(List) k chosen representatives
def get_repPerCell(list_org, k=1):
    return median(list_org)


# Implantation of of what I think is the problem given by Danny
# Choosing a representative from each section using the standard k-means alg.
#
# --receives:	point_list - (List) dataset
# 				eps -	(float) the margin of error. Also deferments the
# 						number of sections into which the points are divided.
# --returns: 	(List) k chosen representatives
def get_representatives(points_list, eps):
    n = len(points_list)
    strip_size = round(n * eps)
    cell_size = round(n * eps ** 2)
    section_end = 0
    strips = []
    reps = []
    # cut the into "strips"
    for i in range(round(1 // eps)):
        curr_strip = points_list[i * strip_size:(i + 1) * strip_size - 1]
        strips.append(curr_strip)
        section_begin = section_end  # <--	for plot purposes only
        section_end = strips[-1][-1][0]  # <--	for plot purposes only
        plt.axvline(x=section_end, color='grey', linestyle='--', linewidth=1)  # <--	for plot purposes only
        strips[-1].sort(key=lambda x: x[1])
        cells = []
        # cut each strip into "cells"
        for j in range(round(1 // eps) + 1):
            currCell = strips[-1][j * cell_size:(j + 1) * cell_size - 1]
            if ([] != currCell):
                # choose a representative from each section and add to
                reps.append(get_repPerCell(currCell)[0])
                cells.append(currCell)
                plt.hlines(currCell[-1][1], section_begin, section_end,
                           colors='grey', linestyle='--', linewidth=1)  # <--	for plot purposes only
    plt.scatter(*zip(*reps), label='_by _sections', c='black', s=10)  # <--	for plot purposes only
    # for i_x, i_y in reps:
    #     plt.text(i_x, i_y, '({}, {})'.format(i_x, i_y)) # <--	for plot purposes only, print the coor's
    return reps


if __name__ == "__main__":
    # configure the params
    eps = 1 / 9
    n = 600
    # k = round(n*eps) # number of clusters/representatives
    d = 2
    point_list = createList(n, d)
    plt.scatter(*zip(*point_list), label='Input Points', s=3)

    repList = get_representatives(point_list, eps)

    plt.title('Choosing representatives')
    plt.legend(loc='upper right')
    plt.show()

# plt.xlabel('thats the xlable')
# plt.ylabel('thats the ylable')
