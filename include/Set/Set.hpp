#pragma once

#include "declarations.hpp"
#include "Tree.hpp"
#include "Node.hpp"
#include "TreeIterator.hpp"

template < typename _Tp, 
    typename _OrderTag,
    typename _Compare,
    typename _Allocator>
class Set {
public:

    typedef _Tp            key_type;
    typedef key_type       value_type;
    typedef _Compare       key_compare;
    typedef key_compare    value_compare;
    typedef _Allocator     allocator_type;
    typedef _OrderTag      order_tag;
    typedef std::size_t    size_type;

    typedef Node<key_type>                                            node_type;
    typedef node_type*                                                node_ptr;
    typedef Tree<key_type, node_type, key_compare, allocator_type>    tree_type;

    typedef TreeIterator<_Tp, order_tag>             iterator;
    typedef const iterator                           const_iterator;
    typedef std::reverse_iterator<iterator>          reverse_iterator;
    typedef std::reverse_iterator<const iterator>    const_reverse_iterator; //

    Set()
        :   _tree(),
            _end_node(nullptr)
    {
        _begin_node = _end_node;
        _rbegin_node = _end_node;
    }

    Set(const Set& other) 
        : _tree(other._tree), _end_node(nullptr)
    {
        _begin_node = _find_begin_node(_tree.root(), order_tag());
        _rbegin_node = _find_rbegin_node(_tree.root(), order_tag());
    }

    Set(Set&& other) 
        :   _tree(std::move(other._tree)),
            _begin_node(std::move(other._begin_node)),
            _rbegin_node(std::move(other._rbegin_node)), // 
            _end_node(nullptr)
    {}

    Set& operator=(const Set& other) {
        _tree = other._tree;
        _begin_node = _find_begin_node(_tree.root(), order_tag());
        _rbegin_node = _find_rbegin_node(_tree.root(), order_tag());
        _end_node = nullptr;

        return *this;
    }

    Set& operator=(Set&& other) {
        _tree = std::move(other._tree),
        _begin_node = std::move(other._begin_node),
        _rbegin_node = std::move(other._rbegin_node),
        _end_node(nullptr);
    }

    ~Set() = default;

    bool operator==(const Set& other) { return _tree == other._tree; }

    bool operator!=(const Set& other) { return !((*this) == other); }


    iterator begin() { return iterator(_tree.root(), _begin_node); }

    iterator end() { return iterator(_tree.root(), _end_node); }

    const_iterator cbegin() { return const_iterator(_tree.root(), _begin_node); } // const

    const_iterator cend() { return const_iterator(_tree.root(), _end_node); }

    reverse_iterator rbegin() { return reverse_iterator(iterator(_tree.root(), _end_node)); } 

    reverse_iterator rend() { return reverse_iterator(iterator(_tree.root(), _begin_node)); }

    const_reverse_iterator crbegin() { return const_reverse_iterator(iterator(_tree.root(), _end_node)); } // const

    const_reverse_iterator crend() { return const_reverse_iterator(iterator(_tree.root(), _begin_node)); }

    std::pair<iterator, bool> insert(const _Tp& key) {
        _begin_node = nullptr;
        _rbegin_node = nullptr;

        size_type start_size = size();
        bool insertion_result = true;

        iterator it(_tree.root(), _tree.insert(key));
        if (start_size == size()) { insertion_result = false; }

        _begin_node = _find_begin_node<key_type>(_tree.root(), order_tag());
        _rbegin_node = _find_rbegin_node<key_type>(_tree.root(), order_tag());

        return std::pair<iterator, bool>(it, insertion_result);
    }

    bool erase(const _Tp& key) { // size_type
        
        _begin_node = nullptr;
        _rbegin_node = nullptr;
        
        bool result = _tree.remove(key);
        
        _begin_node = _find_begin_node(_tree.root(), order_tag());
        _rbegin_node = _find_rbegin_node(_tree.root(), order_tag());
        
        return result;
    }

    bool contains(const _Tp& key) { return _tree.find(key) != nullptr; } // const

    iterator find(const key_type& key) { return iterator(_tree.root(), _tree.find()); }

    void clear() {
        _tree.clear();
        _begin_node = nullptr;
        _rbegin_node = nullptr;
    }

    bool empty() { return _tree.empty(); } // const

    size_type size() { return _tree.size(); } // const

private:
    tree_type _tree;

    node_ptr _begin_node;
    node_ptr _rbegin_node;
    node_ptr _end_node = nullptr; //
};