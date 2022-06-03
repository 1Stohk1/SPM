#include <iostream>
#include <chrono>

//#define START(timename) auto timename = std::chrono::system_clock::now();
//#define STOP(timename,elapsed)  auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - timename).count();


class timer_raii {
  std::chrono::system_clock::time_point start;
  std::chrono::system_clock::time_point stop;
  std::string message; 
  using usecs = std::chrono::microseconds;
  using msecs = std::chrono::milliseconds;

private:
  long * us_elapsed;
  
public:

  explicit timer_raii(const std::string m) : message(m), us_elapsed((long *)nullptr) {
    start = std::chrono::system_clock::now();
  }
    
  timer_raii(const std::string m, long * us) : message(m), us_elapsed(us) {
    start = std::chrono::system_clock::now();
  }

  ~timer_raii() {
    stop = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed =
      stop - start;
    auto musec =
      std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    
    std::cout << message << " computed in " << musec << " usec " 
	      << std::endl;
    if(us_elapsed != nullptr)
      (*us_elapsed) = musec;
  }
};
