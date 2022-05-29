#include <iostream>
#include "ff/ff.hpp"
#include "ff/map.hpp"
#include "ff/config.hpp"
#include "ff/parallel_for.hpp"
#include "lib/utimer.hpp"
#include "lib/linear_system.hpp"

using namespace std;
using namespace ff;

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        cout << "Usage is: " << argv[0] << " Dimension_of_the_system(int) Number_of_iterations(int) Number_Workers(int) Check_Results[0/1](bool)" << endl;
        return (0);
    }
    int n_dim = atoi(argv[1]);
    int iterations = atoi(argv[2]);
    int nw = atoi(argv[3]);
    bool check = (atoi(argv[4]) == 0 ? false : true);
    srand(123);
    Linear_System ls(n_dim, check);
    ParallelForPipeReduce<vector<double>*> pfr(nw, true); // spinwait is set to true
    pfr.disableScheduler();

    auto Map = [&](const long start, const long stop, const int /*thid*/, ff_buffernode &node)
    {
        if (start == stop)
            return;
        std::vector<double> *C = new std::vector<double>;
        C->reserve(stop - start);
        for (long i = start; i < stop; ++i)
        {
            C->push_back(ls.A[0][i]);
        }
        node.ff_send_out(C);
    };
    auto Reduce = [&](std::vector<double> *v)
    {
        const std::vector<double> &V = *v;
        for (size_t i = 0; i < V.size(); ++i)
        {
            ls.x_curr[long(V[i]) % n_dim] = V[i];
        }
        delete v;
    };

    {
        timer_raii tim("Reduce phase");

        // Da implementare un reduce che invia i vettori ad un map che si calcola poi i risultati per ogni vettore riga !!Attenzione lo speedup si perderebbe se si eseguono le fasi in maniera distinta, andrebbero nascoste l'un l'altra

        pfr.parallel_reduce_idx(0, n_dim, 1, n_dim / nw, Map, Reduce);
    }
/*     {
        timer_raii tim("Map phase");

        // Da implementare un reduce che invia i vettori ad un map che si calcola poi i risultati per ogni vettore riga !!Attenzione lo speedup si perderebbe se si eseguono le fasi in maniera distinta, andrebbero nascoste l'un l'altra
        for (int id = 0; id < n_dim; id++)
        {
            ls.x_curr[id] = (ls.b[id] - ls.x_curr[id]) / ls.A[id][id];
        }
    } */
    ls.print_results();
    return 0;
}
