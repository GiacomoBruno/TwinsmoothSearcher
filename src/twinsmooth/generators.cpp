#include "../../include/bigfloat.h"
#include "../../include/twinsmooth/generators.h"
#include "../../include/twinsmooth/chunkers.h"

void get_ranges(TreeNode nd, bigint_tree *S, unsigned long long &lower_range, unsigned long long &upper_range, const bigfloat& k)
{
    bigfloat flnum{*nd->val};
    flnum /= k;

    bigint num{&flnum.number};
    mpf_set_z(flnum.number, nd->val->number);
    flnum *= k;


    lower_range = S->lower_bound(&num)->distance(nd);
    mpz_set_f(num.number, flnum.number);
    upper_range = nd->distance(S->upper_bound(&num));

}

namespace twinsmooth {

    LinkedList<bigint *> *generate_twinsmooth(TreeNode *chunk, bigint_tree *S) {
        auto d = new bigint();
        auto delta = new bigint();
        auto m1 = new bigint();
        auto result = new LinkedList<bigint *>();


        int i = 0;
        while (i < CHUNK_SIZE && chunk[i] != nullptr) {
            auto x = chunk[i]; //costante per il ciclo corrente
            auto y = chunk[i]->next;
            auto z = chunk[i]->prev;

            // (x|y) with y > x
            while (y != nullptr) {
                *m1 = (*x->val) * (*y->val);//            mpz_mul(*m1, *(x->val), *(y->val));

                *m1 += *y->val;//mpz_add(*m1, *m1, *(y->val));
                *delta = (*y->val) - (*x->val);//(*delta, *(y->val), *(x->val));
                *d = *m1 % *delta;//mpz_mod(*d, *m1, *delta);

                if ((*d < 0) == 0) {
                    *m1 /= *delta; //mpz_div(*m1, *m1, *delta);
                    auto nv = new bigint(*m1);
                    *nv -= 1; //mpz_sub_ui(*nv, *m1, 1);


                    if (S->search(nv) == nullptr) {
                        result->push(nv);
                        x->twins_found++;
                        y->twins_found++;
                    } else {
                        delete nv;
                    }
                }

                y = y->next;
            }

            // (x|z) with x > z
            while (z != nullptr) {
                *m1 = (*z->val) * (*x->val);//(*m1, *(z->val), *(x->val));
                *m1 += *x->val; //(*m1, *m1, *(x->val));
                *delta = (*x->val) - (*z->val);//mpz_sub(*delta, *(x->val), *(z->val));
                *d = *m1 % *delta; //mpz_mod(*d, *m1, *delta);

                if ((*d < 0) == 0) {
                    *m1 /= *delta; //mpz_div(*m1, *m1, *delta);
                    auto nv = new bigint(*m1);
                    *nv -= 1;//mpz_sub_ui(*nv, *m1, 1);

                    if (S->search(nv) == nullptr) {
                        result->push(nv);
                        x->twins_found++;
                        z->twins_found++;
                    } else {
                        delete nv;
                    }
                }

                z = z->prev;
            }

            i++;
        }

        delete d;
        delete delta;
        delete m1;
        return result;
    }

