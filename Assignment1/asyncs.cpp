#include <iostream>
#include <chrono>
#include <thread>
#include <future>
#include "utimer.cpp"

using namespace std;

void joking_with_deferred_async(){
    int c = 0;
    int* ptr = &c;
    auto summed = async(launch::deferred, [](int* var){
        return *var += 1;
    }, ptr);
    int bellasommma;
    cout<<"the var c is still "<<c<<endl;
    try {
        bellasommma = summed.get();
    } catch (const std::exception& e) {
        cout << "Got " << e.what()<< endl;
    }
    cout<<"the value of c is "<<c<<endl;
    cout<<"the comed back var is "<<bellasommma<<endl;
}

int simple_operation(int var){
    return var += 1;
}

void handmade_chrono(){
    int c = 25;
    int summed = 0;
    auto start = std::chrono::high_resolution_clock::now();
    future<int> will_sum = async(launch::async,[](int var){
        return ++var;
        }, c);
    try {
        summed = will_sum.get();
    } catch (const std::exception& e) {
        cout << "Got " << e.what()<< endl;
    }
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(
            elapsed).count();
    cout<<"Elapsed "<<microseconds<<" to compute "<<summed<<endl;
}

void repeated_calls(){
    int c = 0;
    int* ptr = &c;
    long long microseconds = 0;
    for (int i = 0; i < 10; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        future<void> will_sum = async(launch::async,[](int* var){ //if you insert deferred, it will
            // launch the async when you need the variable indeed the timer will announce a ridiculous amount of time
            *var += 1;
        }, ptr);
        try {
            will_sum.get();
        } catch (const std::exception& e) {
            cout << "Got " << e.what()<< endl;
        }
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        microseconds += std::chrono::duration_cast<std::chrono::microseconds>(
                elapsed).count();
    }
    cout<<"Elapsed "<<microseconds<<" to compute "<<*ptr<<" in avg "<<microseconds/c<<endl;
}

int main(){
    int c = 25;
    auto start = std::chrono::high_resolution_clock::now();
    int returned = simple_operation(c);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(
            elapsed).count();
    cout<<"To compute only the single operation with a function call and a passed variable = "<< returned <<
        " elapsed "<< microseconds<<endl;
    handmade_chrono();
    repeated_calls();
}