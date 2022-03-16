//
// Created by Giacomo Bruno on 16/03/22.
//

#include "twinsmooth_k.h"

LinkedTree* generate_twinsmooth_from_k_chunks(chunk* rp)
{
    auto d = bigint_new;
    auto delta = bigint_new;
    auto m1 = bigint_new;
    mpz_init2(*d, MPZ_INIT_BITS);
    mpz_init2(*delta, MPZ_INIT_BITS);
    mpz_init2(*m1, MPZ_INIT_BITS);
    auto result = new LinkedTree();

    LinkedList* chunk = rp->nodes;
    auto iter = chunk->begin();
    size_t counter = 0;
    size_t upper_bound_distance;
    if(rp->upper_bound == nullptr)
        upper_bound_distance = VAL(iter)->distance(rp->upper_bound);
    else upper_bound_distance = rp->upper_start->distance(rp->upper_bound);


    size_t lower_bound_distance;
    if(rp->lower_bound == nullptr)
        lower_bound_distance = rp->lower_bound->distance(VAL(iter));
    else lower_bound_distance = rp->lower_bound->distance(rp->lower_start);


    while(iter != nullptr)
    {
        auto x = VAL(iter); //costante per il ciclo corrente
        auto y = x->next;
        auto z = x->prev;
        //gmp_printf("%Zd %Zd\n", *x->val, *y->val);

        // (x|y) with y > x
        while(counter < upper_bound_distance && y != nullptr)
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
        while(counter < lower_bound_distance && z != nullptr)
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
            counter++;
            z = z->prev;
        }

        iter = iter->next;
    }

    bigint_free(d);
    bigint_free(delta);
    bigint_free(m1);
    return result;
}

chunk* twinsmooth_k::create_chunk(LinkedList* chnk, double small_k)
{
    auto res = new struct chunk;
    auto num = bigint_new;
    mpz_init2(*num, MPZ_INIT_BITS);

    auto flnum = bigfloat_new;
    mpf_init2(*flnum, MPZ_INIT_BITS);

    //Calculate lower bound
    mpf_set_z(*flnum, *VAL(chnk->begin())->val);

    mpf_div(*flnum, *flnum, *k);
    mpz_set_f(*num, *flnum);

    res->nodes = chnk;
    res->lower_bound = results->lower_bound(num);

    //Calculate upper bound
    mpf_set_z(*flnum, *VAL(chnk->begin())->val);
    mpf_mul(*flnum, *flnum, *k);
    mpz_set_f(*num, *flnum);
    res->upper_bound = results->upper_bound(num);

    if(small_k == 0)
    {
        res->lower_start = nullptr;
        res->upper_start = nullptr;
    }
    else
    {
        auto old_k = bigfloat_new;
        bigfloat_init(old_k, small_k);

        //Calculate lower start
        mpf_set_z(*flnum, *VAL(chnk->begin())->val);
        mpf_div(*flnum, *flnum, *old_k);
        mpz_set_f(*num, *flnum);

        res->lower_start = results->lower_bound(num);

        //Calculate upper start
        mpf_set_z(*flnum, *VAL(chnk->begin())->val);
        mpf_mul(*flnum, *flnum, *old_k);
        mpz_set_f(*num, *flnum);
        res->upper_start = results->upper_bound(num);


    }


    bigfloat_free(flnum);
    bigint_free(num);
    return res;
}

LinkedList* twinsmooth_k::create_chunks_from_list(LinkedList* input, size_t chunk_size)
{
    auto chunks = new LinkedList();
    auto chnk = new LinkedList();

    size_t counter = 0;


    while(!input->empty())
    {
        if(counter > chunk_size)
        {
            counter = 0;

            auto rp = create_chunk(chnk, 0);

            chunks->push_front(rp);
            chnk = new LinkedList();
        }

        chnk->push_front(input->pop());
        counter++;
    }


    if(counter != 0)
    {
        if(chunks->empty() && (results->get_size() == chnk->size()))
        {
            auto rp = new chunk;
            rp->nodes = chnk;
            rp->lower_bound = results->begin();
            rp->upper_bound = results->end();
            rp->lower_start = results->begin();
            rp->upper_start = results->begin();
            chunks->push_front(rp);
            return chunks;
        }
        auto rp = create_chunk(chnk, 0);

        chunks->push_front(rp);
    }
    return chunks;
}

LinkedList* twinsmooth_k::create_chunks_from_tree(size_t chunk_size, double small_k)
{
    auto chunks = new LinkedList();
    auto chnk = new LinkedList();

    size_t counter = 0;

    auto iter = results->begin();
    while(iter != nullptr)
    {
        if(counter > chunk_size)
        {
            counter = 0;

            auto rp = create_chunk(chnk, small_k);

            chunks->push_front(rp);
            chnk = new LinkedList();
        }

        chnk->push_front(iter);
        counter++;
        iter = iter->next;
    }


    if(counter != 0)
    {
        if(chunks->empty() && (results->get_size() == chnk->size()))
        {
            auto rp = new chunk;
            rp->nodes = chnk;
            rp->lower_bound = results->begin();
            rp->upper_bound = results->end();
            rp->lower_start = results->begin();
            rp->upper_start = results->begin();
            chunks->push_front(rp);
            return chunks;
        }
        auto rp = create_chunk(chnk, small_k);

        chunks->push_front(rp);
    }
    return chunks;
}

LinkedTree* twinsmooth_k::iteration_S_S_no_small_k(double small_k)
{
    auto chunks = create_chunks_from_tree(CHUNK_SIZE, small_k);

    auto result_tree = new LinkedTree();

    while(!chunks->empty())
    {
        auto results = new LinkedTree*[NUM_THREADS];
        auto points_arr = new chunk*[NUM_THREADS];

        for (int i = 0; i < (NUM_THREADS); i++) {
            points_arr[i] = nullptr;
        }
        for (int i = 0; i < (NUM_THREADS); i++) {
            results[i] = nullptr;
        }
        for (int i = 0; i < (NUM_THREADS); i++) {
            auto pts = (chunk*)(chunks->pop());
            if (pts == nullptr) break;
            points_arr[i] = pts;
        }

        #pragma omp parallel num_threads(NUM_THREADS)
        //for(int i = 0; i < NUM_THREADS; i++)
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = generate_twinsmooth_from_k_chunks(points_arr[i]);
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

LinkedTree *twinsmooth_k::iteration_S_S() {
    return iteration_S_S_no_small_k(0);
}

LinkedTree* twinsmooth_k::iteration_S_N(LinkedList* input)
{
    auto chunks = create_chunks_from_list(input, CHUNK_SIZE);

    auto result_tree = new LinkedTree();

    while(!chunks->empty())
    {
        auto results = new LinkedTree*[NUM_THREADS];
        auto points_arr = new chunk*[NUM_THREADS];

        for (int i = 0; i < (NUM_THREADS); i++) {
        points_arr[i] = nullptr;
        }
            for (int i = 0; i < (NUM_THREADS); i++) {
                results[i] = nullptr;
        }
        for (int i = 0; i < (NUM_THREADS); i++) {
            auto pts = (chunk*)(chunks->pop());
            if (pts == nullptr) break;
            points_arr[i] = pts;
        }

        #pragma omp parallel num_threads(NUM_THREADS)
        //for(int i = 0; i < NUM_THREADS; i++)
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = generate_twinsmooth_from_k_chunks(points_arr[i]);
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

void twinsmooth_k::set_k(double _k) {
    if(k != nullptr)
    {
        bigfloat_free(k);
    }

    k = bigfloat_new;
    bigfloat_init(k, _k);
}