    LinkedList<bigint *> *generate_twinsmooth(TreeNode *chunk, bigint_tree *S, int range) {
        auto d = new bigint();
        auto delta = new bigint();
        auto m1 = new bigint();
        auto result = new LinkedList<bigint *>();


        int i = 0;
        while (i < CHUNK_SIZE && chunk[i] != nullptr) {
            auto x = chunk[i]; //costante per il ciclo corrente
            auto y = chunk[i]->next;
            auto z = chunk[i]->prev;
            int counter = 0;
            // (x|y) with y > x
            while (y != nullptr && counter < range) {
                *m1 = (*x->val) * (*y->val);//            mpz_mul(*m1, *(x->val), *(y->val));

                *m1 += *y->val;//mpz_add(*m1, *m1, *(y->val));
                *delta = (*y->val) - (*x->val);//(*delta, *(y->val), *(x->val));
                *d = *m1 % *delta;//mpz_mod(*d, *m1, *delta);

                if ((*d < 0) == 0) {
                    *m1 /= *delta; //mpz_div(*m1, *m1, *delta);
                    auto nv = new bigint(*m1);
                    *nv -= 1; //mpz_sub_ui(*nv, *m1, 1);

                    if (S->search(nv) == nullptr) {
                        result->push(nv);
                        x->twins_found++;
                        y->twins_found++;
                    } else {
                        delete nv;
                    }
                }
                ++counter;
                y = y->next;
            }
            counter = 0;
            // (x|z) with x > z
            while (z != nullptr && counter < range) {
                *m1 = (*z->val) * (*x->val);//(*m1, *(z->val), *(x->val));
                *m1 += *x->val; //(*m1, *m1, *(x->val));
                *delta = (*x->val) - (*z->val);//mpz_sub(*delta, *(x->val), *(z->val));
                *d = *m1 % *delta; //mpz_mod(*d, *m1, *delta);

                if ((*d < 0) == 0) {
                    *m1 /= *delta; //mpz_div(*m1, *m1, *delta);
                    auto nv = new bigint(*m1);
                    *nv -= 1;//mpz_sub_ui(*nv, *m1, 1);

                    if (S->search(nv) == nullptr) {
                        result->push(nv);
                        x->twins_found++;
                        z->twins_found++;
                    } else {
                        delete nv;
                    }
                }
                ++counter;
                z = z->prev;
            }

            i++;
        }

        delete d;
        delete delta;
        delete m1;
        return result;
    }

    LinkedList<bigint *> *generate_twinsmooth(TreeNode *chunk, bigint_tree *S, const bigfloat& k)
    {
        auto d = new bigint();
        auto delta = new bigint();
        auto m1 = new bigint();
        auto result = new LinkedList<bigint *>();
        unsigned long long lower_range, upper_range;

        if(chunk[0] != nullptr)
            get_ranges(chunk[0], S, lower_range, upper_range, k);
        else return result;

        int i = 0;
        while (i < CHUNK_SIZE && chunk[i] != nullptr) {
            auto x = chunk[i]; //costante per il ciclo corrente
            auto y = chunk[i]->next;
            auto z = chunk[i]->prev;
            unsigned long long counter = 0;
            // (x|y) with y > x
            while (y != nullptr && counter < upper_range) {
                *m1 = (*x->val) * (*y->val);//            mpz_mul(*m1, *(x->val), *(y->val));

                *m1 += *y->val;//mpz_add(*m1, *m1, *(y->val));
                *delta = (*y->val) - (*x->val);//(*delta, *(y->val), *(x->val));
                *d = *m1 % *delta;//mpz_mod(*d, *m1, *delta);

                if ((*d < 0) == 0) {
                    *m1 /= *delta; //mpz_div(*m1, *m1, *delta);
                    auto nv = new bigint(*m1);
                    *nv -= 1; //mpz_sub_ui(*nv, *m1, 1);
                    x->twins_found++;
                    y->twins_found++;
                    if (S->search(nv) == nullptr) {
                        result->push(nv);

                    } else {
                        delete nv;
                    }
                }
                ++counter;
                y = y->next;
            }
            counter = 0;
            // (x|z) with x > z
            while (z != nullptr && counter < lower_range) {
                *m1 = (*z->val) * (*x->val);//(*m1, *(z->val), *(x->val));
                *m1 += *x->val; //(*m1, *m1, *(x->val));
                *delta = (*x->val) - (*z->val);//mpz_sub(*delta, *(x->val), *(z->val));
                *d = *m1 % *delta; //mpz_mod(*d, *m1, *delta);

                if ((*d < 0) == 0) {
                    *m1 /= *delta; //mpz_div(*m1, *m1, *delta);
                    auto nv = new bigint(*m1);
                    *nv -= 1;//mpz_sub_ui(*nv, *m1, 1);
                    x->twins_found++;
                    z->twins_found++;
                    if (S->search(nv) == nullptr) {
                        result->push(nv);

                    } else {
                        delete nv;
                    }
                }
                ++counter;
                z = z->prev;
            }

            i++;
        }

        delete d;
        delete delta;
        delete m1;
        return result;
    }
}