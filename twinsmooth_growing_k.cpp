//
// Created by Giacomo Bruno on 15/03/22.
//

#include "twinsmooth_growing_k.h"
#include "file_manager.h"

void twinsmooth_growing_k::select_current_k() {

    if(k_current + k_step > k_end) return;

    k_current += k_step;

    if(current_k != nullptr) {bigfloat_free(current_k);}

    current_k = bigfloat_new;
    bigfloat_init(current_k, k_current);
}


LinkedList* twinsmooth_growing_k::create_chunks(LinkedList* input, int chunk_size)
{
    auto chunks = new LinkedList();
    auto chunk = new LinkedList();

    int counter = 0;

    while(!input->empty())
    {
        if(counter > chunk_size)
        {
            counter = 0;

            auto num = bigint_new;
            mpz_init2(*num, MPZ_INIT_BITS);

            auto flnum = bigfloat_new;
            mpf_init2(*flnum, MPZ_INIT_BITS);
            mpf_set_z(*flnum, *VAL(chunk->begin())->val);

            mpf_div(*flnum, *flnum, *current_k);
            mpz_set_f(*num, *flnum);

            mpf_set_z(*flnum, *VAL(chunk->begin())->val);
            mpf_mul(*flnum, *flnum, *current_k);

            auto rp = new struct range_pair;
            rp->nodes = chunk;
            rp->lower_bound_distance = results->lower_bound(num)->distance(VAL(chunk->begin()));
            mpz_set_f(*num, *flnum);
            rp->upper_bound_distance = VAL(chunk->begin())->distance(results->upper_bound(num));

            bigfloat_free(flnum);
            bigint_free(num);

            chunks->push_front(rp);
            chunk = new LinkedList();
        }

        chunk->push_front(input->pop());
        counter++;
    }


    if(counter != 0)
    {
        if(chunks->empty() && (results->get_size() == chunk->size()))
        {
            auto rp = new struct range_pair;
            rp->nodes = chunk;
            rp->lower_bound_distance = results->get_size();
            rp->upper_bound_distance = results->get_size();
            chunks->push_front(rp);
            return chunks;
        }

        auto num = bigint_new;
        mpz_init2(*num, MPZ_INIT_BITS);

        auto flnum = bigfloat_new;
        mpf_init2(*flnum, MPZ_INIT_BITS);
        mpf_set_z(*flnum, *VAL(chunk->begin())->val);

        mpf_div(*flnum, *flnum, *current_k);
        mpz_set_f(*num, *flnum);


        mpf_set_z(*flnum, *VAL(chunk->begin())->val);
        mpf_mul(*flnum, *flnum, *current_k);

        auto rp = new struct range_pair;
        rp->nodes = chunk;
        rp->lower_bound_distance = results->lower_bound(num)->distance(VAL(chunk->begin()));
        mpz_set_f(*num, *flnum);
        rp->upper_bound_distance = VAL(chunk->begin())->distance(results->upper_bound(num));

        bigfloat_free(flnum);
        bigint_free(num);

        chunks->push_front(rp);
    }

    return chunks;
}

LinkedList* twinsmooth_growing_k::create_chunks_from_tree(LinkedTree* input, int chunk_size)
{
    auto chunks = new LinkedList();
    auto chunk = new LinkedList();

    int counter = 0;
    auto iter = input->begin();
    while(iter != nullptr)
    {
        if(counter > chunk_size)
        {
            counter = 0;

            auto num = bigint_new;
            mpz_init2(*num, MPZ_INIT_BITS);

            auto flnum = bigfloat_new;
            mpf_init2(*flnum, MPZ_INIT_BITS);
            mpf_set_z(*flnum, *VAL(chunk->begin())->val);

            mpf_div(*flnum, *flnum, *current_k);
            mpz_set_f(*num, *flnum);

            mpf_set_z(*flnum, *VAL(chunk->begin())->val);
            mpf_mul(*flnum, *flnum, *current_k);

            auto rp = new struct range_pair;
            rp->nodes = chunk;
            rp->lower_bound_distance = results->lower_bound(num)->distance(VAL(chunk->begin()));
            mpz_set_f(*num, *flnum);
            rp->upper_bound_distance = VAL(chunk->begin())->distance(results->upper_bound(num));

            bigfloat_free(flnum);
            bigint_free(num);

            chunks->push_front(rp);
            chunk = new LinkedList();
        }

        chunk->push_front(iter);
        iter = iter->next;
        counter++;
    }


    if(counter != 0)
    {
        if(chunks->empty() && (results->get_size() == chunk->size()))
        {
            auto rp = new struct range_pair;
            rp->nodes = chunk;
            rp->lower_bound_distance = results->get_size();
            rp->upper_bound_distance = results->get_size();
            chunks->push_front(rp);
            return chunks;
        }

        auto num = bigint_new;
        mpz_init2(*num, MPZ_INIT_BITS);

        auto flnum = bigfloat_new;
        mpf_init2(*flnum, MPZ_INIT_BITS);
        mpf_set_z(*flnum, *VAL(chunk->begin())->val);

        mpf_div(*flnum, *flnum, *current_k);
        mpz_set_f(*num, *flnum);


        mpf_set_z(*flnum, *VAL(chunk->begin())->val);
        mpf_mul(*flnum, *flnum, *current_k);

        auto rp = new struct range_pair;
        rp->nodes = chunk;
        rp->lower_bound_distance = results->lower_bound(num)->distance(VAL(chunk->begin()));
        mpz_set_f(*num, *flnum);
        rp->upper_bound_distance = VAL(chunk->begin())->distance(results->upper_bound(num));

        bigfloat_free(flnum);
        bigint_free(num);

        chunks->push_front(rp);
    }

    return chunks;
}

