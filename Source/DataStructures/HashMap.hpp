/**
 * Fundamentals :: Data Structures :: Hash Map
 * Author: Quinn Mortimer
 *
 * This is an implementation of a Hash Map, which is one approach to
 * making an Associative Array-style data strucure.
 * It is intended as a reference for those looking to brush up on  important
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

#ifndef Fundamentals_HashMap_hpp_
#define Fundamentals_HashMap_hpp_

#define HASHMAP_DEFAULT_CAPACITY 8
#define HASHMAP_GROWTH_FACTOR 2
#define HASHMAP_MAX_LOAD_FACTOR 0.75
#define HASHMAP_COLLISION_SHIFT 4;

/**
 * A Node class for the slots in our HashMap
 *
 * Stores the key and value, as well as some flags for internal properties of
 * the node.
 */
template <typename Key, typename Value>
class HashMap_Node {
public:
	// Constructor and destructor for a node.
	HashMap_Node();
	~HashMap_Node();
	
	// Set or clear the values on this node.
	void set(const Key& k, const Value& v);
	void clear();
	
	// Informational queries on this node.
	bool empty() const;
	bool unused() const;
	bool keyEqual(const Key& k) const;
	
	// Access to this node's important data members.
	const Key& key() const;
	const Value& value() const;
	Value& value();
private:
	// Key and value for the node.
	Key* mp_key;
	Value* mp_value;
	// A flag indicating whether or not this node has ever been used.
	bool m_unused;
};

/**
 * A HashMap data structure.
 */
template <typename Key, typename Value>
class HashMap {
	public:
		typedef size_t size_type;
		typedef size_type (*hash_type)(const Key&);
		
		// Constructors
		HashMap();
		HashMap(const hash_type& hash);
		HashMap(const hash_type& hash, size_type size);
		HashMap(HashMap<Key, Value>&& other);
		HashMap(const HashMap<Key, Value>& other);
		
		// Dynamic memory isn't directly allocated,
		// so the default destructor is fine
		~HashMap() = default;
		
		// Assignment
		HashMap<Key, Value>& operator=(const HashMap<Key, Value>& other);
		HashMap<Key, Value>& operator=(HashMap<Key, Value>&& other);
		
		// Equality Testing
		bool operator==(const HashMap<Key, Value>& other) const;
		bool operator!=(const HashMap<Key, Value>& other) const;
		
		// Access to current size
		size_type size() const;
		bool empty() const;
		
		// Element insertion and deletion
		// - Functions that can throw exceptions
		void insert(const Key& k, const Value& v);
		void remove(const Key& k);
		// - Function that will not throw exceptions
		void set(const Key& k, const Value& v);
		void unset(const Key& k);
		
		// Data Access
		// - Check for a key
		bool hasKey(const Key& k) const;
		// - Get elements by key
		Value& operator[](const Key& k);
		const Value& operator[](const Key& k) const;
		Value& getValue(const Key& k);
		const Value& getValue(const Key& k) const;
		// - Get all keys or all values
		std::vector<Key> keys() const;
		std::vector<Value> values() const;
		
	private:
		typedef HashMap_Node<Key, Value> Node;
		
		// The hash function for finding slot indices.
		hash_type m_hash;
		// The number of used slots in the underlying array.
		size_type m_size;
		// The size at which we will consider our hashmap too crowded.
		size_type m_loadThreshold;
		// The underlying array of nodes for our HashMap.
		std::vector<Node> m_nodes;
		
		// These are utilites for internal use.
		// - Used to resize and rehash when the load factor is too big.
		void m_rehash();
		// - Swaps contents with another HashMap.
		void m_swap(HashMap<Key, Value>& other);
		// - Adds all keys in another hash map to the current map.
		void m_update(const HashMap<Key, Value>& other);
		
		// This is in some ways the real workhorse function for this class.
		// It's how every other member function knows where keys should end up.
		size_type m_findIndex(const Key& key) const;
};

// ----------------//
// HashMap Methods //
// ----------------//
/**
 * Default HashMap constructor - Invalid
 *
 * A hash map requires at least a hash function to be constructed.
 *
 * @throws	MissingHashFunctionError	always.
 */
template <typename Key, typename Value>
HashMap<Key, Value>::HashMap() {
	throw MissingHashFunctionError();
}
/**
 * Constucts a HashMap with a hash function for the key type.
 * @param	hash	The hash function for this to use.
 */
