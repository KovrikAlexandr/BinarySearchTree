#pragma once
#include "declarations.hpp"

template <typename _Tp>
struct Node {
    typedef Node*   pointer;
    typedef _Tp     key_type;

    key_type _key;

    pointer _left;
    pointer _right;
    pointer _parent;

    Node()
        : _left(nullptr), _right(nullptr), _parent(nullptr)
    {}

    Node(key_type key)
        : _key(key), _left(nullptr), _right(nullptr), _parent(nullptr)
    {}
    
    Node(key_type key, pointer left, pointer right)
        : _key(key), _left(left), _right(right), _parent(nullptr)
    {}
};

// Subtree checks

template <typename _Tp>
bool _has_left_subtree(Node<_Tp>* node)
    { return (node->_left != nullptr); }

template <typename _Tp>
bool _has_right_subtree(Node<_Tp>* node)
    { return (node->_right != nullptr); }

// Find begin node

template <typename _Tp>
Node<_Tp>* _find_begin_node(Node<_Tp>* root, iterator_order_traits::inorder_iterator_tag) {
    Node<_Tp>* node = root;
    if (node == nullptr) { return nullptr; }
    while (_has_left_subtree(node)) {
        node = node->_left;
    }
    return node;
}

template <typename _Tp>
Node<_Tp>* _find_begin_node(Node<_Tp>* root, iterator_order_traits::preorder_iterator_tag) 
    { return root; }  

template <typename _Tp>
Node<_Tp>* _find_begin_node(Node<_Tp>* root, iterator_order_traits::postorder_iterator_tag) {
    Node<_Tp>* node = root;
    if (node == nullptr) { return nullptr; }
    while (_has_left_subtree(node) || _has_right_subtree(node)) {
        if (_has_left_subtree(node)) { node = node->_left; }
        else if (_has_right_subtree(node)) { node = node->_right; }
    }
    return node;
}

// Find rbegin node

template <typename _Tp>
Node<_Tp>* _find_rbegin_node(Node<_Tp>* root, iterator_order_traits::inorder_iterator_tag) {
    Node<_Tp>* node = root;
    if (node == nullptr) { return nullptr; }
    while (_has_right_subtree(node)) {
        node = node->_right;
    }
    return node;
}

template <typename _Tp>
Node<_Tp>* _find_rbegin_node(Node<_Tp>* root, iterator_order_traits::preorder_iterator_tag) {
    Node<_Tp>* node = root;
    if (node == nullptr) { return nullptr; }
    while (_has_left_subtree(node) || _has_right_subtree(node)) {
        if (_has_right_subtree(node)) { node = node->_right; }
        else if (_has_left_subtree(node)) { node = node->_left; }
    }
    return node;
}

template <typename _Tp>
Node<_Tp>* _find_rbegin_node(Node<_Tp>* root, iterator_order_traits::postorder_iterator_tag) 
    { return root; }
