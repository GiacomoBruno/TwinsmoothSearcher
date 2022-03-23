//
// Created by Giacomo Bruno on 23/03/22.
//

#include "static_twinsmooth.h"

LinkedList *static_twinsmooth::create_chunks(LinkedList *input, size_t chunk_size) {
    auto all_chunks = new LinkedList();
    auto single_chunk = new LinkedList();

    size_t counter = 0;

    while(!input->empty())
    {
        if(counter > chunk_size)
        {
            counter = 0;
            all_chunks->push_front(single_chunk);
            single_chunk = new LinkedList();
        }

        single_chunk->push_front(input->pop());
        counter++;
    }

    if(counter != 0) all_chunks->push_front(single_chunk);

    return all_chunks;
}

LinkedList *static_twinsmooth::create_chunks(LinkedTree *input, size_t chunk_size) {
    auto all_chunks = new LinkedList();
    auto single_chunk = new LinkedList();

    size_t counter = 0;
    auto iter = input->begin();
    while(iter != nullptr)
    {
        if(counter > chunk_size)
        {
            counter = 0;
            all_chunks->push_front(single_chunk);
            single_chunk = new LinkedList();
        }

        single_chunk->push_front(iter);
        iter = iter->next;
        counter++;
    }

    if(counter != 0) all_chunks->push_front(single_chunk);

    return all_chunks;
}

void static_twinsmooth::k_get_ranges(Node nd, LinkedTree *results, size_t &lower_range, size_t &upper_range,
                                     size_t &lower_skip, size_t &upper_skip, bigfloat k, bigfloat oldk) {
    auto num = bigint_new;
    mpz_init2(*num, MPZ_INIT_BITS);

    auto flnum = bigfloat_new;
    mpf_init2(*flnum, MPZ_INIT_BITS);
    mpf_set_z(*flnum, *nd->val);

    mpf_div(*flnum, *flnum, *k);
    mpz_set_f(*num, *flnum);

    mpf_set_z(*flnum, *nd->val);
    mpf_mul(*flnum, *flnum, *k);

    lower_range = results->lower_bound(num)->distance(nd);
    mpz_set_f(*num, *flnum);
    upper_range = nd->distance(results->upper_bound(num));

    mpf_set_z(*flnum, *nd->val);

    mpf_div(*flnum, *flnum, *oldk);
    mpz_set_f(*num, *flnum);
    lower_skip = results->lower_bound(num)->distance(nd) + 1;

    mpf_set_z(*flnum, *nd->val);
    mpf_mul(*flnum, *flnum, *oldk);
    mpz_set_f(*num, *flnum);

    upper_skip = nd->distance(results->upper_bound(num)) + 1;

    bigfloat_free(flnum);
    bigint_free(num);
}

void static_twinsmooth::k_get_ranges(Node nd, LinkedTree *results, size_t &lower_range, size_t &upper_range, bigfloat k) {
    auto num = bigint_new;
    mpz_init2(*num, MPZ_INIT_BITS);

    auto flnum = bigfloat_new;
    mpf_init2(*flnum, MPZ_INIT_BITS);
    mpf_set_z(*flnum, *nd->val);

    mpf_div(*flnum, *flnum, *k);
    mpz_set_f(*num, *flnum);

    mpf_set_z(*flnum, *nd->val);
    mpf_mul(*flnum, *flnum, *k);

    lower_range = results->lower_bound(num)->distance(nd);
    mpz_set_f(*num, *flnum);
    upper_range = nd->distance(results->upper_bound(num));

    bigfloat_free(flnum);
    bigint_free(num);
}


LinkedTree *static_twinsmooth::generate_twinsmooth(LinkedList *chunk) {
    auto d = bigint_new;
    auto delta = bigint_new;
    auto m1 = bigint_new;
    mpz_init2(*d, MPZ_INIT_BITS);
    mpz_init2(*delta, MPZ_INIT_BITS);
    mpz_init2(*m1, MPZ_INIT_BITS);
    auto result = new LinkedTree();

    auto iter = chunk->begin();
    while(iter != nullptr)
    {
        auto x = VAL(iter); //costante per il ciclo corrente
        auto y = VAL(iter)->next;
        auto z = VAL(iter)->prev;

        // (x|y) with y > x
        while(y != nullptr)
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
        }

        // (x|z) with x > z
        while(z != nullptr)
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
    }

    bigint_free(d);
    bigint_free(delta);
    bigint_free(m1);
    return result;
}

