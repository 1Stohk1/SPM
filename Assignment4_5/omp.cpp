#include <iostream>
#include <omp.h>
using namespace std;

int main(){
    // Praticamente quando stai utilizzando pragma, il clang-tidy ti consiglia di specificare quali sono le variabili
    // in shared memory in modo tale da facilitare il debug, naturalmente cout essendo una libreria esterna le chiamate
    //  ad essa devono essere specificate
#pragma omp parallel default(none) shared(cout)
    for (int i = 0; i < 10; ++i) {
        cout<<"Ciao " <<omp_get_thread_num()<<" "<<endl;
    }
}