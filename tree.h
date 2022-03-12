#pragma once

#include "nb.h"
#include "list.h"
#include <vector>




class LinkedTree {
private:
    Node root = nullptr;
    size_t size;
    Node first_element{};
    Node last_element{};

    //insert a key in the tree and returns the new root
    Node fast_insert_node(Node nd, bigint key);
    //insert a key in the tree and returns the new root, also deallocate memory used by key if it was already present
    Node fast_insert_node_delete_source(Node nd, bigint key);

    //insert a key in the tree, returns the new root and populate inserted node with the node that has been inserted
    //if inserted_node == nullptr then no element was inserted
    Node insert_node(Node nd, bigint key, Node& insterted_node);

    //insert a key in the tree, returns the new root and populate inserted node with the node that has been inserted
    //if inserted_node == nullptr then no element was inserted and key has been deallocated
    Node insert_node_delete_source(Node nd, bigint key, Node& inserted_node);

    Node erase_node(Node nd, bigint key);
    Node destroy_node(Node nd, bigint key);

    static Node search_node(Node nd, bigint key);

    void print_tree(Node nd, std::string indent, bool last);

    void post_order(Node nd);

public:
    explicit LinkedTree() : size(0) { }
    ~LinkedTree();
    [[nodiscard]] size_t get_size() const { return size; }

    Node begin() { return first_element; }
    Node end() { return last_element; }

    Node insert(bigint key);
    Node insert_delete_source(bigint key);
    void fast_insert(bigint key);
    void fast_insert_delete_source(bigint key);

    void remove_node(Node nd);

    void erase(bigint key);
    void destroy(bigint key);

    void cleanup();
    void light_cleanup();

    Node search(bigint key);


    void print(std::string& indent, bool last);
    void print_order();


    void merge(LinkedTree* other);
    LinkedList* merge_return_inserted(LinkedTree* other);

};
