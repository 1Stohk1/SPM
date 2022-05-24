#include <iostream>
#include <thread>
#include <queue>
#include <condition_variable>
#include <vector>
#include <functional>
#include <mutex>

using namespace std;

// When using a placeholder you have to initialize the function as if it has only the missing arguments required
struct Item_placed{
    function<void(int)> f; // <----
    bool eol = false;
};

queue<Item_placed> qplaced;
condition_variable search_task;
mutex my_mutex;

void thread_pool_placed() {
    Item_placed task;
    while (true){
        {
            unique_lock<mutex> lk(my_mutex);
            search_task.wait(lk, [&]{return not(qplaced.empty());}); // Here we check that the notification arrived is still true
            task = qplaced.front(); // Taking the item from the queue and the popping it safely in the mutex
            qplaced.pop();
        }
        if (task.eol) // If the queue sends the End Of List signal we exit from the loop
            return;
        {
            unique_lock<mutex> lk(my_mutex);
            task.f(2);
            // Executing the function in the mutex is not a good habit because it is a bottleneck for our performances,
            // but it is used to print properly
        }
    }
}

int main() {
    using namespace placeholders;
    vector<thread> threads(4);
    for (auto &t: threads){
        t = thread(thread_pool_placed);
    }
    function<void(int, int)> fun = [](int i, int j) {
        cout<<"Ciao sono il thread numero " <<this_thread::get_id()<< " il numero magico e' "<< j <<endl;
    };
    // Using a lambda you can call the function with args within a function
    // Instead with the bind you simply assign the args to the function,
    //  furthermore you can give to the function placeholders that will be assigned in future
    auto f3 = [fun] { return fun(7, 2); };
    auto f2 = bind(fun, 2, 4);
    auto f1 = bind(fun, 2, _1);
    {
        unique_lock<mutex> lk(my_mutex);
        qplaced.push(Item_placed{f1, false});
        qplaced.push(Item_placed{f1, true});
        qplaced.push(Item_placed{f1, true});
        qplaced.push(Item_placed{f1, false});
        qplaced.push(Item_placed{f1, false});
        qplaced.push(Item_placed{f1, true});
        qplaced.push(Item_placed{f1, true});
        search_task.notify_all();
    }
    for (auto &t: threads){
        if (t.joinable())
            t.join();
    }
    cout << qplaced.front().eol << endl;
    return 0;
}
