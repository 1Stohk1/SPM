#include <iostream>
#include <queue>
#include <thread>
#include <omp.h>
#include <functional>
#include <chrono>

using namespace std::chrono_literals;
using namespace std;

struct Item {
    function<void()> f;
    bool eol = false;
};
queue<Item> my_q;

Item task_randomizer(function<void(int)> fun) {
    bool b = rand() % 2;
    cout << (b ? "Stiamo inviando task" : "Il lavoro e' finito a casa") << endl;
    Item task{[fun] {
        int num = rand() % 100;
        return fun(num);
    }, b};
    return task;
}

void thread_pool() {
    Item task;
    bool loop = true;
    bool ready = false;
    // This is the loop that permit to continue to check for tasks
    while (loop) {
        if (ready) {
            ready = false;
            if (task.eol)
                loop = false;
            task.f();
        }
        this_thread::sleep_for(1000ms);
    // This is the section that has to be protected, because we are taking the
    // tasks from the queue, and we don't want a race condition b/w them
#pragma omp critical
        {
            cout << "Cerco nella queue, essa al momento e' " << (my_q.empty() ? "vuota" : "piena")
                 << endl;
            if (not(my_q.empty())) {
                cout << "Prendendo l'item" << endl;
                task = my_q.front();
                my_q.pop();
                ready = true;
            }
        }
    }
}

int main() {
    //The shared variable are necessary for permit the threads to access to the shared memory variables
#pragma omp parallel default(none) shared(my_q, cout) num_threads(4)
    {
        //The sections are important cause the pragma omp clause involve all the threads included the main
#pragma omp sections
        {
            //This is the thread pool itself that consume the tasks, it is implemented with
            // the boolean flags that signal the EOL
#pragma omp section
            {
                thread_pool();
            }
            // This is the emitter of the thread pool, it sends random signals b/w continuing the work and stopping it
#pragma omp section
            {
                function<void(int)> function = [](int i) {
                    cout << "Ciao sono il thread numero " << omp_get_thread_num() << " il numero magico e' " << i
                         << endl;
                };
                for (int i = 0; i < 10; i++) {
                    my_q.push(task_randomizer(function));
                }
            }
        }
    }
}