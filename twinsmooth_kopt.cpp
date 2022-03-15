//
// Created by Giacomo Bruno on 14/03/22.
//

#include "twinsmooth_kopt.h"
#include "file_manager.h"


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
            auto rp = new struct range_pair;
            rp->nodes = chunk;
            //TODO calculate upper bound
            //TODO calculate lower bound

            chunks->push_front(rp);
            chunk = new LinkedList();
        }

        chunk->push_front(input->pop());
        counter++;
    }

    //delete input;

    if(counter != 0)
    {
        //TODO prepare chunk range
    }

    return chunks;
}

LinkedTree* twinsmooth_kopt::iteration(LinkedList *pList) {
    return nullptr;
}

void twinsmooth_kopt::execute() {
    std::cout << "executing twinsmooth calculation on threads: {" << NUM_THREADS << "}" << std::endl;
    std::cout << "mode = k optimization with k1: {" << k1 << "} and k2: {" << k2 << "}" <<std::endl;
    std::cout << "smoothness = {" << smoothness << "}" << std::endl;

    load_files();

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

    computation_numbers->clear();
    delete computation_numbers;
    output.reorder();
}

void twinsmooth_kopt::terminate() {

}

