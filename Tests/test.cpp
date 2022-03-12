//
// Created by Giacomo Bruno on 12/03/22.
//

#include "test.h"
#include "../tree.h"

bigint* create_array(int* nums, int size)
{
    auto res = new bigint[size];
    for(int i = 0; i < size; i++)
    {
        res[i] = bigint_new;
        bigint_init(res[i], nums[i]);
    }
    return res;
}

void free_array(bigint* array, int size)
{
    for(int i = 0; i < size; i++)
    {
        bigint_free(array[i]);
    }
    delete[] array;
}

bool test_insert(bigint* array)
{
    auto lr = new LinkedTree();

    bool res = true;
    for(int i = 0; i < 8; i++)
    {
        res &= lr->get_size() == i;
        lr->insert(array[i]);
    }

    for(int i = 0; i < 8; i++)
    {
        res&= lr->get_size() == 8;
        lr->insert(array[i]);
    }



    lr->cleanup();
    delete lr;
    return res;
}

bool test_fast_insert(bigint* array)
{
    auto lr = new LinkedTree();
    bool res = true;
    for(int i = 0; i < 8; i++)
    {
        res &= lr->get_size() == i;
        lr->fast_insert(array[i]);
    }

    for(int i = 0; i < 8; i++)
    {
        res&= lr->get_size() == 8;
        lr->insert(array[i]);
    }
    lr->cleanup();
    delete lr;

    return res;
}

bool test_insert_delete_source(bigint* array)
{
    auto lr = new LinkedTree();


    bool res = true;
    for(int i = 0; i < 8; i++)
    {
        res &= lr->get_size() == i;
        lr->insert_delete_source(array[i]);
    }
    lr->cleanup();
    delete lr;

    return res;
}

bool test_fast_insert_delete_source(bigint* array)
{
    auto lr = new LinkedTree();


    bool res = true;
    for(int i = 0; i < 8; i++)
    {
        res &= lr->get_size() == i;
        lr->fast_insert_delete_source(array[i]);
    }


    lr->cleanup();
    delete lr;

    return res;
}

bool test::run_tests() {
    int arr[8] = {1,2,3,4,5,6,7, 8};


    bigint* array = create_array(arr, 8);
    if(!test_insert(array))
    {
        std::cout << "test_insert failed..." << std::endl;
        return false;
    } else std::cout << "test_insert passed." << std::endl;
    delete[] array;


    array = create_array(arr, 8);
    if(!test_fast_insert(array))
    {
        std::cout << "test_fast_insert failed..." << std::endl;
        return false;
    } else std::cout << "test_fast_insert passed." << std::endl;
    delete[] array;

    array = create_array(arr, 8);
    if(!test_insert_delete_source(array))
    {
        std::cout << "test_insert_delete_source failed..." << std::endl;
        return false;
    } else std::cout << "test_insert_delete_source passed." << std::endl;
    delete[] array;


    array = create_array(arr, 8);
    if(!test_fast_insert_delete_source(array))
    {
        std::cout << "test_fast_insert_delete_source failed..." << std::endl;
        return false;
    } else std::cout << "test_fast_insert_delete_source passed." << std::endl;

    delete[] array;

    return true;
}