#include "twinsmooth.h"
#include "file_manager.h"

void benchmark::start_bench()
{
    start_time = CURRENT_TIME;
    st = clock();
}

void benchmark::conclude_bench()
{
    et = clock();
    end_time = CURRENT_TIME;
    log->log("time in seconds : ", ELAPSED(start_time, end_time));
    log->logl(" - CPU time in seconds: ", (double)(et-st)/CLOCKS_PER_SEC);
}


LinkedTree* twinsmooth::generate_twinsmooth(LinkedList* input)
{
    auto d = bigint_new;
    auto delta = bigint_new;
    auto m1 = bigint_new;
    mpz_init2(*d, MPZ_INIT_BITS);
    mpz_init2(*delta, MPZ_INIT_BITS);
    mpz_init2(*m1, MPZ_INIT_BITS);
    auto result = new LinkedTree();

    auto iter = input->begin();
    while(iter != nullptr)
    {
        auto x = VAL(iter); //costante per il ciclo corrente
        auto y = VAL(iter)->next;

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
        iter = iter->next;
    }

    bigint_free(d);
    bigint_free(delta);
    bigint_free(m1);
    return result;
}


LinkedList* twinsmooth::create_chunks(LinkedList* input, int chunk_size)
{
    auto chunks = new LinkedList();
    auto chunk = new LinkedList();

    int counter = 0;

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

    //delete input;

    if(counter != 0) chunks->push_front(chunk);

    return chunks;
}

void twinsmooth::load_files()
{

    for(uint64_t i = 1; i < smoothness; i++)
    {
        CappedFile previously_found_twins = CappedFile(TWINSMOOTH_FN, OUT_FOLDER(i), std::fstream::in, i);
        if(previously_found_twins.exists()) {
            std::cout << "loading file of smoothness = " << i << std::endl;
            previously_found_twins.load_file(results);
        }
    }
}

void twinsmooth::save_files()
{
    auto iter = results->begin();
    CappedFile output(TWINSMOOTH_FN, OUT_FOLDER(smoothness), std::fstream::out, smoothness);
    while(iter != nullptr)
    {
        output.printn("%Zd\n", *iter->val);
        iter = iter->next;
    }
}

void twinsmooth::init_starting_set() {
    for(uint64_t i = 1; i <= smoothness; i++)
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
}

void twinsmooth::start()
{
    load_files();
    init_starting_set();
}