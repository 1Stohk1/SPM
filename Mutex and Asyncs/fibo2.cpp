#include <iostream>
#include <vector>
#include "utimer.cpp"
#include <thread>
#include <utility>
#include <future>

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

  vector<int> f(n), rseq(n);
  vector<shared_future<int>> rpar(n);  // necessario perchè altrimenti il secondo get fallisce ... 
  
  srand(seed);
  
  for(int i=0; i<n; i++) {
    f[i] = rand() % max;
  }

  {
    timer_raii tim("sequential        ");

    for(int i=0; i<n; i++)
      rseq[i] = fibonacci(f[i]);
  }

  {
    timer_raii tim("parallel async_all");

    for(int i=0; i<n; i++) 
      rpar[i] = async(launch::async, fibonacci, f[i]);
    // asycn launched, now wait results
    for(int i=0; i<n; i++)
      try {
	rpar[i].get();
      } catch (const std::exception& e) {
	cout << "Got " << e.what() << " i= " << i << endl; 
      }
  }
  
  {
    timer_raii("Checking");
    bool same = true;
    for(int i=0; i<n; i++) {
      try {
	if(rseq[i] != rpar[i].get()) {
	  same = false;
	  break;
	}
      }  catch (const std::exception& e) {
	cout << "Got " << e.what() << " i= " << i << endl; 
      }

    }

    cout << "Sequential and parallel results are "
	 << (same ? "the same" : "different") << endl;
  }
  
  return(0);
}
    

							  
