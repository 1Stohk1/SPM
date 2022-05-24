#include <iostream>
#include <chrono>
#include <future>
#include <valarray>
#include <vector>

using namespace std;

// USED TO REPRESENT VECTOR CHUNKS
typedef struct {
    int start;
    int end;
} RANGE;

void block_method (RANGE range, float collection[]) {   // function to compute a chunk
    for(int i=range.start; i<range.end; i++) {
        collection[i] = sqrtf(collection[i]);
    }
}

void active_delay(int msecs) {
    // read current time
    auto start = std::chrono::high_resolution_clock::now();
    auto end   = false;
    while(!end) {
        auto elapsed = std::chrono::high_resolution_clock::now() - start;
        auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
        if(msec>msecs)
            end = true;
    }
}

void map_block(vector<float> &collection, int pardegree){
    auto start = chrono::high_resolution_clock::now();
    int m = collection.size();
    int delta { m / pardegree };
    vector<RANGE> ranges(pardegree);
    vector <shared_future<void>> aid(pardegree);

    for(int i=0; i<pardegree; i++) {// split the vector into chunks
        ranges[i].start = i*delta;
        ranges[i].end   = (i != (pardegree-1) ? (i+1)*delta : m);
    }

    for (int i=0; i<pardegree; ++i) {
        aid[i] = async(launch::async, [&collection] (RANGE range) {
            for(int i=range.start; i<range.end; i++) {
                collection[i] = sqrtf(collection[i]);
            }

        }, ranges[i]);
    }

    for (int i=0; i<pardegree; ++i) {
        try {
            aid[i].get();
        } catch (const std::exception& e) {
            cout << "Got " << e.what()<< endl;
        }
    }
    auto elapsed = chrono::high_resolution_clock::now() - start;
    long long microseconds = chrono::duration_cast<chrono::microseconds>(
            elapsed).count();
    cout<<"Elapsed "<<microseconds<<" to process the vector with "<<pardegree<<" pardegree"<<endl;
}

void map_cyclic(int m, int pardegree, function<void(int)> f){
    auto start = chrono::high_resolution_clock::now();
    vector <future<void>> aid(pardegree);

    for (int i=0; i<pardegree; ++i) {
        aid[i] = async(launch::async, [m, pardegree](int index, function<void(int)> cyclic_method) {
            for (int j = 0; j < m; j+=pardegree) {
                cyclic_method(index);
            }
        }, i, f);
    }

    for (int i=0; i<pardegree; ++i) {
        try {
            aid[i].get();
        } catch (const std::exception& e) {
            cout << "Got " << e.what()<< endl;
        }
    }
    auto elapsed = chrono::high_resolution_clock::now() - start;
    long long microseconds = chrono::duration_cast<chrono::microseconds>(
            elapsed).count();
    cout<<"Elapsed "<<microseconds<<" to compute the vector sqrt with "<<pardegree<<" pardegree"<<endl;
}

int main(int argc, char * argv[]) {
    if(argc == 1) {
        cout << "Usage is: " << argv[0] << " v-len max-nw mode(0/1) seed " << endl;
        return(0);
    }
    int n    = atoi(argv[1]);
    int max_nw = atoi(argv[2]);
    int mode = atoi(argv[3]);
    int seed = atoi(argv[4]);
    srand(seed);

    vector<float> collection(n);

    for (int i = 0; i<n; i++){
        collection[i] = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(100-0)));
    }

    //This is a void function made using a lambda and obj passed by reference
    function<void(RANGE)> compute_block = [&collection](RANGE range) {   // function to compute a chunk
        for(int i=range.start; i<range.end; i++) {
            collection[i] = sqrtf(collection[i]);
        }
    };

    function<void(int)> compute_cycle = [&collection](int i) {   // function to compute a chunk
        collection[i] = sqrtf(collection[i]);
    };

    if (mode == 0) { //Block Static Template for Load Balancing
        cout<<"Selected Block method"<<endl;
        for (int nw = 1; nw <= max_nw; ++nw) {
            map_block(collection, nw);
        }
    } else { //Cyclic Static Template for Load Balancing
        cout<<"Selected Cyclic method"<<endl;
        for (int nw = 1; nw <= max_nw; nw++) {
            map_cyclic(n, nw, compute_cycle);
        }
    }
}

