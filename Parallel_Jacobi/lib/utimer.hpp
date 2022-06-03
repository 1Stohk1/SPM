#include <iostream>
#include <chrono>

class timer_raii
{
  std::chrono::system_clock::time_point start;
  std::chrono::system_clock::time_point stop;
  std::string message;
  using usecs = std::chrono::microseconds;
  using msecs = std::chrono::milliseconds;

private:
  long *us_elapsed;

public:
  /**
   * @brief Creates an object that start to time (in musec) as soon as it is 
   * created and the using the Resource Acquisition Is Instantiation property, 
   * exiting from the scope it prints it. There is also the possibility to give a 
   * long memory address to store there the value 
   * 
   * @param m Message printed with the time
   * 
   * @param *us Pointer to the variable in which save the value
   * 
   */

  explicit timer_raii(const std::string m) : message(m), us_elapsed((long *)nullptr)
  {
    start = std::chrono::system_clock::now();
  }

  timer_raii(const std::string m, long *us) : message(m), us_elapsed(us)
  {
    start = std::chrono::system_clock::now();
  }

  ~timer_raii()
  {
    stop = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed =
        stop - start;
    auto musec =
        std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    std::cout << message << " computed in " << musec << " usec "
              << std::endl;
    if (us_elapsed != nullptr)
      (*us_elapsed) = musec;
  }
};
