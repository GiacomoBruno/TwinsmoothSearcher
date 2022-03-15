#include "tree.h"

//node functions

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

//insertions

/// inserts key in tree, returns new root
/// \param nd node used for searching where to place key
/// \param key value to store in tree
/// \return root of the resulting tree after insertion
Node LinkedTree::fast_insert_node(Node nd, bigint key) {
    if(nd == nullptr)
    {
        auto res = new struct node(key);
        first_element = res;
        last_element = res;
        size++;
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

        }
        else nd->left = fast_insert_node(nd->left, key);
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
        }
        else nd->right = fast_insert_node(nd->right, key);
    else
    {
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

/// inserts key in tree, if key already present then deletes key from memory
/// \param nd node used for searching where to place key
/// \param key value to store in tree
/// \return root of the resulting tree after insertion
Node LinkedTree::fast_insert_node_delete_source(Node nd, bigint key) {
    if(nd == nullptr)
    {
        auto res = new struct node(key);
        first_element = res;
        last_element = res;
        size++;
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

        }
        else nd->left = fast_insert_node_delete_source(nd->left, key);
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
        }
        else nd->right = fast_insert_node_delete_source(nd->right, key);
    else
    {
        bigint_free(key);
        return nd;
    }


    // Update the balance factor of each node and
    // balance the tree
    nd->height = (int8_t)(1 + std::max(get_height(nd->left),get_height(nd->right)));

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

/// inserts key in tree, returns new root, populates inserted_node with the node inserted in the tree.
/// if key already present then deletes key from memory
/// \param nd node used for searching where to place key
/// \param key value to store in tree
/// \param inserted_node node inserted in the tree, nullptr if nothing was inserted
/// \return root of the resulting tree after insertion
Node LinkedTree::insert_node_delete_source(Node nd, bigint key, Node& inserted_node)
{
    // Find the correct postion and insert the node
    if(nd == nullptr)
    {
        auto res = new struct node(key);
        first_element = res;
        last_element = res;
        inserted_node = res;
        size++;
        return res;
    }

    int cmp = mpz_cmp(*key, *(nd->val));
    if (cmp < 0)
    {
        if (nd->left == nullptr)
        {
            size++;
            //insert on left node
            auto new_node = new struct node(key);

            new_node->next = nd;
            new_node->prev = nd->prev;

            nd->left = new_node;

            nd->prev = new_node;

            if (new_node->prev == nullptr) first_element = new_node;
            else new_node->prev->next = new_node;

            inserted_node = new_node;
        }
        else nd->left = insert_node_delete_source(nd->left, key, inserted_node);
    }
    else if (cmp > 0) {
        if (nd->right == nullptr)
        {
            size++;
            auto new_node = new struct node(key);

            new_node->next = nd->next;
            new_node->prev = nd;

            nd->right = new_node;

            nd->next = new_node;
            if (new_node->next == nullptr) last_element = new_node;
            else new_node->next->prev = new_node;
            inserted_node = new_node;

        }
        else nd->right = insert_node_delete_source(nd->right, key, inserted_node);
    }
    else
    {
        bigint_free(key);
        inserted_node = nullptr;
        return nd;
    }


    // Update the balance factor of each node and
    // balance the tree
    nd->height = (int8_t)(1 + std::max(get_height(nd->left),get_height(nd->right)));

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

    if (balanceFactor < -1)
    {
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

/// insert key in tree
/// \param key value to insert
void LinkedTree::fast_insert(bigint key)
{
    root = fast_insert_node(root, key);
}

/// insert key in tree, deletes key if already present
/// \param key value to insert
void LinkedTree::fast_insert_delete_source(bigint key)
{
    root = fast_insert_node_delete_source(root, key);
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
    root = insert_node_delete_source(root, key, res);
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

/// search for key inside the tree
/// \return node with key in the tree or nullptr
Node LinkedTree::search_node(Node nd, bigint key)
{
    if(nd == nullptr)
        return nullptr;

    int cmp = mpz_cmp(*key, *(nd->val));
    if( cmp > 0 )
    {
        return search_node(nd->right, key);
    }
    else if( cmp < 0 )
    {
        return search_node(nd->left, key);
    }
    else
    {
        return nd;
    }
}

/// search for key inside the tree
/// \return node with key in the tree or nullptr
Node LinkedTree::search(bigint key) {
    return search_node(root, key);
}

//merge

/// moves all content of other into the tree, all nodes of other are deleted in the process
/// \param other tree to merge in current tree, unusable after completion of merge
void LinkedTree::merge(LinkedTree* other)
{
    auto iter = other->begin();

    while(iter != nullptr)
    {
        this->fast_insert_delete_source(iter->val);
        auto tmp = iter->next;
        delete iter;
        iter = tmp;
    }
    delete other;
}

/// moves all content of other into the tree, all nodes of other are deleted in the process, returns the inserted nodes
/// \param other tree to merge in current tree, unusable after completion of merge
/// \return inserted nodes
LinkedList* LinkedTree::merge_return_inserted(LinkedTree* other) {
    auto iter = other->begin();
    auto result = new LinkedList();

    while (iter != nullptr)
    {
        auto inserted = this->insert_delete_source(iter->val);
        auto tmp = iter->next;
        delete iter;
        iter = tmp;
        if(inserted != nullptr)
            result->push_front(inserted);

    }
    delete other;
    return result;
}
