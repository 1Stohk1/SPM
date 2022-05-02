/*
 
  Shared queue, one producer, multiple consumers. 
  We assume the queue could never be "filled". 
  Queue item producer just mutex and notify
  Queue item consumer may block upon empty queue

 */

#include <mutex>
#include <queue>
#include <condition_variable>


template<typename QTYPE>
class sharedQueue {

private:
  std::queue<QTYPE> q;
  std::mutex m;
  std::condition_variable c; 
public:

  QTYPE pop()  {
    std::unique_lock<std::mutex> l(m);
    while(q.empty())
      c.wait();
    QTYPE ret = q.front();
    q.pop();
    return ret;
  }

  void push(QTYPE v) { // unique lock is a RAII pattern style lock
    std::unique_lock<std::mutex> l(m);
    q.push(v);
    c.notify_one();
  }
  
}; 
