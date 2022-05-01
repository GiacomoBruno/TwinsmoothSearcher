//
// Created by Giacomo Bruno on 21/04/22.
//

#pragma once
#include "list.h"
#include "bigint.h"


struct treenode {
    treenode* left = nullptr;
    treenode* right = nullptr;

    treenode* next = nullptr;
    treenode* prev = nullptr;

    int8_t height = 1;
    //uint16_t twins_found = 0;

    bigint* val;

    explicit treenode(bigint* v) : val(v){}
    ~treenode() {
        delete val;
    }

    treenode* rotate_right();
    treenode* rotate_left();
    size_t distance(treenode* other);
    treenode* skip(size_t n);
    treenode* skip_back(size_t n);
};


using TreeNode =  treenode*;

void printTree(TreeNode root, std::string indent, bool last);
class bigint_tree {
private:
    size_t      _size            = 0;
    TreeNode        last_element    = nullptr;
    TreeNode        first_element   = nullptr;


    TreeNode insert_node(TreeNode nd, bigint* key, TreeNode& insterted_node);
    TreeNode delete_node(TreeNode root, bigint* key, int cmp, const bool& del);

    static TreeNode search_node(TreeNode nd, bigint* key);
    static TreeNode search_node(TreeNode nd, const mpz_t& key);
    TreeNode lower_bound_node(TreeNode nd, bigint* key);
    TreeNode upper_bound_node(TreeNode nd, bigint* key);

public:
    explicit bigint_tree() : _size(0) { }
    ~bigint_tree() = default;
    [[nodiscard]]
    size_t size() const { return _size; }
    [[nodiscard]]
    bool empty() const { return _size == 0;}
    [[nodiscard]]
    TreeNode begin() const { return first_element; }
    [[nodiscard]]
    TreeNode end() const { return last_element; }

    TreeNode insert(bigint* key);
    TreeNode insert_delete_source(bigint* key);
    void cleanup();
    TreeNode search(bigint* key);
    [[nodiscard]] TreeNode search(const mpz_t& key) const;
    TreeNode search_delete_source(bigint* key);
    TreeNode lower_bound(bigint* key);
    TreeNode upper_bound(bigint* key);
    void merge(bigint_tree* other);
    LinkedList<TreeNode>* merge_return_inserted(LinkedList<bigint*>* other);

    void remove(bigint* key);
    void remove_del(bigint* key);

    TreeNode        root            = nullptr;
};


