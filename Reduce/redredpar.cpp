#include <iostream>
#include <vector>
#include <numeric>
#include <thread>
#include <atomic>

#include "utimer.cpp"

#include <chrono>
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
  return;
}

int main(int argc, char * argv[]) {

  if(argc != 4) {
    std::cout << "Usage:\n" << argv[0] << " n seed nw " << std::endl;
    return(0);
  }
  
  int n = atoi(argv[1]);    // vector lenght
  int seed = atoi(argv[2]); // seed
  int nw = atoi(argv[3]);   // number of workers
  std::vector<int> partials(nw); // partial sums
  srand(seed);

  std::vector<int> v(n);
  for(int i=0; i<n; i++) {
    v[i] = rand() % 8;
    std::cout << v[i] << " ";
  }
  std::cout << std::endl;

  {
    utimer u("reduce");

    std::vector<std::pair<int,int>> ranges(nw); // vector to compute the ranges 
    int delta { n / nw };
    std::vector<std::thread> tids;
    
    for(int i=0; i<nw; i++) {    // split the string into pieces
      ranges[i] = std::make_pair(i*delta,
				 (i != (nw-1) ? (i+1)*delta - 1 : n - 1));
      std::cout << "ranges[" << i << "] " << ranges[i].first << " , "
		<< ranges[i].second << std::endl;
    }

    auto oplus = [](const int& x, const int& y) {
		   auto r = x + y;
		   active_delay(1);
		   return(r);
		 };
    
    std::atomic<int> res;
    
    res = 0;
    auto compute_chunk = [&](std::pair<int,int> r, int nt) {
			   auto pres = v[r.first];
			   for(int i=r.first+1; i<=r.second; i++)
			     pres = oplus(pres, v[i]);
			   partials[nt] = pres;
			   return;
			 };
    
    for(int i=0; i<nw; i++) {    // assign chuncks to threads
      tids.push_back(std::thread(compute_chunk, ranges[i], i));
    }
    for(std::thread& t: tids) {  // await thread termination
      t.join();
    }
    for(int i=0; i<nw; i++)
      res += partials[i];
    
    std::cout << res << std::endl;
  }
  return(0);
}
