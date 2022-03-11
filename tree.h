#pragma once

#include "nb.h"
#include <vector>

struct node {
    node* left = nullptr;
    node* right = nullptr;

    node* next = nullptr;
    node* prev = nullptr;

    int8_t height = 1;

    bigint val = nullptr;

    explicit node(bigint v) : val(v){}

    node* rotate_right();
    node* rotate_left();
};

typedef node* Node;



class LinkedTree {
private:
    Node root = nullptr;
    size_t size;
    Node first_element{};
    Node last_element{};

    Node insert_node(Node nd, bigint key, Node& insterted_node);
    Node insert_node_del(Node nd, bigint* key, Node& inserted_node);

    Node erase_node(Node nd, bigint key);
    Node delete_node(Node nd, bigint key);

    static Node search_node(Node nd, bigint key);

    void print_tree(Node nd, std::string indent, bool last);

    void post_order(Node nd);

public:
    explicit LinkedTree() : size(0) { }
    ~LinkedTree() { light_cleanup(); }
    [[nodiscard]] size_t get_size() const { return size; }

    Node begin() { return first_element; }
    Node end() { return last_element; }

    Node insert(bigint key);
    Node insert_del(bigint* key);

    void erase(bigint key);
    void del(bigint key);

    void cleanup();
    void light_cleanup();

    Node search(bigint key);

    std::vector<Node>* simple_merge(LinkedTree* other);

    void print(std::string& indent, bool last);
    void print_order();
};
