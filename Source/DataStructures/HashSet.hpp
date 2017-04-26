/**
 * Fundamentals :: Data Structures :: Hash Set
 * Author: Quinn Mortimer
 *
 * This is an implementation of a Hash Set, which stores elements in an
 * unordered container in a way that guarantees uniqueness and allows for
 * efficient and accurate querying and insertion.
 * It is intended as a reference for those looking to brush up on important
 * data structures.
 */
/**
 * In this data structure, memory management is not something we're trying to
 * demonstrate. As such, we'll allow the use of some of the standard library.
 * We will be using std::vector and std::swap in the code.
 */

#include <cmath>
#include <cstddef>
#include <utility>
#include <vector>

#include "Exceptions.hpp"

#ifndef Fundamentals_HashSet_hpp_
#define Fundamentals_HashSet_hpp_

#define HASHSET_DEFAULT_CAPACITY 8
#define HASHSET_GROWTH_FACTOR 2
#define HASHSET_MAX_LOAD_FACTOR 0.75
#define HASHSET_COLLISION_SHIFT 4

/**
 * A Node class for the slots in our HashSet.
 *
 * Stores an element, as well as some flags for internal properties of
 * the node.
 */
template <typename T>
class HashSet_Node {
public:
	// Constructor and destructor for a node.
	HashSet_Node();
	~HashSet_Node();
	
	// The default copy constructor can potentially copy a pointer to
	// dynamically-allocated memory, and when the copy goes out of scope
	// that memory is freed prematurely. Node copying is avoided easily enough
	// and deleting the copy constructor will stop anyone mistakenly using it.
	HashSet_Node(const HashSet_Node<T>& other) = delete;
	
	// Set or clear the element on this node.
	void set(const T& elem);
	void clear();
	
	// Informational queries on this node.
	bool empty() const;
	bool unused() const;
	bool elemEqual(const T& elem) const;
	
	// Access to this node's data member.
	const T& elem() const;
private:
	// The element in the node.
	T* mp_elem;
	// A flag indicating whether or not this node has ever been used.
	bool m_unused;
};

/**
 * A HashSet data structure.
 */
template <typename T>
class HashSet {
	public:
		typedef size_t size_type;
		typedef size_type (*hash_type)(const T&);
		
		// Constructors
		HashSet();
		HashSet(const hash_type& hash);
		HashSet(const hash_type& hash, size_type size);
		HashSet(HashSet<T>&& other);
		HashSet(const HashSet<T>& other);
		
		// Dynamic memory isn't directly allocated,
		// so the default destructor is fine.
		~HashSet() = default;
		
		// Assignment
		HashSet<T>& operator=(const HashSet<T>& other);
		HashSet<T>& operator=(HashSet<T>&& other);
		
		// Equality Testing
		bool operator==(const HashSet<T>& other) const;
		bool operator!=(const HashSet<T>& other) const;
		
		// Access to current size
		size_type size() const;
		bool empty() const;
		
		// Element insertion and deletion
		// - Functions that can throw exceptions
		void insert(const T& elem);
		void remove(const T& elem);
		// - Function that will not throw exceptions
		void add(const T& elem);
		void discard(const T& elem);
		
		// Data Access
		// - Checks if an element is in the set
		bool contains(const T& elem) const;
		// - Gets all elements in the set.
		std::vector<T> elements() const;
		
	private:
		typedef HashSet_Node<T> Node;
		
		// The hash function for finding slot indices.
		hash_type m_hash;
		// The number of used slots in the underlying array.
		size_type m_size;
		// The size at which we will consider our HashSet too crowded.
		size_type m_loadThreshold;
		// The underlying array of nodes for our HashSet.
		std::vector<Node> m_nodes;
		
		// These are utilites for internal use.
		// - Used to resize and rehash when the load factor is too big.
		void m_rehash();
		// - Swaps contents with another HashSet.
		void m_swap(HashSet<T>& other);
		// - Adds all elements in another HashSet to the current set.
		void m_update(const HashSet<T>& other);
		
		// This is in some ways the real workhorse function for this class.
		// It's how every other member function finds where elements should be.
		size_type m_findIndex(const T& elem) const;
};

// ----------------//
// HashSet Methods //
// ----------------//
/**
 * Default HashSet constructor - Invalid
 *
 * A HashSet requires at least a hash function to be constructed.
 *
 * @throws	MissingHashFunctionError	always.
 */
