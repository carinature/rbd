#!/use/bin/env python3
from collections import defaultdict

# TODO
# Binary Multiplication Counter (Bin*Bin)
global_counter_dict = {'bitbit': 0, 'bitbin': 0, 'binbin': 0, 'add': 0, 'cmp': 0}


class KmeansObj:
    def __init__(self, value):
        self.value = value
        self.add_counter = 0
        self.cmp_counter = 0
        self.mul_counter_dict = {Bit: 0, Binary: 0, Point: 0}

    def __repr__(self):
        return self.__class__.__name__ + '(' + str(self.value) + ')'

    def __str__(self):
        return self.__repr__() \
               + '\t' + str({key.__name__: value for key, value in self.mul_counter_dict.items()})

    def increase_mul_counter(self, other):
        self.mul_counter_dict[other.__class__] += 1
        other.mul_counter_dict[self.__class__] += 1  # todo might be bad practice changing a parameter of a function


class KMeansException(Exception):
    # todo add custom messages
    pass


class Bit(KmeansObj):
    def __add__(self, other):
        # if type(other) is Binary:
        #     raise KMeansException(" Addition of bin and bit")
        #     return other + self
        if not isinstance(other, Bit):
            raise KMeansException('Only addition between bit and bit/Bin is allowed')
        self.add_counter += 1
        other.add_counter += 1
        new_val = self.value or other.value
        # this is to overcome the fact that the "prod" sometimes returns 2 instead of 1
        #   you can look at it as the next function: f(0) = 0; f(1) = 1 = f(2)
        # def zero_or_one(param):
        #     return Bit((3 * param - param * param) // 2)
        # new_val = (3 * param - param * param) // 2
        return Bit(new_val)

    def __sub__(self, other):
        # todo - ideally we should find another way to give the opposite / the answer to 1-bit or !bit or not bit
        return Bit(self.value - other.value)

    def __mul__(self, other):
        if not issubclass(other.__class__, KmeansObj):  # todo is this redundant?
            raise KMeansException('No multiplication other than bit * Bin/bit is allowed')
        if isinstance(other, Bit):
            self.increase_mul_counter(other)
            return Bit(self.value * other.value)
        return other * self  # behavior defined in class Point and Bin


class Binary(KmeansObj):
    # def __init__(self, conversion_arg):
    #     if type(conversion_arg) is Bit:
    #         super().__init__(conversion_arg.value)
    #     elif type(conversion_arg) is float:
    #         super().__init__(conversion_arg.value)
    #         print(conversion_arg)
    #     else:
    #         raise KMeansException("Can only convert Bit into Binary")

    def __add__(self, other):
        if not isinstance(other, Binary) and type(other) is not Bit:
            raise KMeansException('No addition other than Bin + Bin is allowed')
        self.add_counter += 1
        other.add_counter += 1
        new_val = self.value + other.value
        return Binary(new_val)
        # new_val = self.value or other.value
        # this is to overcome the fact that the "prod" sometimes returns 2 instead of 1
        #   you can look at it as the next function: f(0) = 0; f(1) = 1 = f(2)
        # def zero_or_one(param):
        #     return Bit((3 * param - param * param) // 2)
        # new_val = (3 * param - param * param) // 2

    def __mul__(self, other):
        if not issubclass(other.__class__, KmeansObj):
            raise KMeansException('No multiplication other than Bin * Point/Bin/bit is allowed')
        if isinstance(other, Point):
            return other * self  # behavior defined in class Point
        self.increase_mul_counter(other)
        global global_counter_dict
        global_counter_dict['binbin'] += 1
        return Binary(round(self.value * other.value, 2))

    def __gt__(self, other):  # todo consider moving this to the Point class (and then changing the kmeans.compare(a,b)
        if type(other) is not Binary:
            print('self', self)
            print('other', other)
            raise KMeansException("Comparison only between two Binaries")
        self.cmp_counter += 1
        other.cmp_counter += 1
        global global_counter_dict
        global_counter_dict['cmp'] += 1
        # if self.value > other.value: # todo this answer yields empty groups -> group_size=0 -> division by zero
        # todo consult danny/adi
        if self.value >= other.value:
            return Bit(1)
        else:
            return Bit(0)


class Point(KmeansObj):
    def __init__(self, arg1, *args):
        # def __init__(self, arg1=None, *args):
        KmeansObj.__init__(self, arg1)
        coor_vector = [arg1] if arg1 is not None and isinstance(arg1, Binary) else [-1]
        for arg in args:
            if not isinstance(arg, Binary):
                # print(arg)
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
               + '\t' + str({key.__name__: value for key, value in self.mul_counter_dict.items()})

    def __getitem__(self, key):
        return self.coor_vector[key]

    def __set_mul_counter(self, mul_counter_dict):
        self.mul_counter_dict = mul_counter_dict

    def __get_mul_counter(self):
        # return self.mul_counter, self.mul_counter_bit, self.mul_counter_binary, self.mul_counter_point
        return self.mul_counter_dict

    counter = property(__get_mul_counter, __set_mul_counter)

    def __add__(self, other):
        sum_point = self.value
        for d in range(len(self)):
            sum_point[d] += other[d]
        return Point(*sum_point)

    def __mul__(self, other):
        if not issubclass(other.__class__, KmeansObj):
            raise KMeansException('No multiplication other than Point * Bin/bit is allowed')
        if type(other) is Point:
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

        #   gt, or operator '>' returns Bit(0) if point_b is smaller than point_a, Bit(0) otherwise
        #       (use case:  point_a*cmp(point_a, point_b) returns point_a only if it is "bigger" than point_a.
        #       otherwise it's point zero)

    def __gt__(self, other, current_dim=2):
        # receives 2 Binaries and returns a Bit: 1 if a>=b, 0 otherwise
        def helib_compare(aa, bb):  # PLACEHOLDER
            # print(aa, bb)
            return aa > bb  # defined in Binary class

        # for d in range(1, len(other)):
        #     a_is_bigger = helib_compare(self[d], other[d])
        #     b_is_bigger = helib_compare(other[d], self[d])
        d = current_dim - 1
        self.cmp_counter += 1
        other.cmp_counter += 1
        # if Binary == self or Binary == other:
        #     print(self[d], other[d])
        #     raise KMeansException('whaaaaaaaa...?')
        return helib_compare(self[d], other[d])

    def __lt__(self, other, current_dim=2):
        return not __gt__(self, other)



# Receives a list of encrypted points, and the size of the list (since we don't know which points are "real")
#   and returns an encrypted point, which is the representative
def point_zero(dim):
    return Point(*[Binary(0) for _ in range(dim)])


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
    x * x
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
    bit * bin
    print(bit)
    print(bin)
    print(point)
    print('------bit*point-----')
    bit * point
    print(bit)
    print(bin)
    print(point)
    print('-----bin*point------')
    bin * point
    print(bit)
    print(bin)
    print(point)
    print('------bit*bin*point-----')
    bit * bin * point
    print(bit)
    print(bin)
    print(point)
    print('-----bit*bin*point------')
    bit * bin * point
    print(bit)
    print(bin)
    print(point)
    print('-----------')

    # bin_from_bit = Binary(Bit(1))
    # print(bin_from_bit)
