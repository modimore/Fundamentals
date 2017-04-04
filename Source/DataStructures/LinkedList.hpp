/**
 * Fundamentals :: Data Stuctures :: Linked List
 * Author: Quinn Mortimer
 *
 * This is an implementation of a doubly-linked list data structure.
 * It is intended as a reference for those looking to brush up on important
 * data structures.
 */

#include <cstddef>

#include "Exceptions.hpp"

#ifndef Fundamentals_LinkedList_hpp_
#define Fundamentals_LinkedList_hpp_

// Forward declaration of the LinkedList class.
template <typename T> class LinkedList;
// Forward declaration of our two iterator classes.
template <typename T> class LinkedList_Iterator;
template <typename T> class LinkedList_ConstIterator;

/**
 * A small simple structure that LinkedLists can use.
 *
 * Constains the data it is meant to keep track of,
 * as well as pointers to its neighbour nodes.
 */
template <typename T>
struct LinkedList_Node {
	T data;
	struct LinkedList_Node<T>* prev;
	struct LinkedList_Node<T>* next;
};

/**
 * A doubly-linked list with bidirectional iterators.
 */
template <typename T>
class LinkedList {
	public:
		typedef size_t size_type;
		typedef LinkedList_Iterator<T> iterator;
		typedef LinkedList_ConstIterator<T> const_iterator;
		
		// Constructors and Destructor
		LinkedList();
		LinkedList(size_type size);
		LinkedList(size_type size, const T& value);
		LinkedList(const LinkedList<T>& other);
		LinkedList(LinkedList<T>&& other);
		~LinkedList();
		
		// Access to current size
		size_type size() const;
		bool empty() const;
		
		// Element insertion and deletion.
		// - Front
		void push_front(const T& value);
		void pop_front();
		// - Back
		void push_back(const T& value);
		void pop_back();
		// - Arbitrary
		void insert(iterator location, const T& value);
		void remove(iterator location);
		
		// Iterators to the start and end of the list
		iterator begin();
		const_iterator begin() const;
		iterator end();
		const_iterator end() const;
		
		// Direct element access at the front and the back.
		T& front();
		const T& front() const;
		T& back();
		const T& back() const;
		
		// Operators
		// - Asssignment
		LinkedList<T>& operator=(const LinkedList<T>& other);
		LinkedList<T>& operator=(LinkedList<T>&& other);
		// - Equality testing
		bool operator==(const LinkedList<T>& other) const;
		bool operator!=(const LinkedList<T>& other) const;
		
	private:
		typedef LinkedList_Node<T> Node;
		
		Node* m_firstNode;
		Node* m_lastNode;
		size_type m_size;
		
		void m_initialize(size_type a_size, const T& a_value);
		void m_swap(LinkedList<T>& other);
};

/**
 * An iterator over a LinkedList that provides read-write data access.
 */
template <typename T>
class LinkedList_Iterator {
	public:
		// Constructors
		LinkedList_Iterator();
		LinkedList_Iterator(const LinkedList_Iterator<T>& other);
		
		// Assignement operator
		LinkedList_Iterator<T>& operator=(const LinkedList_Iterator<T>& other);
		
		// Equality test operators
		bool operator==(const LinkedList_Iterator<T>& other) const;
		bool operator!=(const LinkedList_Iterator<T>& other) const;
		bool operator==(const LinkedList_ConstIterator<T>& other) const;
		bool operator!=(const LinkedList_ConstIterator<T>& other) const;
		
		// Data access
		T& operator*() const;
		T* operator->() const;
		
		// Increment and Decrement operators
		LinkedList_Iterator<T>& operator++();
		LinkedList_Iterator<T> operator++(int);
		LinkedList_Iterator<T>& operator--();
		LinkedList_Iterator<T> operator--(int);
	private:
		typedef struct LinkedList_Node<T> Node;
		// The linked list which the node in this iterator belongs to.
		LinkedList<T>* r_owner;
		// The node this iterator is pointing to.
		Node* r_node;
		
