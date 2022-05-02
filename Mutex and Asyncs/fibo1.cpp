#include <iostream>
#include <vector>
#include "utimer.cpp"
#include <thread>
#include <utility>

using namespace std;

int fibonacci(int n) {
  if(n<2)
    return 1;
  else
    return (fibonacci(n-1) + fibonacci(n-2));
}

int main(int argc, char * argv[]) {
  
  if(argc == 1) {
    cout << "Usage is: " << argv[0] << " v-len max-v seed " << endl;
    return(0);
  }

  int n    = atoi(argv[1]);
  int max  = atoi(argv[2]);
  int seed = atoi(argv[3]);

  vector<int> f(n), rseq(n), rpar(n);
  srand(seed);
  
  for(int i=0; i<n; i++) {
    f[i] = rand() % max;
  }

  {
    timer_raii tim("sequential");

    for(int i=0; i<n; i++)
      rseq[i] = fibonacci(f[i]);
  }

  {
    timer_raii tim("parallel for static");
    auto pardegree = thread::hardware_concurrency();

    vector<pair<int,int>> chunks(pardegree);
    int chunk_size = n / pardegree;
    for(int i=0; i<pardegree; i++) {
      if(i!=(pardegree-1))
	chunks[i] = make_pair(i*chunk_size, (i+1)*chunk_size-1);
      else
	chunks[i] = make_pair(i*chunk_size, n-1);
    }

    for(int i=0; i<pardegree; i++)
      cout << "chunk " << i << " is " << chunks[i].first << "," << chunks[i].second << endl;

    auto body = [&] (int j) {
      for(int i=chunks[j].first; i<=chunks[j].second; i++)
	rpar[i] = fibonacci(f[i]);
      return;
    };

    vector<thread*> tids(pardegree);
    
    for(int i=0; i<pardegree; i++)
      tids[i] = new thread(body, i);
    for(int i=0; i<pardegree; i++)
      tids[i]->join();
    
  }

  {
    timer_raii checkt("Checking");
    bool same = true;
    for(int i=0; i<n; i++) {
      if(rseq[i] != rpar[i]) {
	same = false;
	break;
      }
    }

    cout << "Sequential and parallel results are "
	 << (same ? "the same" : "different") << endl;
  }
  
  return(0);
}
    

							  
