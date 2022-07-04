/**
 * @file avl_node.hpp
 * @author Giacomo Bruno
 * @brief file for avl_node struct
 * @version 0.1
 * @date 2022-07-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once
#include <gmpxx.h>
#include "node_iterator.hpp"

/**
 * @brief project namespace
 */
namespace twins
{

    /**
     * @brief avl specialization for standard_node, using height for balancing
     * @tparam T type of stored element
     */
    struct avl_node
    {
        using node_type = mpz_class;

        node_type val{};
        /**
         * @brief left child of the node
         */
        avl_node *left{nullptr};
        /**
         * @brief right child of the node
         */
        avl_node *right{nullptr};

        /**
         * @brief successor of the node in the order
         */
        avl_node *next{nullptr};
        /**
         * @brief predecessor of the node in the order
         */
        avl_node *prev{nullptr};

        /**
         * @brief height of the node in the tree
         */
        short height{1};

        /**
         * @brief available extra space due to memory alignment
         * 
         */
        char extra_space[6];

        /**
         * @brief rotate node to the right
         * @return the node at the old position of the current node
         */
        avl_node *rotate_right();

        /**
         * @brief rotate node to the left
         * @return the node at the old position of the current node
         */
        avl_node *rotate_left();

        /**
         * constructor
         * @param val node value
         */
        explicit avl_node(node_type _val) : val{_val} {}

        /**
         * @brief copy constructor
         */
        avl_node(const avl_node &);

        /**
         * @brief move constructor
         */
        avl_node(avl_node &&) noexcept;

        /**
         * @brief copy operator
         * @return copied node
         */
        avl_node &operator=(const avl_node &);

        /**
         * @brief move operator
         * @return moved node
         */
        avl_node &operator=(avl_node &&) noexcept;

        /**
         * destructor, does nothing as the tree is responsible for cleaning the node
         * pointers
         */
        ~avl_node() = default;
    };

    /**
     * avl tree implementation (with connected leaves for faster iteration)
     * @tparam T type of the elements
     * @tparam comparer functor used for comparison, needs to implement function
     * call operator returning int and taking two T as parameters
     */
    class avl_tree
    {
    public:
        /**
         * @brief default constructor
         *
         */
        avl_tree() = default;
        /**
         * @brief copy constructor
         * @param other object to copy
         */
        avl_tree(const avl_tree &other);
        /**
         * @brief move constructor
         *
         * @param other object to move
         */
        avl_tree(avl_tree &&other) noexcept;

        /**
         * @brief destructor, does not take care of eventual pointer type elements
         */
        ~avl_tree();

        /**
         * @brief copy operator
         *
         * @param other object to copy
         * @return self
         */
        avl_tree &operator=(const avl_tree &other);

        /**
         * @brief move operator
         *
         * @param other object to move
         * @return self
         */
        avl_tree &operator=(avl_tree &&other) noexcept;

        node_iterator<avl_node> insert(mpz_class key);

        node_iterator<avl_node> find(const mpz_class &key) const;

        node_iterator<avl_node> lower_bound(const mpz_class &key) const;

        node_iterator<avl_node> begin();

        node_iterator<avl_node> end();

        [[nodiscard]] unsigned long long size() const { return _size; };

        void deallocate();

    private:
        avl_node *root{nullptr};
        avl_node *first_element{nullptr};
        avl_node *last_element{nullptr};
        unsigned long long _size{0};

        avl_node *insert_node(avl_node *nd, mpz_class &key, avl_node *&inserted_node);
    };
}

//#############################################################################################################
//# GENERIC IMPLEMENTATION
//#############################################################################################################
