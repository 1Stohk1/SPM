#include <iostream>
#include "fastflow/ff/ff.hpp"
#include "ff/map.hpp"
#include "ff/config.hpp"
#include "fastflow/ff/parallel_for.hpp"
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

    {
        timer_raii tim("Jacobi (FastFlow ParallelFor) with " + to_string(iterations) + " iterations and " + to_string(nw) + " nw");

        ff::ParallelFor pf(nw);

        for (int i = 0; i < iterations; i++)
        {
            pf.parallel_for(0, n_dim, 1, n_dim / nw, [&](int id)
                            {
            ls.x_curr[id] = ls.b[id];
            for (size_t col = 0; col < n_dim; col++)
            {
                if (id != col)
                    ls.x_curr[id] -= ls.A[id][col] * ls.x_old[col];
            }
            ls.x_curr[id] = ls.x_curr[id] / ls.A[id][id]; });
            ls.update();
        }
    }
    ls.print_results();
    return 0;
}