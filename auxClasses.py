#!/use/bin/env python3

global_mul_counter = 0


class Bit:
    def __init__(self, value):
        self.value = value
        self.mul_counter = 0
        self.bit_mul_counter = 0

    def __str__(self):
        return '\nBit: ' + str(self.value) \
               + '\nNum of times used in multiplication: ' + str(self.mul_counter) \
               + '\nNum of times used in bit_multiplication: ' + str(self.bit_mul_counter) + '\n'

    def __mul__(self, other):
        if isinstance(other, Binary):
            print('bit*Bin')
            self.mul_counter += 1
        elif isinstance(other, Bit):
            print('bit*bit')
            self.bit_mul_counter +=1
        else:
            raise Exception('No multiplication other than bit * Bin/bit is allowed')
        new_val = self.value*other.value
        return Bit(new_val)  
        # todo 
        #  consider if you want to changing current point's value, but maintaining the value of the counters  
        #  or return a NEW point, with all counters equal zero, but you dont change the previous point 
        #  or maybe adding a c'tor with counter values


class Binary:
    def __init__(self, value):
        self.value = value
        self.mul_counter = 0
        self.bit_mul_counter = 0

    def __str__(self):
        return '\nBinary: ' + str(self.value) \
               + '\nNum of times used in multiplication: ' + str(self.mul_counter) \
               + '\nNum of times used in bit_multiplication: ' + str(self.bit_mul_counter) + '\n'

    def __mul__(self, other):
        if isinstance(other, Binary):
            print('Bin*Bin')
            self.mul_counter += 1
        elif isinstance(other, Bit):
            print('Bin*bit')
            self.bit_mul_counter += 1
        else:
            raise Exception('No multiplication other than Bin * Bin/bit is allowed')
        new_val = self.value*other.value
        return Binary(new_val)  
        # todo 
        #  consider if you want to changing current point's value, but maintaining the value of the counters  
        #  or return a NEW point, with all counters equal zero, but you dont change the previous point 
        #  or maybe adding a c'tor with counter values


class Point:

    def __init__(self, arg1=None, *args):
        coor_vector = [arg1] if arg1 is not None else [-1]
        for arg in args:
            coor_vector.append(arg)
        self.mul_counter = 0
        self.bit_mul_counter = 0
        self.coor_vector = coor_vector

    def __len__(self):
        return len(self.coor_vector) if -1 != self.coor_vector[0] is not None else 0

    # print
    def __repr__(self):
        return 'Point()'

    def __str__(self):
        return '\n  === Point: ' + str(self.coor_vector) \
               + ' ===\nDimensions: ' + str(len(self)) \
               + '\nNum of times used in multiplication: ' + str(self.mul_counter) + '\n'

    def __mul__(self, other):
        if isinstance(other, Binary):
            print('Point*Bin')
            self.mul_counter += 1
        elif isinstance(other, Bit):
            print('Point*bit')
            self.bit_mul_counter += 1
        else:
            raise Exception('No multiplication other than Bin * Bin/bit is allowed')
        args = tuple()
        for arg in self.coor_vector:
            args += (arg * other.value, )
        return Point(*args)  
        # todo 
        #  consider if you want to changing current point's value, but maintaining the value of the counters  
        #  or return a NEW point, with all counters equal zero, but you dont change the previous point 
        #  or maybe adding a c'tor with counter values


if '__main__' == __name__:
    # point = Point()
    # print(point)
    # point = Point((1, 2))
    # print(point)

    point = Point(1, 2, 4, 5, 3, 6, 3, 4, 6, 7, 7, 7)
    print(point)

    x = Binary(3)
    # print(x)
    # print(' === x*x ===:\n', x*x)
    # print(x*3)
    y = Bit(2)
    # print(y)
    # print('x*y: ', x*y)
    # print('y*x: ', y*x)
    # print('y*y: ', y*y)
    # # # print(y*3
    # #
    # # print(x)
    # # print(y)
    # # print('x*x: ', x*x)
    # # print('y*y: ', y*y)
    # #
    # # print(x)
    # # print(y)

    print('point*x: ', point*x)
    print('point*y: ', point*y)

    xz = Binary(0)
    yz = Binary(0)

    print('point*x: ', point*xz)
    print('point*y: ', point*yz)

    xz = Binary(1)
    yz = Binary(1)

    print('point*x: ', point*xz)
    print('point*y: ', point*yz)


