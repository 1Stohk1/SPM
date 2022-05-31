#include <iostream>
#include <vector>
#include <ff/node.hpp>
#include <ff/barrier.hpp>
#include "lib/utimer.hpp"
#include "lib/linear_system.hpp"

using namespace ff;
using namespace std;

#define SPIN 1

static barrierSelector<SPIN> bar;

// global object
Linear_System *ls;

#define BARRIER bar.doBarrier(get_my_id())

class Thread : public ff_node
{
public:
    Thread(int nworkers, int n_iterations) : nworkers(nworkers), n_iterations(n_iterations) {}

    void *svc(void *)
    {
        for (int iter = 0; iter < n_iterations; iter++)
        {
            for (int i = get_my_id(); i < ls->n_size; i += nworkers)
            {
                ls->x_curr[i] = ls->b[i];
                for (size_t col = 0; col < ls->n_size; col++)
                {
                    if (i != col)
                        ls->x_curr[i] -= ls->A[i][col] * ls->x_old[col];
                }
                ls->x_curr[i] = ls->x_curr[i] / ls->A[i][i];
            }
            BARRIER;
            ls->update();
            BARRIER;
        }
        return EOS;
    }
    void set_id(ssize_t id)
    {
        ff_node::set_id(id);
    }
    int run(bool = false) { return ff_node::run(); }
    int wait() { return ff_node::wait(); }

private:
    int nworkers;
    int n_iterations;
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
    int nw = atoi(argv[3]);
    bool check = (atoi(argv[4]) == 0 ? false : true);
    srand(123);
    bar.barrierSetup(nw);
    ls = new Linear_System(n_dim, check);
    assert(ls);
    {
        timer_raii tim("Jacobi (FastFlow Barrier) with " + to_string(iterations) + " iterations and " + to_string(nw) + " nw");
        Thread **N = new Thread *[nw];

        for (int i = 0; i < nw; ++i)
        {
            N[i] = new Thread(nw, iterations);
            N[i]->set_id(i);
            N[i]->run();
        }
        for (int i = 0; i < nw; ++i)
            N[i]->wait();
    }
    ls->print_results();

    return 0;
}