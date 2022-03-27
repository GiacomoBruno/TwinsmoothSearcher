#include "tree.h"

//node functions
void printTree(Node root, std::string indent, bool last) {
    if (root != nullptr) {
        std::cout << indent;
        if (last) {
            std::cout << "R----";
            indent += "   ";
        } else {
            std::cout << "L----";
            indent += "|  ";
        }
        gmp_printf("%Zd\n", *root->val);
        printTree(root->left, indent, false);
        printTree(root->right, indent, true);
    }
}

int8_t get_height(Node n)
{
    if(n == nullptr) return 0;
    return n->height;
}

Node node::rotate_right()
{
    Node x = left;
    Node t2 = x->right;
    x->right = this;
    left = t2;

    height = (int8_t)(std::max(get_height(left), get_height(right)) + 1);
    x->height = (int8_t)(std::max(get_height(x->left), get_height(x->right)) + 1);

    return x;
}

Node node::rotate_left()
{
    Node y = right;
    Node t2 = y->left;
    y->left = this;
    right = t2;
    height = (int8_t)(std::max(get_height(left), get_height(right)) + 1);
    y->height = (int8_t)(std::max(get_height(y->left), get_height(y->right)) + 1);
    return y;
}

int8_t balance_factor(Node n)
{
    if(n == nullptr) return 0;
    return (int8_t)(get_height(n->left) - get_height(n->right));
}

[[maybe_unused]]
Node min_val_node(Node n)
{
    Node cursor = n;
    while(cursor->left != nullptr)
        cursor = cursor->left;
    return cursor;
}

size_t node::distance(Node other)
{
    auto iter = this;
    size_t counter = 0;
    while(iter != nullptr)
    {
        if (iter == other)
            return counter;
        counter++;
        iter = iter->next;
    }
    return -1;
}

Node node::skip(size_t n)
{
    size_t counter = 0;
    auto iter = this;
    while(counter < n && iter != nullptr)
    {
        iter = iter->next;
        counter++;
    }
    return iter;
}


Node node::skip_back(size_t n)
{
    size_t counter = 0;
    auto iter = this;
    while(counter < n && iter != nullptr)
    {
        iter = iter->prev;
        counter++;
    }
    return iter;
}

//insertions

/// inserts key in tree, returns new root, populates inserted_node with the node inserted in the tree
/// \param nd node used for searching where to place key
/// \param key value to store in tree
/// \param inserted_node node inserted in the tree, nullptr if nothing was inserted
/// \return root of the resulting tree after insertion
Node LinkedTree::insert_node(Node nd, bigint key, Node& inserted_node) {
    // Find the correct postion and insert the node

    if(nd == nullptr)
    {
        auto res = new struct node(key);
        first_element = res;
        last_element = res;
        size++;
        inserted_node = res;
        return res;
    }

    int cmp = mpz_cmp(*key, *(nd->val));


    if (cmp < 0)
        if(nd->left == nullptr)
        {
            size++;
            //insert on left node
            auto new_node = new struct node(key);

            new_node->next = nd;
            new_node->prev = nd->prev;

            nd->left = new_node;

            nd->prev = new_node;

            if(new_node->prev == nullptr) first_element = new_node;
            else new_node->prev->next = new_node;

            inserted_node = new_node;
        }
        else nd->left = insert_node(nd->left, key, inserted_node);
    else if (cmp > 0)
        if(nd->right == nullptr)
        {
            size++;

            auto new_node = new struct node(key);

            new_node->next = nd->next;
            new_node->prev = nd;

            nd->right = new_node;

            nd->next = new_node;
            if(new_node->next == nullptr) last_element = new_node;
            else new_node->next->prev = new_node;
            inserted_node = new_node;
        }
        else nd->right = insert_node(nd->right, key, inserted_node);
    else
    {
        inserted_node = nullptr;
        return nd;
    }


    // Update the balance factor of each node and
    // balance the tree
    nd->height = int8_t(1 + std::max(get_height(nd->left),get_height(nd->right)));

    int8_t balanceFactor = balance_factor(nd);

    if (balanceFactor > 1)
    {
        cmp = mpz_cmp(*key, *(nd->left->val));
        if (cmp < 0) {
            return nd->rotate_right();
        }
        else if (cmp > 0)
        {
            nd->left = nd->left->rotate_left();
            return nd->rotate_right();
        }
    }

    if (balanceFactor < -1) {
        cmp = mpz_cmp(*key, *(nd->right->val));
        if (cmp > 0)
        {
            return nd->rotate_left();
        }
        else if (cmp < 0) {
            nd->right = nd->right->rotate_right();
            return nd->rotate_left();
        }
    }
    return nd;
}

/// insert key in tree, return inserted node
/// \param key value to insert
/// \return inserted node (nullptr if none)
Node LinkedTree::insert(bigint key)
{
    Node res;
    root = insert_node(root, key, res);
    return res;
}

