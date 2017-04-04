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

#ifndef __Fundamentals_DS_Exceptions_hpp__
#define __Fundamentals_DS_Exceptions_hpp__

/**
 * Exception Base Class
 */
class Exception {
};

class OutOfBoundsError : public Exception {
};

class MismatchedIteratorError : public Exception {
};

#endif // __Fundamentals_DS_Exceptions_hpp__
