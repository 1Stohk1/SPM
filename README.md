# SPM
Assignments of Parallel and Distributed Models subject of Artificial Intelligence Master degree of University of Pisa. 
!!!This repo's purpose is only to share a possible implementation of the exercises proposed by the professor!!!

### Installation of FastFlow

A little intro about the installation.
```
$ cd usr/local/include
$ git clone https://github.com/fastflow/fastflow.git
$ mv fastflow/ff ./
$ cd ff
$ ./mapping_strings.sh
```
***
## Assignments
The page hosts assignments given to students to assess different parallel programming techniques and methdologies in preparation of the final project.

Assignment 1 (March 9): evaluate overhead needed to start/join a thread and to start an async (launch::async policy) and get the result of the computed future. Measures should be taken as averages of a large number of events. Possibly, measure times on your machine AND on the remote virtual machine.To be completed by next Tue, March 15 (ideally).  
Assigment 2 (March 16): provide a vector<float> map(vector<float>v, int mode, function<float(float)> f, int pardegree) computing in parallel (using C++ threads or asyncs) the map(f) over v. Mode is 0 (block) or 1 (cyclic) scheduling.  To be completed by next Tue, March 22 (ideally).  
Assigment 3 (March 22): implement in parallel the odd even sort. Use as input a random generated integer vector. The odd even sort algorithm looks like the following (this is sequential; n is the length of the vector, v is the vector to sort, swap is a procedure that exchanges the two positions in the vector):  
  
```
while(not(sorted)) {
  sorted = true;
  for(i=1; i<n-1; i+=2)
    if(v[i] > v[i+1]) { swap(v[i], v[i+1]); sorted = false; }
  for(i = 0; i<n-1; i+=2)
    if(v[i] > v[i+1]) { swap(v[i], v[i+1]); sorted = false; }
}
```
  
with the intent to discuss scalability of the code.  
Assignment 4 (March 30): implement a task pool using C++ threads and STDLIB. The task pool should support parallelism degree as creation parameter and the then submission of tasks that are computed and produce results through side effects. Alternatively they have to produce a result that can be retrieved by the task submitter.  
Assignment 5 (April 6): implementation of a farm using OpenMP. Tasks to be computed have to be provided through a for loop with iterations producing one of the input tasks and then awaiting for Ta time (parameter) before executing next iteration. The parallelism degree of the farm should be a parameter. Each task should spent some parametric amount of time (possibly using the active_wait functioN) to produce the final result.  
Assignment 6 (April 13): write a parallel  program that finds all prime numbers in a given range of values, passed as parameters of the command line. In order to check if a number is prime, please use the following code:  

see [logo]: https://upload.wikimedia.org/wikipedia/commons/c/c5/Wikipedia-logo-v2-it.png  "Spiegazione dell'algoritmo su Wikipedia" http://en.wikipedia.org/wiki/Primality_test
  
```
static bool is_prime(ull n) {
   if (n <= 3) return n > 1; // 1 is not prime !

   if (n % 2 == 0 || n % 3 == 0) return false;
  for (ull i = 5; i * i <= n; i += 6) {

      if (n % i == 0 || n % (i + 2) == 0)

         return false;

  }

  return true;

}
```

Consider your favorite parallel programming framework among the ones we ve seen so far (C++ native threads, OpenMP, GrPPI) and possibly implement more that one version (e.g. a native C++ and an OpenMP version) comparing the relative performances.
***             
                            
                            *N.B. The Assignment 4 folder refers to both 4 and 5 assignments*
