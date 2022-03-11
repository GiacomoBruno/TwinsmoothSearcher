#pragma once
#include "nb.h"

struct LLNode {
    void* value{};
    LLNode* next{};

    explicit LLNode(void* n) : value(n) {}

};

typedef LLNode* LNode;

class LinkedList {
    LNode first = nullptr;
    LNode last = nullptr;
    size_t _size = 0;
public:

    void push_back(void* n);
    void remove_first();
    void* pop();
    [[nodiscard]] size_t size() const {return _size;}

    LNode begin();
    LNode end();

    void clear();
    [[nodiscard]] bool empty() const;
};