LinkedTree *static_twinsmooth::r_generate_twinsmooth(LinkedList *chunk, size_t range) {
    auto d = bigint_new;
    auto delta = bigint_new;
    auto m1 = bigint_new;
    mpz_init2(*d, MPZ_INIT_BITS);
    mpz_init2(*delta, MPZ_INIT_BITS);
    mpz_init2(*m1, MPZ_INIT_BITS);
    auto result = new LinkedTree();

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
        while(counter < range && z != nullptr)
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
            counter++;
        }

        iter = iter->next;
    }

    bigint_free(d);
    bigint_free(delta);
    bigint_free(m1);
    return result;
}

LinkedTree *static_twinsmooth::r_generate_twinsmooth(LinkedList *chunk, size_t range, size_t skip_range) {
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
        auto y = VAL(iter)->skip(skip_range+(skip_range == 0));
        auto z = VAL(iter)->skip_back(skip_range+(skip_range == 0));

        // (x|y) with y > x
        while(counter < range-skip_range && y != nullptr)
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
        while(counter < range-skip_range && z != nullptr)
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
            counter++;
        }

        iter = iter->next;
    }

    bigint_free(d);
    bigint_free(delta);
    bigint_free(m1);
    return result;
}

LinkedTree *static_twinsmooth::k_generate_twinsmooth(LinkedList *chunk, LinkedTree *S, bigfloat k) {
    auto d = bigint_new;
    auto delta = bigint_new;
    auto m1 = bigint_new;
    mpz_init2(*d, MPZ_INIT_BITS);
    mpz_init2(*delta, MPZ_INIT_BITS);
    mpz_init2(*m1, MPZ_INIT_BITS);
    auto result = new LinkedTree();

    size_t lower_range, upper_range;

    auto iter = chunk->begin();
    k_get_ranges(VAL(iter), S, lower_range, upper_range, k);

    size_t counter = 0;
    while(iter != nullptr)
    {
        auto x = VAL(iter); //costante per il ciclo corrente
        auto y = VAL(iter)->next;
        auto z = VAL(iter)->prev;

        // (x|y) with y > x
        while(counter < lower_range && y != nullptr)
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
        while(counter < upper_range && z != nullptr)
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
            counter++;
        }

        iter = iter->next;
    }

    bigint_free(d);
    bigint_free(delta);
    bigint_free(m1);
    return result;
}

LinkedTree *static_twinsmooth::k_generate_twinsmooth(LinkedList *chunk, LinkedTree *S, bigfloat k, bigfloat oldk) {
    auto d = bigint_new;
    auto delta = bigint_new;
    auto m1 = bigint_new;
    mpz_init2(*d, MPZ_INIT_BITS);
    mpz_init2(*delta, MPZ_INIT_BITS);
    mpz_init2(*m1, MPZ_INIT_BITS);
    auto result = new LinkedTree();

    size_t lower_range, upper_range, lower_skip, upper_skip;

    auto iter = chunk->begin();
    k_get_ranges(VAL(iter), S, lower_range, upper_range, lower_skip, upper_skip, k, oldk);

    size_t counter = 0;
    while(iter != nullptr)
    {
        auto x = VAL(iter); //costante per il ciclo corrente
        auto y = VAL(iter)->skip(upper_skip);
        auto z = VAL(iter)->skip_back(lower_skip);

        // (x|y) with y > x
        while(counter < upper_range-upper_skip && y != nullptr)
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
        while(counter < lower_range-lower_skip && z != nullptr)
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
            counter++;
        }

        iter = iter->next;
    }

    bigint_free(d);
    bigint_free(delta);
    bigint_free(m1);
    return result;
}

LinkedTree *static_twinsmooth::iteration_S_N(LinkedList *N) {
    LinkedList* chunks = create_chunks(N, CHUNK_SIZE);
    auto result_tree = new LinkedTree();

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
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = generate_twinsmooth(points_arr[i]);
                points_arr[i]->clear();
                delete points_arr[i];
            }
        }

        for (int i = 0; i < (NUM_THREADS); i++)
        {
            if ((results)[i] != nullptr)
            {
                (result_tree)->merge((results)[i]);
            }
        }

        delete[] points_arr;
        delete[] results;

    }
    chunks->clear();
    delete chunks;

    return result_tree;
}