template <typename T>
HashSet<T>::HashSet() {
	throw MissingHashFunctionError();
}
/**
 * Constucts a HashSet with a hash function for the element type.
 * @param	hash	The hash function for this to use.
 */
template <typename T>
HashSet<T>::HashSet(const hash_type& hash) {
	m_hash = hash;
	m_size = 0;
	m_loadThreshold = HASHSET_DEFAULT_CAPACITY * HASHSET_MAX_LOAD_FACTOR;
	
	m_nodes = std::vector<Node>(HASHSET_DEFAULT_CAPACITY);
}
/**
 * Constructs a HashSet from a hash function and a minimum load.
 *
 * The size represents a minimum for the amount of elements that can be added
 * to the set without needing to resize the underlying array and rehash.
 * The number of used slots after construction is 0.
 *
 * @param	hash	The hash function for this to use.
 * @param	size	An amount of elements the table should be able to hold.
 */
template <typename T>
HashSet<T>::HashSet(const hash_type& hash, size_type size) {
	m_hash = hash;
	m_size = 0;
	
	size = size / HASHSET_MAX_LOAD_FACTOR;
	size_type tableSize;
	
	if (size < HASHSET_DEFAULT_CAPACITY) {
		tableSize = HASHSET_DEFAULT_CAPACITY;
	}
	else {
		tableSize = 1;
		while (size > tableSize) {
			tableSize <<= 1;
		}
	}
	
	m_loadThreshold = tableSize * HASHSET_MAX_LOAD_FACTOR;
	
	m_nodes = std::vector<Node>(tableSize);
}
/**
 * Constructs a HashSet by copying the contents of another.
 * @param	other	The HashSet to copy.
 */
template <typename T>
HashSet<T>::HashSet(const HashSet<T>& other) {
	m_hash = other.m_hash;
	m_loadThreshold = other.m_loadThreshold;
	m_nodes = std::vector<Node>(other.m_nodes.size());
	m_size = 0;
	m_update(other);
}
/**
* Constructs a HashSet by swapping in the contents of another.
* @param	other	The set to swap contents with.
*/
template <typename T>
HashSet<T>::HashSet(HashSet<T>&& other) {
	m_swap(other);
}

/**
 * Copy-assigns the contents of one HashSet to another.
 * @param	other	The set to copy from.
 * @return	The modified version of this, after copying.
 */
template <typename T>
HashSet<T>& HashSet<T>::operator=(const HashSet<T>& other) {
	HashSet<T> tmp(other);
	m_swap(tmp);
	return *this;
}
/**
 * Move-assigns the contents of one HashSet to another.
 * @param	other	The set to swap contents with.
 * @return	The modified version of this, after the content swap.
 */
template <typename T>
HashSet<T>& HashSet<T>::operator=(HashSet<T>&& other) {
	m_swap(other);
	return *this;
}

/**
 * Checks to see if two HashSets are equal.
 *
 * This is not as straightforward as checking if sequence containers are equal.
 * We can't assume that all items would be at the same sequence in the
 * underlying array, so we need to iterate through the nodes on this array
 * and perform checks against the public interface of the other.
 *
 * @param	other	The HashSet to compare against.
 * @return	Whether or not all elements of both sets are the same.
 */
template <typename T>
bool HashSet<T>::operator==(const HashSet<T>& other) const {
	if (m_size != other.m_size) {
		return false;
	}
	
	for (size_type i = 0; i < m_nodes.size(); ++i) {
		const Node& n = m_nodes[i];
		if (!n.empty()) {
			if (!other.contains(n.elem())) {
				return false;
			}
		}
	}
	
	return true;
}
/**
 * Checks to see if two HashSets are unequal.
 *
 * @param	other	The HashSet to compare against.
 * @return	false if both sets contain the same elements, else true.
 */
template <typename T>
bool HashSet<T>::operator!=(const HashSet<T>& other) const {
	return !(*this == other);
}
 
/**
 * Reports the number of slots currently in-use in the HashSet.
 * @return	The number of elements in the set.
 */
template <typename T>
typename HashSet<T>::size_type HashSet<T>::size() const {
	return m_size;
}
/**
 * Reports whether or not the HashSet is empty.
 * @return	Whether there are any used slots in the set.
 */
template <typename T>
bool HashSet<T>::empty() const {
	return m_size == 0;
}

/**
 * Adds an element to the set.
 * @throws	DuplicateElementError	When the input is already in the set.
 * @param	elem	The element to add.
 */
