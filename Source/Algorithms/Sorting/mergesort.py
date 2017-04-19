"""
Fundamentals :: Algorithms :: Sorting Algorithms
Author: Quinn Mortimer

This file contains an implementation of the mergesort algorithm.
It is intended as a reference for those looking to brush up on important
algorithms.
"""

def mergesort(a):
    """A mergesort driver function.
    
    In this implementation, a helper function does most of the work,
    but this sets up the pre-conditions for that helper. It requires
    an array that is an exact copy of the array we are meant to sort.
    Here we create that and pass it in to the helper.
    
    It is important that the original array be passed to the helper
    as the sorting destination.
    
    :param a: The array to sort.
    """
    a_source = a[:]
    _mergesort_helper(a, a_source, 0, len(a))

def _mergesort_helper(dest, source, idx_start, idx_end):
    """A helper function for performing a top-down mergesort.
    
    This function first sorts the left and right halves of a subarray
    of the source array, then it merges the two halves into the
    corresponding subarray of the destination array.
    
    The tricky part of this is that the sorting of the two halves is a
    recursive call to this function with the source and destination
    arrays swapping their roles. This leads to a situation where if at
    the beginning of the function source[start:end] and dest[start:end]
    are identical, then at the end of the function dest[start:end] will
    be a sorted permutation of source[start:end].
    
    :param dest: The destination for the sorted elements.
    :param source: The array which will contain the sorted subarrays.
    :param idx_start: The index of the beginning of the subarray to sort.
    :param idx_end: The index of the end of the subarray to sort.
    """
    # A subarray of length 0 or 1 is already sorted.
    if idx_end - idx_start < 2:
        return
    
    # This is the index of the end of the left part of the subarray
    # and the start of the right part of the subarray.
    idx_mid = (idx_start + idx_end) // 2
    
    # These calls sort the left and right parts of the current subarray
    # of the source.
    _mergesort_helper(source, dest, idx_start, idx_mid)
    _mergesort_helper(source, dest, idx_mid, idx_end)
    
    # Now that the left and right subarrays of source are sorted,
    # we are going to merge them into the destination subarray.
    idx_dest = idx_start
    idx_l, idx_r = idx_start, idx_mid
    # While we haven't readed the end of either part of the subarray,
    # take the smaller of the two values and add it in the destination.
    while idx_l < idx_mid and idx_r < idx_end:
        if source[idx_r] < source[idx_l]:
            dest[idx_dest] = source[idx_r]
            idx_r += 1
        else:
            dest[idx_dest] = source[idx_l]
            idx_l += 1
        idx_dest += 1
    
    # When we've reached the end of one of the parts of the subarray, this
    # will add the remaining elements of the other one into the destination.
    while idx_l < idx_mid:
        dest[idx_dest] = source[idx_l]
        idx_dest += 1
        idx_l += 1
    while idx_r < idx_end:
        dest[idx_dest] = source[idx_r]
        idx_dest += 1
        idx_r += 1
