#include "avl_tree.hpp"

namespace twins
{

    avl_node::avl_node(const avl_node &other)
        : val{other.val}, left{other.left}, right{other.right}, next{other.next}, prev{other.prev}
    {
    }

    avl_node::avl_node(avl_node &&other) noexcept
        : val{other.val}, left{other.left}, right{other.right}, next{other.next}, prev{other.prev}
    {
    }

    avl_node &avl_node::operator=(const avl_node &other)
    {
        left = other.left;
        right = other.right;
        next = other.next;
        prev = other.prev;
        val = other.val;

        return *this;
    }

    avl_node &avl_node::operator=(avl_node &&other) noexcept
    {
        left = other.left;
        right = other.right;
        next = other.next;
        prev = other.prev;
        val = other.val;
        return *this;
    }

    short get_height(avl_node *n)
    {
        if (n == nullptr)
            return 0;
        return n->height;
    }

    short balance_factor(avl_node *n)
    {
        if (n == nullptr)
            return 0;
        return (short)(get_height(n->left) - get_height(n->right));
    }

    avl_node *avl_node::rotate_right()
    {
        avl_node *x = left;
        avl_node *t2 = x->right;
        x->right = this;
        left = t2;

        height = (short)(std::max(get_height(left), get_height(right)) + 1);
        x->height = (short)(std::max(get_height(x->left), get_height(x->right)) + 1);

        return x;
    }

    avl_node *avl_node::rotate_left()
    {
        avl_node *y = right;
        avl_node *t2 = y->left;
        y->left = this;
        right = t2;
        height = (short)(std::max(get_height(left), get_height(right)) + 1);
        y->height = (short)(std::max(get_height(y->left), get_height(y->right)) + 1);
        return y;
    }

    avl_tree::avl_tree(const avl_tree &other)
        : root{other.root}, first_element{other.first_element}, last_element{other.last_element}, _size{other._size}
    {
    }


    avl_tree::avl_tree(avl_tree &&other) noexcept
        : root{other.root}, first_element{other.first_element}, last_element{other.last_element}, _size{other._size}
    {
        other.root = nullptr;
        other.first_element = nullptr;
        other.last_element = nullptr;
    }

    avl_tree &avl_tree::operator=(const avl_tree &other)
    {
        root = other.root;
        first_element = other.first_element;
        last_element = other.last_element;
        _size = other._size;
        return *this;
    }


    avl_tree &avl_tree::operator=(avl_tree &&other) noexcept
    {
        std::swap(root, other.root);
        std::swap(first_element, other.first_element);
        std::swap(last_element, other.last_element);
        _size = other._size;
        return *this;
    }


    avl_tree::~avl_tree() { deallocate(); }

    avl_node *avl_tree::insert_node(avl_node *nd, mpz_class &key, avl_node *&inserted_node)
    {
        // Find the correct postion and insert the node

        if (nd == nullptr)
        {
            auto res = new avl_node(key);
            first_element = res;
            last_element = res;
            _size++;
            inserted_node = res;
            return res;
        }

        int cmp = mpz_cmp(key.get_mpz_t(), nd->val.get_mpz_t());
        if (cmp > 0)
            if (nd->right == nullptr)
            {
                _size++;

                auto new_node = new avl_node(key);

                new_node->next = nd->next;
                new_node->prev = nd;

                nd->right = new_node;

                nd->next = new_node;
                if (new_node->next == nullptr)
                    last_element = new_node;
                else
                    new_node->next->prev = new_node;
                inserted_node = new_node;
            }
            else
                nd->right = insert_node(nd->right, key, inserted_node);
        else if (cmp < 0)
        {
            if (nd->left == nullptr)
            {
                _size++;
                // insert on left node
                auto new_node = new avl_node(key);

                new_node->next = nd;
                new_node->prev = nd->prev;

                nd->left = new_node;

                nd->prev = new_node;

                if (new_node->prev == nullptr)
                    first_element = new_node;
                else
                    new_node->prev->next = new_node;

                inserted_node = new_node;
            }
            else
                nd->left = insert_node(nd->left, key, inserted_node);
        }
        else
        {
            inserted_node = nullptr;
            return nd;
        }

        // Update the balance factor of each node and
        // balance the tree
        nd->height = short(1 + std::max(get_height(nd->left), get_height(nd->right)));

        short bf = balance_factor(nd);

        if (bf > 1)
        {
            cmp = mpz_cmp(key.get_mpz_t(), nd->left->val.get_mpz_t());
            if (cmp < 0)
            {
                return nd->rotate_right();
            }
            else if (cmp > 0)
            {
                nd->left = nd->left->rotate_left();
                return nd->rotate_right();
            }
        }

        if (bf < -1)
        {
            cmp = mpz_cmp(key.get_mpz_t(), nd->right->val.get_mpz_t());
            if (cmp > 0)
            {
                return nd->rotate_left();
            }
            else if (cmp < 0)
            {
                nd->right = nd->right->rotate_right();
                return nd->rotate_left();
            }
        }
        return nd;
    }

