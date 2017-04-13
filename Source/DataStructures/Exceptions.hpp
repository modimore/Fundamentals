/**
 * Fundamentals :: Data Structures :: Exceptions
 * Author: Quinn Mortimer
 *
 * This file contains exceptions that may be thrown in
 * sibling `.cpp` and `.hpp` files.
 *
 * They exist to make explicit the reason for failure in the
 * places where each is thrown.
 */

#ifndef Fundamentals_DS_Exceptions_hpp_
#define Fundamentals_DS_Exceptions_hpp_

/**
 * Exception Base Class
 */
class Exception {
};
/**
 * Exception thrown when trying to access beyond the bounds of
 * a container class.
 */
class OutOfBoundsError : public Exception {
};
/**
 * Exception thrown when passing an iterator from a different list
 * into a linked list's insert or remove method.
 */
class MismatchedIteratorError : public Exception {
};

#endif // Fundamentals_DS_Exceptions_hpp_
