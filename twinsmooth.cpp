#include "twinsmooth.h"
#include "file_manager.h"


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
    std::filesystem::create_directories(OUT_FOLDER(smoothness));

    for(int i = 1; i < smoothness; i++)
    {
        CappedFile previously_found_twins = CappedFile(TWINSMOOTH_FN, OUT_FOLDER(i), std::fstream::in, i);
        if(previously_found_twins.exists()) {
            std::cout << "loading file of smoothness = " << i << std::endl;
            previously_found_twins.load_file(results);
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