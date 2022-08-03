#pragma once
#include <gmpxx.h>
#include <type_traits>
//#include <concepts>
namespace twins
{
/*
    template <typename T>
    concept typename = requires(T obj)
    {
        std::is_same_v<decltype(obj.next), T*>;
        std::is_same_v<decltype(obj.prev), T*>;
        std::is_same_v<decltype(obj.val), mpz_class>;
        std::is_same_v<decltype(obj.extra_space), char[6]>;
    };
*/
    template <typename T>
    struct node_iterator
    {
        /**
         * @brief defines the category of this iterator, in this case bidirectional
         */
        using iterator_cateogory = std::bidirectional_iterator_tag;
        /**
         * @brief type of the difference betweenn two iterators
         *
         */
        using difference_type = std::ptrdiff_t;
        /**
         * @brief type of the value pointed by the iterator
         */
        using value_type = mpz_class;
        /**
         * @brief reference type of the value pointed by the iterator
         *
         */
        using pointer = value_type *;
        /**
         * @brief pointer type of the value pointed by the iterator
         *
         */
        using reference = value_type &;

        /**
         * @brief copy constructor
         *
         */
        node_iterator(const node_iterator<T> &);

        /**
         * @brief normal constructor that takes a tree node as input
         *
         */
        explicit node_iterator(T *);

        /**
         * @brief default constructor, used for empty iterator and for default
         * initialization
         *
         */
        explicit node_iterator();

        /**
         * @brief destructr, does nothing
         *
         */
        ~node_iterator();

        /**
         * @brief copy operator
         *
         * @return self
         */
        node_iterator<T> &operator=(const node_iterator<T> &);

        /**
         * @brief pointer operator to get access to pointed value
         *
         * @return T& value pointed from the iterator
         */
        value_type &operator*();

        /**
         * @brief equal operator
         *
         * @return true hold same node
         * @return false hold different node
         */
        bool operator==(const node_iterator<T> &);

        /**
         * @brief not equal operator
         *
         * @return true different nodes
         * @return false same nodes
         */
        bool operator!=(const node_iterator<T> &);

        /**
         * @brief sub operator, used to calculate distance between two iterator of
         * the same tree, input must be bigger that self
         *
         * @return difference_type distance between elements -1 if distance could not
         * be calculated
         */
        difference_type operator-(const node_iterator<T> &) const;

        /**
         * @brief increase operator, moves the iterator forward by one
         *
         * @return node_iterator<T>& self
         */
        node_iterator<T> &operator++();

        /**
         * @brief increase operator, moves the iterator forward by one
         *
         * @return node_iterator<T> temporary copy holding the last iterator
         * position
         */
        node_iterator<T> operator++(int);

        /**
         * @brief decrease operator, moves the iterator backwards by one
         *
         * @return node_iterator<T>& self
         */
        node_iterator<T> &operator--();

        /**
         * @brief decrease operator, moves the iterator backwards by one
         *
         * @return node_iterator<T> temporary copy holding the last iterator
         * position
         */
        node_iterator<T> operator--(int);

        /**
         * @brief node holding the actual value
         */
        T *node{nullptr};
    };
}

namespace twins
{

    template <typename T>
    node_iterator<T>::node_iterator(const node_iterator<T> &it) : node{it.node}
    {
    }

    template <typename T>
    node_iterator<T>::node_iterator(T *nd) : node{nd} {}

    template <typename T>
    node_iterator<T>::node_iterator() = default;

    template <typename T>
    node_iterator<T>::~node_iterator() { node = nullptr; };

    template <typename T>
    node_iterator<T> &node_iterator<T>::operator=(const node_iterator<T> &it)
    {
        node = it.node;
        return *this;
    }

    template <typename T>
    mpz_class &node_iterator<T>::operator*() { return node->val; }

    template <typename T>
    bool node_iterator<T>::operator==(const node_iterator<T> &it)
    {
        return node == it.node;
    }

    template <typename T>
    bool node_iterator<T>::operator!=(const node_iterator<T> &it)
    {
        return node != it.node;
    }

    /**
     * @brief equal operator to simplify checking for end of tree/empty iterator
     *
     * @tparam T type of element
     * @param it iterator to check
     * @return true iterator was empty
     * @return false iterator was not empty
     */
    template <typename T>
    bool operator==(const node_iterator<T> &it, std::nullptr_t) { return it.node == nullptr; }

    /**
     * @brief not equal operator to simplify checking for end of tree/empty
     * iterator
     *
     * @tparam T type of element
     * @param it iterator to check
     * @return true iterator was not empty
     * @return false iterator was empty
     */
    template <typename T>
    bool operator!=(const node_iterator<T> &it, std::nullptr_t) { return it.node != nullptr; }

    template <typename T>
    bool operator!=(const node_iterator<T>&lhs,  const node_iterator<T>&rhs) {return lhs.node != rhs.node;}

    template <typename T>
    node_iterator<T> &node_iterator<T>::operator++()
    {
        node = node->next;
        return *this;
    }

    template <typename T>
    node_iterator<T> node_iterator<T>::operator++(int)
    {
        auto tmp = *this;
        node = node->next;
        ;
        return tmp;
    }

    template <typename T>
    node_iterator<T> &node_iterator<T>::operator--()
    {
        node = node->prev;
        return *this;
    }

    template <typename T>
    node_iterator<T> node_iterator<T>::operator--(int)
    {
        auto tmp = *this;
        node = node->prev;
        return tmp;
    }

    template <typename T>
    typename node_iterator<T>::difference_type
    node_iterator<T>::operator-(const node_iterator<T> &other) const
    {
        auto iter = *this;
        int cnt = 0;
        while (iter != nullptr && iter != other)
        {
            cnt++;
            iter++;
        }

        if (iter == nullptr)
            return -1;

        return cnt;
    }

}