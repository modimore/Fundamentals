"""
Fundamentals :: Algorithms :: Sorting Algorithms
Author: Quinn Mortimer

This file contains an implementation of the insertion sort algorithm.
It is intended as a reference for those looking to brush up on important
algorithms.
"""

def insertion_sort(a):
    """An insertion sort implementation.
    
    The process of insertion sort is to step through an array while
    building up a sorted subarray starting at the front and eventually
    including the whole array. At each step, the current element in the
    array is moved to the correct position in the subarray, shifting all
    elements that should be after it up one index.
    
    :param a: The array to sort.
    """
    # This intentionally starts at 1 because the subarray containing
    # only the first element is already sorted.
    for i in range(1, len(a)):
        j = i-1
        x = a[i]
        while j >= 0 and x < a[j]:
            a[j+1] = a[j]
            j -= 1
        a[j+1] = x
