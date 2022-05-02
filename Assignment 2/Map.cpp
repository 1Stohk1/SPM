#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <vector>
#include <thread>
#include <functional>

#include "utimer.cpp"

using namespace std;

void activewait(std::chrono::milliseconds ms) {
  long msecs = ms.count();
  auto start = std::chrono::high_resolution_clock::now();
  auto end   = false;
  while(!end) {
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
    if(msec>msecs)
      end = true;
  }
  return;
}

int f(int x) {
  activewait(1ms);
  return(x+x);
}

typedef enum {CHUNK=0, SCATTER} scatter_t;

vector<float> map(vector<float> vin, scatter_t mode, function<float(float)> f, int pardegree) {
  // chunk or scatter mode?
  int n = vin.size();
  vector<float> res(n);
  
  if(mode == CHUNK) {
    // first compute chuncks
    auto compute_chunk = [&](pair<int,int> range) {   // function to compute a chunk
			   for(int i=range.first; i<range.second; i++) {
			     res[i] = f(vin[i]);
			   }
			   return; 
			 };
    
    vector<pair<int,int>> ranges(pardegree);                     // vector to compute the ranges 
    int delta { n / pardegree };
    vector<thread> tids;
    
    for(int i=0; i<pardegree; i++) {                     // split the string into pieces
      ranges[i] = make_pair(i*delta,(i != (pardegree-1) ? (i+1)*delta : n)); 
    }
    // fork join model
    for(int i=0; i<pardegree; i++) {                     // assign chuncks to threads
      tids.push_back(thread(compute_chunk, ranges[i]));
    }
    for(thread& t: tids) {                        // await thread termination
      t.join();
    }
    // return computed result
    return res;
  } else {
    // this is scatter mode
    vector<thread> tids;

    auto compute_chunk = [&](int tno) {   // function to compute a chunk
			   for(int i=tno; i<n; i+=pardegree) {
			     res[i] = f(vin[i]);
			   }
			   return; 
			 };
    
    // fork join model
    for(int i=0; i<pardegree; i++) {                     // assign chuncks to threads
      tids.push_back(thread(compute_chunk, i));
    }
    for(thread& t: tids) {                        // await thread termination
      t.join();
    }
    // return computed result
    return res;
  }
}

int main(int argc, char * argv[]) {

  if(argc == 1) {
    cout << "Usage is:" << argv[0] << " seed n pardegree [CHUNK|SCATTER]" << endl;
    return(0);
  }  
  int seed = atoi(argv[1]);
  int n = atoi(argv[2]);
  int nw = atoi(argv[3]);
  scatter_t scatterMode = ((argv[4][0]=='C') ? CHUNK : SCATTER); 

  const int max = 1024;
  srand(seed);
  vector<float> v(n),r1(n),r2(n);
  for(int i=0; i<n; i++)
    v[i] = rand()%max;
  

  // sequential execution
  {
    utimer tseq("seq");
    transform(v.begin(), v.end(), r1.begin(),f);
  }

  // pthread parallel execution
  {                                       
    utimer tpar("par");
    r2 = map(v, scatterMode, f, nw);
  }

  // merge is empty
  
  for(int i=0; i<n; i++)
    if(r1[i] != r2[i]) 
      cout << r1[i] << " != " << r2[i] << " at " << i << endl;

  return(0);
}
