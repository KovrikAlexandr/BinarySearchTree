#pragma once

#include <iostream>
#include "declarations.hpp"
#include "Node.hpp"
#include <vector>

template <
    typename _Tp,
    typename _TreeNode,
    typename _Compare,
    typename _Allocator
>
class Tree {
public:
    typedef _Tp             key_type;
    typedef _TreeNode       node_type;
    typedef _Compare        key_compare;
    typedef _TreeNode*      pointer;
    typedef std::size_t     size_type;
    typedef typename std::allocator_traits<_Allocator>::template rebind_alloc<node_type> allocator_type;

    // Constructor
    Tree() 
        :   _allocator  (), 
            _less       (),
            _size       (0),
            _root       (nullptr)
    {}

    // Copy constructor
    Tree(const Tree& other) 
        :   _root(_copy_subtree(other._root)),
            _less(),
            _allocator(),
            _size(other._size)
    {}

    // Move constructor
    Tree(Tree&& other) 
        :   _allocator(std::move(other._allocator)),
            _less(std::move(other._less)), 
            _size(std::move(other._size)), 
            _root(std::move(other._root))
    {}

    // Copy assigment
    Tree& operator=(const Tree& other) {
        _root = _copy_subtree(other._root),
        _size = other._size;

        return *this;
    }

    // Move assigment
    Tree& operator=(Tree&& other) {
        _allocator = std::move(other._allocator);
        _less = std::move(other._less); 
        _size = std::move(other._size);
        _root = std::move(other._root);

        return *this;
    }

    bool operator==(const Tree& other) { 
        if (_size != other.size()) { return false; }
        return _compare_with(_root, other); 
    }

    bool operator!=(const Tree& other) { return !((*this) == other); }

    void clear() { _clear_subtree(_root); }

    ~Tree() { clear(); }

    pointer root() const { return _root; }

    bool empty() const { return _root == nullptr; }

    size_type size() const { return _size; }

    pointer insert(const key_type& key) { return _insert_to_subtree(_root, key); }

    bool remove(const key_type& key) { return _remove(key); }

    pointer find(const key_type& key) { return _find(_root, key); }

    const pointer find(const key_type& key) const { return _find(_root, key); }

private:
    pointer         _root;
    key_compare     _less;
    size_type       _size;
    allocator_type  _allocator;

    pointer _allocate_node(const key_type& key) {
        pointer ptr = std::allocator_traits<allocator_type>::allocate(_allocator, 1);
        std::allocator_traits<allocator_type>::construct(_allocator, ptr, key);

        return ptr;
    }

    void _deallocate_node(pointer node) {
        std::allocator_traits<allocator_type>::destroy(_allocator, node);
        std::allocator_traits<allocator_type>::deallocate(_allocator, node, 1);
    }

    bool _is_valid_node(pointer node) const 
        { return ( node != nullptr ); }

    bool _has_left_subtree(pointer node) const
        { return ( _is_valid_node(node->_left) ); }

    bool _has_right_subtree(pointer node) const
        { return ( _is_valid_node(node->_right) ); }

    void _clear_subtree(pointer node) {
        if ( node != nullptr ) {
            _clear_subtree(node->_left);
            _clear_subtree(node->_right);
            _deallocate_node(node);
        }
    }

    pointer _insert_to_subtree(pointer root, const key_type& key) {
        if ( empty() ) {
            _root = _allocate_node(key);
            _size++;
            return _root;
        }
        pointer node = root;
        while (_is_valid_node(node)) {
            if (_less(key, node->_key)) {
                if (_has_left_subtree(node)) { node = node->_left; } 
                else { 
                    pointer ptr = _allocate_node(key);
                    node->_left = ptr;
                    _size++;

                    ptr->_parent = node;
                    return ptr;
                }
            } else if (_less(node->_key, key)) {
                if (_has_right_subtree(node)) { node = node->_right; } 
                else {
                    pointer ptr = _allocate_node(key);
                    node->_right = ptr;
                    _size++;

                    ptr->_parent = node;
                    return ptr;
                }
            } else { return node; }
        }

        return nullptr;
    }

    pointer _remove_from_subtree(pointer node, pointer parent, int key) {
        if (!_is_valid_node(node)) { return nullptr; }

        if      (_less(node->_key, key))    {node->_right = _remove_from_subtree(node->_right, node, key); }
        else if (_less(key, node->_key))    {node->_left = _remove_from_subtree(node->_left, node, key); } 
        else    {
            if (!_has_left_subtree(node) && !_has_right_subtree(node)) {
                // None
                _deallocate_node(node);
                _size--;

                return nullptr;

            } else if (!_has_left_subtree(node) && _has_right_subtree(node)) {
                // Only right
                pointer right_subtree = node->_right;
                _deallocate_node(node);
                _size--;
                
                right_subtree->_parent = parent;
                return right_subtree;

            } else if (_has_left_subtree(node) && !_has_right_subtree(node)) {
                // Only left
                pointer left_subtree = node->_left;
                _deallocate_node(node);
                _size--;

                left_subtree->_parent = parent;
                return left_subtree;

            } else if (_has_left_subtree(node) && _has_right_subtree(node)) {
                // Both

                pointer successor = node->_left;

                while (_has_right_subtree(successor)) {
                    successor = successor->_right;
                }

                node->_key = successor->_key;
                node->_left = _remove_from_subtree(node->_left, node, successor->_key);
            }
        }
        
        node->_parent = parent;
        return node;
    }

    bool _remove(const key_type& key) {
        if ( empty() ) { return false; }
        size_type curr_size = size();
        _root = _remove_from_subtree(_root, nullptr, key);
        if (curr_size == size()) { return false; }
        
        return true;
    }

    pointer _find(pointer root, const key_type& key) const {
        pointer node = root;
        while (node != nullptr) {
            if (_less(node->_key, key)) { node = node->_right; } 
            else if (_less(key, node->_key)) { node = node->_left; } 
            else { return node; }
        }

        return nullptr;
    }

    pointer _copy_subtree(pointer other_node) {
        pointer node = nullptr;
        if (_is_valid_node(other_node)) {
            node = _allocate_node(other_node->_key);
            node->_left = _copy_subtree(other_node->_left);
            node->_right = _copy_subtree(other_node->_right);
        }
        return node;
    }

    bool _compare_with(pointer node, const Tree& other) {
        if (_is_valid_node(node)) {
            if (other.find(node->_key) == nullptr) { return false; }
            return (_compare_with(node->_left, other) && _compare_with(node->_right, other));
        }
        return true;
    }
};