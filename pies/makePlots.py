from matplotlib import pyplot as plt
import numpy as np

def gen_encrypted_points_from_file(fileName):
    # with open(fileName) as f:
        # points = [Point(*map(Binary, map(float, i.split(', ')))) for i in f]  # todo encapsulate in Binary
        # points = [Point(*map(Binary, map(float, i.strip(' \n').split(' ')))) for i in f]  # todo encapsulate in Binary
        # points = [map(float, i.strip(' \n').split(' ')) for i in f]  # todo encapsulate in Binary
        # points = [map([] , map(float, i.strip(' \n').split(' '))) for i in f]  # todo encapsulate in Binary
        # points = []
        # for row in f:
        #     split = row.strip('\n').split(' ')
        #     print(split)
        #     points.append(np.array(split, dtype=np.float))
        # points = np.array(points)
    points = np.loadtxt(fileName, dtype=float)
    points = np.reshape(points, (-1,2))
    print(points)

        # points = [i.strip(' \n').split(' ') for i in f]  # todo encapsulate in Binary
        # points = [i.split(' ') for i in f]  # todo encapsulate in Binary
    # print(points)
    return points

def decrypt_data(points, rep_list=None, leftovers_list=None):
    print('  ----------------- decrypted_data ----------------------  ')
    point_set = set()
    for p in points:
        temp = tuple(p[i] for i in range(len(p)))
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


def make_plot(point_list, rep_list, chosen=None, leftovers_list=None, rands=None, strips=None):
    if rands and strips:
        plot_bricks(strips, rands)
    point_set = decrypt_data(point_list)
    rep_set = decrypt_data(rep_list)
    # leftovers_set = decrypt_data(leftovers_list)
    # chosen_set = decrypt_data(chosen)
    print('rep list of size  __', len(rep_list), '__  : ', rep_list)
    # print('leftovers list of size  __', len(leftovers_list), '__  : ', leftovers_list)
    # print('REAL leftovers list of size  __', len(leftovers_set), '__  : ', leftovers_set)

    #   plot
    plt.scatter(*zip(*point_set), label='Input Points', c='blue', s=1)  # <--	for plot purposes only
    # plt.scatter(*zip(*rep_set), label='Mean Representatives', c='black', s=10)  # <--	for plot purposes only
    # plt.scatter(*zip(*leftovers_set), label='Leftovers', c='red', s=2)  # <--	for plot purposes only

    # chosen = gen_encrypted_points_from_file("/home/rbd/workspace/rbd/rbd_helib_with_remote_debugger/chosen")
    # leftover = gen_encrypted_points_from_file("/home/rbd/workspace/rbd/rbd_helib_with_remote_debugger/leftover")
    plt.scatter(*zip(*rep_set), label='means', c='black', s=7)  # <--	for plot purposes only
    # plt.scatter(*zip(*chosen_set), label='chosen', c='green', s=2)  # <--	for plot purposes only
    # plt.scatter(*zip(*leftovers_set), label='leftover', c='red', s=2)  # <--	for plot purposes only

    plt.title('Choosing representatives')  # <--	for plot purposes only
    plt.legend(loc='upper right')  # <--	for plot purposes only
    plt.show()  # <--	for plot purposes only


if '__main__' == __name__:
    # points_list = gen_encrypted_points_from_file("points")
    points_list  = gen_encrypted_points_from_file("/home/rbd/workspace/rbd/rbd_helib_with_remote_debugger/points")

    # means        = gen_encrypted_points_from_file("/home/rbd/workspace/rbd/rbd_helib_with_remote_debugger/means")
    means_test        = gen_encrypted_points_from_file("/home/rbd/workspace/rbd/rbd_helib_with_remote_debugger/means_test")
    # chosen       = gen_encrypted_points_from_file("/home/rbd/workspace/rbd/rbd_helib_with_remote_debugger/chosen")
    # leftover     = gen_encrypted_points_from_file("/home/rbd/workspace/rbd/rbd_helib_with_remote_debugger/leftover")

    # chosen       = [Point(Binary(0), Binary(0))]
    # leftover     = [Point(Binary(0), Binary(0))]
    # print(points_list)
    # points_list = decrypt_data(points_list)
    # chosen = decrypt_data(chosen)
    # leftover = decrypt_data(leftover)

    # print(points_list)
    # print(means)
    # print(chosen)
    # print(leftover)

    make_plot(points_list, means_test)
    # make_plot(points_list, means, chosen, leftover)


