#include "twinsmooth_k.h"

LinkedList* twinsmooth_k::create_chunks(LinkedList* input, size_t chunk_size)
{
    auto chunks = new LinkedList();
    auto chunk = new LinkedList();

    size_t counter = 0;

    while(!input->empty())
    {
        if(counter > chunk_size)
        {
            counter = 0;
            chunks->push_front(chunk);
            chunk = new LinkedList();
        }

        chunk->push_front(input->pop());
        counter++;
    }

    if(counter != 0) chunks->push_front(chunk);

    return chunks;
}

LinkedList* twinsmooth_k::create_chunks(LinkedTree* input, size_t chunk_size)
{
    auto chunks = new LinkedList();
    auto chunk = new LinkedList();

    size_t counter = 0;
    auto iter = input->begin();
    while(iter != nullptr)
    {
        if(counter > chunk_size)
        {
            counter = 0;
            chunks->push_front(chunk);
            chunk = new LinkedList();
        }

        chunk->push_front(iter);
        iter = iter->next;
        counter++;
    }

    if(counter != 0) chunks->push_front(chunk);

    return chunks;
}

void twinsmooth_k::get_ranges
(Node nd, LinkedTree* results, size_t& lower_range, size_t& upper_range, size_t& lower_skip, size_t& upper_skip, bigfloat k, bigfloat oldk)
{
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

void twinsmooth_k::get_ranges
        (Node nd, LinkedTree* results, size_t& lower_range, size_t& upper_range, bigfloat k)
{
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


LinkedTree *twinsmooth_k::generate_twinsmooth(LinkedTree *S, LinkedList *input, bigfloat k) {
    auto d = bigint_new;
    auto delta = bigint_new;
    auto m1 = bigint_new;
    mpz_init2(*d, MPZ_INIT_BITS);
    mpz_init2(*delta, MPZ_INIT_BITS);
    mpz_init2(*m1, MPZ_INIT_BITS);
    auto result = new LinkedTree();

    size_t lower_range, upper_range;

    auto iter = input->begin();
    get_ranges(VAL(iter), S, lower_range, upper_range, k);

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

LinkedTree *twinsmooth_k::generate_twinsmooth_no_oldk(LinkedTree *S, LinkedList *input, bigfloat k, bigfloat oldk) {
    auto d = bigint_new;
    auto delta = bigint_new;
    auto m1 = bigint_new;
    mpz_init2(*d, MPZ_INIT_BITS);
    mpz_init2(*delta, MPZ_INIT_BITS);
    mpz_init2(*m1, MPZ_INIT_BITS);
    auto result = new LinkedTree();

    size_t lower_range, upper_range, lower_skip, upper_skip;

    auto iter = input->begin();
    get_ranges(VAL(iter), S, lower_range, upper_range, lower_skip, upper_skip, k, oldk);

    size_t counter = 0;
    while(iter != nullptr)
    {
        auto x = VAL(iter); //costante per il ciclo corrente
        auto y = VAL(iter)->skip(upper_skip);
        auto z = VAL(iter)->skip_back(lower_skip);

        // (x|y) with y > x
        while(counter < upper_range && y != nullptr)
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
        while(counter < lower_range && z != nullptr)
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

LinkedTree *twinsmooth_k::iteration_S_S(LinkedTree *S, bigfloat k) {

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
        //for(int i = 0; i < NUM_THREADS; i++)
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = generate_twinsmooth(S, points_arr[i], k);
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

LinkedTree *twinsmooth_k::iteration_S_N(LinkedTree *S, LinkedList *N, bigfloat k)
{
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
        //for(int i = 0; i < NUM_THREADS; i++)
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = generate_twinsmooth(S, points_arr[i], k);
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

LinkedTree *twinsmooth_k::iteration_S_S_no_oldk(LinkedTree* S, bigfloat k, bigfloat oldk)
{
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
        //for(int i = 0; i < NUM_THREADS; i++)
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = generate_twinsmooth_no_oldk(S, points_arr[i], k, oldk);
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