template <typename Key, typename Value>
HashMap<Key, Value>::HashMap(const hash_type& hash) {
	m_hash = hash;
	m_size = 0;
	m_loadThreshold = HASHMAP_DEFAULT_CAPACITY * HASHMAP_MAX_LOAD_FACTOR;
	
	m_nodes = std::vector<Node>(HASHMAP_DEFAULT_CAPACITY);
}
/**
 * Constructs a HashMap from a hash function and a minimum load.
 *
 * The size represents a minimum for the amount of elements that can be added
 * to the map without needing to enlarge the underlying array and rehash.
 * The number of used slots after construction is 0.
 *
 * @param	hash	The hash function for this to use.
 * @param	size	An amount of elements the table should be able to hold.
 */
template <typename Key, typename Value>
HashMap<Key, Value>::HashMap(const hash_type& hash, size_type size) {
	m_hash = hash;
	m_size = 0;
	
	size = size / HASHMAP_MAX_LOAD_FACTOR;
	size_type tableSize;
	
	if (size < HASHMAP_DEFAULT_CAPACITY) {
		tableSize = HASHMAP_DEFAULT_CAPACITY;
	}
	else {
		tableSize = 1;
		while (size > tableSize) {
			tableSize <<= 1;
		}
	}
	
	m_loadThreshold = tableSize * HASHMAP_MAX_LOAD_FACTOR;
	
	m_nodes = std::vector<Node>(tableSize);
}
/**
 * Constructs a HashMap by copying the contents of another.
 * @param	other	The HashMap to copy.
 */
template <typename Key, typename Value>
HashMap<Key, Value>::HashMap(const HashMap<Key, Value>& other) {
	m_hash = other.m_hash;
	m_loadThreshold = other.m_loadThreshold;
	m_nodes = std::vector<Node>(other.m_nodes.size());
	m_size = 0;
	m_update(other);
}
/**
* Constructs a HashMap by swapping in the contents of another.
* @param	other	The map to swap contents with.
*/
template <typename Key, typename Value>
HashMap<Key, Value>::HashMap(HashMap<Key, Value>&& other) {
	m_swap(other);
}

/**
 * Copy-assigns the contents of one HashMap to another.
 * @param	other	The map to copy from.
 * @return	The modified version of this, after copying.
 */
template <typename Key, typename Value>
HashMap<Key, Value>& HashMap<Key, Value>::operator=(const HashMap<Key, Value>& other) {
	HashMap<Key, Value> tmp(other);
	m_swap(tmp);
	m_hash = other.m_hash;
	return *this;
}
/**
 * Move-assigns the contents of one HashMap to another.
 * @param	other	The map to swap contents with.
 * @return	The modified version of this, after the content swap.
 */
template <typename Key, typename Value>
HashMap<Key, Value>& HashMap<Key, Value>::operator=(HashMap<Key, Value>&& other) {
	m_swap(other);
	return *this;
}

/**
 * Checks to see if two HashMaps are equal.
 *
 * This is not as straightforward as checking if sequence containers are equal.
 * We can't assume that all items would be at the same sequence in the
 * underlying array, so we need to iterate through the nodes on this array
 * and perform checks against the public interface of the other.
 *
 * @param	other	The HashMap to compare against.
 * @return	true if all keys of both map to the same values, otherwise false.
 */
template <typename Key, typename Value>
bool HashMap<Key, Value>::operator==(const HashMap<Key, Value>& other) const {
	if (m_size != other.m_size) {
		return false;
	}
	
	for (size_type i = 0; i < m_nodes.size(); ++i) {
		const Node& n = m_nodes[i];
		if (!n.empty()) {
			if (!other.hasKey(n.key()) || other[n.key()] != n.value()) {
				return false;
			}
		}
	}
	
	return true;
}
/**
 * Checks to see if two HashMaps are unequal.
 *
 * @param	other	The HashMap to compare against.
 * @return	false if all keys of both map to the same values, otherwise true.
 */
template <typename Key, typename Value>
bool HashMap<Key, Value>::operator!=(const HashMap<Key, Value>& other) const {
	return !(*this == other);
}
 
/**
 * Reports the number of slots currently in-use in the HashMap.
 * @return	The number of valid key-value pairs in the map.
 */
template <typename Key, typename Value>
typename HashMap<Key, Value>::size_type HashMap<Key, Value>::size() const {
	return m_size;
}
/**
 * Reports whether or not the HashMap is empty.
 * @return	Whether there are any used slots in the map.
 */
