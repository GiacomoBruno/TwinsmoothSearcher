//
// Created by Giacomo Bruno on 14/03/22.
//

#include "twinsmooth_kopt.h"
#include "file_manager.h"

size_t contatore = 0;

void twinsmooth_kopt::get_ranges(Node nd, size_t& lower_range, size_t& upper_range)
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

LinkedTree* twinsmooth_kopt::generate_twinsmooth_from_k_chunks(LinkedList* chunk)
{
    auto d = bigint_new;
    auto delta = bigint_new;
    auto m1 = bigint_new;
    mpz_init2(*d, MPZ_INIT_BITS);
    mpz_init2(*delta, MPZ_INIT_BITS);
    mpz_init2(*m1, MPZ_INIT_BITS);
    auto result = new LinkedTree();

    auto iter = chunk->begin();

    size_t counter = 0;
    size_t upper_bound_distance;
    size_t lower_bound_distance;

    get_ranges(VAL(iter), lower_bound_distance, upper_bound_distance);

    while(iter != nullptr)
    {
        auto x = VAL(iter);
        auto y = VAL(iter)->next;
        auto z = VAL(iter)->prev;

        // (x|y) with y > x
        while(counter < upper_bound_distance && y != nullptr)
        {
            contatore++;
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
            contatore++;
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

LinkedTree* twinsmooth_kopt::iteration(LinkedList *points) {
    auto chunks = create_chunks(points, 100);

    auto result_tree = new LinkedTree();

    while(!chunks->empty())
    {
        auto results = new LinkedTree*[NUM_THREADS];
        auto points_arr = new LinkedList*[NUM_THREADS];
        for (int i = 0; i < (NUM_THREADS); i++) {
            points_arr[i] = nullptr;
        }
        for (int i = 0; i < (NUM_THREADS); i++) {
            results[i] = nullptr;
        }
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
void twinsmooth_kopt::execute() {
    std::cout << "executing twinsmooth calculation on threads: {" << NUM_THREADS << "}" << std::endl;
    std::cout << "mode = k optimization with k: {";
    gmp_printf("%.5Ff}\n", *k);
    std::cout << "smoothness = {" << smoothness << "}" << std::endl;


    CappedFile output(TWINSMOOTH_FN, OUT_FOLDER(smoothness), std::fstream::app | std::fstream::out, smoothness);

    while(!computation_numbers->empty())
    {
        output.save_list(computation_numbers);
        auto new_res = iteration(computation_numbers);
        computation_numbers->clear();
        delete computation_numbers;
        computation_numbers = results->merge_return_inserted(new_res);
        std::cout << "found " << computation_numbers->size() << " new twinsmooth" << std::endl;
    }

    output.reorder();
    computation_numbers->clear();
    delete computation_numbers;
}

void twinsmooth_kopt::terminate() {
    std::cout << "found in total: " << results->get_size() <<"tot computations: "<< contatore << std::endl;
    results->cleanup();
    delete results;
}

