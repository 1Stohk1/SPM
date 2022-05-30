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
            ls.update();
        }
    }
    ls.print_results();
    return 0;
}
