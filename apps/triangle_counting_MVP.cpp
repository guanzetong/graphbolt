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
    commandLine P(argc, argv);                                      //  Command line input arguments
    char *graph_struct_file = P.getArgument(0);                     //  Get the data graph file path
    graph<asymmetricVertex> G =
        readGraph<asymmetricVertex>(graph_struct_file, 0, 0, 0);     //  Read the data graph from file
    int *vertex_prop = newA(int, G.n);                              //  Allocate vertex property array

    //
    // Do initial computation here
    //


    Ingestor<asymmetricVertex> ingestor(G, P);                       //  Create an ingestor object for streaming
    ingestor.validateAndOpenFifo();                                 //  Open the streaming file
    while (ingestor.processNextBatch()) {
        edgeArray &edge_additions = ingestor.getEdgeAdditions();    //  Return added edges, useful for pruning computation
        edgeArray &edge_deletions = ingestor.getEdgeDeletions();    //  Return deleted edges, useful for pruning computation

        //
        // Do delta-based computation or traditional computation here
        // 

    }

}


#endif
