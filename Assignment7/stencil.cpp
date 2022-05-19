#include <ff/ff.hpp>
#include <iostream>
#include <vector>
#include <iterator>

using namespace std;
using namespace ff;

// WARNING: To run this code you must use a Linux or Apple based OS and install https://github.com/fastflow/fastflow.git
// library, in the path you prefer and specify to the compiler or otherwise install it in the usr/local/include path
// that is already specified for searching in it. For further information on how to install FastFlow follow the
// instruction specified in the GitHub page.

typedef struct {
    int start;
    int end;
} RANGE;


void stencil_seq(vector<float> &vec){ 
    int size = vec.size();
    vector<float> tmp_vec(size);
    tmp_vec[0] = vec[0];
    tmp_vec[size-1] = vec[size-1];
    for (auto i = 1; i < size - 1; i++)
    {
        tmp_vec[i] = (vec[i-1] + vec[i] + vec[i+1])/3;
    }
    vec = tmp_vec;
}

void stencil_par(vector<float> &tmp, vector<float> &vec, RANGE delta){ 
    for (auto i = delta.start; i <= delta.end; i++)
    {
        tmp[i] = (vec[i-1] + vec[i] + vec[i+1])/3;
    }
}


// struct F : ff_node_t<int> {
//     int *svc(int *t) {
//         stencil(t);
//         return t;
//     }
// }; 


int main(int argc, char * argv[]) {
    if(argc == 1) {
        cout << "Usage is: " << argv[0] << " v-len iterations nw " << endl;
        return(0);
    }
    int v_len = atoi(argv[1]);
    int iterations = atoi(argv[2]);
    int nw = atoi(argv[3]);

    // Preparing the vectors
    vector<float> initial_vec(v_len);
    vector<float> temp_vec(v_len);
    for (auto item = initial_vec.begin(); item != initial_vec.end(); item++)
    {
        *item = 0;
    }
    initial_vec[v_len-1] = 100;
    initial_vec[v_len/2] = 25;
    temp_vec = initial_vec;

{   //Calling the sequential operator for comparing the results
    vector<float> seq_comp;
    seq_comp = initial_vec;
    for (int i = 0; i < iterations; i++)
    {
        stencil_seq(seq_comp);
    }
    
    for (auto item = seq_comp.begin(); item != seq_comp.end(); item++)
    {
        cout<<*item<<" "; 
    }
    cout<<endl; 
}
    // Creation of the RANGEs
    int delta = v_len / nw;
    vector<RANGE> ranges(nw);
    for (int i = 0; i < nw; i++)
    {
        ranges[i].start = delta * i + 1;
        ranges[i].end = delta * (i + 1);
    }
    ranges[nw-1].end = v_len - 2;
    
    // Range checker
    for (auto i = ranges.begin(); i != ranges.end(); i++){cout<<" Start "<<i->start<< " " << i->end;}
    cout<<endl;

    for (auto i = 0; i < iterations; i++)
    {
        for (auto j = 0; j < nw; j++)
        {
            stencil_par(temp_vec, initial_vec, ranges[j]);
        }
        initial_vec = temp_vec;
    }

    cout<<"Testing the parallel code"<<endl; 
    for (auto item = initial_vec.begin(); item != initial_vec.end(); item++)
    {
        cout<<*item<<" "; 
    }
    cout<<endl; 

    return (0);
}