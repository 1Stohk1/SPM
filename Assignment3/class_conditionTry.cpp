#include "utimer.cpp"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

// With the usage of a class we can declare explicitly the place in which the mutex belong
// furthermore we are encapsulating all the proper code in a more readable method
class Sort
{
private:
    bool sorted=false;
    mutex even, odd;
    condition_variable even_cond, odd_cond;
    unsigned int n_threads;
    int even_c=0, odd_c=0;
public:
    void ranging(vector<int> &vin, unsigned int &size, unsigned int &num_threads, unsigned int &block_size){
        size = vin.size();
        if(!size) exit(0);
        unsigned long const min_per_thread=25;
        unsigned long const max_threads=(size+min_per_thread-1)/min_per_thread;
        unsigned long const hardware_threads= std::thread::hardware_concurrency();
        num_threads=std::min(hardware_threads !=0 ? hardware_threads:2,max_threads);
        n_threads=num_threads;
        block_size=size/num_threads;
}
    void loop(vector<int>::iterator block_start, vector<int>::iterator block_end){
        while(not(sorted)) {
            sorted = true;
            for (auto i = block_start; i != prev(block_end,1); i+=2) {
                //The problem is that using iterator and jumping by 2 in 2 you can exceed the block.end and therefore
                // fall in to segmentation error because the loop will continue to run outside the memory heap
                // associated with the iterators
                if(*i > *next(i, 1)) { iter_swap(i, next(i, 1)); sorted = false; }}
            {
                std::unique_lock<std::mutex> lk(even);
                even_c++;
                even_cond.wait(
                        lk,[this]{return even_c == n_threads;});
                odd_c=0;
                even_cond.notify_all();
            }
            for (auto i = next(block_start, 1); i != block_end; i+=2) {
                if(*i > *next(i, 1)) { iter_swap(i, next(i, 1)); sorted = false; }}
            {
                std::unique_lock<std::mutex> lk(odd);
                odd_c++;
                odd_cond.wait(
                        lk,[this]{return odd_c == n_threads;});
                even_c=0;
                odd_cond.notify_all();
            }
        }
    }
};

class ciao {
public:
    void printing(vector<int>::iterator start, vector<int>::iterator end) {
        for (auto i = start; i != end-1; i++) {
            cout<<*i<<" ";
            iter_swap(i, next(i,1));
        }
    }
};

// Of course the usage of a class means that we should point out what is the class owner of the member function,
// in order to pass the member function we must call the function inside the proper class and the pass a pointer to
// the class instantiated
int main(int argc, char * argv[]) {
    if(argc == 1) { cout << "Usage is: " << argv[0] << " v-len seed " << endl; return(0);}
    int n    = atoi(argv[1]);
    int seed = atoi(argv[2]);
    srand(seed);
    vector<int> messy_vec(n);
    for (int i = 0; i<n; i++){
        messy_vec[i]=(rand() / ( (RAND_MAX / 10000)));
    }
    unsigned int size;
    unsigned int num_threads;
    unsigned int block_size;
    Sort x;
    x.ranging(messy_vec, size, num_threads, block_size);
    vector<int>::iterator block_start, block_end;
    std::vector<std::thread> threads(num_threads-1);
    block_start=messy_vec.begin();
    ciao c;
    for(auto& v:messy_vec){
        cout<<v<<" ";
    }

    cout<<"\n";
    thread test(&ciao::printing,&c, block_start, messy_vec.end());
    if (test.joinable()){
        test.join();
    }
    cout<<"\n";
    for(auto& v:messy_vec){
        cout<<v<<" ";
    }
    cout<<"\n";
    for(int i=0;i<(num_threads-1);++i) {
        block_end=block_start;
        advance(block_end,block_size);
        threads[i]=thread(&Sort::loop, &x, block_start, block_end);
        block_start=block_end;
    }
    x.loop(block_start, prev(messy_vec.end(), 1));

    for (thread& t: threads) {
        if (t.joinable()) t.join();
    }
    cout<<"\n";
    for(auto& v:messy_vec){
        cout<<v<<" ";
    }
}

