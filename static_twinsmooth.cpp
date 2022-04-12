//
// Created by Giacomo Bruno on 23/03/22.
//

#include "static_twinsmooth.h"

LinkedList<Node*>* static_twinsmooth::create_chunks(LinkedList<Node> *input) {

    auto result = new LinkedList<Node*>();
    auto single_chunk = new Node[CHUNK_SIZE];

    size_t counter = 0;
    while(!input->empty())
    {
        if(counter >= CHUNK_SIZE)
        {
            counter = 0;
            result->push(single_chunk);
            single_chunk = new Node[CHUNK_SIZE];
        }

        single_chunk[counter] = input->top(); input->pop();
        counter++;
    }

    if(counter != 0) {
        for(size_t i = counter; i < CHUNK_SIZE; i++)
        {
            single_chunk[i] = nullptr;
        }
        result->push(single_chunk);
    }

    return result;



}

LinkedList<Node*>* static_twinsmooth::create_chunks(LinkedTree* input)
{
    auto result = new LinkedList<Node*>();
    auto single_chunk = new Node[CHUNK_SIZE];

    size_t counter = 0;
    auto iter = input->begin();
    while(iter != nullptr)
    {
        if(counter >= CHUNK_SIZE)
        {
            counter = 0;
            result->push(single_chunk);
            single_chunk = new Node[CHUNK_SIZE];
        }

        single_chunk[counter] = iter;
        iter = iter->next;
        counter++;
    }

    if(counter != 0) {
        for(size_t i = counter; i < CHUNK_SIZE; i++)
        {
            single_chunk[i] = nullptr;
        }
        result->push(single_chunk);
    }

    return result;
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



LinkedList<bigint>* static_twinsmooth::generate_twinsmooth(Node* chunk, LinkedTree* S) {
    auto d = bigint_new;
    auto delta = bigint_new;
    auto m1 = bigint_new;
    mpz_init2(*d, MPZ_INIT_BITS);
    mpz_init2(*delta, MPZ_INIT_BITS);
    mpz_init2(*m1, MPZ_INIT_BITS);
    auto result = new LinkedList<bigint>();


    int i = 0;
    while(i < CHUNK_SIZE && chunk[i] != nullptr)
    {
        auto x = chunk[i]; //costante per il ciclo corrente
        auto y = chunk[i]->next;
        auto z = chunk[i]->prev;

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

                if(S->search(nv) == nullptr)
                {
                    result->push(nv);
                    x->twins_found++;
                }
                else
                {
                    bigint_free(nv);
                }
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

                if(S->search(nv) == nullptr)
                {
                    result->push(nv);
                    x->twins_found++;
                }
                else
                {
                    bigint_free(nv);
                }
            }

            z = z->prev;
        }

        i++;
    }

    bigint_free(d);
    bigint_free(delta);
    bigint_free(m1);
    return result;
}

LinkedList<bigint>* static_twinsmooth::r_generate_twinsmooth(Node* chunk, LinkedTree* S, size_t range) {
    auto d = bigint_new;
    auto delta = bigint_new;
    auto m1 = bigint_new;
    mpz_init2(*d, MPZ_INIT_BITS);
    mpz_init2(*delta, MPZ_INIT_BITS);
    mpz_init2(*m1, MPZ_INIT_BITS);
    auto result = new LinkedList<bigint>();


    int i = 0;
    size_t counter = 0;
    while(i < CHUNK_SIZE && chunk[i] != nullptr)
    {
        auto x = chunk[i]; //costante per il ciclo corrente
        auto y = chunk[i]->next;
        auto z = chunk[i]->prev;

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

                if(S->search(nv) == nullptr)
                {
                    result->push(nv);
                    x->twins_found++;
                }
                else
                {
                    bigint_free(nv);
                }
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

                if(S->search(nv) == nullptr)
                {
                    result->push(nv);
                    x->twins_found++;
                }
                else
                {
                    bigint_free(nv);
                }
            }
            z = z->prev;
            counter++;
        }
        i++;
    }

    bigint_free(d);
    bigint_free(delta);
    bigint_free(m1);
    return result;
}

