#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>


using namespace std::chrono_literals;
int  x = 0; 

void body2(int n) {
  x += n;
  return;
}

int main(int argc, char * argv[]) {

  int nthread = atoi(argv[1]);
  std::thread * t[nthread];

  std::cout << "x = " << x << std::endl; 
  std::cout << "Running " << nthread << " threads" << std::endl;
  for(int i=0; i<nthread; i++) {
    t[i] = new std::thread(body2,i);
  }

  for(int i=0; i<nthread; i++) {
    t[i]->join();
  }
  std::cout << "x = " << x << std::endl; 
  return(0);
}
