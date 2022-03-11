#include "tree.h"


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


void LinkedTree::light_cleanup()
{
    Node cursor = first_element;
    while(cursor != nullptr)
    {
        Node tmp = cursor->next;
        delete cursor;
        cursor = tmp;
    }

    first_element = nullptr;
    last_element = nullptr;
    size = 0;
}

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

// Insert a node
Node LinkedTree::insert_node(Node nd, bigint key, Node& inserted_node) {
    // Find the correct postion and insert the node

    if(nd == nullptr)
    {
        auto res = new struct node(key);
        first_element = res;
        last_element = res;
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

//tries to insert a node, if the node is already present clean up key
Node LinkedTree::insert_node_del(Node nd, bigint* key, Node& inserted_node)
{
    // Find the correct postion and insert the node
    if(nd == nullptr)
    {
        auto res = new node(*key);
        first_element = res;
        last_element = res;
        inserted_node = res;
        return res;
    }

    int cmp = mpz_cmp(**key, *(nd->val));


    if (cmp < 0)
        if(nd->left == nullptr)
        {
            size++;
            //insert on left node
            auto new_node = new struct node(*key);

            new_node->next = nd;
            new_node->prev = nd->prev;

            nd->left = new_node;

            nd->prev = new_node;

            if(new_node->prev == nullptr) first_element = new_node;
            else new_node->prev->next = new_node;

            inserted_node = new_node;
        }
        else nd->left = insert_node_del(nd->left, key, inserted_node);
    else if (cmp > 0)
        if(nd->right == nullptr)
        {
            size++;
            auto new_node = new struct node(*key);

            new_node->next = nd->next;
            new_node->prev = nd;

            nd->right = new_node;

            nd->next = new_node;
            if(new_node->next == nullptr) last_element = new_node;
            else new_node->next->prev = new_node;
            inserted_node = new_node;

        }
        else nd->right = insert_node_del(nd->right, key, inserted_node);
    else
    {
        bigint_free(*key);
        *key = nullptr;
        inserted_node = nullptr;
        return nd;
    }


    // Update the balance factor of each node and
    // balance the tree
    nd->height = (int8_t)(1 + std::max(get_height(nd->left),get_height(nd->right)));

    int8_t balanceFactor = balance_factor(nd);



    if (balanceFactor > 1)
    {
        cmp = mpz_cmp(**key, *(nd->left->val));
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
        cmp = mpz_cmp(**key, *(nd->right->val));
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


Node LinkedTree::erase_node(Node nd, bigint key)
{
    // Find the node and delete it
    if (nd == nullptr)
        return nd;

    int cmp = mpz_cmp(*key, *(nd->val));

    if (cmp < 0)
        root->left = erase_node(root->left, key);
    else if (cmp > 0)
        root->right = erase_node(root->right, key);
    else {
        if ((root->left == nullptr) || (root->right == nullptr))
        {
            size--;
            Node temp = root->left ? root->left : root->right;

            if (temp == nullptr)
            {
                temp = root;
                root = nullptr;
            }
            else
            {
                *root = *temp;
            }
            delete temp;
        }
        else
        {
            Node temp = min_val_node(root->right);
            root->val = temp->val;
            root->right = erase_node(root->right, temp->val);
        }
    }

    if (root == nullptr){
        return root;

    }

    // Update the balance factor of each node and
    // balance the tree
    root->height = int8_t(1 + std::max( get_height(root->left), get_height(root->right)));
    int8_t balanceFactor = balance_factor(root);
    if (balanceFactor > 1)
    {
        if (balance_factor(root->left) >= 0)
        {
            return root->rotate_right();
        }
        else
        {
            root->left = root->left->rotate_left();
            return root->rotate_right();
        }
    }
    else if (balanceFactor < -1)
    {
        if (balance_factor(root->right) <= 0)
        {
            return root->rotate_left();
        }
        else
        {
            root->right = root->right->rotate_right();
            return root->rotate_left();
        }
    }
    return root;
}

Node LinkedTree::delete_node(Node nd, bigint key)
{
    // Find the node and delete it, also cleans up key
    if (nd == nullptr)
        return nd;

    int cmp = mpz_cmp(*key, *(nd->val));

    if (cmp < 0)
        nd->left = delete_node(nd->left, key);
    else if (cmp > 0)
        nd->right = delete_node(nd->right, key);
    else {
        if ((nd->left == nullptr) || (nd->right == nullptr))
        {
            bigint_free(nd->val);
            size--;
            Node temp = nd->left ? nd->left : nd->right;

            if (temp == nullptr)
            {
                temp = nd;
                nd = nullptr;
            }
            else
            {
                *root = *temp;
            }

            delete temp;
        }
        else
        {
            Node temp = min_val_node(nd->right);
            nd->val = temp->val;
            nd->right = delete_node(nd->right, temp->val);
        }
    }

    if (nd == nullptr){
        return nd;

    }

    // Update the balance factor of each node and
    // balance the tree
    nd->height = (int8_t)(1 + std::max( get_height(nd->left), get_height(nd->right)));
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

Node LinkedTree::insert(bigint key)
{
    Node res;
    root = insert_node(root, key, res);
    return res;
}

Node LinkedTree::insert_del(bigint* key)
{
    Node res;
    root = insert_node_del(root, key, res);
    return res;
}

void LinkedTree::erase(bigint key)
{
    root = erase_node(root, key);
}

void LinkedTree::del(bigint key)
{
    root = delete_node(root, key);
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

Node LinkedTree::search_node(Node nd, bigint key)
{
    while(nd != nullptr)
    {
        int cmp = mpz_cmp(*key, *(nd->val));
        if( cmp > 0 )
        {
            nd = nd->left;
        }
        else if( cmp < 0 )
        {
            nd = nd->right;
        }
        else
        {
            return nd;
        }
    }
    return nullptr;
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

Node LinkedTree::search(bigint key) {
    return search_node(root, key);
}

LinkedList* LinkedTree::simple_merge(LinkedTree* other) {
    auto iter = other->begin();
    auto res = new LinkedList();
    while(iter != nullptr)
    {
        Node inserted = this->insert_del(&iter->val);
        if(inserted != nullptr)
        {
            res->push_back(inserted);
        }

        iter = iter->next;
    }
    return res;
}
