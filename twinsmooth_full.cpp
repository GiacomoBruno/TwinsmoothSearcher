#include "twinsmooth_full.h"

LinkedTree* generate_twinsmooth_from_chunks(std::vector<Node>* chunk)
{
    auto d = bigint_new;
    auto delta = bigint_new;
    auto m1 = bigint_new;
    mpz_init2(*d, MPZ_INIT_BITS);
    mpz_init2(*delta, MPZ_INIT_BITS);
    mpz_init2(*m1, MPZ_INIT_BITS);
    auto result = new LinkedTree();

    auto iter = chunk->begin();
    while(iter != chunk->end())
    {
        auto x = *iter; //costante per il ciclo corrente
        auto y = (*iter)->next;
        auto z = (*iter)->prev;

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

                result->insert_del(&nv);
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

                result->insert_del(&nv);
            }

            z = z->prev;
        }

        iter++;
    }

    bigint_free(d);
    bigint_free(delta);
    bigint_free(m1);
    return result;
}

LinkedTree* iteration(std::vector<std::vector<Node>*>* points)
{
    auto result_tree = new LinkedTree();

    auto results = new LinkedTree*[NUM_THREADS];
    NULL_INIT_ARRAY(results, NUM_THREADS);

    while(!points->empty())
    {
        #pragma omp parallel num_threads(NUM_THREADS)
        {
            int i = omp_get_thread_num();
            if(points->size() > i ) {
                //results[i] = generate_twinsmooth(points[i], X->get_size());
                results[i] = generate_twinsmooth_from_chunks(points->at(i));
            }
        }

        REMOVE_FIRST_N_FROM_ARRAY(points, NUM_THREADS);
        INSERT_ARRAY_MEMBERS_INTO_TREE(results, result_tree, NUM_THREADS);
        DEALLOCATE_ARRAY_MEMBERS(results, NUM_THREADS);

    }
    delete[] results;
    return result_tree;


}

void twinsmooth_full::execute()
{
    initialize_smooth_set();
    load_files();

    LinkedTree* current_results = generate_twinsmooth_complete(results->begin());

    size_t new_found = 0;
    do
    {
        auto points = results->simple_merge(current_results);
        auto chunks = create_chunks(points, 100);
        new_found = points->size();
        CLEAR(points);

        current_results = iteration(chunks);

        std::cout << "found " << new_found << " new numbers" << std::endl;
    } while(new_found > 0);

    std::cout << "found in total: " << results->get_size() << std::endl;
}

void twinsmooth_full::load_files() {}

void twinsmooth_full::initialize_smooth_set()
{
    for(uint64_t i = 1; i <= smoothness; i++)
    {
        auto n = bigint_new;
        bigint_init(n,i);
        results->insert(n);
    }
    std::filesystem::create_directories(OUT_FOLDER);
}