		// Node-and-owner constructor
		// This is on its own in the private section because
		// the user should not call it, but it is used by the LinkedList class.
		LinkedList_Iterator(Node* n, LinkedList<T>* owner);
	
	// LinkedList and LinkedList_ConstIterator both need access to private
	// members of this class.
	friend class LinkedList<T>;
	friend class LinkedList_ConstIterator<T>;
};

/**
 * An iterator over a LinkedList that provides read-only data access.
 */
template <typename T>
class LinkedList_ConstIterator {
	public:
		// Constructors
		LinkedList_ConstIterator();
		LinkedList_ConstIterator(const LinkedList_ConstIterator<T>& other);
		LinkedList_ConstIterator(const LinkedList_Iterator<T> &other);
		
		// Assignemnt operators
		LinkedList_ConstIterator<T>& operator=(const LinkedList_ConstIterator<T>& other);
		LinkedList_ConstIterator<T>& operator=(const LinkedList_Iterator<T>& other);
		
		// Equality test operators
		bool operator==(const LinkedList_ConstIterator<T>& other) const;
		bool operator!=(const LinkedList_ConstIterator<T>& other) const;
		bool operator==(const LinkedList_Iterator<T>& other) const;
		bool operator!=(const LinkedList_Iterator<T>& other) const;
		
		// Data access operators
		const T& operator*() const;
		const T* operator->() const;
		
		// Increment and Decrement operators
		LinkedList_ConstIterator<T>& operator++();
		LinkedList_ConstIterator<T> operator++(int);
		LinkedList_ConstIterator<T>& operator--();
		LinkedList_ConstIterator<T> operator--(int);
	private:
		typedef struct LinkedList_Node<T> Node;
		const LinkedList<T>* r_owner;
		const Node* r_node;
		
		// Node-and-owner constructor
		// This is on its own in the private section because
		// the user should not call it, but it is used by the LinkedList class.
		LinkedList_ConstIterator(const Node* n, const LinkedList<T>* owner);
	
	// LinkedList and LinkedList_Iterator both need access to private
	// members of this class.
	friend class LinkedList<T>;
	friend class LinkedList_Iterator<T>;
};

// ------------------- //
// Linked List Methods //
// ------------------- //
/**
 * Constructs an empty LinkedList
 */
template <typename T>
LinkedList<T>::LinkedList() {
	m_firstNode = nullptr;
	m_lastNode = nullptr;
	m_size = 0;
}
/**
 * Constructs a LinkedList with a known starting size.
 *
 * Nodes initially in the list will have default-constructed data.
 *
 * @param	size	The number of nodes to be created initially
 */
template <typename T>
LinkedList<T>::LinkedList(size_type size) {
	m_initialize(size, T());
}
/**
 * Constructs a LinkedList with a starting size and a default value.
 *
 * Nodes initially in this list will copy provided value to their data.
 *
 * @param	size	The number of nodes to create
 * @param	value	The value to copy to the starting nodes
 */
template <typename T>
LinkedList<T>::LinkedList(size_type size, const T& value) {
	m_initialize(size, value);
}
/**
 * Constructs a LinkedList by copying another element-wise.
 * @param	other	The LinkedList to copy from
 */
template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) {
	if (other.m_firstNode != nullptr) {
		// Initialize our two swap variables.
		Node* curr = nullptr;
		Node* prev;
		
		// Iterate forward, link backwards
		for (const Node* n_other = other.m_firstNode; n_other != nullptr; n_other = n_other->next) {
			prev = curr;
			curr = new Node { n_other->data, prev, nullptr };
		}
		
		m_lastNode = curr;
		
		// Iterate backwards, link forwards
		while (curr->prev != nullptr) {
			curr->prev->next = curr;
			curr = curr->prev;
		}
		
		m_firstNode = curr;
	}
	
	m_size = other.m_size;
}
/**
 * Constructs a LinkedList by swapping members with another.
 * @param	other	The LinkedList to swap with
 */
