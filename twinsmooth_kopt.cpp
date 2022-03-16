//
// Created by Giacomo Bruno on 14/03/22.
//

#include "twinsmooth_kopt.h"
#include "file_manager.h"

LinkedTree* twinsmooth_kopt::generate_twinsmooth_from_k_chunks(range_pair* rp)
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
    auto upper_bound_distance = VAL(iter)->distance(rp->upper_bound_node);
    auto lower_bound_distance = rp->lower_bound_node->distance(VAL(iter));
    while(iter != nullptr)
    {
        auto x = VAL(iter); //costante per il ciclo corrente
        auto y = VAL(iter)->next;
        auto z = VAL(iter)->prev;
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

range_pair* twinsmooth_kopt::get_range_pair(LinkedList* chunk)
{
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
    rp->lower_bound_node = results->lower_bound(num);
    mpz_set_f(*num, *flnum);
    rp->upper_bound_node = results->upper_bound(num);

    bigfloat_free(flnum);
    bigint_free(num);
    return rp;
}

LinkedList* twinsmooth_kopt::create_chunks(LinkedList* input, int chunk_size)
{
    auto chunks = new LinkedList();
    auto chunk = new LinkedList();

    int counter = 0;

    while(!input->empty())
    {
        if(counter > chunk_size)
        {
            counter = 0;

            auto rp = get_range_pair(chunk);

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
            rp->lower_bound_node = results->begin();
            rp->upper_bound_node = results->end();
            chunks->push_front(rp);
            return chunks;
        }
        auto rp = get_range_pair(chunk);

        chunks->push_front(rp);
    }

    return chunks;
}

LinkedTree* twinsmooth_kopt::iteration(LinkedList *points) {
    auto chunks = create_chunks(points, 100);

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

void twinsmooth_kopt::select_current_k()
{
    if(current_new_results < new_found_results)
    {
        if (current_k != nullptr) {
            bigfloat_free(current_k);
        }
        current_k = bigfloat_new;
        bigfloat_init(current_k, k2);
    }
    else
    {
        if(current_k != nullptr)
        {
            bigfloat_free(current_k);
        }
        current_k = bigfloat_new;
        bigfloat_init(current_k, k1);
    }

    new_found_results = current_new_results;
}

void twinsmooth_kopt::execute() {
    std::cout << "executing twinsmooth calculation on threads: {" << NUM_THREADS << "}" << std::endl;
    std::cout << "mode = k optimization with k1: {" << k1 << "} and k2: {" << k2 << "}" << std::endl;
    std::cout << "smoothness = {" << smoothness << "}" << std::endl;


    CappedFile output(TWINSMOOTH_FN, OUT_FOLDER(smoothness), std::fstream::app | std::fstream::out, smoothness);

    while(!computation_numbers->empty())
    {
        current_new_results = computation_numbers->size();
        select_current_k();
        output.save_list(computation_numbers);
        auto new_res = iteration(computation_numbers);
        computation_numbers->clear();
        delete computation_numbers;
        computation_numbers = results->merge_return_inserted(new_res);
        std::cout << "found " << computation_numbers->size() << " new twinsmooth" << std::endl;
    }

    computation_numbers->clear();
    delete computation_numbers;
    output.reorder();
}

void twinsmooth_kopt::terminate() {
    std::cout << "found in total: " << results->get_size() << std::endl;
    results->cleanup();
    delete results;
}

