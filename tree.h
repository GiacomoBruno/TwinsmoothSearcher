#pragma once

#include "type_definitions.h"
#include "list.h"

struct
__attribute__((__packed__))
node {
    node* left = nullptr;
    node* right = nullptr;

    node* next = nullptr;
    node* prev = nullptr;

    int8_t height = 1;
    int16_t twins_found = 0;

    bigint val = nullptr;

    explicit node(bigint v) : val(v){}

    node* rotate_right();
    node* rotate_left();
    size_t distance(node* other);
    node* skip(size_t n);
    node* skip_back(size_t n);
};

typedef node* Node;

void printTree(Node root, std::string indent, bool last);
class LinkedTree {
private:
    size_t      size            = 0;
    Node        last_element    = nullptr;
    Node        first_element   = nullptr;

    Node insert_node(Node nd, bigint key, Node& insterted_node);
    static Node search_node(Node nd, bigint key);
    Node lower_bound_node(Node nd, bigint key);
    Node upper_bound_node(Node nd, bigint key);


public:
    explicit LinkedTree() : size(0) { }
    ~LinkedTree() = default;
    [[nodiscard]]
    size_t get_size() const { return size; }
    [[nodiscard]]
    bool empty() const { return size == 0;}
    [[nodiscard]]
    Node begin() const { return first_element; }
    [[nodiscard]]
    Node end() const { return last_element; }

    Node insert(bigint key);
    Node insert_delete_source(bigint key);
    void cleanup();
    Node search(bigint key);
    Node search_delete_source(bigint key);
    Node lower_bound(bigint key);
    Node upper_bound(bigint key);
    void merge(LinkedTree* other);
    LinkedList<Node>* merge_return_inserted(LinkedList<bigint>* other);

    void remove(bigint key);
    void remove_del(bigint key);


    Node        root            = nullptr;
};


