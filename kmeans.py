#!/usr/bin/env python3
import copy
from builtins import int
from matplotlib import pyplot as plt
from genPoints import *
from auxClasses import *
from auxEncFuncs import *
# from auxClasses import global_bin_mul_counter

# todo:
#  -1-  consider eliminating the multi for efficiency - create another dict ?
#     in this line :  is_above_smaller_reps = prod([cmp[p][r] * cmp[R][r] + cmp[r][p] * cmp[r][R] for r in rand_points])
#  -2-  fix end cases, in the strip size and the number of rand_reps
#  -3-  maybe use type hinting ?


#   create_cmp_dict is used for efficiency and readability.
#       It creates a dictionary containing the result of calculating HELIB comparison
#       cmp[point1][point2] = MU, where MU=(p1>p2) is the value returned in
#       compareTwoNumbers(MU, ni, CtPtrs_VecCt(enca), CtPtrs_VecCt(encb),
#                               &unpackSlotEncoding);
def create_cmp_dict(strip, rand_points):  # todo should cmp for i-D
    cmp = {p: {} for p in strip}
    ctr = 0
    for rp in rand_points:  # O(1/eps)
        for p in strip:  # O(n*eps)
            cmp[rp][p] = rp > p
            cmp[p][rp] = p > rp
            # cmp[p][rp] = Bit(1) - cmp[rp][p]
        # cmp[point] = {point2: compare(point, point2) for point2 in strip}
        ctr += len(cmp[rp])
        print(ctr)
    print('======== ', ctr)
    return cmp


def get_rep_per_cell(cell_points, num_points):
    sum_point = point_zero(len(cell_points[0]))
    for point_or_not in cell_points:
        sum_point = sum_point + point_or_not
    group_y_rep = ask_the_client_server(sum_point, num_points)
    return group_y_rep


def get_representatives(points, eps):
    strip_size = round(n * eps)  # strip_size = int(n * eps)  # (#points) in a strip
    # cell_size = int(strip_size * eps * eps)  # (#points) in a cell       # cell_size = round(n * (eps ** 2))
    rands = []  # <--	for plot purposes only
    strips = [points[i * strip_size: (i + 1) * strip_size - 1] for i in range(int(1 / eps))]
    reps, reps_groups = [], dict()
    # for i in range(int(1 / eps)):  # strip segmentation
    #     strip = points[i * strip_size: (i + 1) * strip_size - 1]  # choose representatives randomly
    for strip in strips:
        temp_strip = strip[:]
        rand_points = [temp_strip.pop(temp_strip.index(choice(temp_strip))) for i in range(int(len(strip) * eps))]
        # rand_points = [temp_strip.pop(temp_strip.index(choice(temp_strip))) for i in range(min(len(strip), int(lim)))]
        cmp = create_cmp_dict(strip, rand_points)  # create a HElibs compare-results dict - for better performance
        for R in rand_points:
            group_y, group_y_size = [], Binary(0)
            for p in strip:
                is_above_smaller_reps = prod(
                    [cmp[p][r] * cmp[R][r] + cmp[r][p] * cmp[r][R] for r in rand_points])  # O(1/eps)
                is_in_brick = cmp[R][p] * is_above_smaller_reps  # Bit
                group_y.append(p * is_in_brick)  # group of Points
                group_y_size += is_in_brick  # Binary
            group_y_rep = get_rep_per_cell(group_y, group_y_size)
            reps.append(group_y_rep)
            reps_groups[group_y_rep] = group_y
            # leftovers = get_leftovers(rep, group, eps)
        rands.append(rand_points)  # <--	for plot purposes only
    leftovers = get_leftovers(reps_groups, eps)  # todo move the get_leftovers to the inner loop
    # leftovers = get_leftovers(reps_groups, True, points, reps, eps )
    make_plot(strips, rands, points, reps, leftovers)
    return reps, leftovers
    # return reps


def get_leftovers(reps_groups, threshold=0):
    print("get_leftovers")
    leftovers = []
    for rep, group in reps_groups.items():
        # print(rep, '----->', group)
        # print(rep, '----->', point)
        leftovers.extend([point * (rep > point) for point in group])
        # leftovers.extend([point * compare(rep, point) for point in group])
    # print(leftovers)

    leftovers = [p for p in leftovers if p]  # <--	for plot purposes only
    return leftovers


if __name__ == "__main__":
    n = 1000
    epsilon = 1 / 9  # 0.25
    # k = round(n * epsilon) # number of clusters/representatives
    dimension = 2
    # points_list = get_points(n, dimension)
    # points_list = get_points_from_file()
    points_list = gen_encrypted_points_from_file()

    rep_list, leftovers_list = get_representatives(points_list, epsilon)
    # rep_list = get_representatives(points_list, epsilon)
