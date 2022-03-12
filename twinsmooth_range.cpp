//
// Created by Giacomo Bruno on 12/03/22.
//

#include "twinsmooth_range.h"

LinkedTree* generate_twinsmooth_from_chunks(LinkedList* chunk, int range)
{
    auto d = bigint_new;
    auto delta = bigint_new;
    auto m1 = bigint_new;
    mpz_init2(*d, MPZ_INIT_BITS);
    mpz_init2(*delta, MPZ_INIT_BITS);
    mpz_init2(*m1, MPZ_INIT_BITS);
    auto result = new LinkedTree();

    // print_as_bigint(chunk);

    auto iter = chunk->begin();
    int counter = 0;
    while(iter != nullptr)
    {
        auto x = VAL(iter); //costante per il ciclo corrente
        auto y = VAL(iter)->next;
        auto z = VAL(iter)->prev;

        // (x|y) with y > x
        while(counter < range && y != nullptr)
        {
            mpz_mul(*m1, *(x->val), *(y->val));
            mpz_add(*m1, *m1, *(y->val));
            mpz_sub(*delta, *(y->val), *(x->val));
            mpz_mod(*d, *m1, *delta);

            if (mpz_cmp_ui(*d, 0) == 0)
            {
                auto nv = bigint_new;
                mpz_init2(*nv, MPZ_INIT_BITS);

                mpz_div(*m1, *m1, *delta);
                mpz_sub_ui(*nv, *m1, 1);

                result->insert_delete_source(nv);
            }

            y = y->next;
            counter++;
        }

        counter = 0;
        // (x|z) with x > z
        while(counter < 0 && z != nullptr)
        {
            mpz_mul(*m1, *(z->val), *(x->val));
            mpz_add(*m1, *m1, *(x->val));
            mpz_sub(*delta, *(x->val), *(z->val));
            mpz_mod(*d, *m1, *delta);

            if (mpz_cmp_ui(*d, 0) == 0)
            {
                auto nv = bigint_new;
                mpz_init2(*nv, MPZ_INIT_BITS);

                mpz_div(*m1, *m1, *delta);
                mpz_sub_ui(*nv, *m1, 1);

                result->insert_delete_source(nv);
            }

            z = z->prev;
        }

        iter = iter->next;
        counter++;
    }

    bigint_free(d);
    bigint_free(delta);
    bigint_free(m1);
    return result;
}

LinkedTree* iteration(LinkedList* points, int range)
{
    auto chunks = create_chunks(points, 100);

    //delete after use
    auto result_tree = new LinkedTree();
    //set all array values to nullptr

    while(!chunks->empty())
    {
        auto results = new LinkedTree*[NUM_THREADS];
        auto points_arr = new LinkedList*[NUM_THREADS];
        NULL_INIT_ARRAY(points_arr, NUM_THREADS);
        NULL_INIT_ARRAY(results, NUM_THREADS);


        for (int i = 0; i < (NUM_THREADS); i++) {
            auto pts = (LinkedList*)(chunks->pop());
            if (pts == nullptr) break;
            points_arr[i] = pts;
        }

#pragma omp parallel num_threads(NUM_THREADS)
        //for(int i = 0; i < NUM_THREADS; i++)
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = generate_twinsmooth_from_chunks(points_arr[i], range);
                points_arr[i]->clear();
                delete points_arr[i];
            }
        }

        for (int i = 0; i < (8); i++)
        {
            if ((results)[i] != nullptr)
            {
                (result_tree)->merge((results)[i]);
            }
        }
        //INSERT_ARRAY_MEMBERS_INTO_TREE(results, result_tree, NUM_THREADS);

        //DEALLOCATE_ARRAY_MEMBERS(results, NUM_THREADS);
        delete[] points_arr;
        delete[] results;

    }
    chunks->clear();
    delete chunks;

    return result_tree;


}


void twinsmooth_range::execute() {

    load_files();

    size_t new_found = 0;
    while(!computation_numbers->empty())
    {
        auto new_res = iteration(computation_numbers, range);
        computation_numbers->clear();
        delete computation_numbers;
        computation_numbers = results->merge_return_inserted(new_res);
        std::cout << "found " << computation_numbers->size() << " new twinsmooth" << std::endl;
    }

    computation_numbers->clear();
    delete computation_numbers;

}

void twinsmooth_range::terminate() {
    std::cout << "found in total: " << results->get_size() << std::endl;
    save_files();
    results->cleanup();
    delete results;
}