/// insert key in tree, deletes key if already present, return inserted node
/// \param key value to insert
/// \return inserted node (nullptr if none)
Node LinkedTree::insert_delete_source(bigint key)
{
    Node res;
    root = insert_node(root, key, res);
    if(res == nullptr) {
        bigint_free(key);
    }
    return res;
}

/// deletes all elements of the tree and all nodes
void LinkedTree::cleanup()
{
    Node cursor = first_element;
    while(cursor != nullptr)
    {
        Node tmp = cursor->next;
        bigint_free(cursor->val);
        free(cursor);
        cursor = tmp;
    }
    first_element =nullptr;
    last_element = nullptr;
    size = 0;
}

Node LinkedTree::search_node(Node nd, bigint key)
{
    if(nd == nullptr) return nullptr;
    //assumption tree is not empty
    int cmp = mpz_cmp(*nd->val, *key);

    if(cmp > 0)
    {
        if(nd->prev == nullptr) return nullptr;
        return search_node(nd->left, key);
    }
    else if(cmp < 0)
    {
        if(nd->next == nullptr) return nullptr;
        return search_node(nd->right, key);
    }
    else return nd;
}

/// search for key inside the tree
/// \return node with key in the tree or nullptr
Node LinkedTree::search(bigint key) {
    return search_node(root, key);
}
/// search for key inside the tree, deletes key if key is found
/// \param key value to search
/// \return node with value = key in tree or nullptr
Node LinkedTree::search_delete_source(bigint key) {
    Node res = search_node(root, key);
    if(res != nullptr) {
        bigint_free(key);
    }
    return res;
}

Node LinkedTree::lower_bound_node(Node nd, bigint key) {
    if(nd == nullptr)
        return first_element;
    int cmp = mpz_cmp(*nd->val, *key);
    if(cmp > 0)
    {
        if(nd->prev == nullptr) return nd;
        else if(nd->left == nullptr)
        {
            auto iter = nd->prev;
            while(iter != nullptr)
            {
                int cmp2 = mpz_cmp(*iter->val, *key);
                if(cmp2 < 0) return iter;
                else if(cmp == 0) return iter->prev ? iter->prev : iter;

                iter = iter->prev;
            }
        }
        return lower_bound_node(nd->left, key);
    }
    else if(cmp < 0)
    {
        if(nd->next == nullptr) return nd;
        else if(nd->right == nullptr)
        {
            auto iter = nd->next;
            while(iter != nullptr)
            {
                int cmp2 = mpz_cmp(*iter->val, *key);
                if(cmp2 > 0) return iter;
                else if(cmp == 0) return iter->prev;
                iter = iter->next;
            }
        }
        return lower_bound_node(nd->right, key);
    }
    else
    {
        if(nd->prev != nullptr) return nd->prev;
        else return nd;
    }
}


Node LinkedTree::upper_bound_node(Node nd, bigint key) {
    if(nd == nullptr) return last_element;
    int cmp = mpz_cmp(*nd->val, *key);
    if(cmp > 0)
    {
        if(nd->prev == nullptr) return nd;
        else if(nd->left == nullptr)
        {
            auto iter = nd->prev;
            while(iter != nullptr)
            {
                int cmp2 = mpz_cmp(*iter->val, *key);
                if(cmp2 < 0) return iter;
                else if(cmp == 0) return iter->next;
                iter = iter->prev;
            }
        }
        return upper_bound_node(nd->left, key);
    }
    else if(cmp < 0)
    {
        if(nd->next == nullptr) return nd;
        else if(nd->right == nullptr)
        {
            auto iter = nd->next;
            while(iter != nullptr)
            {
                int cmp2 = mpz_cmp(*iter->val, *key);
                if(cmp2 > 0) return iter;
                else if (cmp == 0) return iter->next ? iter->next : iter;
                iter = iter->next;
            }
        }
        return upper_bound_node(nd->right, key);
    }
    else
    {
        if(nd->next != nullptr) return nd->next;
        else return nd;
    }}


Node LinkedTree::lower_bound(bigint key)
{
    return lower_bound_node(root, key);
}
Node LinkedTree::upper_bound(bigint key)
{
    return upper_bound_node(root, key);
}

/// moves all content of other into the tree, all nodes of other are deleted in the process
/// \param other tree to merge in current tree, unusable after completion of merge
void LinkedTree::merge(LinkedTree* other)
{
    auto iter = other->begin();

    while(iter != nullptr)
    {
        this->insert_delete_source(iter->val);
        auto tmp = iter->next;
        delete iter;
        iter = tmp;
    }
    delete other;
}

/// moves all content of other into the tree, all nodes of other are deleted in the process, returns the inserted nodes
/// \param other tree to merge in current tree, unusable after completion of merge
/// \return inserted nodes
LinkedList<Node>* LinkedTree::merge_return_inserted(LinkedList<bigint>* other) {
    auto result = new LinkedList<Node>();

    while (!other->empty())
    {
        auto inserted = this->insert_delete_source(other->top()); other->pop();
        if(inserted != nullptr)
            result->push(inserted);

    }
    delete other;
    return result;
}


