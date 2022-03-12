#include "twinsmooth_full.h"

LinkedTree* generate_twinsmooth_from_chunks(LinkedList* chunk)
{
    auto d = bigint_new;
    auto delta = bigint_new;
    auto m1 = bigint_new;
    mpz_init2(*d, MPZ_INIT_BITS);
    mpz_init2(*delta, MPZ_INIT_BITS);
    mpz_init2(*m1, MPZ_INIT_BITS);
    auto result = new LinkedTree();

   // print_as_bigint(chunk);
    if(chunk->size() < 200)
    {
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
    }}

    bigint_free(d);
    bigint_free(delta);
    bigint_free(m1);
    return result;
}

LinkedTree* iteration(LinkedList* points)
{
    auto chunks = create_chunks(points, 100);

    //delete after use
    auto result_tree = new LinkedTree();

    auto results = new LinkedTree*[NUM_THREADS];
    auto points_arr = new LinkedList*[NUM_THREADS];

    //set all array values to nullptr
    NULL_INIT_ARRAY(points_arr, NUM_THREADS);
    NULL_INIT_ARRAY(results, NUM_THREADS);



    while(!chunks->empty())
    {
        //EXTRACT_POINTS(points_arr, chunks, NUM_THREADS);
        for (int i = 0; i < (8); i++) {
            auto pts = static_cast<LinkedList*>((chunks)->pop());
            if (pts == nullptr)break;
            points_arr[i] = pts;
        }

        //#pragma omp parallel num_threads(NUM_THREADS)
        for(int i = 0; i < NUM_THREADS; i++)
        {
            //int i = omp_get_thread_num();

            if(points_arr[i] != nullptr ) {
                //std::cout << points_arr[i]->size() <<std::endl;
                //results[i] = generate_twinsmooth(points[i], X->get_size());
                results[i] = generate_twinsmooth_from_chunks(points_arr[i]);
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

    }
    chunks->clear();
    delete[] points_arr;
    delete chunks;
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
        auto points = results->merge_return_inserted(current_results);


        new_found = points->size();
        if(new_found > 0)
        {

            current_results = iteration(points);
            std::cout << "found " << new_found << " new numbers" << std::endl;

        }  
        CLEAR(points);

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

void twinsmooth_full::terminate() {
    results->cleanup();
    delete results;
}
