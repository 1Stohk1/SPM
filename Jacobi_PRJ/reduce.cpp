#include <iostream>
#include <thread>
#include <queue>
#include <condition_variable>
#include <future>
#include <vector>
#include <functional>
#include <mutex>
#include "lib/utimer.hpp"
#include "lib/linear_system.hpp"

using namespace std;

queue<packaged_task<void()>> my_q;
condition_variable search_task;
mutex queue_mutex;
bool EOL = false;

void thread_pool()
{
    packaged_task<void()> task;
    while (true)
    {
        {
            unique_lock<mutex> lk(queue_mutex);
            search_task.wait(lk, [&]
                             { return (!my_q.empty() || EOL); });
            if (!my_q.empty())
            {
                task = move(my_q.front());
                my_q.pop();
            }
            if (EOL)
                return;
        }
        task();
    }
}

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

    auto submit = [&](packaged_task<void()> &f)
    {
        my_q.push(move(f));
        search_task.notify_one();
    };

    function<void(int)> Jacobi = [&](int row)
    {
        for (size_t row = 0; row < n_dim; row++)
        {
            ls.x_curr[row] = 0;
            for (size_t col = 0; col < n_dim; col++)
            {
                if (row != col)
                    ls.x_curr[row] += ls.A[row][col] * ls.x_old[col]; // Can be exploited with reduce pattern
            }
            ls.x_curr[row] = (ls.b[row] - ls.x_curr[row]) / ls.A[row][row]; //Each row can be filled with padding to avoid cache coherency protocol
        }
    };

    function<void()> update = [&]()
    {
        for (int i = 0; i < n_dim; ++i)
        {
            ls.x_old[i] = ls.x_curr[i];
        }
    };

    auto stopTp = [&]()
    {
        {
            unique_lock<mutex> lock(queue_mutex);
            EOL = true;
        }
        search_task.notify_all();
    };

    vector<thread> threads(nw); //Async can be used to avoid wasting of time

    for (auto &t : threads)
    {
        t = thread(thread_pool);
    }

    {
        timer_raii tim("Sequential Jacobi (thread_pool) with " + to_string(iterations) + " iterations");
        for (size_t iter = 0; iter < iterations; iter++)
        {
            vector<future<void>> vf(n_dim);
            for (size_t row = 0; row < n_dim; row++)
            {
                packaged_task<void()> pt(bind(Jacobi, row));
                vf[row] = pt.get_future();
                submit(pt);
            }
            for (size_t i = 0; i < n_dim; i++)
            {
                vf[i].get();
            }
            update();
        }
        stopTp();
        for (auto &t : threads)
        {
            if (t.joinable())
                t.join();
        }
    }
    //ls.print_results();
    return 0;
}
