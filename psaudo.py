#!/usr/bin/env python3
from builtins import int
from matplotlib import pyplot as plt
from functools import reduce
import operator
from genPoints import *


# todo need to implement a class for Strips - picking random points
# todo need to implement a class for Cells
# todo implement for dimensions other than 2D. implement compare for i-D


#   return the product of a group - replacement for big Pai
def prod(iterable):
    return reduce(operator.mul, iterable)
    # return reduce(operator.mul, iterable, 1)


#   cmp should return 1 if a is bigger than b, 0 if smaller or equal
#       (use case:  a*cmp(a, b) returns a only if it is bigger than b.
#       otherwise it's zero, or "empty" in case of a list/group)
def compare(a, b, dim=2):
    def helib_compare(aa, bb):  # PLACEHOLDER todo remove
        return aa > bb
        # return aa >= bb

    d = dim - 1
    a_is_bigger = helib_compare(a[d], b[d])
    b_is_bigger = helib_compare(b[d], a[d])
    return 0 if b_is_bigger else 1
    # return 1 if a_is_bigger else 0  # todo this answer yields empty groups -> group_size=0 -> division by zero
    # todo consult danny/adi


#   create_cmp_dict is used for efficiency and readability.
#       It creates a dictionary containing the result of calculating HELIB comparison
#       cmp[point1][point2] = MU, where MU=(p1>p2) is the value returned in
#       compareTwoNumbers(MU, ni, CtPtrs_VecCt(enca), CtPtrs_VecCt(encb),
#                               &unpackSlotEncoding);
def create_cmp_dict(strip, rand_points):  # todo should cmp for i-D
    cmp = {}
    for point in strip:  # todo consider changing into random)Reps only, for efficiency
        cmp[point] = {point2: compare(point, point2) for point2 in strip}
    return cmp


def get_rep_per_cell(cell_points, num_points, dim=2, k=1):
    if 0 == num_points:
        return  # [-1 for _ in range(dim) ] # 'No Rep for this group'
    group_y_sum = [0 for _ in range(dim)]
    for point_or_not in cell_points:
        for d in range(len(point_or_not)):
            group_y_sum[d] += point_or_not[d]
    group_y_rep = tuple(round(group_y_sum[d] / num_points, 2) for d in range(dim))
    return group_y_rep


def get_representatives(points, eps, dim=2):
    lim = 1 / eps  # essentially this is the number of strips, or the number of cell in a strip
    strip_size = int(n / lim)  # (#points) in a strip               # strip_size = round(n * eps)
    # cell_size = int(strip_size / lim)  # (#points) in a cell       # cell_size = round(n * (eps ** 2))
    # print("number of strips - ", int(lim)) print("number of cells - ", int(lim ** 2)) print("strip_size - ", strip_size) print("cell_size - ", cell_size) print()

    section_end = 0  # <--	for plot purposes only
    reps = []
    for i in range(int(lim)):  # strip segmentation
        strip = points[i * strip_size: (i + 1) * strip_size]  # choose representatives randomly

        section_begin = section_end  # <--	for plot purposes only
        section_end = strip[-1][0]      # <--	for plot purposes only

        temp_strip = strip[:]
        rand_points = [temp_strip.pop(temp_strip.index(choice(temp_strip))) for i in range(int(lim))]
        #   create a compare-results dict - for better performance
        cmp = create_cmp_dict(strip, rand_points)
        print(cmp)
        for Y in rand_points:
            group_y, group_y_size = [], 0
            for point in strip:
                is_point_above_smaller_reps = \
                    [cmp[point][A] * cmp[Y][A] or cmp[A][point] * cmp[A][Y] for A in rand_points]
                    # [cmp[point][A] * cmp[Y][A] + cmp[A][point] * cmp[A][Y] for A in rand_points]
                    # todo consider the addition returns 2, instead of 1 or 0, when the point is at the same level (has same value of y)
                    #   should as danny/adi what is preferable
                product = prod(is_point_above_smaller_reps)
                under_y_above_else = product
                # under_y_above_else = (1 if product else 0)
                point_or_not = point * cmp[Y][point] * under_y_above_else
                group_y.append(point_or_not)
                group_y_size += cmp[Y][point] * under_y_above_else
            group_y_rep = get_rep_per_cell(group_y, group_y_size, dim)
            reps.append(group_y_rep)

            print("=======      current Y (random representative point): ", Y)
            print("group_Y: ", group_y)
            print("group_y_size: ", group_y_size)
            print("group_y_Rep: ", group_y_rep)
            count = sum([1 for pp in group_y if pp])
            print('count', count)
            print()

            plt.hlines(Y[dim - 1], section_begin, section_end,
                       colors='grey', linestyle='--', linewidth=1)

        plt.axvline(x=section_end, color='grey', linestyle='--', linewidth=1)  # <--	for plot purposes only

    print(reps)
    return reps


if __name__ == "__main__":
    n = 600
    epsilon = 1 / 9  # 0.25
    dimension = 2
    points = get_points(n, dimension)
    # points = get_points_from_file()
    # k = round(n*eps) # number of clusters/representatives

    #   the meat
    repList = get_representatives(points, epsilon, dimension)

    #   plot
    plt.scatter(*zip(*points), label='Input Points', s=3)  # <--	for plot purposes only
    plt.scatter(*zip(*repList), label='Median Representatives', c='black', s=10)  # <--	for plot purposes only
    plt.title('Choosing representatives')  # <--	for plot purposes only
    plt.legend(loc='upper right')  # <--	for plot purposes only
    plt.show()  # <--	for plot purposes only

#  (no "if")
