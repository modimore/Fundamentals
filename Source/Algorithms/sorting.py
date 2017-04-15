"""
Fundamentals :: Algorithms :: Sorting Algorithms
Author: Quinn Mortimer

This file contains implementations of several sorting algorithms.
It is intended as a reference for those looking to brush up on important
algorithms.

Note: Despite the above message, this file only contains a quicksort
implementation. More algorithms to come later.
"""

def quicksort(a, low, high):
    """A quicksort driver function.
    
    The associated partition function actually does all of the work here.
    This function has two major responsiblities:
        1) Check for edge cases.
        2) Dispatch recursive calls.
    
    This function operates in-place on the input array. The argument `a` is
    expected to be a list and lists are passed by reference in Python.
    
    See also :func:`_quicksort_partition` for the partition implementation.
    
    :param a: The array to sort.
    :param low: The index to start sorting at.
    :param high: The index to stop sorting at.
    """
    global ct_ct
    if high > low + 1:
        idx_p = _quicksort_partition(a, low, high)
        quicksort(a, low, idx_p)
        quicksort(a, idx_p, high)

def _quicksort_partition(a, low, high):
    """An implementation of a partitioning function for quicksort algorithms.
    
    This function exists as a helper for :func:`quicksort` (above).
    
    At this function's end, the input array is partitioned into two halves.
    No element on the left side compares greater than an element on the right.
    
    This particular partioning function is a minor variation on Hoare's method.
    Rather than use one of the end elements as a pivot, it uses the element at
    the center of the list. This results in greater efficiency when the list
    is nearly sorted.
    
    N.B. The `high` index here is past the end of the subarray to be partioned
    this call.
    
    :param a: The array to partition.
    :param low: The index to start sorting at.
    :param high: The index to stop sorting at.
    :return: The index of the partition.
        This is the index of the first element in the right sub-array.
        Equivalently, it is just past the last element of the left sub-array.
    """
    """The general idea here is we move two indexes from opposite sides of the
    array towards each other iteratively. Elements that could belong on the
    other side of the pivot are swapped as we go. When the indexes meet or pass
    each other, then we stop and return one of them as the partition point.
    """
    pivot = a[(low + high) // 2]
    
    idx_l = low
    idx_r = high - 1
    
    while True:
        while a[idx_l] < pivot:
            idx_l += 1
        
        while a[idx_r] > pivot:
            idx_r -= 1
        
        if idx_r <= idx_l:
            break
        
        a[idx_l], a[idx_r] = a[idx_r], a[idx_l]
        
        idx_l += 1
        idx_r -= 1
    
    return idx_l
