/**
 * Fundamentals :: Data Structures :: Dynamic Array
 * Author: Quinn Mortimer
 *
 * This is an implementation of a DynamicArray-style data structure.
 * It is intended as a reference for those looking to brush up on�important
 * data structures.
 */

#include <cstddef>

#include "Exceptions.hpp"

#ifndef Fundamentals_DynamicArray_hpp_
#define Fundamentals_DynamicArray_hpp_

#define DYNAMICARRAY_DEFAULT_CAPACITY 8
#define DYNAMICARRAY_GROWTH_FACTOR 2

/**
 * A dynamically-sized array.
 */
template <typename T>
class DynamicArray {
	public:
		typedef size_t size_type;
		typedef T* iterator;
		typedef const T* const_iterator;
		
		// Constructors and Destructor
		DynamicArray();
		DynamicArray(size_type a_size);
		DynamicArray(size_type a_size, const T& a_value);
		DynamicArray(const DynamicArray<T>& other);
		DynamicArray(DynamicArray<T>&& other);
		~DynamicArray();
		
		// Access to current size and capacity
		size_type size() const;
		size_type capacity() const;
		bool empty() const;
		
		// Bulk-memory affecting methods
		void resize(size_type size);
		void reserve(size_type size);
		
		// Element insertion and deletion
		// - Back
		void push_back(const T& value);
		void pop_back();
		// - Front
		void push_front(const T& value);
		void pop_front();
		// - Arbitrary
		void insert(const_iterator position, const T& value);
		void remove(const_iterator position);
		
		// Iterators
		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;
		
		// Data Access at notable indices
		T& front();
		const T& front() const;
		T& back();
		const T& back() const;
		
		// Operators
		// - Assignment
		DynamicArray<T>& operator=(const DynamicArray<T>& other);
		DynamicArray<T>& operator=(DynamicArray<T>&& other);
		// - Indexing
		T& operator[](size_type index);
		const T& operator[](size_type index) const;
		// - Equality testing
		bool operator==(const DynamicArray<T>& other) const;
		bool operator!=(const DynamicArray<T>& other) const;
	
	private:
		T* m_data;
		size_type m_capacity;
		size_type m_size;
		
		// Swap function for a DynamicArray.
		void m_swap(DynamicArray<T>& other);
};

/**
 * Constructs an empty array.
 *
 * This constructor assumes that elements will be added later, and reserves a
 * small implementation-defined amount of space for them initially. For greater
 * control over how much space is initially allocated, either the `reserve`
 * method or a constructor specifying an initial size should be used.
 */
template <typename T>
DynamicArray<T>::DynamicArray() {
	m_data = new T[DYNAMICARRAY_DEFAULT_CAPACITY];
	m_capacity = DYNAMICARRAY_DEFAULT_CAPACITY;
	m_size = 0;
}
/**
 * Constructs an array with a specified starting size.
 *
 * Initial elements are made with the default constructor.
 *
 * @param	a_size	The initial size of the array
 */
template <typename T>
DynamicArray<T>::DynamicArray(size_type size) {
	m_data = new T[size];
	m_capacity = size;
	m_size = size;
	
	for (T* p = m_data; p != m_data + m_size; ++p) {
		*p = T();
	}
}
/**
 * Constructs an array filled with the specified value.
 * @param	a_size	The initial size of the array
 * @param	a_value	The value used to fill the elements
 */
template <typename T>
DynamicArray<T>::DynamicArray(size_type size, const T& value) {
	m_data = new T[size];
	m_capacity = size;
	m_size = size;
	
	for (T* p = m_data; p != m_data + m_size; ++p) {
		*p = value;
	}
}
/**
 * Constructs an array by copying from a constant reference to another array.
 * @param	other	The DynamicArray to copy data from
 */
template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other) {
	this->m_capacity = other.m_capacity;
	this->m_size = other.m_size;
	this->m_data = new T[this->m_capacity];
	
	for (size_type i = 0; i < this->m_size; ++i) {
		this->m_data[i] = other.m_data[i];
	}
}
/**
 * Constructs an array by swapping contents with another array.
 * @param	other	The DynamicArray to swap contents with
 */
template <typename T>
DynamicArray<T>::DynamicArray(DynamicArray<T>&& other) {
	this->m_swap(other);
}
/**
 * Destructor for a DynamicArray, frees the underlying array from the heap.
 */
template <typename T>
DynamicArray<T>::~DynamicArray() {
	delete [] m_data;
}

/**
 * Gets the size of the array for the user.
 * @return	The number of elements in the array
 */
template <typename T>
typename DynamicArray<T>::size_type DynamicArray<T>::size() const {
	return m_size;
}
/**
 * Gets the capacity of the array for the user.
 * @return	The number of elements (used and unused) allocated in the array
 */
template <typename T>
typename DynamicArray<T>::size_type DynamicArray<T>::capacity() const {
	return m_capacity;
}
/**
 * Reports on whether or not this DynamicArray is empty.
 * @return	Whether or not this array has any in-use elements
 */
template <typename T>
bool DynamicArray<T>::empty() const {
	return m_size == 0;
}

