#include <iostream>
#include <thread>
#include <queue>
#include <condition_variable>
#include <future>
#include <vector>
#include <functional>
#include <mutex>

using namespace std;

struct Item{
    function<void()> f;
    bool eol = false;
};

queue<Item> my_q;
condition_variable search_task;
mutex my_mutex;

void thread_pool() {
    Item task;
        while (true){
            {
                unique_lock<mutex> lk(my_mutex);
                search_task.wait(lk, [&]{return not(my_q.empty());});
                task = my_q.front();
                my_q.pop();
            }
        if (task.eol)
            return;
        {
            unique_lock<mutex> lk(my_mutex);
            task.f();
        }
    }
}


int main() {
    vector<thread> threads(4);
    for (auto &t: threads){
        t = thread(thread_pool);
    }
    function<void(int)> fun = [](int i) {
        cout<<"Ciao sono il thread numero " <<this_thread::get_id()<< " il numero magico e' "<< i <<endl;
    };
    // Using a lambda you can call the function with args within a function
    // Instead with the bind you simply assign the args to the function,
    //  further more you can give to the function placeholders that will be assigned in future
    auto f1 = [fun] { return fun(7); };
    auto f2 = bind(fun, 2);
    {
        unique_lock<mutex> lk(my_mutex);
        my_q.push(Item{f1, false});
        my_q.push(Item{f1, true});
        my_q.push(Item{f1, true});
        my_q.push(Item{f2, false});
        my_q.push(Item{f1, true});
        my_q.push(Item{f1, true});
        search_task.notify_all();
    }
    for (auto &t: threads){
        if (t.joinable())
            t.join();
    }
    cout<<my_q.front().eol<<endl;
    return 0;
}
