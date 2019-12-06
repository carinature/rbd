#!/use/bin/env python3
from collections import defaultdict


# Binary Multiplication Counter (Bin*Bin)
# global_bin_mul_counter = 0


class KmeansObj:
    def __init__(self, value):
        self.value = value
        self.add_counter = 0
        # self.mul_counter = 0
        # self.mul_counter_bit = 0
        # self.mul_counter_binary = 0
        # self.mul_counter_point = 0
        self.counter_dict = {Bit: 0, Binary: 0, Point: 0}

    def __repr__(self):
        return self.__class__.__name__ + '(' + str(self.value) + ')'

    def __str__(self):
        return self.__repr__() \
                   + '\t' + str({key.__name__: value for key, value in self.counter_dict.items()})

    def increase_mul_counter(self, other):
        self.counter_dict[other.__class__] += 1
        other.counter_dict[self.__class__] += 1  # todo might be bad practice changing a parameter of a function


class KMeansException(Exception):
    pass


class Bit(KmeansObj):
    def __add__(self, other):
        if not isinstance(other, Bit):
            raise KMeansException('No multiplication other than bit * Bin/bit is allowed')
        print('bit + bit')
        self.add_counter += 1
        other.add_counter += 1
        new_val = self.value or other.value
        # new_val = (3 * param - param * param) // 2
        return Bit(new_val)

    def __mul__(self, other):
        if not issubclass(other.__class__, KmeansObj):  # todo is this redundant?
            raise KMeansException('No multiplication other than bit * Bin/bit is allowed')
        if isinstance(other, Bit):
            print("bit * bit")
            self.increase_mul_counter(other)
            return Bit(self.value * other.value)
        return other * self  # behavior defined in class Point and Bin
        # self.increase_mul_counter(other)
        # ctor = other.__class__
        # return ctor(new_val)
        # todo
        #  consider if you want to changing current point's value, but maintaining the value of the counters
        #  or return a NEW point, with all counters equal zero, but you dont change the previous point
        #  or maybe adding a c'tor with counter values


class Binary(KmeansObj):
    def __add__(self, other):
        if not isinstance(other, Binary):
            raise KMeansException('No addition other than Bin + Bin is allowed')
        self.add_counter += 1
        # other.add_counter += 1
        new_val = self.value + other.value
        # new_val = self.value or other.value
        # new_val = (3 * param - param * param) // 2
        return Binary(new_val)

    def __mul__(self, other):
        if not issubclass(other.__class__, KmeansObj):
            raise KMeansException('No multiplication other than Bin * Point/Bin/bit is allowed')
        if isinstance(other, Point):
            return other * self  # behavior defined in class Point
        self.increase_mul_counter(other)
        return Binary(self.value * other.value)
        # todo
        #  consider if you want to changing current point's value, but maintaining the value of the counters  
        #  or return a NEW point, with all counters equal zero, but you dont change the previous point 
        #  or maybe adding a c'tor with counter values


class Point(KmeansObj):
    def __init__(self, arg1, *args):
        # def __init__(self, arg1=None, *args):
        KmeansObj.__init__(self, arg1)
        coor_vector = [arg1] if arg1 is not None and isinstance(arg1, Binary) else [-1]
        for arg in args:
            if not isinstance(arg, Binary):
                print(arg)
                raise KMeansException("Coordinate must be binary (encrypted)")
            coor_vector.append(arg)
        self.cmp_counter = 0
        self.coor_vector = coor_vector
        self.value = coor_vector

    def __len__(self):  # return the dimension of the point
        return len(self.coor_vector) if -1 != self.coor_vector[0] is not None else 0

    def __str__(self):
        return self.__repr__() \
               + '\tDim: ' + str(len(self)) \
               + '\t' + str({key.__name__: value for key, value in self.counter_dict.items()})

    def __getitem__(self, key):
        return self.coor_vector[key]

    def __set_mul_counter(self, counter_dict):
        self.counter_dict = counter_dict

    def __get_mul_counter(self):
        # return self.mul_counter, self.mul_counter_bit, self.mul_counter_binary, self.mul_counter_point
        return self.counter_dict

    counter = property(__get_mul_counter, __set_mul_counter)

    def __mul__(self, other):
        if not issubclass(other.__class__, KmeansObj):
            raise KMeansException('No multiplication other than Point * Bin/bit is allowed')
        if type(other) is Point:
            # if isinstance(other, Point):  # todo consider allowing Point X Point multiplication, for dist function?
            raise KMeansException('No Point * Point multiplication is allowed')
        args = tuple()
        for arg in self.coor_vector:
            args += (arg * other,)
        new_point = Point(*args)
        self.increase_mul_counter(other)
        new_point.counter = self.counter
        # global global_bin_mul_counter
        # global_bin_mul_counter += 1
        # print('------------ ', global_bin_mul_counter)
        return new_point
        # todo 
        #  consider if you want to changing current point's value, but maintaining the value of the counters  
        #  or return a NEW point, with all counters equal zero, but you dont change the previous point 
        #  or maybe adding a c'tor with counter values