/**
 * Set the size of the DynamicArray to the specified value.
 *
 * If elements need to be added, uses the default constructor for T.
 *
 * @param	size	The desired size for the array
 */
template <typename T>
void DynamicArray<T>::resize(size_type size) {
	if (m_capacity < size) {
		reserve(size);
	}
	
	if (m_size < size) {
		for (size_type i = m_size; i < size; ++i) {
			m_data[i] = T();
		}
	}
	
	m_size = size;
}

/**
 * Reserves a specific amount of memory for use.
 *
 * This will only ensure that enough memory has been allocated.
 * This method will not shrink the capacity of the array.
 *
 * @param	capacity	Desired capacity for this array
 */
template <typename T>
void DynamicArray<T>::reserve(size_type capacity) {
	if (m_capacity < capacity) {
		T* next_data = new T[capacity];
		
		for (size_type i = 0; i < m_size; ++i) {
			next_data[i] = m_data[i];
		}
		
		delete [] m_data;
		m_data = next_data;
		m_capacity = capacity;
	}
}

/**
 * Adds the provided element to the back of the array.
 * @param	value	The value to place at the back of the array
 */
template <typename T>
void DynamicArray<T>::push_back(const T& value) {
	if (m_size == m_capacity) {
		size_type next_capacity = m_capacity * DYNAMICARRAY_GROWTH_FACTOR;
		if (next_capacity == 0) {
			next_capacity = 1;
		}
		
		reserve(next_capacity);
	}
	
	m_data[m_size] = value;
	++m_size;
}
/**
 * Removes one element from the back of the array.
 * @throws	OutOfBoundsError	when the array is empty
 */
template <typename T>
void DynamicArray<T>::pop_back() {
	if (m_size <= 0) {
		throw OutOfBoundsError();
	}
	
	--m_size;
}
/**
 * Adds the provided element to the front of the array.
 *
 * This method needs to shift every element of the array one index forward
 * in order to work.
 *
 * @param	value	The value to place at the front of the array
 */
template <typename T>
void DynamicArray<T>::push_front(const T& value) {
	if (m_size == 0) {
		push_back(value);
	}
	else {
		push_back(m_data[m_size-1]);
		for (size_type i = m_size - 2; i > 1; --i) {
			m_data[i] = m_data[i-1];
		}
		m_data[0] = value;
	}
}
/**
 * Removes an element from the front of the array.
 *
 * This method needs to shift every element of the array one index backward
 * in order to work.
 *
 * @throws	OutOfBoundsError	when the array is empty
 */
template <typename T>
void DynamicArray<T>::pop_front() {
	if (m_size <= 0) {
		throw OutOfBoundsError();
	}
	
	for (size_type i = 1; i < m_size; ++i) {
		m_data[i-1] = m_data[i];
	}
	
	m_size--;
}
/**
 * Inserts an element before an arbitrary iterator in the array.
 * @throws	OutOfBoundsError	when the given iterator is beyond the array's end
 * @param	position	The location to insert the element before
 * @param	value	The value to insert into the array
 */
template <typename T>
void DynamicArray<T>::insert(const_iterator position, const T& value) {
	size_type index = position - m_data;
	
	if (m_size == m_capacity) {
		size_type next_capacity = m_capacity * DYNAMICARRAY_GROWTH_FACTOR;
		if (next_capacity == 0) {
			next_capacity = 1;
		}
		
		reserve(next_capacity);
	}
	
	for (size_type i = m_size; i > index; --i) {
		m_data[i] = m_data[i-1];
	}
	
	m_data[index = value];
	++m_size;
}
/**
 * Removes an element from an arbitrary point in the array.
 * @throws	OutOfBoundsError	when the given iterator is beyond the array's end
 * @param	position	An iterator pointing to the element to remove
 */
template <typename T>
void DynamicArray<T>::remove(const_iterator position) {
	if (m_size == 0) {
		throw OutOfBoundsError();
	}
	if (position < m_data || position >= m_data + m_size) {
		throw OutOfBoundsError();
	}
	
	for (T* itr = position + 1; itr < m_data + m_size; ++itr) {
		*itr = *(itr+1);
	}
	
	--m_size;
}

/**
 * Provides a reference to the first element in the array.
 * @throws	OutOfBoundsError	when the array is empty
 * @return	A reference to the first element of the array
 */
template <typename T>
T& DynamicArray<T>::front() {
	if (m_size == 0) {
		throw OutOfBoundsError();
	}
	
	return m_data[0];
}
/**
 * Provides a constant reference to the first element of the array.
 * @throws	OutOfBoundsError	when the array is empty
 * @return	A constant reference to the first element of the array
 */
template <typename T>
const T& DynamicArray<T>::front() const {
	if (m_size == 0) {
		throw OutOfBoundsError();
	}
	
	return m_data[0];
}
/**
 * Provides a reference to the last element of the array.
 * @throws	OutOfBoundsError	when the array is empty
 * @return	A reference to the last element of the array
 */