LinkedList<bigint>* static_twinsmooth::r_generate_twinsmooth(Node* chunk, LinkedTree* S, size_t range, size_t old_range)
{
    auto d = bigint_new;
    auto delta = bigint_new;
    auto m1 = bigint_new;
    mpz_init2(*d, MPZ_INIT_BITS);
    mpz_init2(*delta, MPZ_INIT_BITS);
    mpz_init2(*m1, MPZ_INIT_BITS);
    auto result = new LinkedList<bigint>();

    int i = 0;
    size_t counter = 0;
    while(i < CHUNK_SIZE && chunk[i] != nullptr)
    {
        auto x = chunk[i]; //costante per il ciclo corrente
        auto y = chunk[i]->skip(old_range + (old_range == 0));
        auto z = chunk[i]->skip_back(old_range + (old_range ==0));

        // (x|y) with y > x
        while(counter < range - old_range && y != nullptr)
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

                if(S->search(nv) == nullptr)
                {
                    result->push(nv);
                    x->twins_found++;
                }
                else
                {
                    bigint_free(nv);
                }
            }

            y = y->next;
            counter++;
        }

        counter = 0;
        // (x|z) with x > z
        while(counter < range-old_range && z != nullptr)
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

                if(S->search(nv) == nullptr)
                {
                    result->push(nv);
                    x->twins_found++;
                }
                else
                {
                    bigint_free(nv);
                }
            }

            z = z->prev;
            counter++;
        }

        i++;
    }

    bigint_free(d);
    bigint_free(delta);
    bigint_free(m1);
    return result;
}

LinkedList<bigint>* static_twinsmooth::k_generate_twinsmooth(Node* chunk, LinkedTree *S, bigfloat k) {
    auto d = bigint_new;
    auto delta = bigint_new;
    auto m1 = bigint_new;
    mpz_init2(*d, MPZ_INIT_BITS);
    mpz_init2(*delta, MPZ_INIT_BITS);
    mpz_init2(*m1, MPZ_INIT_BITS);
    auto result = new LinkedList<bigint>();

    size_t lower_range, upper_range;

    k_get_ranges(chunk[0], S, lower_range, upper_range, k);
    size_t counter = 0;
    int i = 0;
    while(i < CHUNK_SIZE && chunk[i] != nullptr)
    {
        auto x = chunk[i]; //costante per il ciclo corrente
        auto y = chunk[i]->next;
        auto z = chunk[i]->prev;

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

                if(S->search(nv) == nullptr)
                {
                    result->push(nv);
                    x->twins_found++;
                }
                else
                {
                    bigint_free(nv);
                }
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

                if(S->search(nv) == nullptr)
                {
                    result->push(nv);
                    x->twins_found++;
                }
                else
                {
                    bigint_free(nv);
                }
            }

            z = z->prev;
            counter++;
        }

        i++;
    }

    bigint_free(d);
    bigint_free(delta);
    bigint_free(m1);
    return result;
}

LinkedList<bigint>* static_twinsmooth::k_generate_twinsmooth(Node* chunk, LinkedTree *S, bigfloat k, bigfloat oldk) {
    auto d = bigint_new;
    auto delta = bigint_new;
    auto m1 = bigint_new;
    mpz_init2(*d, MPZ_INIT_BITS);
    mpz_init2(*delta, MPZ_INIT_BITS);
    mpz_init2(*m1, MPZ_INIT_BITS);
    auto result = new LinkedList<bigint>();

    size_t lower_range, upper_range, lower_skip, upper_skip;
    k_get_ranges(chunk[0], S, lower_range, upper_range, lower_skip, upper_skip, k, oldk);
    //std::cout << lower_range << "|" << lower_skip << "|" << upper_range << "|" << upper_skip << std::endl;

    int i = 0;
    size_t counter = 0;
    while(i < CHUNK_SIZE && chunk[i] != nullptr)
    {
        auto x = chunk[i]; //costante per il ciclo corrente
        auto y = chunk[i]->skip(upper_skip);
        auto z = chunk[i]->skip_back(lower_skip);

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

                if(S->search(nv) == nullptr)
                {
                    result->push(nv);
                    x->twins_found++;
                }
                else
                {
                    bigint_free(nv);
                }
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

                if(S->search(nv) == nullptr)
                {
                    result->push(nv);
                    x->twins_found++;
                }
                else
                {
                    bigint_free(nv);
                }
            }

            z = z->prev;
            counter++;
        }

        i++;
    }

    bigint_free(d);
    bigint_free(delta);
    bigint_free(m1);
    return result;
}

