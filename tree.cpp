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

Node min_val_node(Node n)
{
    Node cursor = n;
    while(cursor->left != nullptr)
        cursor = cursor->left;
    return cursor;
}

//insertions

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

void LinkedTree::fast_insert(bigint key)
{
    root = fast_insert_node(root, key);
}

void LinkedTree::fast_insert_delete_source(bigint key)
{
    root = fast_insert_node_delete_source(root, key);
}

Node LinkedTree::insert(bigint key)
{
    Node res;
    root = insert_node(root, key, res);
    return res;
}

Node LinkedTree::insert_delete_source(bigint key)
{
    Node res;
    root = insert_node_delete_source(root, key, res);
    return res;
}

//removal

Node LinkedTree::erase_node(Node nd, bigint key)
{
    // Find the node and delete it
    if (nd == nullptr)
        return nd;

    int cmp = mpz_cmp(*key, *(nd->val));

    if (cmp < 0)
        nd->left = erase_node(nd->left, key);
    else if (cmp > 0)
        nd->right = erase_node(nd->right, key);
    else {
        if ((nd->left == nullptr) || (nd->right == nullptr))
        {
            size--;
            Node temp = nd->left ? nd->left : nd->right;

            if (temp == nullptr)
            {
                temp = nd;
                nd = nullptr;
            }
            else
            {
                *nd = *temp;
            }
            delete temp;
        }
        else
        {
            Node temp = min_val_node(nd->right);
            nd->val = temp->val;
            nd->right = erase_node(nd->right, temp->val);
        }
    }

    if (nd == nullptr){
        return nd;
    }

    // Update the balance factor of each node and
    // balance the tree
    nd->height = int8_t(1 + std::max( get_height(nd->left), get_height(nd->right)));
    int8_t balanceFactor = balance_factor(nd);
    if (balanceFactor > 1)
    {
        if (balance_factor(nd->left) >= 0)
        {
            return nd->rotate_right();
        }
        else
        {
            nd->left = nd->left->rotate_left();
            return nd->rotate_right();
        }
    }
    else if (balanceFactor < -1)
    {
        if (balance_factor(nd->right) <= 0)
        {
            return nd->rotate_left();
        }
        else
        {
            nd->right = nd->right->rotate_right();
            return nd->rotate_left();
        }
    }
    return nd;
}

Node erase_node_keep_list(Node nd, bigint key)
{
    // Find the node and delete it
    if (nd == nullptr)
        return nd;

    int cmp = mpz_cmp(*key, *(nd->val));

    if (cmp < 0)
        nd->left = erase_node_keep_list(nd->left, key);
    else if (cmp > 0)
        nd->right = erase_node_keep_list(nd->right, key);
    else {
        if ((nd->left == nullptr) || (nd->right == nullptr))
        {
            Node temp = nd->left ? nd->left : nd->right;

            if (temp == nullptr)
            {
                temp = nd;
                nd = nullptr;
            }
            else
            {
                *nd = *temp;
            }
            delete temp;
        }
        else
        {
            Node temp = min_val_node(nd->right);
            nd->val = temp->val;
            nd->right = erase_node_keep_list(nd->right, temp->val);
        }
    }

    if (nd == nullptr){
        return nd;
    }

    // Update the balance factor of each node and
    // balance the tree
    nd->height = int8_t(1 + std::max( get_height(nd->left), get_height(nd->right)));
    int8_t balanceFactor = balance_factor(nd);
    if (balanceFactor > 1)
    {
        if (balance_factor(nd->left) >= 0)
        {
            return nd->rotate_right();
        }
        else
        {
            nd->left = nd->left->rotate_left();
            return nd->rotate_right();
        }
    }
    else if (balanceFactor < -1)
    {
        if (balance_factor(nd->right) <= 0)
        {
            return nd->rotate_left();
        }
        else
        {
            nd->right = nd->right->rotate_right();
            return nd->rotate_left();
        }
    }
    return nd;
}

Node LinkedTree::destroy_node(Node nd, bigint key)
{
    // Find the node and delete it
    if (nd == nullptr)
        return nd;

    int cmp = mpz_cmp(*key, *nd->val);
    if (cmp < 0)
        nd->left = destroy_node(nd->left, key);
    else if (cmp > 0)
        nd->right = destroy_node(nd->right, key);
    else {
        if ((nd->left == nullptr) || (nd->right == nullptr)) {
            Node temp = nd->left != nullptr ? nd->left : nd->right;
            if (temp == nullptr)
            {
                bigint_free(nd->val);
                if(nd->next != nullptr) nd->next->prev = nd->prev;
                if(nd->prev != nullptr) nd->prev->next = nd->prev;
                delete nd;
                nd = nullptr;
            } else
            {
                bigint_free(nd->val);
                if(nd->next != nullptr) nd->next->prev = nd->prev;
                if(nd->prev != nullptr) nd->prev->next = nd->next;

                nd->val = temp->val;
                nd->next = temp->next;
                nd->prev = temp->prev;
                nd->right = temp->right;
                nd->left = temp->left;

                if(temp->next != nullptr) temp->next->prev = nd;
                if(temp->prev != nullptr) temp->prev->next = nd;
                delete temp;
            }
        } else {
            Node temp = min_val_node(nd->right);
            nd->val = temp->val;
            nd->right = destroy_node(nd->right,temp->val);
        }
    }

    if (nd == NULL)
        return nd;

    // Update the balance factor of each node and
    // balance the tree
    nd->height = 1 + std::max(get_height(nd->left),
                           get_height(nd->right));
    int8_t balanceFactor = balance_factor(nd);
    if (balanceFactor > 1) {
        if (balance_factor(nd->left) >= 0) {
            return (nd->rotate_right());
        } else {
            nd->left = (nd->left)->rotate_left();
            return (nd)->rotate_right();
        }
    }
    if (balanceFactor < -1) {
        if (balance_factor(nd->right) <= 0) {
            return (nd)->rotate_left();
        } else {
            nd->right = (nd->right)->rotate_right();
            return (nd)->rotate_left();
        }
    }
    return nd;
}

void LinkedTree::erase(bigint key)
{
    root = erase_node(root, key);
}

void LinkedTree::destroy(bigint key)
{
    root = destroy_node(root, key);
}

//clean up of tree

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
}

//search

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

Node LinkedTree::search(bigint key) {
    return search_node(root, key);
}

//merge

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

void LinkedTree::post_order(Node nd) {
    if(nd == nullptr) return;

    post_order(nd->left);
    gmp_printf("%Zd\n", *nd->val);
    post_order(nd->right);
}

void LinkedTree::print_order() {
    post_order(root);
}


void LinkedTree::print_tree(Node nd, std::string indent, bool last) {
    if (nd != nullptr) {
        std::cout << indent;
        if (last) {
            std::cout << "R----";
            indent += "   ";
        } else {
            std::cout << "L----";
            indent += "|  ";
        }
        gmp_printf("%Zd - (%d)\n", *(nd->val), get_height(nd));
        print_tree(nd->left, indent, false);
        print_tree(nd->right, indent, true);
    }
}

void LinkedTree::print(std::string& indent, bool last)
{
    print_tree(root, indent, last);
}


LinkedTree::~LinkedTree() = default;
