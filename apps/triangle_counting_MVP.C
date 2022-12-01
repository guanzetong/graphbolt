#ifndef TRIANGLE_COUNTING_MVP_C
#define TRIANGLE_COUNTING_MVP_C

#include "../core/common/utils.h"
#include "../core/graph/IO.h"
#include "../core/graph/graph.h"
#include "../core/graph/vertex.h"
#include "../core/graphBolt/ingestor.h"

using namespace std;

int main (int argc, char *argv[]) {
    // Initialization
    commandLine P(argc, argv);
    char *graph_struct_file = P.getArgument(0);
    graph<symmetricVertex> G =
        readGraph<symmetricVertex>(graph_struct_file, 0, 0, 0);
    int *vertex_prop = newA(int, G.n);

    Ingestor<symmetricVertex> ingestor(G, P);

    

}


#endif