template <typename Key, typename Value>
bool HashMap<Key, Value>::empty() const {
	return m_size == 0;
}

/**
 * Insert a key-value pair into the map.
 * @throws	DuplicateKeyError	When the key provided is already in the map.
 * @param	key	The key to insert a value for.
 * @param	value	The value that correspond to the key.
 */
template <typename Key, typename Value>
void HashMap<Key, Value>::insert(const Key& key, const Value& value) {
	if (m_size >= m_loadThreshold) {
		m_rehash();
	}
	
	size_type index = m_findIndex(key);
	
	if (!m_nodes[index].empty()) {
		throw DuplicateKeyError();
	}
	
	m_nodes[index].set(key, value);
	++m_size;
}
/**
 * Removes a key, and its associated value, from the map.
 * @throws	MissingKeyError	If the requested key is not in the map.
 * @param	key	The key to remove.
 */
template <typename Key, typename Value>
void HashMap<Key, Value>::remove(const Key& key) {
	size_type index = m_findIndex(key);
	
	if (m_nodes[index].empty()) {
		throw MissingKeyError();
	}
	
	m_nodes[index].clear();
	--m_size;
}
/**
 * Sets the value corresponding to a key in this map.
 *
 * This can be used as a permissive version of insert.
 * If the key was already in the map, it will just overwrite the old value.
 *
 * @param	key	The key to insert a value for.
 * @param	value	The value that correspond to the key.
 */
template <typename Key, typename Value>
void HashMap<Key, Value>::set(const Key& key, const Value& value) {
	if (m_size >= m_loadThreshold) {
		m_rehash();
	}
	
	size_type index = m_findIndex(key);
	
	if (m_nodes[index].empty()) {
		m_nodes[index].set(key, value);
		++m_size;
	}
	else {
		m_nodes[index].set(key, value);
	}
}
/**
 * Removes a key, and its associated value, from the map.
 *
 * This is the permissiver version of remove. If the key was not in the map
 * in the first place, it just won't do anything.
 *
 * @param	key	The key to remove.
 */
template <typename Key, typename Value>
void HashMap<Key, Value>::unset(const Key& key) {
	size_type index = m_findIndex(key);
	
	if (!m_nodes[index].empty()) {
		m_nodes[index].clear();
		--m_size;
	}
}

/**
 * Checks if a key is currently in the map.
 * @param	key	The key to check for.
 * @return	A boolean representing whether or not the key exists.
 */
template <typename Key, typename Value>
bool HashMap<Key, Value>::hasKey(const Key& key) const {
	size_type index = m_findIndex(key);
	return !m_nodes[index].empty();
}

/**
 * Gets a reference to the value stored at a given key.
 *
 * This is a strange situation compared to the vector, because the value isn't
 * actually guaranteed to exist before this, even if the key is valid.
 * In fact, all keys of type Key are valid, they just haven't all necessarily
 * been added to the map yet.
 *
 * @param	key	The key to get the mapped value for
 * @return	A reference to the value key refers to
 */
template <typename Key, typename Value>
Value& HashMap<Key, Value>::operator[](const Key& key) {
	size_type index = m_findIndex(key);
	
	if (m_nodes[index].empty()) {
		m_nodes[index].set(key, Value());
	}
	
	return m_nodes[index].value();
}
/**
 * Gets a constant reference to the value stored at a given key.
 *
 * Since this reference can't be assigned to, it's not necessary to allow
 * people to look up non-existing keys using this method.
 * In fact, we can't allow that as it would mean modifying the map to add a
 * new key-value pair.
 *
 * @throws	MissingKeyError	When the requested key is not in the map.
 * @param	key	The key to get the mapped value for
 * @return	A constant reference to the value key refers to
 */
template <typename Key, typename Value>
const Value& HashMap<Key, Value>::operator[](const Key& key) const {
	size_type index = m_findIndex(key);
	
	if (m_nodes[index].empty()) {
		throw MissingKeyError();
	}
	
	return m_nodes[index].value();
}
/**
 * Gets a reference to the value stored at a given key.
 *
 * Unlike the indexing operator, we're not going to require that this will
 * correct missing key errors for the user.
 *
 * @throws	MissingKeyError	When the requested key is not in the map.
 * @param	key	The key to get the mapped value for
 * @return	A reference to the value key refers to
 */