template <typename T>
T& DynamicArray<T>::back() {
	if (m_size == 0) {
		throw OutOfBoundsError();
	}
	
	return m_data[m_size - 1];
}
/**
 * Provides a constant reference to the last element of the array.
 * @throws	OutOfBoundsError	when the array is empty
 * @return	A constant reference to the last element of the array
 */
template <typename T>
const T& DynamicArray<T>::back() const {
	if (m_size == 0) {
		throw OutOfBoundsError();
	}
	
	return m_data[m_size - 1];
}

/**
 * Provides an iterator pointing to the start of the underlying array.
 *
 * For a DynamicArray, this is a simple pointer (T*) to the start of m_data.
 *
 * @return	An iterator at the start of this DynamicArray
 */
template <typename T>
typename DynamicArray<T>::iterator DynamicArray<T>::begin() {
	return m_data;
}
/**
 * Provides a const_iterator to the start of the underlying array.
 *
 * For a DynamicArray, this is a simple const pointer (const T*)
 * to the start of m_data.
 *
 * @return	A const_iterator to the start of this DynamicArray
 */
template <typename T>
typename DynamicArray<T>::const_iterator DynamicArray<T>::begin() const {
	return m_data;
}
/**
 * Provides an iterator pointing to the end of the underlying array.
 *
 * For a DynamicArray, this is a simple pointer (T*) to just past the end
 * of m_data.
 *
 * @return	An iterator at the start of this DynamicArray
 */
template <typename T>
typename DynamicArray<T>::iterator DynamicArray<T>::end() {
	return m_data + m_size;
}
/**
 * Provides a const_iterator to the end of the underlying array.
 *
 * For a DynamicArray, this is a simple const pointer (const T*)
 * to just past the end of m_data.
 *
 * @return	A const_iterator to the end of this DynamicArray
 */
template <typename T>
typename DynamicArray<T>::const_iterator DynamicArray<T>::end() const {
	return m_data + m_size;
}

/**
 * Copy-assigns the contents of another array to this array.
 *
 * The assigment is acheieved via the copy-and-swap idiom.
 * This means it relies on the copy constructor and a swap method
 * (provided as member function m_swap in this class).
 *
 * @param	other	The array to copy data from
 * @return	A reference to this after copying
 */
template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other) {
	DynamicArray<T> tmp(other);
	this->m_swap(tmp);
	return *this;
}
/**
 * Move-assigns the contents of another array to this array.
 * @param	other	The vector to swap data with
 * @return	A reference to this after the swap has happened
 */
template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray<T>&& other) {
	this->m_swap(other);
	return *this;
}
/**
 * Provides a reference to an element at any index of the array.
 * @throws	OutOfBoundsError	when the requested index is < 0 or > m_size
 * @param	index	The index in the array to retrieve data from
 * @return A reference to the data at the requested index
 */
template <typename T>
T& DynamicArray<T>::operator[](size_type index) {
	if (index < 0 || index >= m_size) {
		throw OutOfBoundsError();
	}
	
	return m_data[index];
}
/**
 * Provides a consant reference to an element at any index of the array.
 * @throws	OutOfBoundsError	when the requested index is < 0 or > m_size
 * @param	index	The index in the array to retrieve data from
 * @return A constant reference to the data at the requested index
 */
template <typename T>
const T& DynamicArray<T>::operator[](size_type index) const {
	if (index < 0 || index >= m_size) {
		throw OutOfBoundsError();
	}
	
	return m_data[index];
}
/**
 * Check if two arrays are equal.
 *
 * In this context, that will mean they are the same size and the contents
 * compare equal index-by-index.
 *
 * @param	other	The array to compare this against
 * @return	Boolean representing whether these arrays are equal
 */
template <typename T>
bool DynamicArray<T>::operator==(const DynamicArray<T>& other) const {
	if (this->m_size != other.m_size) {
		return false;
	}
	
	for (size_type i = 0; i < m_size; ++i) {
		if (this->m_data[i] != other.m_data[i]) {
			return false;
		}
	}
	
	return true;
}
/**
 * Check if two arrays are not equal.
 *
 * In this context, that will mean either they are different sizes or they have
 * at least one index at which the elements do not compare equal.
 *
 * @param	other	The array to compare this against
 * @return	Boolean representing whether these arrays are not equal
 */
template <typename T>
bool DynamicArray<T>::operator!=(const DynamicArray<T>& other) const {
	return (!(*this == other));
}

/**
 * Swaps the member variables of this with those of another DynamicArray.
 * @param	other	The array to swap contents with
 */
template <typename T>
void DynamicArray<T>::m_swap(DynamicArray<T>& other) {
	T* tmp_data;
	size_type tmp_size;
	size_type tmp_capacity;
	
	// Swap the data pointers.
	tmp_data = this->m_data;
	this->m_data = other.m_data;
	other.m_data = tmp_data;
	
	// Swap the sizes.
	tmp_size = this->m_size;
	this->m_size = other.m_size;
	other.m_size = tmp_size;
	
	// Swap the capacities.
	tmp_capacity = this->m_capacity;
	this->m_capacity = other.m_capacity;
	other.m_capacity = tmp_capacity;
}

#endif // Fundamentals_DynamicArray_hpp_
