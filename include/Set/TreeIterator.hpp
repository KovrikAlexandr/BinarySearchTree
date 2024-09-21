#pragma once

#include "declarations.hpp"
#include "Node.hpp"

template <typename _Tp, typename _OrderTag>
class TreeIterator {
public:
    
    typedef _Tp                                             key_type;
    typedef _OrderTag                                       order_tag;

    typedef Node<key_type>                                  node_type;
    typedef node_type*                                      pointer;
    typedef node_type&                                      reference;
    typedef const node_type&                                const_reference;

    typedef TreeIterator<key_type, order_tag>               iterator;
    typedef iterator&                                       iterator_reference;
    typedef const iterator&                                 iterator_const_reference;
    typedef const iterator                                  const_iterator;
    
    // Default
    TreeIterator() = delete;

    // Copy
    TreeIterator(const TreeIterator& other) 
        : _root(other._root), _node(other._node)
    {}

    // Move
    TreeIterator(TreeIterator&& other) 
        : _root(std::move(other._root)), _node(std::move(other._node))
    {}

    // Copy assigment
    TreeIterator& operator=(const TreeIterator& other) {
        this->_root = other._root;
        this->_node = other._node;

        return *this;
    }

    // Move assigment
    TreeIterator& operator=(TreeIterator&& other) {
        this->_root = other._root;
        this->_node = other._node;

        other._root = nullptr;
        other._node = nullptr;
        
        return *this;
    }

    // Destructor
    ~TreeIterator() = default;

    bool operator==(const TreeIterator& other) const
        { return (_root == other._root && _node == other._node); }

    bool operator!=(const TreeIterator& other) const
        { return !((*this) == other); }

    iterator_reference operator++() {
        _node = _next_node(order_tag());
        return *this;
    }

    iterator_reference operator++(int) {
        _node = _next_node(order_tag());
        return *this;
    }

    iterator_reference operator--() {
        _node = _prev_node(order_tag());
        return *this;
    }

    iterator_reference operator--(int) {
        _node = _prev_node(order_tag());
        return *this;
    }

    key_type& operator*() { return _node->_key; }

    const key_type& operator*() const { return _node->_key; }

    // template <typename> friend class Set;

    TreeIterator(pointer root, pointer _node)
        : _root(root), _node(_node)
    {}

private:
    pointer     _root;
    pointer     _node;

    //  TreeIterator(pointer root, pointer _node)
    //     : _root(root), _node(_node)
    // {}

    pointer _next_node(iterator_order_traits::inorder_iterator_tag) { // const
        if (_node == nullptr) { exit(EXIT_FAILURE); }
        if (_has_right_subtree<key_type>(_node)) {
            pointer current = _node->_right;
            while (current->_left != nullptr) { current = current->_left; }
            return current;
        }

        pointer current = _node;
        pointer parent = current->_parent;
        while(parent != nullptr) {
            if (current == parent->_left)
                { return parent ;}
            current = current->_parent;
            parent = current->_parent;
        }
        return nullptr;
    }

    pointer _next_node(iterator_order_traits::preorder_iterator_tag) {
        if ( _node == nullptr ) { exit(EXIT_FAILURE); } // exit

        pointer current = _node;
        if (_has_left_subtree(current)) { return current->_left; }
        if (_has_right_subtree(current)) { return current->_right; }

        pointer parent = current->_parent;
        while (parent != nullptr) {
            if (current == parent->_left) {
                if (_has_right_subtree(parent)) 
                    { return parent->_right; }
            } 
            current = parent;
            parent = current->_parent;
        }
        return nullptr;
    }

    pointer _next_node(iterator_order_traits::postorder_iterator_tag) {
        pointer next = nullptr;
        pointer current = _node;
        if (current->_parent) {
            if (current->_parent->_left && current == current->_parent->_left) {
                if (current->_parent->_right) {
                    next = current->_parent->_right;
                    while (next->_left || next->_right) {
                        if (next->_left) {
                            next = next->_left;
                            continue;
                        }
                        if (next->_right) {
                            next = next->_right;
                            continue;
                        }
                    }
                } else { next = current->_parent; }
            } else { next = current->_parent; }
        }

        return next;
    }

    pointer _prev_node(iterator_order_traits::inorder_iterator_tag) {
        if (_node == nullptr) { return _find_rbegin_node<key_type>(_root, order_tag()); }
        if (_has_left_subtree<key_type>(_node)) {
            pointer current = _node->_left;
            while (_has_right_subtree<key_type>(current)) { current = current->_right; }
            return current;
        }

        pointer current = _node;
        pointer parent = current->_parent;
        while(parent != nullptr) {
            if (current == parent->_right)
                { return parent ;}
            current = current->_parent;
            parent = current->_parent;
        }
        return nullptr;
    }

    pointer _prev_node(iterator_order_traits::preorder_iterator_tag) {
        if ( _node == nullptr ) 
            { return _find_rbegin_node<key_type>(_root, order_tag()); }
        
        pointer current = _node;
        pointer parent = current->_parent;

        if (current == parent->_left) { return parent; }
        else if (!_has_left_subtree(parent)) { return parent; }
        current = parent->_left;
        while(_has_left_subtree(current) || _has_right_subtree(current)) {
            if (_has_right_subtree(current)) { current = current->_right; }
            else if (_has_left_subtree(current)) { current = current->_left; }
        }

        return current;
    }

    pointer _prev_node(iterator_order_traits::postorder_iterator_tag) {
        if (_node == nullptr) { return _find_rbegin_node<key_type>(_root, order_tag()); }
        pointer current = _node;
        if (current->_right)
            return current->_right;

        if (current->_left)
            return current->_left;

        pointer pred = current->_parent;
        while (pred && (current == pred->_left || !pred->_left)) {
            current = pred;
            pred = pred->_parent;
        }
        if (pred != nullptr) { return pred->_left; }

        return nullptr;
    }
};


template<class _Tp>
struct std::iterator_traits<TreeIterator<_Tp, iterator_order_traits::inorder_iterator_tag> > {
    typedef  std::size_t                            difference_type;
    typedef  _Tp                                    key_type;
    typedef  _Tp                                    value_type;
    typedef  _Tp*                                   pointer;
    typedef  _Tp&                                   reference;
    typedef  std::bidirectional_iterator_tag        iterator_category;
};

template<class _Tp>
struct std::iterator_traits<TreeIterator<_Tp, iterator_order_traits::preorder_iterator_tag> > {
    typedef  std::size_t                            difference_type;
    typedef  _Tp                                    key_type;
    typedef  _Tp                                    value_type;
    typedef  _Tp*                                   pointer;
    typedef  _Tp&                                   reference;
    typedef  std::bidirectional_iterator_tag        iterator_category;
};

template<class _Tp>
struct std::iterator_traits<TreeIterator<_Tp, iterator_order_traits::postorder_iterator_tag> > {
    typedef  std::size_t                            difference_type;
    typedef  _Tp                                    key_type;
    typedef  _Tp                                    value_type;
    typedef  _Tp*                                   pointer;
    typedef  _Tp&                                   reference;
    typedef  std::bidirectional_iterator_tag        iterator_category;
};
