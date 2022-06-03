#include <iostream>
#include <vector>
#include <ff/ff.hpp>
#include "lib/utimer.hpp"
#include "lib/linear_system.hpp"

using namespace std;
using namespace ff;

// global object
Linear_System *ls;
int nw;

struct Worker : ff_node_t<long>
{
    long *svc(long *in)
    {
        for (id = ((intptr_t)in) - 1; id < ls->n_size; id += nw)
        {
            ls->x_curr[id] = ls->b[id];
            for (int col = 0; col < ls->n_size; col++)
            {
                if (id != col)
                    ls->x_curr[id] -= ls->A[id][col] * ls->x_old[col];
            }
            ls->x_curr[id] = ls->x_curr[id] / ls->A[id][id];
        }
        return in;
    }
    int id = 0;
};

struct Emitter : ff_monode_t<long>
{
    int svc_init()
    {
        received = 0;
        return 0;
    }
    long *svc(long *in)
    {
        if (in == nullptr)
        {
            for (int j = 1; j <= nw; ++j)
                ff_send_out(reinterpret_cast<int *>(j));
            return GO_ON;
        }
        ++received;
        if (received >= nw)
        {
            ls->update();
            return EOS;
        }
        return GO_ON;
    }
    int received = 0;
};

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        cout << "Usage is: " << argv[0] << " Dimension_of_the_system(int) Number_of_iterations(int) Number_Workers(int) Check_Results[0/1](bool)" << endl;
        return (0);
    }
    int n_dim = atoi(argv[1]);
    int iterations = atoi(argv[2]);
    nw = atoi(argv[3]);
    bool check = (atoi(argv[4]) == 0 ? false : true);
    srand(123);

    ls = new Linear_System(n_dim, check);
    assert(ls);
    {
        timer_raii tim("FastFlow correct");
        Emitter E;
        ff_Farm<> farm([&]()
                       {
            std::vector<std::unique_ptr<ff_node> > W;
            for(size_t i=0;i<nw;++i) 
                W.push_back(make_unique<Worker>());
            return W; }(),
                       E);
        farm.wrap_around();

        OptLevel2 opt;
        opt.max_mapped_threads = opt.max_nb_threads;
        opt.no_default_mapping = true;
        opt.verbose_level = 0;

        optimize_static(farm, opt);

        for (int i = 0; i < iterations; ++i)
        {
            farm.run_then_freeze();
            farm.wait_freezing();
        }
        farm.ffStats(cout);
    }



    ls->print_results();

    return 0;
}
