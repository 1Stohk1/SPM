#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

void body1(int n) {
  std::cout << "This is thread no. " << n << std::endl;
  std::this_thread::sleep_for(1000ms);
  std::cout << "thread " << n << " ending now! " << std::endl; 
  return;
}

void body2(int n, int& x) {
  x = x + n;
  return;
}

int main(int argc, char * argv[]) {

  int nthread = atoi(argv[1]);
  std::thread * t[nthread];
  
  std::cout << "Running " << nthread << " threads" << std::endl;
  for(int i=0; i<nthread; i++) {
    t[i] = new std::thread(body1,i);
  }

  for(int i=0; i<nthread; i++) {
    t[i]->join();
  }
  return(0);
}