LinkedList<Node>* static_twinsmooth::iteration_S_N(LinkedTree* S, LinkedList<Node> *N) {
    auto chunks = create_chunks(N);
    auto result = new LinkedList<Node>();

    while(!chunks->empty())
    {
        auto results = new LinkedList<bigint>*[NUM_THREADS];
        auto points_arr = new Node*[NUM_THREADS];
        NULL_INIT_ARRAY(points_arr, NUM_THREADS);
        NULL_INIT_ARRAY(results, NUM_THREADS);


        for (int i = 0; i < (NUM_THREADS) && !chunks->empty(); i++) {
            auto pts = chunks->top(); chunks->pop();
            if (pts == nullptr) break;
            points_arr[i] = pts;
        }

        #pragma omp parallel num_threads(NUM_THREADS)
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = generate_twinsmooth(points_arr[i], S);
                delete[] points_arr[i];
            }
        }

        for (int i = 0; i < (NUM_THREADS); i++)
        {
            if (results[i] != nullptr)
            {
                result->push(S->merge_return_inserted(results[i]));
            }
        }

        delete[] points_arr;
        delete[] results;

    }
    chunks->clear();
    delete chunks;

    return result;
}

LinkedList<Node>* static_twinsmooth::iteration_S_S(LinkedTree *S) {
    auto chunks = create_chunks(S);
    auto result = new LinkedList<Node>();

    while(!chunks->empty())
    {
        auto results = new LinkedList<bigint>*[NUM_THREADS];
        auto points_arr = new Node*[NUM_THREADS];
        NULL_INIT_ARRAY(points_arr, NUM_THREADS);
        NULL_INIT_ARRAY(results, NUM_THREADS);


        for (int i = 0; i < (NUM_THREADS) && !chunks->empty(); i++) {
            auto pts = chunks->top(); chunks->pop();
            if (pts == nullptr) break;
            points_arr[i] = pts;
        }

        #pragma omp parallel num_threads(NUM_THREADS)
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = generate_twinsmooth(points_arr[i],S);
                delete[] points_arr[i];
            }
        }

        for (int i = 0; i < (NUM_THREADS); i++)
        {
            if ((results)[i] != nullptr)
            {
                result->push(S->merge_return_inserted(results[i]));
            }
        }

        delete[] points_arr;
        delete[] results;

    }
    chunks->clear();
    delete chunks;

    return result;
}

LinkedList<Node>* static_twinsmooth::r_iteration_S_S(LinkedTree *S, size_t range){
    auto chunks = create_chunks(S);
    auto result = new LinkedList<Node>();

    while(!chunks->empty())
    {
        auto results = new LinkedList<bigint>*[NUM_THREADS];
        auto points_arr = new Node*[NUM_THREADS];
        NULL_INIT_ARRAY(points_arr, NUM_THREADS);
        NULL_INIT_ARRAY(results, NUM_THREADS);


        for (int i = 0; i < (NUM_THREADS) && !chunks->empty(); i++) {
            auto pts = chunks->top(); chunks->pop();
            if (pts == nullptr) break;
            points_arr[i] = pts;
        }

        #pragma omp parallel num_threads(NUM_THREADS)
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = r_generate_twinsmooth(points_arr[i], S, range);
                delete[] points_arr[i];
            }
        }

        for (int i = 0; i < (NUM_THREADS); i++)
        {
            if ((results)[i] != nullptr)
            {
                result->push(S->merge_return_inserted(results[i]));
            }
        }

        delete[] points_arr;
        delete[] results;

    }
    chunks->clear();
    delete chunks;

    return result;
}

