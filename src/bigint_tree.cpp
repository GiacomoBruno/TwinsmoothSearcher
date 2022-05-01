//
// Created by Giacomo Bruno on 21/04/22.
//

#include "../include/bigint_tree.h"

//node functions
void printTree(TreeNode root, std::string indent, bool last) {
    if (root != nullptr) {
        std::cout << indent;
        if (last) {
            std::cout << "R----";
            indent += "   ";
        } else {
            std::cout << "L----";
            indent += "|  ";
        }
        std::cout << *root->val <<std::endl;
        //gmp_printf("%Zd\n", *root->val);
        printTree(root->left, indent, false);
        printTree(root->right, indent, true);
    }
}

int8_t get_height(TreeNode n)
{
    if(n == nullptr) return 0;
    return n->height;
}

TreeNode treenode::rotate_right()
{
    TreeNode x = left;
    TreeNode t2 = x->right;
    x->right = this;
    left = t2;

    height = (int8_t)(std::max(get_height(left), get_height(right)) + 1);
    x->height = (int8_t)(std::max(get_height(x->left), get_height(x->right)) + 1);

    return x;
}

TreeNode treenode::rotate_left()
{
    TreeNode y = right;
    TreeNode t2 = y->left;
    y->left = this;
    right = t2;
    height = (int8_t)(std::max(get_height(left), get_height(right)) + 1);
    y->height = (int8_t)(std::max(get_height(y->left), get_height(y->right)) + 1);
    return y;
}

int8_t balance_factor(TreeNode n)
{
    if(n == nullptr) return 0;
    return (int8_t)(get_height(n->left) - get_height(n->right));
}

[[maybe_unused]]
TreeNode min_val_node(TreeNode n)
{
    TreeNode cursor = n;
    while(cursor->left != nullptr)
        cursor = cursor->left;
    return cursor;
}

size_t treenode::distance(TreeNode other)
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

TreeNode treenode::skip(size_t n)
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

