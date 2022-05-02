#include "utimer.cpp"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <ctime>

using namespace std;

// With the usage of a class we can declare explicitly the place in which the mutex belong
// furthermore we are encapsulating all the proper code in a more readable method
class some_class
{   int c = 0;
private:
    mutex m;
public:
    void print_time(){
        {
            lock_guard<mutex> guard(m);
            auto end = std::chrono::system_clock::now();
            std::time_t end_time = std::chrono::system_clock::to_time_t(end);
            c++;
            cout << ctime(&end_time) << " of the thread " << this_thread::get_id()<< " \n this is the run n "<< c << endl;
        }
    }
};

// Of course the usage of a class means that we should point out what is the class owner of the member function,
// in order to pass the member function we must call the function inside the proper class and the pass a pointer to
// the class instantiated
int main(){
    unsigned int max_threads = thread::hardware_concurrency();
    vector<thread> threads(max_threads);
    some_class x;
    for (int i = 0; i < max_threads-1; ++i) {
        threads.emplace_back(thread(&some_class::print_time, &x)); // <-- like here
    }
    x.print_time();
    for (thread& t: threads) {
        if (t.joinable()) t.join();
    }
}