LinkedList<Node>* static_twinsmooth::r_iteration_S_N(LinkedTree *S, LinkedList<Node> *N, size_t range){
    auto chunks = create_chunks(N);
    auto result = new LinkedList<Node>();

    while(!chunks->empty())
    {
        auto results = new LinkedList<bigint>*[NUM_THREADS];
        auto points_arr = new Node*[NUM_THREADS];
        NULL_INIT_ARRAY(points_arr, NUM_THREADS);
        NULL_INIT_ARRAY(results, NUM_THREADS);


        for (int i = 0; i < (NUM_THREADS) && !chunks->empty(); i++) {
            auto pts = chunks->top(); chunks->pop();
            if (pts == nullptr) break;
            points_arr[i] = pts;
        }

        #pragma omp parallel num_threads(NUM_THREADS)
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = r_generate_twinsmooth(points_arr[i], S, range);
                delete[] points_arr[i];
            }
        }

        for (int i = 0; i < (NUM_THREADS); i++)
        {
            if ((results)[i] != nullptr)
            {
                result->push(S->merge_return_inserted(results[i]));
            }
        }

        delete[] points_arr;
        delete[] results;

    }
    chunks->clear();
    delete chunks;

    return result;
}

LinkedList<Node> *static_twinsmooth::r_iteration_S_S(LinkedTree *S, size_t range, size_t old_range) {
    auto chunks = create_chunks(S);
    auto result = new LinkedList<Node>();

    while(!chunks->empty())
    {
        auto results = new LinkedList<bigint>*[NUM_THREADS];
        auto points_arr = new Node*[NUM_THREADS];
        NULL_INIT_ARRAY(points_arr, NUM_THREADS);
        NULL_INIT_ARRAY(results, NUM_THREADS);


        for (int i = 0; i < (NUM_THREADS) && !chunks->empty(); i++) {
            auto pts = chunks->top(); chunks->pop();
            if (pts == nullptr) break;
            points_arr[i] = pts;
        }

        #pragma omp parallel num_threads(NUM_THREADS)
        //for(int i = 0; i < NUM_THREADS; i++)
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = r_generate_twinsmooth(points_arr[i], S, range, old_range);
                delete[] points_arr[i];
            }
        }

        for (int i = 0; i < (NUM_THREADS); i++)
        {
            if ((results)[i] != nullptr)
            {
                result->push(S->merge_return_inserted(results[i]));
            }
        }

        delete[] points_arr;
        delete[] results;

    }
    chunks->clear();
    delete chunks;

    return result;}

LinkedList<Node>* static_twinsmooth::k_iteration_S_S(LinkedTree *S, bigfloat k, bigfloat oldk) {
    auto chunks = create_chunks(S);
    auto result = new LinkedList<Node>();

    while(!chunks->empty())
    {
        auto results = new LinkedList<bigint>*[NUM_THREADS];
        auto points_arr = new Node*[NUM_THREADS];
        NULL_INIT_ARRAY(points_arr, NUM_THREADS);
        NULL_INIT_ARRAY(results, NUM_THREADS);


        for (int i = 0; i < (NUM_THREADS) && !chunks->empty(); i++) {
            auto pts = chunks->top(); chunks->pop();
            if (pts == nullptr) break;
            points_arr[i] = pts;
        }

        #pragma omp parallel num_threads(NUM_THREADS)
        //for(int i = 0; i < NUM_THREADS; i++)
        {
             int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = k_generate_twinsmooth(points_arr[i], S, k, oldk);
                delete[] points_arr[i];
            }
        }

        for (int i = 0; i < (NUM_THREADS); i++)
        {
            if ((results)[i] != nullptr)
            {
                result->push(S->merge_return_inserted(results[i]));
            }
        }

        delete[] points_arr;
        delete[] results;

    }
    chunks->clear();
    delete chunks;

    return result;
}

