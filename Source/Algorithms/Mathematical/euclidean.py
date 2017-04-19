"""
Fundamentals :: Algorithms :: Euclidean Algorithms
Author: Quinn Mortimer

This file contains implementations of two algorithms, the Euclidean algorithm
and the extended Euclidean algorithm.
It is inteded as a reference for those looking to brush up on important
algorithms.

N.B. Here the GCD of two integers will refer to a positive integer. Programming
languages differ on the result of modulo when negative numbers are involved,
and people have differing opinions on whether a negative number is a valid GCD.
Enforcing that it is positive means that we have a correct result for all
definitions of the GCD and programming languages.
"""

def gcd(a, b):
    """An implementation of the Euclidean algorithm.
    
    The Euclidean algorithm is used to compute the greatest common divisor of
    two integers. As originally conceived, these are the steps:
    At start, we have two integers a and b as our inputs.
    1 > Check if b is 0.
        -> If so, the algorithm is complete. The value of a is the result.
        -> If not, continue to step 2.
    2 > Subtract b from a until a is less than b.
    3 > Swap the values of a and b.
    4 > Return to step 1.
    
    Here this process is represented as a while loop, and the repeated
    subtraction is replaced with a single calculation of a modulo b.
    
    :param a: The first integer input.
    :param b: The second integer input.
    :return: The greatest common denominator of the two inputs.
    """
    while b != 0:
        a, b = b, a % b
    return abs(a)

def gcd_recursive(a, b):
    """A recursive implementation of the Euclidean algorithm.
    
    This is functionally identical to the previous presented version of
    Euclid's algorithm, but it uses recursion rather than iteration to find the
    value.
    
    :param a: The first integer input.
    :param b: The second integer input.
    :return: The greatest common denominator of the two inputs.
    """
    if b == 0:
        return abs(a)
    return gcd(b, a % b)

def gcd_extended(a, b):
    """A recursive implementation of the extended Euclidean algorithm.
    
    The result of this algorithm is three integers x, y and d that satisfy
    the following relationship for the two inputs a and b.
        a * x + b * y = d = gcd(a, b)
    This relationship is known as Bezout's identity, and its unknowns
    are the Bezout coefficients (x, y) that satisfy this equation.
    
    Calculating the Bezout coefficients is trickier than calculating the GCD.
    One solution when working on paper is to construct a series of equations
    where each equation is defined by the relationship.
        a - q * b   = r
    Once the series of equations has been written out, it is possible to build
    back out from the last equation to find the final result.
    The base case for (a, b) = (n, 0) results in (x, y, d) = (1, 0, n).
    At each step on the way back out, the coefficients can be determined from
    the cumulative result to the current point and the quotient of a and b
    at the current step.
    
    Bezout coefficients are not guaranteed to be unique for any input (a, b).
    The solution from this equation will be a valid, but others exist.
    
    :param a: The first integer input.
    :param b: The second integer input.
    :return: A tuple (x, y, d) where the first two terms are the Bezout
        coefficients of a and b, and the third term is gcd(a, b).
    """
    if b == 0:
        # Making sure to find the positive GCD here. x changes accordingly.
        magn_a = abs(a)
        sign_a = 1 if a == magn_a else -1
        return sign_a, 0, magn_a
    
    x, y, d = gcd_extended(b, a % b)
    return y, x - y * (a // b), d

def gcd_extended_iterative(a, b):
    """An iterative implementation of the extended Euclidean algorithm.
    
    The reasoning behind the iterative version of the algorithm relies on
    some matrix algebra to explain properly, which is just too much to be
    explaining in a Python comment. If you want to know how it works, the
    wikipedia page on the Euclidean algorithm has a section on the matrix
    method for solving for these coeffiecients.
    https://en.wikipedia.org/wiki/Euclidean_algorithm#Matrix_method
    
    :param a: The first integer input.
    :param b: The second integer input.
    :return: A tuple (x, y, d) where the first two terms are the Bezout
        coefficients of a and b, and the third term is gcd(a, b).
    """
    x0, x1 = 1, 0
    y0, y1 = 0, 1
    
    while b != 0:
        q = a // b
        x0, x1 = x1, x0 - (q * x1)
        y0, y1 = y1, y0 - (q * y1)
        
        a, b = b, a % b
    
    if a < 0:
        return -x0, -y0, -a
    else:
        return x0, y0, a