template <typename T>
void HashSet<T>::insert(const T& elem) {
	if (m_size >= m_loadThreshold) {
		m_rehash();
	}
	
	size_type index = m_findIndex(elem);
	
	if (!m_nodes[index].empty()) {
		throw DuplicateElementError();
	}
	
	m_nodes[index].set(elem);
	++m_size;
}
/**
 * Removes a element from the set.
 * @throws	MissingElementError	If the requested element is not in the set.
 * @param	elem	The element to remove.
 */
template <typename T>
void HashSet<T>::remove(const T& elem) {
	size_type index = m_findIndex(elem);
	
	if (m_nodes[index].empty()) {
		throw MissingElementError();
	}
	
	m_nodes[index].clear();
	--m_size;
}
/**
 * Ensures that an element is in the set.
 *
 * This can be used as a permissive version of insert.
 * If an element is already in the set, nothing will happen.
 *
 * @param	elem	The element to add.
 */
template <typename T>
void HashSet<T>::add(const T& elem) {
	if (m_size >= m_loadThreshold) {
		m_rehash();
	}
	
	size_type index = m_findIndex(elem);
	
	if (m_nodes[index].empty()) {
		m_nodes[index].set(elem);
		++m_size;
	}
}
/**
 * Removes an element from the set.
 *
 * This is the permissive version of remove. If the element was not in the set
 * in the first place, it just won't do anything.
 *
 * @param	elem	The element to remove.
 */
template <typename T>
void HashSet<T>::discard(const T& elem) {
	size_type index = m_findIndex(elem);
	
	if (!m_nodes[index].empty()) {
		m_nodes[index].clear();
		--m_size;
	}
}

/**
 * Checks if an element is currently in the set.
 * @param	elem	The element to check for.
 * @return	A boolean representing whether or not the element exists.
 */
template <typename T>
bool HashSet<T>::contains(const T& elem) const {
	size_type index = m_findIndex(elem);
	return !m_nodes[index].empty();
}

/**
 * Gets all a sequence of all the elements in this set.
 *
 * Since a contiguous ordered sequence of elements is not something the HashSet
 * stores, it will have to be constructed when this method is called.
 *
 * @return	A vector containing all elements in this set
 */
template <typename T>
std::vector<T> HashSet<T>::elements() const {
	std::vector<T> elements(0);
	elements.reserve(m_size);
	
	for (size_type i = 0; elements.size() < m_size; i++) {
		if (!m_nodes[i].empty()) {
			elements.push_back(m_nodes[i].elem());
		}
	}
	
	return elements;
}

/**
 * Resizes the underlying array and moves all elements to their new positions.
 *
 * The method for doing this is close to a copy-and-swap technique.
 * The HashSet that we swap with is functionally identical to the one we copy
 * from, but the underlying vector will be larger.
 */
template <typename T>
void HashSet<T>::m_rehash() {
	HashSet<T> other(m_hash, HASHSET_GROWTH_FACTOR * m_size);
	
	for (size_type i = 0; i < m_nodes.size(); i++) {
		if (!m_nodes[i].empty()) {
			other.insert(m_nodes[i].elem());
		}
	}
	
	m_swap(other);
}

/**
 * Swaps the contents of this HashSet with that of another.
 * @param	other	Another HashSet to swap contents with
 */
template <typename T>
void HashSet<T>::m_swap(HashSet<T>& other) {
	std::swap(m_hash, other.m_hash);
	std::swap(m_nodes, other.m_nodes);
	std::swap(m_size, other.m_size);
	std::swap(m_loadThreshold, other.m_loadThreshold);
}
/**
 * Adds the contents of another HashSet into this set.
 * @param	other	The set to add the contents from.
 */
template <typename T>
void HashSet<T>::m_update(const HashSet<T>& other) {
	for (size_type i = 0; i < other.m_nodes.size(); i++) {
		if (!other.m_nodes[i].empty()) {
			insert(other.m_nodes[i].elem());
		}
	}
}

