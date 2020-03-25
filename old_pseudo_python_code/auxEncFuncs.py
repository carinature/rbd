#!/usr/bin/env python3

from functools import reduce
import operator
from old_pseudo_python_code.auxClasses import *
from matplotlib import pyplot as plt


# from genPoints import *


#   return the product of a group - replacement for big Pai
def prod(iterable):
    # print("\n ------------------------- \n iterable")
    # print(iterable)
    product = reduce(operator.mul, iterable)
    # if 1 == product.value:
    #      print('1 == product.value')
    return product
    # return reduce(operator.mul, iterable, 1)


# placeholder for a function that will send the client the (encrypted) (sum, size) of a group,
#   and will get in return the result of (encrypted) sum/size, with noice
#   currently returns the result of sum(cell_points)/num_points
def ask_the_client_server(group_y_sum, num_points):
    new_rep = (Binary(round(group_y_sum[d].value / num_points.value, 2)) for d in range(len(group_y_sum)))
    return Point(*new_rep)


def decrypt_data(points, rep_list=None, leftovers_list=None):
    print('  ----------------- decrypted_data ----------------------  ')
    point_set = set()
    for p in points:
        temp = tuple(p[i].value for i in range(len(p)))
        point_set.add(temp)
    return point_set


def plot_bricks(strips, rands):
    section_end = 0  # <--	for plot purposes only
    dim = 2
    for i in range(len(strips)):
        section_begin = section_end  # <--	for plot purposes only
        section_end = strips[i][-1][0].value  # <--	for plot purposes only
        for Y in rands[i]:
            plt.hlines(Y[dim - 1].value, section_begin, section_end,
                       colors='grey', linestyle='--', linewidth=1)  # <--	for plot purposes only
        plt.axvline(x=section_end, color='grey', linestyle='--', linewidth=1)  # <--	for plot purposes only


def make_plot(point_list, rep_list, chosen, leftovers_list, rands=None, strips=None):
    if rands and strips:
        plot_bricks(strips, rands)
    rep_set = decrypt_data(rep_list)
    leftovers_set = decrypt_data(leftovers_list)
    point_set = decrypt_data(point_list)
    chosen_set = decrypt_data(chosen)
    print('rep list of size  __', len(rep_list), '__  : ', rep_list)
    print('leftovers list of size  __', len(leftovers_list), '__  : ', leftovers_list)
    print('REAL leftovers list of size  __', len(leftovers_set), '__  : ', leftovers_set)

    #   plot
    plt.scatter(*zip(*point_set), label='Input Points', c='blue', s=1)  # <--	for plot purposes only
    # plt.scatter(*zip(*rep_set), label='Mean Representatives', c='black', s=10)  # <--	for plot purposes only
    # plt.scatter(*zip(*leftovers_set), label='Leftovers', c='red', s=2)  # <--	for plot purposes only

    # chosen = gen_encrypted_points_from_file("/home/rbd/workspace/rbd/rbd_helib_with_remote_debugger/chosen")
    # leftover = gen_encrypted_points_from_file("/home/rbd/workspace/rbd/rbd_helib_with_remote_debugger/leftover")
    plt.scatter(*zip(*rep_set), label='means', c='black', s=7)  # <--	for plot purposes only
    plt.scatter(*zip(*chosen_set), label='chosen', c='green', s=2)  # <--	for plot purposes only
    plt.scatter(*zip(*leftovers_set), label='leftover', c='red', s=2)  # <--	for plot purposes only

    plt.title('Choosing representatives')  # <--	for plot purposes only
    plt.legend(loc='upper right')  # <--	for plot purposes only
    plt.show()  # <--	for plot purposes only


if '__main__' == __name__:
    pass
