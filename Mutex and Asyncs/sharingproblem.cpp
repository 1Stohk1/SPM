#include <iostream>
#include <thread>
#include <mutex>

#include <unistd.h>  // for the usleep

int main(int argc, char * argv[]) {

  int mybanckaccount = 100;

  auto withdraw = [&] (int howmuch) {
    auto tid = std::this_thread::get_id(); 
    if(mybanckaccount>howmuch) {
      mybanckaccount -= howmuch;
      std::cout << "Thread " << tid << " successfully wiwthdrawn " << howmuch << " euros" << " (now = " << mybanckaccount << ")"  << std::endl;
    } else {
      std::cout << "Thread " << tid  << " cannot withdraw " << howmuch <<": total is (only) " << mybanckaccount << std::endl;
    }
    return;
  }; 


  std::thread t1 = std::thread(withdraw, 60);
  std::thread t2 = std::thread(withdraw, 60); 

  t1.join();
  t2.join();

  auto slowwithdraw = [&] (int howmuch) {
    auto tid = std::this_thread::get_id();
    long delay = 10000;            // 10 msecs
    auto total = mybanckaccount;   // read my total
    usleep(delay);                 // take some time
    if(total>howmuch) {            // compare
      usleep(delay);               // take some time
      mybanckaccount -= howmuch;   // withdraw
      std::cout << "Thread " << tid << " successfully wiwthdrawn " << howmuch << " euros" << " (now = " << mybanckaccount << ")"  << std::endl;
    } else {
      std::cout << "Thread " << tid  << " cannot withdraw " << howmuch <<": total is (only) " << mybanckaccount << std::endl;
    }
    return;
  };

  mybanckaccount = 100;
  
  std::thread t3 = std::thread(slowwithdraw,60);
  std::thread t4 = std::thread(slowwithdraw,60);

  t3.join();
  t4.join();

  std::mutex m;
  
  auto fairslowwithdraw = [&] (int howmuch) {
    std::unique_lock<std::mutex> l(m); 
    auto tid = std::this_thread::get_id();
    long delay = 10000;            // 10 msecs
    auto total = mybanckaccount;   // read my total
    usleep(delay);                 // take some time
    if(total>howmuch) {            // compare
      usleep(delay);               // take some time
      mybanckaccount -= howmuch;   // withdraw
      std::cout << "Thread " << tid << " successfully wiwthdrawn " << howmuch << " euros" << " (now = " << mybanckaccount << ")"  << std::endl;
    } else {
      std::cout << "Thread " << tid  << " cannot withdraw " << howmuch <<": total is (only) " << mybanckaccount << std::endl;
    }
    return;
  };

  mybanckaccount = 100; 

  std::thread t5(fairslowwithdraw,60);
  std::thread t6(fairslowwithdraw,60);

  t5.join();
  t6.join(); 
  
  return(0);

}