/**
 * Finds the index in the underlying array for an element.
 *
 * The index refers to the position of the slot in the array that this method
 * will tell whatever calls is where the provided element should be.
 * We will refer to that slot at this index as 'slot_e' from here forth.
 *
 * slot_e is guaranteed to either be empty or already contain the element.
 *
 * This class uses a open addressing to find slots for each element.
 * This is why our underlying array can be one-dimensional, but finding the
 * correct slot is not as straightforward as it could be with a two-dimensional
 * separate-chaining approach.
 *
 * Here we use probe sequence that is essentially identical to that used for
 * Python's dictionaries and sets.
 *
 * A full explanation of why this works is best not written in the comments of
 * this code, but the sequence is essentially defined by:
 * 	`idx = (idx * m + 1)  % sz`
 * where `idx` is the idx variable and `sz` is the size of our hashtable.
 * A constant `m` is what we wil call our probe multiplier.
 * In this function we'll use 5 as `m`.
 *
 * Note as the writer: While it might seem that you can use any value for `m`
 * that satisfied `m%2 == 1`, a small amount of testing will reveal that this
 * is not the case. The necessary condition really seems to be `m%4 == 1`.
 * Why exactly this is is unknown to me. In particular, I tried at one point to
 * use 7, which spectacularly fails at the minimum table size I chose of 8.
 *
 * @param	elem	The element to find the slot for
 * @return	The current best valid index for the element
 */
template <typename T>
typename HashSet<T>::size_type HashSet<T>::m_findIndex(const T& elem) const {
	size_type hashValue = m_hash(elem);
	size_type perturb = hashValue;
	
	size_type idx_current = hashValue % m_nodes.size();
	
	while (!m_nodes[idx_current].empty() && m_nodes[idx_current].elem() != elem) {
		idx_current = (idx_current * 5 + 1 + perturb) % m_nodes.size();
		perturb = perturb >> HASHSET_COLLISION_SHIFT;
	}
	
	size_type idx_firstCandidate = idx_current;
	
	while (!m_nodes[idx_current].unused() && !m_nodes[idx_current].elemEqual(elem)) {
		idx_current = (idx_current * 5 + 1 + perturb) % m_nodes.size();
		perturb = perturb >> HASHSET_COLLISION_SHIFT;
	}
	
	if (!m_nodes[idx_current].unused()) {
		return idx_current;
	}
	
	return idx_firstCandidate;
}

// ---------------------//
// HashSet_Node Methods //
// ---------------------//
/**
 * Constructor for a HashSet node.
 *
 * The node created does not have an element set.
 */
template <typename T>
HashSet_Node<T>::HashSet_Node()
	: mp_elem(nullptr), m_unused(true)
{}
	/**
	 * Destructor for a HashSet node.
	 *
	 * Deallocates the element on this node, if it exists.
	 */
template <typename T>
HashSet_Node<T>::~HashSet_Node() {
	clear();
}
/**
 * Sets the element on a node.
 *
 * Additionally, sets the unused flag on the node to false. This is relevant
 * for the HashSet when looking for an index for a particular element.
 *
 * @param	elem	The element that will be assigned to this node
 */
template <typename T>
void HashSet_Node<T>::set(const T& elem) {
	if (mp_elem == nullptr) {
		mp_elem = new T(elem);
	}
	else {
		*mp_elem = elem;
	}
	
	m_unused = false;
}
/**
 * Removes the element on this node, leaving an empty node.
 */
template <typename T>
void HashSet_Node<T>::clear() {
	if (mp_elem != nullptr) {
		delete mp_elem;
		mp_elem = nullptr;
	}
}
/**
 * Reports on whether or not this node is empty.
 * @return	Whether or not this node currently has an element
 */
template <typename T>
bool HashSet_Node<T>::empty() const {
	return mp_elem == nullptr;
}
/**
 * Reports whether the node has ever had an element.
 * @return The value of the unused flag on the node.
 */
template <typename T>
bool HashSet_Node<T>::unused() const {
	return m_unused;
}
/**
 * Checks if the provided element is equal to this node's element (if any).
 *
 * This is false if this node is empty or if its element is not equal to the
 * argument.
 *
 * @param	elem	The element to check against
 * @return	Whether or not this node refers to that element
 */
template <typename T>
bool HashSet_Node<T>::elemEqual(const T& elem) const {
	if (mp_elem == nullptr) {
		return false;
	}
	
	return elem == *mp_elem;
}
/**
 * Provides a constant reference to the element on this node.
 *
 * It's unsafe to use this method if this node is empty. That condition should
 * always be checked before use, as this method dereferences mp_elem
 * whether or not it is a null pointer.
 *
 * @return	The element at this node
 */
template <typename T>
const T& HashSet_Node<T>::elem() const {
	return *mp_elem;
}

#endif // Fundamentals_HashSet_hpp_
