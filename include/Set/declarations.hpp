#pragma once
#include <iterator>

// Iterator order traits
struct iterator_order_traits {
    struct inorder_iterator_tag {};
    struct preorder_iterator_tag {};
    struct postorder_iterator_tag {};
};

// Node
template <typename _Tp>
struct Node;


// Set
template < 
    typename _Tp, 
    typename _OrderTag = iterator_order_traits::inorder_iterator_tag,
    typename _Compare = std::less<_Tp>,
    typename _Allocator = std::allocator<_Tp> 
>
class Set;

// TreeIterator
template <typename _Tp, typename _NodeType>
class TreeIterator;


// Tree
template <  
    typename _Tp,
    typename _TreeNode,
    typename _Compare,
    typename _Allocator
>
class Tree;

