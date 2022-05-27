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

    function<void(int)> Jacobi = [&](int id)
    {
        for (int row = id; row < n_dim; row += nw)
        {
            ls.x_curr[row] = 0;
            for (size_t col = 0; col < n_dim; col++)
            {
                if (row != col)
                    ls.x_curr[row] += ls.A[row][col] * ls.x_old[col];
            }
            ls.x_curr[row] = (ls.b[row] - ls.x_curr[row]) / ls.A[row][row];
        }
    };

    auto stopTp = [&]()
    {
        EOL = true;
        search_task.notify_all();
    };

    vector<thread> threads(nw);

    for (auto &t : threads)
    {
        t = thread(thread_pool);
    }
    vector<future<void>> vf(nw);

    {
        timer_raii tim("Jacobi (thread_pool) with " + to_string(iterations) + " iterations");
        for (int iter = 0; iter < iterations; iter++)
        {
            for (int id = 0; id < nw; id++)
            {
                packaged_task<void()> pt(bind(Jacobi, id));
                vf[id] = pt.get_future();
                submit(pt);
            }
            for (int i = 0; i < nw; i++)
            {
                vf[i].get();
            }
            ls.update();
        }
        stopTp();
        for (auto &t : threads)
        {
            if (t.joinable())
                t.join();
        }
    }
    ls.print_results();
    return 0;
}