template <typename Key, typename Value>
Value& HashMap<Key, Value>::getValue(const Key& key) {
	size_type index = m_findIndex(key);
	
	if (m_nodes[index].empty()) {
		throw MissingKeyError();
	}
	
	return m_nodes[index].value();
}
/**
 * Gets a constant reference to the value stored at a given key.
 *
 * This function is the same as the constant version of the indexing operator,
 * but since there is a non-const method by this name, a const one should exist
 * also for consistency.
 *
 * @throws	MissingKeyError	When the requested key is not in the map.
 * @param	key	The key to get the mapped value for.
 * @return	A constant reference to the value key refers to
 */
template <typename Key, typename Value>
const Value& HashMap<Key, Value>::getValue(const Key& key) const {
	size_type index = m_findIndex(key);
	
	if (m_nodes[index].empty()) {
		throw MissingKeyError();
	}
	
	return m_nodes[index].value();
}

/**
 * Gets all a sequence of all the keys in this map.
 *
 * Since a contiguous ordered sequence of keys is not something the HashMap
 * stores, it will have to be constructed when this method is called.
 *
 * @return	A vector containing all keys in this map
 */
template <typename Key, typename Value>
std::vector<Key> HashMap<Key, Value>::keys() const {
	std::vector<Key> keys(0);
	keys.reserve(m_size);
	
	for (size_type i = 0; keys.size() < m_size; i++) {
		if (!m_nodes[i].empty()) {
			keys.push_back(m_nodes[i].key());
		}
	}
	
	return keys;
}
/**
 * Gets a sequence of all the values in this map.
 *
 * Since a contiguous ordered sequence of values is not something the HashMap
 * stores, it will have to be constructed when this method is called.
 *
 * Using this method is not recommended unless incurring the cost of copying
 * is acceptable. Generating the sequence of keys and iterating through that
 * is a better approach most of the time.
 *
 * @return	A vector containing all values in this map
 */
template <typename Key, typename Value>
std::vector<Value> HashMap<Key, Value>::values() const {
	std::vector<Value> values(0);
	values.reserve(m_size);
	
	for (size_type i = 0; values.size() < m_size; ++i) {
		if (!m_nodes[i].empty()) {
			values.push_back(m_nodes[i].value());
		}
	}
	
	return values;
}

/**
 * Resizes the underlying array and moves all elements to their new positions.
 *
 * The method for doing this is close to a copy-and-swap technique.
 * The HashMap that we swap with is functionally identical to the one we copy
 * from, but the underlying vector will be larger.
 */
template <typename Key, typename Value>
void HashMap<Key, Value>::m_rehash() {
	HashMap<Key, Value> other(m_hash, HASHMAP_GROWTH_FACTOR * m_size);
	
	for (size_type i = 0; i < m_nodes.size(); i++) {
		if (!m_nodes[i].empty()) {
			other.insert(m_nodes[i].key(), m_nodes[i].value());
		}
	}
	
	m_swap(other);
}

/**
 * Swaps the contents of this hashmap with that of another.
 * @param	other	Another HashMap to swap contents with
 */
template <typename Key, typename Value>
void HashMap<Key, Value>::m_swap(HashMap<Key, Value>& other) {
	std::swap(m_hash, other.m_hash);
	std::swap(m_nodes, other.m_nodes);
	std::swap(m_size, other.m_size);
	std::swap(m_loadThreshold, other.m_loadThreshold);
}
/**
 * Adds the contents of another HashMap into this map.
 * @param	other	The map to add the contents from.
 */
template <typename Key, typename Value>
void HashMap<Key, Value>::m_update(const HashMap<Key, Value>& other) {
	for (size_type i = 0; i < other.m_nodes.size(); i++) {
		if (!other.m_nodes[i].empty()) {
			insert(other.m_nodes[i].key(), other.m_nodes[i].value());
		}
	}
}

/**
 * Finds the index in the underlying array that the key maps to.
 *
 * The index refers to the position of the slot in the array that this method
 * will tell whatever calls is where the provided key should be or should go.
 * We will refer to that slot at this index as 'slot_k' from here forth.
 *
 * slot_k is guaranteed to either be empty or already contain the key.
 *
 * This class uses a open addressing to find slots for each key.
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
 * @param	key	The key to find the slot for
 * @return	The current best valid index for key
 */