    node_iterator<avl_node> avl_tree::insert(mpz_class key)
    {
        avl_node *res;
        root = insert_node(root, key, res);
        return node_iterator<avl_node>(res);
    }


    node_iterator<avl_node> avl_tree::find(const mpz_class &key) const
    {
        auto current_node = root;

        while (current_node != nullptr)
        {
            int cmp = mpz_cmp(key.get_mpz_t(), current_node->val.get_mpz_t());
            if (cmp < 0)
            {
                current_node = current_node->left;
            }
            else if (cmp > 0)
            {
                current_node = current_node->right;
            }
            else
            {
                return  node_iterator<avl_node>{current_node};
            }
        }

        return node_iterator<avl_node>{nullptr};
    }

    node_iterator<avl_node> avl_tree::lower_bound(const mpz_class &key) const
    {
        avl_node *x{root};
        int last_pos = 0;

        while (x != nullptr)
        {
            int cmp = mpz_cmp(x->val.get_mpz_t(), key.get_mpz_t());

            // if val is smaller than key then I have to look at bigger elements (go
            // right)
            if (cmp < 0)
            {
                if (last_pos == -1)
                {
                    // last time x was bigger and this time it's smaller, therefore
                    // the lower bound is in between last x and this x
                    while (x->next != nullptr) // it's impossible to reach nullptr here
                    {
                        cmp = mpz_cmp(x->next->val.get_mpz_t(), key.get_mpz_t());
                        // if next is still smaller than key then keep going
                        // otherwise return the previous element
                        if (cmp >= 0)
                            return node_iterator<avl_node>{x};
                        x = x->next;
                    }
                }

                last_pos = 1;
                x = x->right;
            }
            // if val is bigger than key then I have to look at smaller elements (go
            // right)
            else if (cmp > 0)
            {
                // last time x was smaller and this time it's bigger, therefore lower
                // bound is in between this x and last x
                if (last_pos == 1)
                {
                    while (x->prev != nullptr)
                    {
                        cmp = mpz_cmp(x->prev->val.get_mpz_t(), key.get_mpz_t());
                        // if prev is still bigger than key then keep going otherwise
                        // return current element
                        if (cmp < 0)
                            return node_iterator<avl_node>{x->prev};
                        if (cmp == 0)
                            return x->prev->prev == nullptr
                                       ? node_iterator<avl_node>{x->prev}
                                       : node_iterator<avl_node>{x->prev->prev};
                        x = x->prev;
                    }
                }

                last_pos = -1;
                x = x->left;
            }
            else
            {
                if (x->prev == nullptr)
                    return node_iterator<avl_node>{(x)};
                return node_iterator<avl_node>{(x->prev)};
            }
        }

        if (last_pos == 1)
            return node_iterator<avl_node>{last_element};
        return node_iterator<avl_node>{first_element};
    }


    node_iterator<avl_node> avl_tree::begin()
    {
        return node_iterator<avl_node>{first_element};
    }


    node_iterator<avl_node> avl_tree::end()
    {
        return node_iterator<avl_node>{nullptr};
    }


    void avl_tree::deallocate()
    {
        auto iter = first_element;
        while (iter != nullptr)
        {
            auto next = iter->next;
            delete iter;
            iter = next;
        }
    }
}