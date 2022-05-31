#include "utimer.cpp"
#include <iostream>
#include <vector>
#include <thread>
#include <barrier>
#include <functional>

using namespace std;

int const MIN_PER_THREAD = 25;

vector<int> evenodd_sort(vector<int> vin){
    timer_raii tim ("Sequential Sort");
    bool sorted = false;
    int i, n = int(vin.size());
    while(not(sorted)) {
        sorted = true;
        for(i=1; i<n-1; i+=2)
            if(vin[i] > vin[i+1]) { swap(vin[i], vin[i+1]); sorted = false; }
        for(i = 0; i<n-1; i+=2)
            if(vin[i] > vin[i+1]) { swap(vin[i], vin[i+1]); sorted = false; }
    }
    return vin;
}

int main(int argc, char * argv[]) {
    // Initialization, creation and printing of the vector to be sorted
    if(argc == 1) { cout << "Usage is: " << argv[0] << " v-len seed " << endl; return(0);}
    int n    = atoi(argv[1]);
    int seed = atoi(argv[2]);
    bool dbg = (atoi(argv[3]) == 1);

    srand(seed);
    vector<int> messy_vec(n);
    for (int i = 0; i<n; i++){
        messy_vec[i]=(rand() / ( (RAND_MAX / 10000)));
    }
    vector<int> sorted_vec = evenodd_sort(messy_vec);

    int size = int(messy_vec.size());
    if(!size) exit(0);
    {
        timer_raii tim("Parallel sorting");
        /* To get a number of threads worth the work
        unsigned long const max_threads = (size+MIN_PER_THREAD-1)/MIN_PER_THREAD;
        unsigned long const hardware_threads = std::thread::hardware_concurrency();
        int num_threads = int(std::min(hardware_threads !=0 ? hardware_threads : 2, max_threads));*/
        int num_threads = int(std::thread::hardware_concurrency());
        int block_size = size/num_threads;
        int block_start = 0, block_end;
        atomic<bool> sorted = false;

        std::vector<std::thread*> threads(num_threads-1);
        barrier sync_a(num_threads, [&](){
            sorted = true; return;});
        barrier sync_b(num_threads, [&](){return;});

        function<void(int, int)> loop = [&] (int start, int end){
            bool sort;
            while(not(sorted)) {
                sort = true;
                for (auto i = start + 1; i < end; i+=2) {
                    if(messy_vec[i] > messy_vec[i+1]) { swap(messy_vec[i], messy_vec[i+1]); sort = false; }}
                sync_a.arrive_and_wait();
                for (auto i = start; i < end; i+=2) {
                    if(messy_vec[i] > messy_vec[i+1]) { swap(messy_vec[i], messy_vec[i+1]); sort = false; }}
                if(not(sort)){
                    if (sorted) sorted = false;
                }
                sync_b.arrive_and_wait();
            }
            return;
        };

        for(int i=0;i<(num_threads-1);i++) {
            block_end   = block_start;
            block_end  += block_size;
            threads[i]  = new thread(loop, block_start, block_end);
            block_start = block_end;
        }
        loop(block_start, int(messy_vec.size()-1));

        for (thread *t: threads) {
            if (t->joinable()) t->join();
        }
    }
    cout<< (is_sorted(messy_vec.begin(), messy_vec.end()) ? "works" : "error")<<endl;
    if(dbg){
        cout<<"\n";
        for(auto& v:messy_vec){
            cout<<v<<" ";
        }
    }
}