LinkedTree *static_twinsmooth::iteration_S_S(LinkedTree *S) {
    LinkedList* chunks = create_chunks(S, CHUNK_SIZE);
    auto result_tree = new LinkedTree();

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
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = generate_twinsmooth(points_arr[i]);
                points_arr[i]->clear();
                delete points_arr[i];
            }
        }

        for (int i = 0; i < (NUM_THREADS); i++)
        {
            if ((results)[i] != nullptr)
            {
                (result_tree)->merge((results)[i]);
            }
        }

        delete[] points_arr;
        delete[] results;

    }
    chunks->clear();
    delete chunks;

    return result_tree;
}

LinkedTree *static_twinsmooth::r_iteration_S_S(LinkedTree *S, size_t range) {
    LinkedList* chunks = create_chunks(S, CHUNK_SIZE);
    auto result_tree = new LinkedTree();

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
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = r_generate_twinsmooth(points_arr[i], range);
                points_arr[i]->clear();
                delete points_arr[i];
            }
        }

        for (int i = 0; i < (NUM_THREADS); i++)
        {
            if ((results)[i] != nullptr)
            {
                (result_tree)->merge((results)[i]);
            }
        }

        delete[] points_arr;
        delete[] results;

    }
    chunks->clear();
    delete chunks;

    return result_tree;
}

LinkedTree *static_twinsmooth::r_iteration_S_N(LinkedList *N, size_t range) {
    LinkedList* chunks = create_chunks(N, CHUNK_SIZE);
    auto result_tree = new LinkedTree();

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
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = r_generate_twinsmooth(points_arr[i], range);
                points_arr[i]->clear();
                delete points_arr[i];
            }
        }

        for (int i = 0; i < (NUM_THREADS); i++)
        {
            if ((results)[i] != nullptr)
            {
                (result_tree)->merge((results)[i]);
            }
        }

        delete[] points_arr;
        delete[] results;

    }
    chunks->clear();
    delete chunks;

    return result_tree;
}

LinkedTree *static_twinsmooth::k_iteration_S_S(LinkedTree *S, bigfloat k, bigfloat oldk) {
    LinkedList* chunks = create_chunks(S, CHUNK_SIZE);
    auto result_tree = new LinkedTree();

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
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = k_generate_twinsmooth(points_arr[i], S, k, oldk);
                points_arr[i]->clear();
                delete points_arr[i];
            }
        }

        for (int i = 0; i < (NUM_THREADS); i++)
        {
            if ((results)[i] != nullptr)
            {
                (result_tree)->merge((results)[i]);
            }
        }

        delete[] points_arr;
        delete[] results;

    }
    chunks->clear();
    delete chunks;

    return result_tree;
}

LinkedTree *static_twinsmooth::k_iteration_S_S(LinkedTree *S, bigfloat k) {
    LinkedList* chunks = create_chunks(S, CHUNK_SIZE);
    auto result_tree = new LinkedTree();

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
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = k_generate_twinsmooth(points_arr[i], S, k);
                points_arr[i]->clear();
                delete points_arr[i];
            }
        }

        for (int i = 0; i < (NUM_THREADS); i++)
        {
            if ((results)[i] != nullptr)
            {
                (result_tree)->merge((results)[i]);
            }
        }

        delete[] points_arr;
        delete[] results;

    }
    chunks->clear();
    delete chunks;

    return result_tree;
}

LinkedTree *static_twinsmooth::k_iteration_S_N(LinkedTree *S, LinkedList *N, bigfloat k) {
    LinkedList* chunks = create_chunks(N, CHUNK_SIZE);
    auto result_tree = new LinkedTree();

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
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = k_generate_twinsmooth(points_arr[i], S, k);
                points_arr[i]->clear();
                delete points_arr[i];
            }
        }

        for (int i = 0; i < (NUM_THREADS); i++)
        {
            if ((results)[i] != nullptr)
            {
                (result_tree)->merge((results)[i]);
            }
        }

        delete[] points_arr;
        delete[] results;

    }
    chunks->clear();
    delete chunks;

    return result_tree;
}