template <typename T>
LinkedList<T>::LinkedList(LinkedList<T>&& other) {
	m_firstNode = nullptr;
	
	m_swap(other);
}
/**
 * Destructor for a LinkedList.
 *
 * Clears the contents of this list so memory is not leaked.
 */
template <typename T>
LinkedList<T>::~LinkedList() {
	while (m_firstNode != nullptr) {
		Node* tmp = m_firstNode;
		m_firstNode = m_firstNode->next;
		delete tmp;
	}
}

/**
 * Gets the size of the linked list for the user.
 * @return	The number of nodes in the list
 */
template <typename T>
typename LinkedList<T>::size_type LinkedList<T>::size() const {
	return m_size;
}
/**
 * Reports whether or not the list is empty.
 * @return	Whether or not there are any nodes in the list
 */
template <typename T>
bool LinkedList<T>::empty() const {
	return m_firstNode == nullptr;
}

/**
 * Adds a new node at the start of the list with specified data.
 * @param	value	The data that the new node should hold
 */
template <typename T>
void LinkedList<T>::push_front(const T& value) {
	Node* tmp = m_firstNode;
	m_firstNode = new Node;
	m_firstNode->data = value;
	m_firstNode->prev = nullptr;
	m_firstNode->next = tmp;
	
	if (m_lastNode == nullptr) {
		m_lastNode = m_firstNode;
	}
	
	++m_size;
}
/**
 * Removes the first node from the list.
 */
template <typename T>
void LinkedList<T>::pop_front() {
	if (m_firstNode != nullptr) {
		Node* tmp = m_firstNode;
		m_firstNode = m_firstNode->next;
		
		if (m_firstNode != nullptr) {
			m_firstNode->prev = nullptr;
		}
		
		if (m_lastNode == tmp) {
			m_lastNode = nullptr;
		}
		
		--m_size;
		delete tmp;
	}
}
/**
 * Adds a new node at the end of the list with specified data.
 * @param	value	The data that the new node should hold
 */
template <typename T>
void LinkedList<T>::push_back(const T& value) {
	Node* tmp = m_lastNode;
	m_lastNode = new Node;
	m_lastNode->data = value;
	m_lastNode->prev = tmp;
	m_lastNode->next = nullptr;
	
	if (tmp != nullptr) {
		tmp->next = m_lastNode;
	}
	
	if (m_firstNode == nullptr) {
		m_firstNode = m_lastNode;
	}
	
	++m_size;
}
/**
 * Removes a node from the end of the list.
 */
template <typename T>
void LinkedList<T>::pop_back() {
	if (m_lastNode != nullptr) {
		Node* tmp = m_lastNode;
		m_lastNode = m_lastNode->prev;
		
		if (m_lastNode != nullptr) {
			m_lastNode->next = nullptr;
		}
		
		if (m_firstNode == tmp) {
			m_firstNode = nullptr;
		}
		
		--m_size;
		delete tmp;
	}
}
/**
 * Inserts a new node into the list before the provided iterator's node.
 *
 * If the provided iterator is the end or the beginning of the list,
 * this will use the dedicated method to adding to those locations.
 *
 * When the provided iterator's owner is not this, an exception is thrown.
 * This check is not performed by std::list, which can result in a
 * std::list having an inconsistent state (size != number of nodes).
 */
template <typename T>
void LinkedList<T>::insert(iterator location, const T& value) {
	if (location.r_owner != this) {
		throw MismatchedIteratorError();
	}
	
	if (location == end()) {
		push_back(value);
	}
	else if (location == begin()) {
		push_front(value);
	}
	else {
		Node* n = new Node;
		n->data = value;
		n->next = location.r_node;
		n->prev = n->next->prev;
		n->next->prev = n;
		n->prev->next = n;
		++m_size;
	}
}
/**
 * Removes the node before the provided iterator's node from the list.
 *
 * If the provided iterator is the end or the beginning of the list,
 * this will use the dedicated method to adding to those locations.
 *
 * When the provided iterator's owner is not this, an exception is thrown.
 * This check is not performed by std::list, which can result in a
 * std::list having an inconsistent state (size != number of nodes).
 */
