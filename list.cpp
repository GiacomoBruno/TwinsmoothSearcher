#include "list.h"

//actually push front
void LinkedList::push_front(void* n) {
    _size++;

    if(first == nullptr){
        first = new LLNode(n);
    }
    else
    {
        auto tmp = new LLNode(n);
        tmp->next = first;
        first = tmp;
    }
}

void LinkedList::remove_first() {
    if(first == nullptr) return;

    _size--;

    auto tmp = first;
    first = first->next;
    delete tmp;
}

void* LinkedList::pop()
{
    if(first == nullptr) return nullptr;
    auto res = first->value;
    _size--;
    auto tmp = first;
    first = first->next;
    delete tmp;
    return res;
}


LNode LinkedList::begin() const { return first; }

void LinkedList::clear() {
    while(first != nullptr)
    {
        auto tmp = first->next;
        delete first;
        first = tmp;
    }
}

bool LinkedList::empty() const { return _size == 0; }

void print_as_bigint(LinkedList* l)
{
    auto iter = l->begin();
    while(iter != nullptr)
    {
        gmp_printf("%Zd\n", *VAL(iter)->val);
        iter = iter->next;
    }
}
