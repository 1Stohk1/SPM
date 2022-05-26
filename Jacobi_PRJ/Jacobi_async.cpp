#include <iostream>
#include <thread>
#include <future>
#include <vector>
#include <functional>
#include "lib/utimer.hpp"
#include "lib/linear_system.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        cout << "Usage is: " << argv[0] << " n-dim iterations nw seed" << endl;
        return (0);
    }
    int n_dim = atoi(argv[1]);
    int iterations = atoi(argv[2]);
    int nw = atoi(argv[3]);
    srand(atoi(argv[4]));

    Linear_System ls(n_dim);

    function<void(int)> Jacobi = [&](int id)
    {
        for (int row = id; row < n_dim; row += nw)
        {
            ls.x_curr[row] = 0;
            for (int col = 0; col < n_dim; col++)
            {
                if (row != col)
                    ls.x_curr[row] += ls.A[row][col] * ls.x_old[col];
            }
            ls.x_curr[row] = (ls.b[row] - ls.x_curr[row]) / ls.A[row][row];
        }
        return 0;
    };

    function<void()> update = [&]()
    {
        ls.x_old = ls.x_curr;
    };

    vector<future<void>> workers(nw);

    {
        timer_raii tim("Jacobi (std::async) with " + to_string(iterations) + " iterations");
        for (int iter = 0; iter < iterations; iter++)
        {
            for (int id = 0; id < nw; id++)
            {
                workers[id] = async(launch::async, Jacobi, id);
            }
            for (int i = 0; i < nw; i++)
            {
                try
                {
                    workers[i].get();
                }
                catch (const std::exception &e)
                {
                    cout << "Got " << e.what() << endl;
                }
            }
            update();
        }
    }
    ls.print_results();
    return 0;
}