template <typename T>
void LinkedList<T>::remove(iterator location) {
	if (location.r_owner != this) {
		throw MismatchedIteratorError();
	}
	
	if (location == begin()) {
		pop_front();
	}
	else if (location.r_node == m_lastNode) {
		pop_back();
	}
	else {
		Node* n = location.r_node;
		if (n->prev != nullptr) {
			n->prev->next = n->next;
		}
		if (n->next != nullptr) {
			n->next->prev = n->prev;
		}
		delete n;
	}
}

/**
 * Provides an iterator pointing to the start of the list.
 * @return An iterator pointing at the list's first node
 */
template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::begin() {
	return iterator(m_firstNode, this);
}
/**
 * Provides a const_iterator point to the start of the list.
 * @return	A const_iterator pointing to the list's first node
 */
template <typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::begin() const {
	return const_iterator(m_firstNode, this);
}
/**
 * Provides an iterator pointing to the end of the list.
 *
 * Though the iterator's node is the null pointer, it can be decremented
 * because it keeps a pointer to this list as its owner also.
 *
 * @return An iterator to just past the end of the list
 */
template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::end() {
	return iterator(nullptr, this);
}
/**
 * Provides a const_iterator pointing to the end of the list.
 *
 * Though the iterator's node is the null pointer, it can be decremented
 * because it keeps a pointer to this list as its owner also.
 *
 * @return A const_iterator to just past the end of the list
 */
template <typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::end() const {
	return const_iterator(nullptr, this);
}

/**
 * Provides a reference to the first element in the list.
 * @throws	OutOfBoundsError	when the list is empty
 * @return	A reference to the first element of the list
 */
template <typename T>
T& LinkedList<T>::front() {
	if (m_firstNode == nullptr) {
		throw OutOfBoundsError();
	}
	
	return m_firstNode->data;
}
/**
 * Provides a constant reference to the first element of the list.
 * @throws	OutOfBoundsError	when the list is empty
 * @return	A constant reference to the first element of the list
 */
template <typename T>
const T& LinkedList<T>::front() const {
	if (m_firstNode == nullptr) {
		throw OutOfBoundsError();
	}
	
	return m_firstNode->data;
}
/**
 * Provides a reference to the last element of the list.
 * @throws	OutOfBoundsError	when the list is empty
 * @return	A reference to the last element of the list
 */
template <typename T>
T& LinkedList<T>::back() {
	if (m_lastNode == nullptr) {
		throw OutOfBoundsError();
	}
	
	return m_lastNode->data;
}
/**
 * Provides a constant reference to the last element of the list.
 * @throws	OutOfBoundsError	when the list is empty
 * @return	A constant reference to the last element of the list
 */
template <typename T>
const T& LinkedList<T>::back() const {
	if (m_lastNode == nullptr) {
		throw OutOfBoundsError();
	}
	
	return m_lastNode->data;
}

/**
 * Uses the copy-and-swap technique to copy another list's contents into this.
 * @return	This LinkedList, after copying
 */
template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other) {
	LinkedList<T> tmp(other);
	this->m_swap(tmp);
	return *this;
}
/**
 * Swaps the contents of this linked list with another.
 * @return	This LinkedList, after copying
 */
template <typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList<T>&& other) {
	this->m_swap(other);
	return *this;
}
/**
 * Check if two linked lists are equal.
 *
 * In this context, that will mean they are the same size and the contents
 * compare equal node-by-node.
 *
 * @param	other	The linked list to compare this against
 * @return	Boolean representing whether these linked lists are equal
 */
