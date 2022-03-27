//
// Created by Giacomo Bruno on 26/03/22.
//

#pragma once
#include "type_definitions.h"

struct tlnode {
    struct tlnode* next = nullptr;
    struct tlnode* prev = nullptr;
    struct tlnode* left = nullptr;
    struct tlnode* right = nullptr;

    int* val;

    explicit tlnode(int* v) : val(v) {}
};


class treelist {

};



