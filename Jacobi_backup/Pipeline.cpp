#include <ff/ff.hpp>
#include <iostream>

using namespace std;
using namespace ff;

// WARNING: To run this code you must use a Linux or Apple based OS and install https://github.com/fastflow/fastflow.git
// library, in the path you prefer and specify to the compiler or otherwise install it in the usr/local/include path
// that is already specified for searching in it. For further information on how to install FastFlow follow the
// instruction specified in the GitHub page.

void summation(int *num) {
    *num = *num + rand() % 100;
}

struct Source : ff_node_t<int> {
    int *svc(int *t) {
        for (int i = 0; i < 10; i++)
            ff_send_out(new int[i]);
        return (EOS);
    }
};

struct Sink : ff_node_t<int> {
    int *svc(int *task) {
        cout << *task << endl;
        return (GO_ON);
    }
};

struct F : ff_node_t<int> {
    int *svc(int *t) {
        summation(t);
        return t;
    }
};

int main() {
    Source Node1;
    F Node2;
    Sink Node3;
    ff_Pipe<> pipe(Node1, Node2, Node3);

    pipe.run_and_wait_end();
    return (0);
}