template <typename T>
bool LinkedList<T>::operator==(const LinkedList<T>& other) const {
	if (this->m_size != other.m_size) {
		return false;
	}
	
	Node* this_n = this->m_firstNode;
	Node* other_n = other.m_firstNode;
	
	while( this_n != nullptr) {
		if (this_n->data != other_n->data) {
			return false;
		}
		
		this_n = this_n->next;
		other_n = other_n->next;
	}
	
	return true;
}
/**
 * Check if two linked lists are not equal.
 *
 * In this context, that will mean either they are different sizes or the
 * contents do not compare equal node-by-node.
 *
 * @param	other	The linked list to compare this against
 * @return	Boolean representing whether these linked lists are not equal
 */
template <typename T>
bool LinkedList<T>::operator!=(const LinkedList<T>& other) const {
	return (!(*this == other));
}

/**
 * Creates the desired number of nodes with the specified value.
 * @param	a_size	The number of nodes to create
 * @param	a_value	The data that should be stored on all nodes
 */
template <typename T>
void LinkedList<T>::m_initialize(size_type a_size, const T& a_value) {
	if (a_size > 0) {
		Node* tmpFirst;
		Node* tmpLast;
		Node* tmp;
		
		tmpFirst = new Node { a_value, nullptr, nullptr };
		tmpLast = tmpFirst;
		
		for (size_type i = 1; i < a_size; i++) {
			tmp = new Node { a_value, tmpLast, nullptr };
			tmpLast->next = tmp;
			tmpLast = tmp;
			
			++m_size;
		}
		
		m_firstNode = tmpFirst;
		m_lastNode = tmpLast;
		m_size = a_size;
	}
}
/**
 * Swaps the content of this LinkedList and another LinkedList.
 * @param	other	The LinkedList to swap with
 */
template <typename T>
void LinkedList<T>::m_swap(LinkedList<T>& other) {
	Node* tmp_firstNode;
	Node* tmp_lastNode;
	size_type tmp_size;
	
	// Swap the first nodes.
	tmp_firstNode = this->m_firstNode;
	this->m_firstNode = other.m_firstNode;
	other.m_firstNode = tmp_firstNode;
	
	// Swap the last nodes.
	tmp_lastNode = this->m_lastNode;
	this->m_lastNode = other.m_lastNode;
	other.m_lastNode = tmp_lastNode;
	
	// Swap the sizes.
	tmp_size = this->m_size;
	this->m_size = other.m_size;
	other.m_size = tmp_size;
}

// --------------------------- //
// LinkedList_Iterator methods //
// --------------------------- //
/**
 * Creates an iterator with no node or owner.
 */
template <typename T>
LinkedList_Iterator<T>::LinkedList_Iterator()
	: r_owner(nullptr), r_node(nullptr)
{}
/**
 * The copy constructor for a linked list iterator.
 *
 * Given another linked list iterator, initializes this to point at the
 * same node and the same list.
 *
 * @param	other	The iterator to copy from
 */
template <typename T>
LinkedList_Iterator<T>::LinkedList_Iterator(const LinkedList_Iterator<T>& other)
	: r_owner(other.r_owner), r_node(other.r_node)
{}
/**
 * Assignement operator from a LinkedList_Iterator.
 * @param	other	The iterator to copy from
 */
template <typename T>
LinkedList_Iterator<T>& LinkedList_Iterator<T>::operator=(const LinkedList_Iterator<T>& other) {
	r_owner = other.r_owner;
	r_node = other.r_node;
	return *this;
}
/**
 * Constructs an iterator from a node and an owning list.
 *
 * This method is private and expected to only be called by a LinkedList
 * in order to create an iterator to one of its nodes.
 *
 * @param	n	A pointer to the node this iterator should point to
 * @param	owner	A pointer to the list that this iterator will belong to
 */
template <typename T>
LinkedList_Iterator<T>::LinkedList_Iterator(Node* n, LinkedList<T>* owner)
	: r_owner(owner), r_node(n)
{}

/**
 * Dereferences this iterator to get the data at the current node.
 *
 * This method allows the end-user to effectively treat this iterator as a
 * pointer to the data, which can be read from and written to using this
 * method.
 *
 * @return	A reference to the data object on the current node
 */
