#!/usr/bin/env python3
import copy
from builtins import int
from matplotlib import pyplot as plt
from functools import reduce
import operator
from genPoints import *
from auxClasses import *
# from auxClasses import global_bin_mul_counter


#   return the product of a group - replacement for big Pai
def prod(iterable):
    return reduce(operator.mul, iterable)
    # return reduce(operator.mul, iterable, 1)


#   cmp should return 1 if a is bigger than b, 0 if smaller or equal
#       (use case:  a*cmp(a, b) returns a only if it is bigger than b.
#       otherwise it's zero, or "empty" in case of a list/group)
def compare(a, b, current_dim=2):
    # print(a, '------------', b)

    def helib_compare(aa, bb):  # PLACEHOLDER todo remove
        # print(aa, '~~~~~~~~~~', bb)
        # print(aa.value, '~~~~~~~~~~', bb.value)
        return aa.value > bb.value
        # return aa >= bb

    a_is_bigger = 0
    b_is_bigger = 0
    # d = current_dim - 1
    for d in range(1, len(b)):
        a_is_bigger = helib_compare(a[d], b[d])
        b_is_bigger = helib_compare(b[d], a[d])
    return Bit(0 if b_is_bigger else 1)
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


# placeholder for a function that will send the client the (encrypted) (sum, size) of a group,
#   and will get in return the result of (encrypted) sum/size, with noice
#   currently returns the result of sum(cell_points)/num_points
def ask_the_client_server(group_y_sum, num_points):  # todo consider doing with *args or **kwargs
    new_rep = (Binary(round(group_y_sum[d].value / num_points.value, 2)) for d in range(len(group_y_sum)))
    return Point(*new_rep)


def get_rep_per_cell(cell_points, num_points):  # todo consider checking cell_points is not empty
    group_y_sum = [Binary(0) for _ in range(len(cell_points[0]))]
    for point_or_not in cell_points:
        for d in range(len(point_or_not)):
            print(group_y_sum[d] , point_or_not[d])
            group_y_sum[d] += point_or_not[d]  # todo use the addition of Point
    group_y_rep = ask_the_client_server(group_y_sum, num_points)
    return group_y_rep


# this is to overcome the fact that the "prod" sometimes returns 2 instead of 1
#   you can look at it as the next function: f(0) = 0; f(1) = 1 = f(2)
# def zero_or_one(param):
#     return Bit((3 * param - param * param) // 2)

# todo consider end cases, in the strip size and the number of rand_reps
def get_representatives(points, eps):
    lim = 1 / eps  # essentially this is the number of strips, or the number of cell in a strip
    strip_size = int(n / lim)  # (#points) in a strip               # strip_size = round(n * eps)
    # cell_size = int(strip_size / lim)  # (#points) in a cell       # cell_size = round(n * (eps ** 2))

    # section_end = 0  # <--	for plot purposes only
    reps = []
    reps_groups = dict()
    for i in range(int(lim)):  # strip segmentation
        strip = points[i * strip_size: (i + 1) * strip_size - 1]  # choose representatives randomly

        # section_begin = section_end  # <--	for plot purposes only
        # section_end = strip[-1][0]  # <--	for plot purposes only

        temp_strip = strip[:]
        # print('================== ', temp_strip)
        # print('================== ', len(strip))

        # rand_points = [temp_strip.pop(temp_strip.index(choice(temp_strip))) for i in range(min(len(strip), int(lim)))]
        rand_points = [temp_strip.pop(temp_strip.index(choice(temp_strip))) for i in range(int(len(strip) * eps))]
        print(rand_points)
        #   create a compare-results dict - for better performance
        cmp = create_cmp_dict(strip, rand_points)

        for Y in rand_points:
            group_y, group_y_size = [], Binary(0)
            for point in strip:
                is_point_above_smaller_reps = \
                    [cmp[point][A] * cmp[Y][A] + cmp[A][point] * cmp[A][Y] for A in rand_points]
                # [cmp[point][A] * cmp[Y][A] or cmp[A][point] * cmp[A][Y] for A in rand_points]
                # under_y_above_else = zero_or_one(prod(is_point_above_smaller_reps))
                under_y_above_else = prod(is_point_above_smaller_reps)  # todo consider eliminating the multi for effeciency
                # print(under_y_above_else)
                flag = cmp[Y][point] * under_y_above_else
                point_or_not = point * flag
                group_y.append(point_or_not)
                print(group_y_size , flag)
                group_y_size += flag
            group_y_rep = get_rep_per_cell(group_y, group_y_size)
            reps.append(group_y_rep)
            reps_groups[group_y_rep] = group_y

            # print("=======      current Y (random representative point): ", Y)
            # print("group_Y: ", group_y)
            # print("group_y_size: ", group_y_size)
            # print("group_y_Representative: ", group_y_rep)
            # # count = sum([1 for pp in group_y if pp])  # <-- for dbg purposes only
            # # print('count', count)
            # print()

            dim = 2
        #     plt.hlines(Y[dim - 1], section_begin, section_end,
        #                colors='grey', linestyle='--', linewidth=1)  # <--	for plot purposes only
        # plt.axvline(x=section_end, color='grey', linestyle='--', linewidth=1)  # <--	for plot purposes only

    leftovers = get_leftovers(reps_groups, False)
    # leftovers = get_leftovers(reps_groups, True, points, reps, eps )
    return reps, leftovers
    # return reps


