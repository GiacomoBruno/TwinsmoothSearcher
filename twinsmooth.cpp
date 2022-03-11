#include "twinsmooth.h"


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
                //gmp_printf("computing %Zd %Zd...", *(cursor_1->val), *(cursor_2->val));

                result->insert_del(&nv);
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

std::vector<std::vector<Node>*>* create_chunks(std::vector<Node>* input, int chunk_size)
{
    auto chunks = new std::vector<std::vector<Node>*>();

    auto chunk = new std::vector<Node>();
    int counter = 0;
    auto iter = input->begin();

    while(iter != input->end())
    {
        if(counter > chunk_size)
        {
            counter = 0;
            chunks->push_back(chunk);
            chunk = new std::vector<Node>();
        }

        chunk->push_back(*iter);

        iter++;
        counter++;
    }

    if(counter != 0) chunks->push_back(chunk);



    return chunks;
}