template <typename T>
T& LinkedList_Iterator<T>::operator*() const {
	return r_node->data;
}
/**
 * Facilitates member access to the underlying node's data.
 *
 * This method allows the end-user to effectively treat this iterator as a
 * pointer to the data, which can be read from and written to using this
 * method.
 *
 * @return	A pointer to the data in question, used for member access
 */
template <typename T>
T* LinkedList_Iterator<T>::operator->() const {
	return &r_node->data;
}
/**
 * Implementation of a pre-increment on an iterator.
 *
 * Moves the node pointer to the next node in the list, then returns
 * a reference to this iterator. Anything that is done with the result of
 * this method call will use the incremented version of this.
 *
 * This method is invoked when `++itr` is written in the program.
 *
 * @return	This, after the node pointer has been changed
 */
template <typename T>
LinkedList_Iterator<T>& LinkedList_Iterator<T>::operator++() {
	if (r_node == nullptr) {
		throw OutOfBoundsError();
	}
	
	r_node = r_node->next;
	
	return *this;
}
/**
 * Implementation of a post-increment on an iterator.
 *
 * Creates a copy of this iterator, then moves the node pointer to the
 * next node in the list, then returns a reference to the copied iterator.
 * Anything that is done with the result of this method call will use the
 * an iterator which has not been incremented.
 *
 * This method is invoked when `itr++` is written in the program.
 *
 * @return	A copy of this before the node pointer was changed
 */
template <typename T>
LinkedList_Iterator<T> LinkedList_Iterator<T>::operator++(int) {
	if (r_node == nullptr) {
		throw OutOfBoundsError();
	}
	
	LinkedList_Iterator<T> tmp(*this);
	
	r_node = r_node->next;
	
	return tmp;
}
/**
 * Implementation of a pre-decrement on an iterator.
 *
 * Moves the node pointer to the previous node in the list, then returns
 * a reference to this iterator. Anything that is done with the result of
 * this method call will use the decremented version of this.
 *
 * This method is invoked when `--itr` is written in the program.
 *
 * @return	This, after the node pointer has been changed
 */
template <typename T>
LinkedList_Iterator<T>& LinkedList_Iterator<T>::operator--() {
	if (r_node == r_owner.m_firstNode) {
		throw OutOfBoundsError();
	}
	
	if (r_node == nullptr) {
		r_node = r_owner->m_lastNode;
	}
	else {
		r_node = r_node->prev;
	}
	
	return *this;
}
/**
 * Implementation of a post-decrement on an iterator.
 *
 * Creates a copy of this iterator, then moves the node pointer to the
 * previous node in the list, then returns a reference to the copied iterator.
 * Anything that is done with the result of this method call will use the
 * an iterator which has not been decremented.
 *
 * This method is invoked when `itr--` is written in the program.
 *
 * @return	A copy of this before the node pointer was changed
 */
template <typename T>
LinkedList_Iterator<T> LinkedList_Iterator<T>::operator--(int) {
	if (r_node == r_owner.m_firstNode) {
		throw OutOfBoundsError();
	}
	
	LinkedList_Iterator<T> tmp(*this);
	
	if (r_node == nullptr) {
		r_node = r_owner->m_lastNode;
	}
	else {
		r_node = r_node->prev;
	}
	return tmp;
}

/**
 * Checks if two iterators are equal.
 * @return	true if they point to the same owner and node, false otherwise
 */
template <typename T>
bool LinkedList_Iterator<T>::operator==(const LinkedList_Iterator<T>& other) const {
	return (r_owner == other.r_owner && r_node == other.r_node);
}
/**
 * Checks if two iterators are unequal.
 * @return	true if they point to different owners or different nodes
 */
template <typename T>
bool LinkedList_Iterator<T>::operator!=(const LinkedList_Iterator<T>& other) const {
	return (r_owner != other.r_owner || r_node != other.r_node);
}
/**
 * Checks if an iterator and a const_iterator are equal.
 * @return	true if they point to the same owner and node, false otherwise
 */
