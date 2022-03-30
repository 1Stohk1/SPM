#include <iostream>
#include <chrono>
#include <thread>
#include "utimer.cpp"

using namespace std;

void pointer_joking(){
    int c = 0;
    int* ptr = &c;
    int** ptr_of_ptr = &ptr;
    **ptr_of_ptr += 1;
    printf("%d, %d, %d, %d, %d", ptr, &c, *ptr, c, &ptr);
}

void simple_operation(int* ptr){
    *ptr += 1;
}

void prof_chrono(){
    int c = 0;
    int* ptr = &c;
    { timer_raii tim("Professor timer says");
        thread t2([](int* ptr){
            *ptr += 1;
        }, ptr);
        t2.join();
    }
}

void handmade_chrono(){
    int c = 0;
    int* ptr = &c;
    auto start = std::chrono::high_resolution_clock::now();
    thread t1([](int* ptr){
        *ptr += 1;
    }, ptr);
    t1.join();
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(
            elapsed).count();
    cout<<"To compute " << c << " taken " <<microseconds<<endl;
}

void repeated_calls(){
    int c = 0;
    int i;
    int* ptr = &c;
    long long microseconds = 0;
    for (i = 0; i < 20; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        thread t1([](int* ptr){
            *ptr += 1;
        }, ptr);
        t1.join();
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        microseconds += std::chrono::duration_cast<std::chrono::microseconds>(
                elapsed).count();
    }
    cout<<"To compute "<< c <<" elapsed "<<microseconds<< "\n The avg is "<< microseconds/i <<endl;
}



int main() {
    int c = 0;
    int* ptr = &c;
    auto start = std::chrono::high_resolution_clock::now();
    simple_operation(ptr);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(
            elapsed).count();
    cout<<"To compute only the single operation with a function call and a passed pointer "<< c <<
    " elapsed "<< microseconds<<endl;
    prof_chrono();
    handmade_chrono();
    repeated_calls();
}