LinkedTree* twinsmooth_growing_k::generate_twinsmooth_from_k_chunks(range_pair* rp)
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
    while(iter != nullptr)
    {
        auto x = VAL(iter); //costante per il ciclo corrente
        auto y = VAL(iter)->next;
        auto z = VAL(iter)->prev;
        //gmp_printf("%Zd %Zd\n", *x->val, *y->val);

        // (x|y) with y > x
        while(counter < rp->upper_bound_distance && y != nullptr)
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
        while(counter < rp->lower_bound_distance && z != nullptr)
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

LinkedTree* twinsmooth_growing_k::iteration(LinkedList *points)
{
    auto chunks = create_chunks_from_tree(results, 100);

    auto result_tree = new LinkedTree();

    while(!chunks->empty())
    {
        auto results = new LinkedTree*[NUM_THREADS];
        auto points_arr = new range_pair*[NUM_THREADS];
        for (int i = 0; i < (NUM_THREADS); i++) {
            points_arr[i] = nullptr;
        }
        for (int i = 0; i < (NUM_THREADS); i++) {
            results[i] = nullptr;
        }
        for (int i = 0; i < (NUM_THREADS); i++) {
            auto pts = (range_pair*)(chunks->pop());
            if (pts == nullptr) break;
            points_arr[i] = pts;
        }

        #pragma omp parallel num_threads(NUM_THREADS)
        //for(int i = 0; i < NUM_THREADS; i++)
        {
            int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                results[i] = generate_twinsmooth_from_k_chunks(points_arr[i]);
                //points_arr[i]->nodes->clear();
                //delete points_arr[i]->nodes;
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

void twinsmooth_growing_k::execute() {
    mpf_set_default_prec(20);
    std::cout << "executing twinsmooth calculation on threads: {" << NUM_THREADS << "}" << std::endl;
    std::cout << "mode = growing k optimization with k start: {" << k_start << "}, k max: {" << k_end << "}, and k step: {" << k_step << "}" << std::endl;
    std::cout << "smoothness = {" << smoothness << "}" << std::endl;

    load_files();

    CappedFile output(TWINSMOOTH_FN, OUT_FOLDER(smoothness), std::fstream::app | std::fstream::out, smoothness);
    while(k_current < k_end)
    {
        for(size_t i = 1; i <= smoothness; i++)
        {
            auto num = bigint_new;
            bigint_init(num, i);

            auto inserted = results->search(num);
            if(inserted == nullptr)
            {
                computation_numbers->push_front(results->insert(num));
            }
            else
            {
                computation_numbers->push_front(inserted);
                bigint_free(num);
            }
        }
        auto first_res = twinsmooth::generate_twinsmooth(computation_numbers);
        computation_numbers->clear();
        computation_numbers = results->merge_return_inserted(first_res);


        select_current_k();
        std::cout << "started for k = " << k_current << std::endl;
        size_t found = 0;
        while(!computation_numbers->empty())
        {
            output.save_list(computation_numbers);
            auto new_res = iteration(computation_numbers);
            computation_numbers->clear();
            delete computation_numbers;
            computation_numbers = results->merge_return_inserted(new_res);
            found += computation_numbers->size();
            //std::cout << "found " << computation_numbers->size() << " new twinsmooth" << std::endl;
        }

        std::cout << "found {" << found <<"} twinsmooth for k = " << k_current <<std::endl;
    }


    computation_numbers->clear();
    delete computation_numbers;
    output.reorder();
}