template <typename T>
bool LinkedList_Iterator<T>::operator==(const LinkedList_ConstIterator<T>& other) const {
	return (r_owner == other.r_owner && r_node == other.r_node);
}
/**
 * Checks if an iterator and a const_iterator are not equal.
 * @return	true if they point to different owners or different nodes
 */
template <typename T>
bool LinkedList_Iterator<T>::operator!=(const LinkedList_ConstIterator<T>& other) const {
	return (r_owner != other.r_owner || r_node != other.r_node);
}

// -------------------------------- //
// LinkedList_ConstIterator methods //
// -------------------------------- //
/**
 * The default constructor for a linked list const_iterator.
 * Creates an iterator with no node or owner.
 */
template <typename T>
LinkedList_ConstIterator<T>::LinkedList_ConstIterator()
	: r_owner(nullptr), r_node(nullptr)
{}
/**
 * The copy constructor for a linked list const_iterator.
 *
 * Given another linked list const_iterator, initializes this to point at the
 * same node and the same list.
 *
 * @param	other	A const_iterator to copy
 */
template <typename T>
LinkedList_ConstIterator<T>::LinkedList_ConstIterator(const LinkedList_ConstIterator<T>& other)
	: r_owner(other.r_owner), r_node(other.r_node)
{}
/**
 * Another copy constructor for a linked list const_iterator.
 *
 * Given a linked list iterator, initializes this to point at the
 * same node and the same list. Note that this is an iterator,
 * as opposed to a const_iterator.
 *
 * @param	other	An iterator to copy
 */
template <typename T>
LinkedList_ConstIterator<T>::LinkedList_ConstIterator(const LinkedList_Iterator<T> &other)
	: r_owner(other.r_owner), r_node(other.r_node)
{}
/**
 * Constructs a const_iterator from a node and an owning list.
 *
 * This method is private and expected to only be called by a LinkedList
 * in order to create a const_iterator to one of its nodes.
 *
 * @param	n	A pointer to the node this iterator should point to
 * @param	owner	A pointer to the list that this iterator will belong to
 */
template <typename T>
LinkedList_ConstIterator<T>::LinkedList_ConstIterator(const Node* n, const LinkedList<T>* owner)
	: r_owner(owner), r_node(n)
{}
/**
 * Assignment operator from a LinkedList_ConstIterator.
 * @param	other	A const_iterator to copy from
 */
template <typename T>
LinkedList_ConstIterator<T>& LinkedList_ConstIterator<T>::operator=(const LinkedList_ConstIterator<T>& other) {
	r_owner = other.r_owner;
	r_node = other.r_node;
	return *this;
}
/**
 * Assignemnt operator from a LinkedList_Iterator.
 * @param	other	An iterator to copy from
 */
template <typename T>
LinkedList_ConstIterator<T>& LinkedList_ConstIterator<T>::operator=(const LinkedList_Iterator<T>& other) {
	r_owner = other.r_owner;
	r_node = other.r_node;
	return *this;
}

/**
 * Dereferences this iterator to get the data at the current node.
 *
 * This method allows the end-user to effectively treat this iterator as a
 * pointer to the data, which can be read from and written to using this
 * method.
 *
 * @return	A reference to the data object on the current node
 */
template <typename T>
const T& LinkedList_ConstIterator<T>::operator*() const {
	return r_node->data;
}
/**
 * Facilitates member access to the underlying node's data.
 *
 * This method allows the end-user to effectively treat this iterator as a
 * pointer to the data, which can be read from and written to using this
 * method.
 *
 * @return	A pointer to the data in question, used for member access
 */
template <typename T>
const T* LinkedList_ConstIterator<T>::operator->() const {
	return &r_node->data;
}
/**
 * Implementation of a pre-increment on a const_iterator.
 *
 * Moves the node pointer to the next node in the list, then returns
 * a reference to this iterator. Anything that is done with the result of
 * this method call will use the incremented version of this.
 *
 * This method is invoked when `++itr` is written in the program.
 *
 * @return	This, after the node pointer has been changed
 */
