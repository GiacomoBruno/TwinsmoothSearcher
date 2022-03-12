#include "twinsmooth.h"
#include "file_manager.h"

void load_file(CappedFile& fm, LinkedTree* output)
{
    string line;
    while(fm.getline(line))
    {
        auto val = bigint_new;
        bigint_str(val, line);
        output->fast_insert_delete_source(val);
    }
}


LinkedTree* generate_twinsmooth_complete(Node start_number)
{
    auto d = bigint_new;
    auto delta = bigint_new;
    auto m1 = bigint_new;
    mpz_init2(*d, MPZ_INIT_BITS);
    mpz_init2(*delta, MPZ_INIT_BITS);
    mpz_init2(*m1, MPZ_INIT_BITS);

    auto x = start_number;

    auto result = new LinkedTree();

    while(x != nullptr)
    {
        auto y = x->next;
        while(y != nullptr)
        {
            mpz_mul(*m1, *(x->val), *(y->val));
            mpz_add(*m1, *m1, *(y->val));
            mpz_sub(*delta, *(y->val), *(x->val));
            mpz_mod(*d, *m1, *delta);

            if(mpz_cmp_ui(*d, 0) == 0)
            {
                auto nv = bigint_new;
                mpz_init2(*nv, MPZ_INIT_BITS);
                mpz_div(*m1, *m1, *delta);
                mpz_sub_ui(*nv, *m1, 1);

                result->insert_delete_source(nv);
            }

            y = y->next;
        }
        x = x->next;
    }

    bigint_free(d);
    bigint_free(delta);
    bigint_free(m1);
    return result;
}

LinkedList* create_chunks(LinkedList* input, int chunk_size)
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
    std::filesystem::create_directories(OUT_FOLDER(smoothness));

    for(int i = 1; i < smoothness; i++)
    {
        CappedFile previously_found_twins = CappedFile(TWINSMOOTH_FN, OUT_FOLDER(i), true, i);
        if(previously_found_twins.is_open()) {
            std::cout << "loading file of smoothness = " << i << std::endl;
            load_file(previously_found_twins, results);
        }
    }

    for(int i = 1; i <= smoothness; i++)
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

void twinsmooth::save_files() {
    for(int i = 1; i < smoothness; i++)
    {
        CappedFile file(TWINSMOOTH_FN, OUT_FOLDER(i), true, i);
        auto nums = new LinkedTree();
        if(file.is_open())
        {
            load_file(file, nums);
            //TODO fix this and remove already saved numbers
        }
        nums->cleanup();
        delete nums;
    }

    auto iter = results->begin();
    CappedFile output(TWINSMOOTH_FN, OUT_FOLDER(smoothness), false, smoothness);
    while(iter != nullptr)
    {
        output.printn("%Zd\n", *iter->val);
        iter = iter->next;
    }
}