TreeNode treenode::skip_back(size_t n)
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
TreeNode bigint_tree::insert_node(TreeNode nd, bigint* key, TreeNode& inserted_node) {
    // Find the correct postion and insert the node

    if(nd == nullptr)
    {
        auto res = new struct treenode(key);
        first_element = res;
        last_element = res;
        _size++;
        inserted_node = res;
        return res;
    }

    int cmp = *key < *nd->val;//mpz_cmp(*key, *(nd->val));


    if (cmp < 0)
        if(nd->left == nullptr)
        {
            _size++;
            //insert on left node
            auto new_node = new struct treenode(key);

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
            _size++;

            auto new_node = new struct treenode(key);

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
        cmp = *key < (*nd->left->val);//mpz_cmp(*key, *(nd->left->val));
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
        cmp = *key < (*nd->right->val);//mpz_cmp(*key, *(nd->right->val));
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
TreeNode bigint_tree::insert(bigint* key)
{
    TreeNode res;
    root = insert_node(root, key, res);
    return res;
}

/// insert key in tree, deletes key if already present, return inserted node
/// \param key value to insert
/// \return inserted node (nullptr if none)
TreeNode bigint_tree::insert_delete_source(bigint* key)
{
    TreeNode res;
    root = insert_node(root, key, res);
    if(res == nullptr) {
        delete key;
    }
    return res;
}

/// deletes all elements of the tree and all nodes
void bigint_tree::cleanup()
{
    TreeNode cursor = first_element;
    while(cursor != nullptr)
    {
        TreeNode tmp = cursor->next;
        //delete &(cursor->val);
        //bigint_free(cursor->val);
        delete cursor;
        cursor = tmp;
    }
    first_element =nullptr;
    last_element = nullptr;
    _size = 0;
}

TreeNode bigint_tree::search_node(TreeNode nd, bigint* key)
{
    if(nd == nullptr) return nullptr;
    //assumption tree is not empty
    int cmp = *nd->val < (*key);//mpz_cmp(*nd->val, *key);

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
TreeNode bigint_tree::search(bigint* key) {
    return search_node(root, key);
}

/// search for key inside the tree, deletes key if key is found
/// \param key value to search
/// \return node with value = key in tree or nullptr
TreeNode bigint_tree::search_delete_source(bigint* key) {
    TreeNode res = search_node(root, key);
    if(res != nullptr) {
        delete key;
    }
    return res;
}

TreeNode bigint_tree::lower_bound_node(TreeNode nd, bigint* key) {
    if(nd == nullptr)
        return first_element;
    int cmp = *nd->val < (*key);//mpz_cmp(*nd->val, *key);
    if(cmp > 0)
    {
        if(nd->prev == nullptr) return nd;
        else if(nd->left == nullptr)
        {
            auto iter = nd->prev;
            while(iter != nullptr)
            {
                int cmp2 = *iter->val < (*key);//mpz_cmp(*iter->val, *key);
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
                int cmp2 = *iter->val < (*key);//mpz_cmp(*iter->val, *key);
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


TreeNode bigint_tree::upper_bound_node(TreeNode nd, bigint* key) {
    if(nd == nullptr) return last_element;
    int cmp = *nd->val < (*key);//mpz_cmp(*nd->val, *key);
    if(cmp > 0)
    {
        if(nd->prev == nullptr) return nd;
        else if(nd->left == nullptr)
        {
            auto iter = nd->prev;
            while(iter != nullptr)
            {
                int cmp2 = *iter->val < (*key);//mpz_cmp(*iter->val, *key);
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
                int cmp2 = *iter->val < (*key);//mpz_cmp(*iter->val, *key);
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


TreeNode bigint_tree::lower_bound(bigint* key)
{
    return lower_bound_node(root, key);
}

TreeNode bigint_tree::upper_bound(bigint* key)
{
    return upper_bound_node(root, key);
}

/// moves all content of other into the tree, all nodes of other are deleted in the process
/// \param other tree to merge in current tree, unusable after completion of merge
void bigint_tree::merge(bigint_tree* other)
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
LinkedList<TreeNode>* bigint_tree::merge_return_inserted(LinkedList<bigint*>* other) {
    auto result = new LinkedList<TreeNode>();

    while (!other->empty())
    {
        auto inserted = this->insert_delete_source(other->top()); other->pop();
        if(inserted != nullptr)
            result->push(inserted);

    }
    delete other;
    return result;
}




TreeNode bigint_tree::delete_node(TreeNode _root, bigint* key, int cmp, const bool& del) {
    TreeNode rt = nullptr;
    if(cmp < 0) {
        auto ndleft = _root->left;
        if(ndleft == nullptr) return nullptr;

        int cmp1 = *key < (*ndleft->val);//mpz_cmp(*key, *ndleft->val);
        if(cmp1 == 0)
        {
            if(ndleft->next != nullptr) ndleft->next->prev = ndleft->prev;
            if(ndleft->prev != nullptr) ndleft->prev->next = ndleft->next;
            _root->left = ndleft->left ? ndleft->left : ndleft->right;
            rt = _root->left;
            if(del) { delete ndleft->val; }
            delete ndleft;
            _size--;
        }
        else return delete_node(_root->left, key, cmp1, del);
    }
    else if(cmp > 0) {
        auto ndright = _root->right;
        if(ndright == nullptr) return nullptr;

        int cmp1 = *key < (*ndright->val);//mpz_cmp(*key, *ndright->val);
        if(cmp1 == 0) {
            if (ndright->next != nullptr) ndright->next->prev = ndright->prev;
            if (ndright->prev != nullptr) ndright->prev->next = ndright->next;
            _root->right = ndright->right ? ndright->right : ndright->left;
            rt = _root->right;

            if (del) { delete ndright->val; }
            delete ndright;
            _size--;

        } else return delete_node(_root->right, key, cmp1, del);
    }

    if (rt == nullptr)
        return rt;

    // Update the balance factor of each node and
    // balance the tree
    rt->height = std::max(get_height(rt->left), get_height(rt->right)) + 1;
    int8_t balanceFactor = balance_factor(rt);
    if (balanceFactor > 1) {
        if (balance_factor(rt->left) >= 0) {
            return rt->rotate_right();
        } else {
            rt->left = rt->left->rotate_left();
            return rt->rotate_right();
        }
    }
    if (balanceFactor < -1) {
        if (balance_factor(rt->right) <= 0) {
            return rt->rotate_left();
        } else {
            rt->right = rt->right->rotate_right();
            return rt->rotate_left();
        }
    }
    return rt;
}

void bigint_tree::remove(bigint* key) {
    if(root == nullptr) return;
    int cmp = *key < (*root->val);//mpz_cmp(*key, *root->val);
    if(cmp == 0)
    {
        auto temp = root->left ? root->left : root->right;
        if(root->next != nullptr) root->next->prev = root->prev;
        if(root->prev != nullptr) root->prev->next = root->next;

        delete root;
        _size--;
        root = temp;
    }
    else delete_node(root, key, cmp, false);
}

void bigint_tree::remove_del(bigint* key) {
    if(root == nullptr) return;
    int cmp = *key < (*root->val);//mpz_cmp(*key, *root->val);
    if(cmp == 0)
    {
        auto temp = root->left ? root->left : root->right;
        if(root->next != nullptr) root->next->prev = root->prev;
        if(root->prev != nullptr) root->prev->next = root->next;


        //bigint_free(root->val);
        delete root;
        _size--;
        root = temp;
    }
    else delete_node(root, key, cmp, true);
}

TreeNode bigint_tree::search_node(TreeNode nd, const mpz_t& key) {
    if(nd == nullptr) return nullptr;
    //assumption tree is not empty
    int cmp = *nd->val < (key);//mpz_cmp(*nd->val, *key);

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

TreeNode bigint_tree::search(const mpz_t& key) const {
    return search_node(root, key);
}
