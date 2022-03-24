#pragma once
#include "nb.h"


struct LLNode {
    void* value = nullptr;
    LLNode* next = nullptr;
    explicit LLNode(void* n) : value(n) {}
};

typedef LLNode* LNode;

class LinkedList {
    LNode first = nullptr;

    size_t _size = 0;
public:

    void push_front(void* n);
    void remove_first();
    void* pop();
    [[nodiscard]] size_t size() const { return _size; }
    [[nodiscard]] LNode begin() const;
    void clear();
    [[nodiscard]] bool empty() const;
    void simple_merge(LinkedList* other);
};