# class counter:
#     def __init__(self):
#         self.value = 0
#
#     def plus_one(self):
#         self.value += 1
#
#     def __add__(self, other):
#         self.value += other


if '__main__' == __name__:

    b0 = Bit(0)
    b1 = Bit(1)
    b2 = Bit(2)
    B0 = Binary(0)
    B1 = Binary(1)
    B2 = Binary(2)
    B3 = Binary(3)
    print(b2)
    print('x*x: ', b2 * b2)
    print(b2)
    print('===========')
    print(B2)
    print('y*y: ', B2 * B2)
    print(B2)
    print('===========')
    print('x*y: ', b2 * B2)
    print(b2)
    print(B2)
    print('===========')
    print('y*x: ', B2 * b2)
    print(b2)
    print(B2)
    try:
        print(b2 * 3)
    except KMeansException:
        print(" ~ Caught Error: multiplication allowed only between 2 kmean_obj - CHECK ~ ")
    try:
        print(B2 * 3)
    except KMeansException:
        print(" ~ Caught Error: multiplication allowed only between 2 kmean_obj - CHECK ~ ")

    point = Point(B2, B2 * B2)
    print(point)
    try:
        print(point * 3)
    except KMeansException:
        print(" ~ Caught Error: multiplication allowed only between 2 kmean_obj - CHECK ~ ")
    try:
        print(point * point)
    except KMeansException:
        print(" ~ Caught Error: big NoNo - CHECK ~ ")

    x = b0
    y = b1
    print('===========')
    print('x: ', x)
    print('y: ', y)
    print('point*x: ', point * x)
    print('point*y: ', point * y)
    print(point)
    #
    xz = Binary(0)
    yz = Binary(0)
    print(point)
    print('x: ', xz)
    print('y: ', yz)

    print('point*x: ', point * xz)
    print('point*y: ', point * yz)

    xz = Binary(1)
    yz = Binary(1)
    print(point)
    print('x: ', xz)
    print('y: ', yz)

    print('point*x: ', point * xz)
    print('point*y: ', point * yz)

    b = Bit(3)
    print(b)
    print('point*y: ', point * b)
    print(point)
    print('x: ', x)
    print('y: ', y)
    x*x
    print('x*x')
    print('x: ', x)
    print('y: ', y)
    print('x: ', xz)
    print('y: ', yz)
    print(point)

    print('=========================')
    bit = Bit(1)
    bin = Binary(2)
    point = Point(bin, bin, bin)
    print(bit)
    print(bin)
    print(point)
    print('-----bit*bin------')
    bit*bin
    print(bit)
    print(bin)
    print(point)
    print('------bit*point-----')
    bit*point
    print(bit)
    print(bin)
    print(point)
    print('-----bin*point------')
    bin*point
    print(bit)
    print(bin)
    print(point)
    print('------bit*bin*point-----')
    bit*bin*point
    print(bit)
    print(bin)
    print(point)
    print('-----bit*bin*point------')
    bit*bin*point
    print(bit)
    print(bin)
    print(point)
    print('-----------')