#pragma once

#include "nb.h"
#include "list.h"
#include <vector>


void printTree(Node root, std::string indent, bool last);
class LinkedTree {
private:
    size_t      size            = 0;
    Node        last_element    = nullptr;
    Node        first_element   = nullptr;
    Node        root            = nullptr;

    Node fast_insert_node(Node nd, bigint key);
    Node fast_insert_node_delete_source(Node nd, bigint key);
    Node insert_node(Node nd, bigint key, Node& insterted_node);
    Node insert_node_delete_source(Node nd, bigint key, Node& inserted_node);
    static Node search_node(Node nd, bigint key);
    Node lower_bound_node(Node nd, bigint key);
    Node upper_bound_node(Node nd, bigint key);


public:
    explicit LinkedTree() : size(0) { }
    ~LinkedTree() = default;
    [[nodiscard]]
    size_t get_size() const { return size; }
    [[nodiscard]]
    Node begin() const { return first_element; }
    [[nodiscard]]
    Node end() const { return last_element; }

    Node insert(bigint key);
    Node insert_delete_source(bigint key);
    void fast_insert(bigint key);
    void fast_insert_delete_source(bigint key);
    void cleanup();
    Node search(bigint key);
    Node lower_bound(bigint key);
    Node upper_bound(bigint key);
    void merge(LinkedTree* other);
    LinkedList* merge_return_inserted(LinkedTree* other);



};
