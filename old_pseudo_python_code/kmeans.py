#!/usr/bin/env python3
from builtins import int
from old_pseudo_python_code.genPoints import *
from old_pseudo_python_code.auxEncFuncs import *


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
def create_cmp_dict(rand_points, strip, cmp_op):  # todo should cmp for i-D
    cmp = {p: {} for p in strip}
    ctr = 0
    for rp in rand_points:  # O(1/eps)
        for p in strip:  # O(n*eps)
            cmp[rp][p] = cmp_op(rp, p)
            cmp[p][rp] = cmp_op(p, rp)
            # print('============================')
            # print(rp, p)
            # print(cmp[rp][p], cmp[p][rp])
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
        cmp = create_cmp_dict(rand_points, strip,
                              operator.gt)  # create a HElibs compare-results dict - for better performance
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
    make_plot(points, reps, None, leftovers, rands, strips)
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

        # pt, mt = threshold, 0 - threshold
        # left, down = Point(Binary(mt), Binary(0)), Point(Binary(0), Binary(mt))
        # right, up = Point(Binary(pt), Binary(0)), Point(Binary(0), Binary(pt))
        # print('rep + left:', rep + left)
        # cmp_left =  create_cmp_dict(rep + left,     group, operator.lt)
        # cmp_right = create_cmp_dict(rep + right,    group, operator.gt)
        # cmp_up =    create_cmp_dict(rep + up,       group, operator.gt)
        # cmp_down =  create_cmp_dict(rep + down,     group, operator.lt)
        # print(cmp_left)
        # print(cmp_right)
        # print(cmp_up)
        # print(cmp_down)

    leftovers = [p for p in leftovers if p]  # <--	for plot purposes only
    return leftovers


if __name__ == "__main__":
    n = 1000
    epsilon = 1 / 9  # 0.25
    # k = round(n * epsilon) # number of clusters/representatives
    dimension = 2
    # points_list = get_points(n, dimension)
    # points_list = get_points_from_file()
    # points_list = gen_encrypted_points_from_file("points")

    # rep_list, leftovers_list = get_representatives(points_list, epsilon)

    # rep_list = get_representatives(points_list, epsilon)


    # points_list = gen_encrypted_points_from_file("points")
    points_list  = gen_encrypted_points_from_file("/home/rbd/workspace/rbd/rbd_helib_with_remote_debugger/points")
    means        = gen_encrypted_points_from_file("/home/rbd/workspace/rbd/rbd_helib_with_remote_debugger/means")
    means_test        = gen_encrypted_points_from_file("/home/rbd/workspace/rbd/rbd_helib_with_remote_debugger/means_test")
    chosen       = gen_encrypted_points_from_file("/home/rbd/workspace/rbd/rbd_helib_with_remote_debugger/chosen")
    leftover     = gen_encrypted_points_from_file("/home/rbd/workspace/rbd/rbd_helib_with_remote_debugger/leftover")
    # chosen       = [Point(Binary(0), Binary(0))]
    # leftover     = [Point(Binary(0), Binary(0))]
    # print(points_list)
    # points_list = decrypt_data(points_list)
    # chosen = decrypt_data(chosen)
    # leftover = decrypt_data(leftover)

    print(points_list)
    print(means)
    print(chosen)
    print(leftover)

    make_plot(points_list, means_test, chosen, leftover)


