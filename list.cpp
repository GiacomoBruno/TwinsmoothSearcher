#include "list.h"

//actually push front
void LinkedList::push_front(void* n) {
    _size++;

    if(first == nullptr){//} || last == nullptr) {
        //last =  ;
        first = new LLNode(n);
    }
    else
    {
        auto tmp = new LLNode(n);
        tmp->next = first;
        first = tmp;
        //last->next = new LLNode(n);
        //last = last->next;
    }
}

void LinkedList::remove_first() {
    if(first == nullptr) return;

    _size--;
    //if(first == last)
   // {
   //     delete first;
   //     first = nullptr;
   //     last = nullptr;
    //}
    //else
    {
        auto tmp = first;
        first = first->next;
        delete tmp;
    }
}

void* LinkedList::pop()
{
    if(first == nullptr) return nullptr;
    auto res = first->value;
    remove_first();
    return res;
}


LNode LinkedList::begin() { return first; }
//LNode LinkedList::end(){ return last; }

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