LinkedList<Node>* static_twinsmooth::k_iteration_S_S(LinkedTree *S, bigfloat k) {
    auto chunks = create_chunks(S);
    auto result = new LinkedList<Node>();

    while(!chunks->empty())
    {
        auto results = new LinkedList<bigint>*[NUM_THREADS];
        auto points_arr = new Node*[NUM_THREADS];
        NULL_INIT_ARRAY(points_arr, NUM_THREADS);
        NULL_INIT_ARRAY(results, NUM_THREADS);


        for (int i = 0; i < (NUM_THREADS) && !chunks->empty(); i++) {
            auto pts = chunks->top(); chunks->pop();
            if (pts == nullptr) break;
            points_arr[i] = pts;
        }

        #pragma omp parallel num_threads(NUM_THREADS)
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = k_generate_twinsmooth(points_arr[i], S, k);
                delete[] points_arr[i];
            }
        }

        for (int i = 0; i < (NUM_THREADS); i++)
        {
            if ((results)[i] != nullptr)
            {
                result->push(S->merge_return_inserted(results[i]));
            }
        }

        delete[] points_arr;
        delete[] results;

    }
    chunks->clear();
    delete chunks;

    return result;
}

LinkedList<Node>* static_twinsmooth::k_iteration_S_N(LinkedTree *S, LinkedList<Node> *N, bigfloat k) {
    auto chunks = create_chunks(N);
    auto result = new LinkedList<Node>();

    while(!chunks->empty())
    {
        auto results = new LinkedList<bigint>*[NUM_THREADS];
        auto points_arr = new Node*[NUM_THREADS];
        NULL_INIT_ARRAY(points_arr, NUM_THREADS);
        NULL_INIT_ARRAY(results, NUM_THREADS);


        for (int i = 0; i < (NUM_THREADS) && !chunks->empty(); i++) {
            auto pts = chunks->top(); chunks->pop();
            if (pts == nullptr) break;
            points_arr[i] = pts;
        }

        #pragma omp parallel num_threads(NUM_THREADS)
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = k_generate_twinsmooth(points_arr[i], S, k);
                delete[] points_arr[i];
            }
        }

        for (int i = 0; i < (NUM_THREADS); i++)
        {
            if ((results)[i] != nullptr)
            {
                result->push(S->merge_return_inserted(results[i]));
            }
        }

        delete[] points_arr;
        delete[] results;

    }
    chunks->clear();
    delete chunks;

    return result;
}

LinkedList<bigint>* static_twinsmooth::generate_primes(LinkedList<bigint*>* chunk) {
    auto result = new LinkedList<bigint>();
    auto iter = chunk->top();
    int i = 0;
    while(iter[i] != nullptr && i < CHUNK_SIZE)
    {
        auto twin = iter[i];
        mpz_mul_2exp(*twin, *twin, 1);
        mpz_add_ui(*twin, *twin, 1);
        if(mpz_probab_prime_p(*twin, 1000))
        {
            result->push(twin);
        }
        i++;
    }
    return result;
}



/*LinkedList *static_twinsmooth::prime_iteration(LinkedTree *S) {
    LinkedList* chunks = create_chunks(S, CHUNK_SIZE);
    auto result_list = new LinkedList();

    while(!chunks->empty())
    {
        auto results = new LinkedList*[NUM_THREADS];
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
                results[i] = generate_primes(points_arr[i]);
                points_arr[i]->clear();
                delete points_arr[i];
            }
        }

        for (int i = 0; i < (NUM_THREADS); i++)
        {
            if ((results)[i] != nullptr)
            {
                (result_list)->simple_merge((results)[i]);
            }
        }

        delete[] points_arr;
        delete[] results;

    }
    chunks->clear();
    delete chunks;

    return result_list;
}
*/

