#pragma once

template<typename T>
struct LLNode {
    T value = nullptr;
    LLNode* next = nullptr;
    explicit LLNode(T n) : value(n) {}
};

template<class T>
using LNode = LLNode<T>*;

template<typename T>
class LinkedList {
    LNode<T> first{nullptr};
    LNode<T> last{nullptr};
    unsigned long long _size{0};
public:

    void push(T n);
    void push(LinkedList<T>* l);
    void push_back(T n);

    void pop();
    T top();
    LNode<T> end();
    LNode<T> begin();
    [[nodiscard]] unsigned long long size() const { return _size; }
    void clear();
    [[nodiscard]] bool empty() const;
};


//actually push front
template<class T>
void LinkedList<T>::push(T n) {
    _size++;

    if(first == nullptr){
        first = new LLNode(n);
        last = first;
    }
    else
    {
        auto tmp = new LLNode(n);
        tmp->next = first;
        first = tmp;
    }
}



template<class T>
void LinkedList<T>::pop()
{
    if(first == nullptr) return;

    _size--;
    auto tmp = first->next;
    delete first;
    first = tmp;
}

template<class T>
T LinkedList<T>::top()
{
    if(first == nullptr) return nullptr;
    return first->value;
}

template<class T>
void LinkedList<T>::clear() {
    while(first != nullptr)
    {
        auto tmp = first->next;
        delete first;
        first = tmp;
    }
    last = nullptr;
    _size = 0;

}

template<class T>
bool LinkedList<T>::empty() const { return _size == 0; }

template<class T>
LNode<T> LinkedList<T>::end() {
    return last;
}

template<class T>
LNode<T> LinkedList<T>::begin() {
    return first;
}

template<class T>
void LinkedList<T>::push(LinkedList<T>* l) {

    if(l->empty()) return;

    auto l_last = l->end();

    l_last->next = first;
    first = l->first;
    _size = _size + l->size();
    if(last == nullptr) last = l->last;
    delete l;
}

template<typename T>
void LinkedList<T>::push_back(T n) {
    auto l = end();
    if(l == nullptr) return push(n);
    l->next = new LLNode(n);
    last = l->next;
    _size++;
}