template <typename T>
LinkedList_ConstIterator<T>& LinkedList_ConstIterator<T>::operator++() {
	if (r_node == nullptr) {
		throw OutOfBoundsError();
	}
	
	r_node = r_node->next;
	
	return *this;
}
/**
 * Implementation of a post-increment on a const_iterator.
 *
 * Creates a copy of this iterator, then moves the node pointer to the
 * next node in the list, then returns a reference to the copied iterator.
 * Anything that is done with the result of this method call will use the
 * an iterator which has not been incremented.
 *
 * This method is invoked when `itr++` is written in the program.
 *
 * @return	A copy of this before the node pointer was changed
 */
template <typename T>
LinkedList_ConstIterator<T> LinkedList_ConstIterator<T>::operator++(int) {
	if (r_node == nullptr) {
		throw OutOfBoundsError();
	}
	
	LinkedList_ConstIterator<T> tmp(*this);
	
	r_node = r_node->next;
	
	return tmp;
}
/**
 * Implementation of a pre-decrement on a const_iterator.
 *
 * Moves the node pointer to the previous node in the list, then returns
 * a reference to this iterator. Anything that is done with the result of
 * this method call will use the decremented version of this.
 *
 * This method is invoked when `--itr` is written in the program.
 *
 * @return	This, after the node pointer has been changed
 */
template <typename T>
LinkedList_ConstIterator<T>& LinkedList_ConstIterator<T>::operator--() {
	if (r_node == r_owner.m_firstNode) {
		throw OutOfBoundsError();
	}
	
	if (r_node == nullptr) {
		r_node = r_owner->m_lastNode;
	}
	else {
		r_node = r_node->prev;
	}
	
	return *this;
}
/**
 * Implementation of a post-decrement on a const_iterator.
 *
 * Creates a copy of this iterator, then moves the node pointer to the
 * previous node in the list, then returns a reference to the copied iterator.
 * Anything that is done with the result of this method call will use the
 * an iterator which has not been decremented.
 *
 * This method is invoked when `itr--` is written in the program.
 *
 * @return	A copy of this before the node pointer was changed
 */
template <typename T>
LinkedList_ConstIterator<T> LinkedList_ConstIterator<T>::operator--(int) {
	if (r_node == r_owner.m_firstNode) {
		throw OutOfBoundsError();
	}
	
	LinkedList_ConstIterator<T> tmp(*this);
	
	if (r_node == nullptr) {
		r_node = r_owner->m_lastNode;
	}
	else {
		r_node = r_node->prev;
	}
	return tmp;
}

/**
 * Checks if two const_iterators are equal.
 * @return	true if they point to the same owner and node, false otherwise
 */
template <typename T>
bool LinkedList_ConstIterator<T>::operator==(const LinkedList_ConstIterator<T>& other) const {
	return (r_owner == other.r_owner && r_node == other.r_node);
}
/**
 * Checks if two const_iterators are unequal.
 * @return	true if they point to different owners or different nodes
 */
template <typename T>
bool LinkedList_ConstIterator<T>::operator!=(const LinkedList_ConstIterator<T>& other) const {
	return (r_owner != other.r_owner || r_node != other.r_node);
}
/**
 * Checks if a const_iterator and an are equal.
 * @return	true if they point to the same owner and node, false otherwise
 */
template <typename T>
bool LinkedList_ConstIterator<T>::operator==(const LinkedList_Iterator<T>& other) const {
	return (r_owner == other.r_owner && r_node == other.r_node);
}
/**
 * Checks if a const_iterator and an iterator are not equal.
 * @return	true if they point to different owners or different nodes
 */
template <typename T>
bool LinkedList_ConstIterator<T>::operator!=(const LinkedList_Iterator<T>& other) const {
	return (r_owner != other.r_owner || r_node != other.r_node);
}

#endif // Fundamentals_LinkedList_hpp_
