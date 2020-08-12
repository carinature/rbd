import os

import numpy as np
from matplotlib import pyplot as plt


def gen_encrypted_points_from_file(fileName):
    # with open(fileName) as f:
    # points = [Point(*map(Binary, map(float, i.strip(' \n').split(' ')))) for i in f]  # todo encapsulate in Binary
    points = np.loadtxt(fileName, dtype=float)
    points = np.reshape(points, (-1, 2))
    print(points)
    return points


def decrypt_data(points):  # , rep_list=None, leftovers_list=None):
    print('  ----------------- decrypted_data ----------------------  ')
    point_set = set()
    for p in points:
        temp = tuple(p[i] for i in range(len(p)))
        point_set.add(temp)
    return point_set


def plot_bricks(fig, axs, strips, bricks):
    for ax in axs.flat:
        section_end = 0
        dim = 2
        for i in range(len(strips)):
            # print("--------------------------")
            # print("i: ", i)
            # print("strips[i]: ", strips[i])
            # print("rands[i]: ", bricks[i])
            # print("strips[i][-1]: ", strips[i][-1])
            # print("--------------------------")
            section_begin = section_end
            section_end = strips[i][-1][0]  # section_end = strips[i][-1][0].value
            for Y in bricks:
                x_val = Y[0]
                if section_begin <= x_val and section_end > x_val:
                    ax.hlines(Y[dim - 1], section_begin, section_end, colors='grey', linestyle='--', linewidth=0.3)
            ax.axvline(x=section_end, color='grey', linestyle='--', linewidth=0.3)


def make_plot_with_bricks(point_list,
                          rands_list, means_list,
                          chosen_list=None, leftovers_list=None,
                          strips=None, bricks=None):
    fig, axs = plt.subplots(2, 2)
    plot_bricks(fig, axs, strips, bricks)
    make_plot(fig, axs, point_list, rands_list, means_list, chosen_list)  # , chosen=None, leftovers_list=None)


def make_plot(fig, axs, point_list, rands_list, means_list, chosen=None, leftovers_list=None):
    point_set = decrypt_data(point_list)
    rands_set = decrypt_data(rands_list)
    means_set = decrypt_data(means_list)
    chosen_set = decrypt_data(chosen)
    print('rep list of size  __', len(means_list), '__  : ', means_list)
    # print('leftovers list of size  __', len(leftovers_list), '__  : ', leftovers_list)
    # print('REAL leftovers list of size  __', len(leftovers_set), '__  : ', leftovers_set)

    # fig, axs = plt.subplots(2, 2)
    axs[0, 0].scatter(*zip(*point_set), label='Input Points', c='green', s=4)
    axs[0, 0].set_title('Input Points')
    axs[0, 1].scatter(*zip(*point_set), label='Input Points', c='green', s=4)
    axs[0, 1].scatter(*zip(*rands_set), label='Rand Points', c='blue', s=6)
    axs[0, 1].set_title('Rand Points')
    axs[1, 0].scatter(*zip(*point_set), label='Input Points', c='green', s=4)
    axs[1, 0].scatter(*zip(*rands_set), label='Rand Points', c='blue', s=6)
    sc = axs[1, 0].scatter(*zip(*means_set), label='means', c='black', s=20)
    i = 0
    # for i in range(len(means_set)):
    #     axs[1, 0].text(rands_list[i][0] + 0.001, rands_list[i][1] + 0.001, str(i))
    #     axs[1, 0].text(means_list[i][0] + 0.001, means_list[i][1] + 0.001, str(i))
    #     i += 1
    axs[1, 0].set_title('means')
    axs[1, 1].scatter(*zip(*point_set), label='Input Points', c='green', s=4)
    axs[1, 1].scatter(*zip(*rands_set), label='Rand Points', c='blue', s=6)
    axs[1, 1].scatter(*zip(*means_set), label='means', c='black', s=20)
    axs[1, 1].scatter(*zip(*chosen_set), label='chosen', c='pink', s=8)
    axs[1, 1].set_title('chosen')

    for ax in axs.flat:
        ax.set(xlabel='x-label', ylabel='y-label')

    # Hide x labels and tick labels for top plots and y ticks for right plots.
    for ax in axs.flat:
        ax.label_outer()

    fig.suptitle('Choosing representatives')
    fig.legend(loc='upper right')
    plt.show()
    fname = 'check_save.png'
    plt.savefig(fname)
    print(fname)
    print('----------------------------------')

    if os.path.exists(fname):
        print(os.path.abspath(fname))

dim = 2  # dimensions
numPoints = 0
rangeLim = 0
epsilon = 0
with open('../impl/properties.h') as f:
    datafile = f.readlines()
    for line in datafile:
        if 'NUM_POINTS' in line:
            numPoints = int(line.split(' ')[2])
        if 'DIM' in line:
            dim = int(line.split(' ')[2])
        if 'RANGE_LIM' in line:
            rangeLim = int(line.split(' ')[2])
        if 'EPSILON' in line:
            epsilon = float(line.split(' ')[2])

if '__main__' == __name__:

    # points_list = gen_encrypted_points_from_file("points")
    points_list = gen_encrypted_points_from_file(points_copy)
    rands = gen_encrypted_points_from_file(rand_means_file)
    means = gen_encrypted_points_from_file(means_file)
    chosen = gen_encrypted_points_from_file(chosen_file)
    # leftover   = gen_encrypted_points_from_file(leftover_file)

    if numPoints != points_list.size / dim:  # sanity check
        print(numPoints != points_list.size / dim)
        print(numPoints)
        print(points_list.size)
        raise Exception("NOT SYNCED!!!")

    numStrips = int(1 / epsilon)
    lacks = (numStrips - int(numPoints * dim % numStrips)) % numStrips
    buffer = np.zeros(lacks, dtype=float)

    fns = points_file
    points = np.loadtxt(fns, dtype=float)
    points = np.append(points, buffer)
    strips = np.resize(points, (numStrips, -1, 2))
    # strips = np.reshape(points, (int(1 / epsilon), -1, 2))
    print("strips\n", strips)
    fnr = rand_means_file
    bricks = np.loadtxt(fnr, dtype=float)
    # np.append(bricks, buffer)
    # bricks = np.resize(bricks, (numStrips, -1, 2))
    # rands = np.reshape(rands, (int(1 / epsilon), -1, 2))
    print("\nbricks\n", bricks)


    # make_plot(points_list, rands, means, chosen)
    make_plot_with_bricks(points_list, rands, means, chosen, strips=strips, bricks=bricks)
    # make_plot(points_list, means, chosen, leftover)

# chosen=[]
# points = np.loadtxt('io/chosen', dtype=float)
# points = np.reshape(points, (-1, 2))
# for p in points:
#     print(p[0])
#     print(p[0]==0)
#     if 0==p[0] and 0==p[1]:
#         chosen.append(p)
#         print(len(chosen))