template <typename Key, typename Value>
typename HashMap<Key, Value>::size_type HashMap<Key, Value>::m_findIndex(const Key& key) const {
	size_type hashValue = m_hash(key);
	size_type perturb = hashValue;
	
	size_type idx_current = hashValue % m_nodes.size();
	
	Node n = m_nodes[idx_current];
	
	while (!n.empty() && n.key() != key) {
		perturb = perturb >> HASHMAP_COLLISION_SHIFT;
		idx_current = (idx_current * 5 + 1 + perturb) % m_nodes.size();
		n = m_nodes[idx_current];
	}
	
	size_type idx_firstCandidate = idx_current;
	
	while (!n.unused() && !n.keyEqual(key)) {
		perturb = perturb >> HASHMAP_COLLISION_SHIFT;
		idx_current = (idx_current * 5 + 1 + perturb) % m_nodes.size();
		n = m_nodes[idx_current];
	}
	
	if (!n.unused()) {
		return idx_current;
	}
	
	return idx_firstCandidate;
}

// ---------------------//
// HashMap_Node Methods //
// ---------------------//
/**
 * Constructor for a HashMap node.
 *
 * The node created does not have a key or value set.
 */
template <typename Key, typename Value>
HashMap_Node<Key, Value>::HashMap_Node()
	: mp_key(nullptr), mp_value(nullptr), m_unused(true)
{}
	/**
	 * Destructor for a HashMap node.
	 *
	 * Deallocates the key and value on this node, if they exist.
	 */
template <typename Key, typename Value>
HashMap_Node<Key, Value>::~HashMap_Node() {
	clear();
}
/**
 * Sets the key and value of a node.
 *
 * Additionally, sets the unused flag on the node to false. This is relevant
 * for the HashMap when looking for an index for a particular key.
 *
 * @param	k	The key that will be assigned to this node
 * @param	v	The value that will be assigned to this node
 */
template <typename Key, typename Value>
void HashMap_Node<Key, Value>::set(const Key& k, const Value& v) {
	if (mp_key == nullptr) {
		mp_key = new Key(k);
		mp_value = new Value(v);
	}
	else {
		*mp_key = k;
		*mp_value = v;
	}
	
	m_unused = false;
}
/**
 * Removes the key and value of this node, leaving an empty node.
 */
template <typename Key, typename Value>
void HashMap_Node<Key, Value>::clear() {
	if (mp_key != nullptr) {
		delete mp_key;
		delete mp_value;
		mp_key = nullptr;
		mp_value = nullptr;
	}
}
/**
 * Reports on whether or not this node is empty.
 * @return	Whether or not this node currently has a key (and value) set
 */
template <typename Key, typename Value>
bool HashMap_Node<Key, Value>::empty() const {
	return mp_key == nullptr;
}
/**
 * Reports whether the node has ever had a key (and value) set.
 * @return The value of the unused flag on the node.
 */
template <typename Key, typename Value>
bool HashMap_Node<Key, Value>::unused() const {
	return m_unused;
}
/**
 * Checks if the provided key is equal to this node's key (if any).
 *
 * This is false if this node has no key or if its key and the argument differ.
 *
 * @param	k	The key to check against
 * @return	Whether or not this node refers to that key
 */
template <typename Key, typename Value>
bool HashMap_Node<Key, Value>::keyEqual(const Key& k) const {
	if (mp_key == nullptr) {
		return false;
	}
	
	return k == *mp_key;
}
/**
 * Provides a constant reference to the key on this node.
 *
 * It's unsafe to use this method if this node is empty. That condition should
 * always be checked before use, as this method dereferences mp_key
 * whether or not it is a null pointer.
 *
 * @return	The key for this node
 */
template <typename Key, typename Value>
const Key& HashMap_Node<Key, Value>::key() const {
	return *mp_key;
}
/**
 * Provides a constant reference to the key on this node.
 *
 * It's unsafe to use this method if this node is empty. That condition should
 * always be checked before use, as this method dereferences mp_value
 * whether or not it is a null pointer.
 *
 * @return	The value for this node
 */
template <typename Key, typename Value>
const Value& HashMap_Node<Key, Value>::value() const {
	return *mp_value;
}
/**
 * Provides a reference to the key on this node.
 *
 * It's unsafe to use this method if this node is empty. That condition should
 * always be checked before use, as this method dereferences mp_value
 * whether or not it is a null pointer.
 *
 * @return	The value for this node
 */
template <typename Key, typename Value>
Value& HashMap_Node<Key, Value>::value() {
	return *mp_value;
}

#endif // Fundamentals_HashMap_hpp_
