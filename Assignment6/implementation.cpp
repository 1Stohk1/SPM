#include <iostream>
#include <string>
#include <chrono>
#include <omp.h>
#include <thread>
#include <condition_variable>
#include <future>
#include <vector>

using namespace std;
using namespace chrono_literals;

// Assignment 6 (April 13): write a parallel  program that finds all prime numbers in a given range of values,
// passed as parameters of the command line. In order to check if a number is prime, please use the following code:
//
//  static bool is_prime(ull n) {
//      if (n <= 3) return n > 1; // 1 is not prime !
//       if (n % 2 == 0 || n % 3 == 0) return false;
//   for (ull i = 5; i * i <= n; i += 6) {
//       if (n % i == 0 || n % (i + 2) == 0)
//           return false;
//   }
//   return true;
//  }
//
//  Consider your favorite parallel programming framework among the ones we've seen so far (C++ native threads, OpenMP,
//  GrPPI) and possibly implement more than one version (e.g. a native C++ and an OpenMP version) comparing the relative
//  performances.

bool is_prime(int n) {
    if (n <= 3) return n > 1; // 1 is not prime, a prime number is the one that can be divided by exactly 2 numbers!
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

void thread_fun(int start, int end){
    for (int i = start; i < end; ++i) {
        if (is_prime(i))
            cout << i << " ";
    }
}

int main(int argc, char * argv[]) {
    if (argc == 0) cout<<"The usage is nw vector_length"<<endl;
    int nw = atoi(argv[1]);
    int vec_size = atoi(argv[2]);
    { // Sequential computation
        auto start = chrono::system_clock::now();
        for (int i = 0; i < vec_size; i++) {
            if (is_prime(i))
                cout << i << " ";
        }
        cout<<endl;
        auto end = chrono::system_clock::now();
        auto musec =
                std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        cout<<"Elapsed time with sequential mode  "<<musec<<" mu"<<endl;
    }
    { // Parallel computation with Open MP
        auto start = chrono::system_clock::now();
#pragma omp parallel default(none) shared(cout, vec_size) num_threads(nw)
#pragma omp for
        for (int i = 0; i < vec_size; i++) {
            if (is_prime(i))
#pragma omp critical
                cout << i << " ";
        }
        cout<<endl;

        auto end = chrono::system_clock::now();
        auto musec =
                std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        cout<<"Elapsed time with Open MP mode     "<<musec<<" mu"<<endl;
    }
    { // Parallel computation with std::thread
        auto start = chrono::system_clock::now();
        vector<thread> threads(nw);
        int delta = vec_size/nw;
        for (int i = 0; i < nw-1; ++i) {
            threads[i] = thread(thread_fun, delta*i, delta*(i+1));
        }
        threads[nw-1] = thread(thread_fun, delta * (nw-1), vec_size);
        for (auto &tid:threads) {
            if (tid.joinable())
                tid.join();
        }
        cout<<endl;

        auto end = chrono::system_clock::now();
        auto musec =
                std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        cout<<"Elapsed time with std::thread mode "<<musec<<" mu"<<endl;
    }
}