# def dist_squared(point, rep):
#     squared_sum = 0
#     for d in range(len(point)):
#         squared_sum += (point[d] - rep[d]) ** 2
#     return round(squared_sum, 2)


def get_leftovers(reps_groups, real, points=[], reps=[], threshold=0):
    print("get_leftovers")
    leftovers = []
    if real:
        dist = dict()
        # building the distances dict
        # for point in points:
        #     dist[point] = {rep: dist_squared(rep, point) for rep in reps}
        # for point, dist_list in dist.items():
        #     leftovers.extend([point for p, distance in dist_list.items() if distance < threshold])

    else:
        for rep, group in reps_groups.items():
            print(rep, '----->', group)
            # print(rep, '----->', point)
            leftovers.extend([point * compare(rep, point) for point in group])
        # print(leftovers)

    leftovers = [p for p in leftovers if p]  # <--	for plot purposes only
    return leftovers


def decrypt_data(points_list, rep_list, leftovers_list):
    print('\n  ----------------- decrypted_data ----------------------  ')
    rep_set, leftovers_set, point_set = set(), set(), set()
    for p in points_list:
        temp = tuple(p[i].value for i in range(len(p)))
        point_set.add(temp)
    print(len(point_set), 'clean points ---->', point_set)
    for rep in rep_list:
        temp = tuple(rep[i].value for i in range(len(rep)))
        rep_set.add(temp)
    print(len(rep_set), 'clean reps ---->', rep_set)
    for lo in leftovers_list:
        temp = tuple(lo[i].value for i in range(len(lo)))
        leftovers_set.add(temp)
    print(len(leftovers_set), 'clean leftover ---->', leftovers_set)
    return rep_set, leftovers_set, point_set


if __name__ == "__main__":
    n = 1000
    epsilon = 1 / 9  # 0.25
    dimension = 2
    points_list = get_points(n, dimension)
    # points_list = get_points_from_file()
    points_list = gen_encrypted_points_from_file()
    # k = round(n*eps) # number of clusters/representatives

    #   the meat
    rep_list, leftovers_list = get_representatives(points_list, epsilon)
    # rep_list = get_representatives(points_list, epsilon)
    print('rep_list of size', len(rep_list), ' : ', rep_list)
    print('leftovers_list of size', len(leftovers_list), ' : ', leftovers_list)

    # ------------------------- print the points -------------------------------
    # print(len(rep_list), 'representatives ----->', rep_list)
    # print('========================')
    # print(len(leftovers_list), 'leftover points ----->', leftovers_list)
    rep_set, leftovers_set, point_set = decrypt_data(points_list, rep_list, leftovers_list)
    # rep_set, leftovers_set, point_set = decrypt_data(points_list, rep_list, [])
    # ------------------------- end of print the points -------------------------------

    #   plot
    # plt.scatter(*zip(*points_list), label='Input Points', s=3)  # <--	for plot purposes only
    plt.scatter(*zip(*point_set), label='Input Points', s=3)  # <--	for plot purposes only
    plt.scatter(*zip(*rep_set), label='Mean Representatives', c='black', s=10)  # <--	for plot purposes only
    # print(leftovers_list)
    plt.scatter(*zip(*leftovers_set), label='Leftovers', c='red', s=2)  # <--	for plot purposes only
    plt.title('Choosing representatives')  # <--	for plot purposes only
    plt.legend(loc='upper right')  # <--	for plot purposes only
    plt.show()  # <--	for plot purposes only

    # print(global_bin_mul_counter)