#include "../core/common/utils.h"
#include "../core/graph/IO.h"
#include "../core/graph/graph.h"
#include "../core/graph/vertex.h"
#include "../core/graphBolt/ingestor.h"

using namespace std;

unsigned long search_triangles(graph<asymmetricVertex> &G);

int main (int argc, char *argv[]) {
    // Initialization
    commandLine P(argc, argv);                                      //  Command line input arguments
    char *graph_struct_file = P.getArgument(0);                     //  Get the data graph file path
    graph<asymmetricVertex> G =
        readGraph<asymmetricVertex>(graph_struct_file, 0, 0, 0);     //  Read the data graph from file
    int *vertex_prop = newA(int, G.n);                              //  Allocate vertex property array

    // Do initial computation here
    unsigned long count = search_triangles(G);
    std::cout << "Num Matches " << count << '\n';

    Ingestor<asymmetricVertex> ingestor(G, P);                       //  Create an ingestor object for streaming
    ingestor.validateAndOpenFifo();                                 //  Open the streaming file
    while (ingestor.processNextBatch()) {
        edgeArray &edge_additions = ingestor.getEdgeAdditions();    //  Return added edges, useful for pruning computation
        edgeArray &edge_deletions = ingestor.getEdgeDeletions();    //  Return deleted edges, useful for pruning computation

        // Do delta-based computation or traditional computation here
        unsigned long count = search_triangles(G);
        std::cout << "Num Matches " << count << '\n';
    }
    return 0;
}

unsigned long search_triangles(graph<asymmetricVertex> &G) {
    unsigned long count = 0;

    for (int node1 = 0; node1 < G.n; ++node1) {
        const int node1_in_degree = G.V[node1].getInDegree();
        const int node1_out_degree = G.V[node1].getOutDegree();
        const int node1_degree = node1_in_degree + node1_out_degree;
        std::unordered_set<int> node1_neighbors;
        for (int node1_neighbour = 0; node1_neighbour < node1_degree; ++node1_neighbour) {
            int node2_temp;
            if (node1_neighbour < node1_in_degree){
                node2_temp = G.V[node1].getInNeighbor(node1_neighbour);
            } else {
                node2_temp = G.V[node1].getOutNeighbor(node1_neighbour - node1_in_degree);
            }
            if( node1_neighbors.count(node2_temp) == 0) {
                node1_neighbors.insert(node2_temp);
            } else {
                continue;
            }
            const int node2 = node2_temp;

            if (node2 < node1) {
                continue;
            }

            const int node2_in_degree = G.V[node2].getInDegree();
            const int node2_out_degree = G.V[node2].getOutDegree();
            const int node2_degree = node2_in_degree + node2_out_degree;
            std::unordered_set<int> node2_neighbors;
            for (int node2_neighbour = 0; node2_neighbour < node2_degree; ++node2_neighbour) {
                int node3_temp;
                if (node2_neighbour < node2_in_degree) {
                    node3_temp = G.V[node2].getInNeighbor(node2_neighbour);
                } else {
                    node3_temp = G.V[node2].getOutNeighbor(node2_neighbour - node2_in_degree);
                }

                if (node2_neighbors.count(node3_temp) == 0){
                    node2_neighbors.insert(node3_temp);
                } else {
                    continue;
                }
                
                const int node3 = node3_temp;

                if (node3 < node2) {
                    continue;
                }

                const int node3_in_degree = G.V[node3].getInDegree();
                const int node3_out_degree = G.V[node3].getOutDegree();
                const int node3_degree = node3_in_degree + node3_out_degree;
                std::unordered_set<int> node3_neighbors;
                for (int node3_neighbour = 0; node3_neighbour < node3_degree; ++node3_neighbour) {
                    int node4_temp;
                    if (node3_neighbour < node3_in_degree) {
                        node4_temp = G.V[node3].getInNeighbor(node3_neighbour);
                    } else {
                        node4_temp = G.V[node3].getOutNeighbor(node3_neighbour - node3_in_degree);
                    }

                    if (node3_neighbors.count(node4_temp) == 0) {
                        node3_neighbors.insert(node4_temp);
                    } else {
                        continue;
                    }
                    
                    if (node4_temp == node1){
                        std::cout << "Match:" << node1 << "->" << node2 << "->" << node3 << '\n'; 
                        ++count;
                    }
                    
                    // if (node3_neighbour < node3_in_degree) {
                    //     if(G.V[node3].getInNeighbor(node3_neighbour) == node1) {
                    //         std::cout << "Match:" << node1 << "->" << node2 << "->" << node3 << '\n'; 
                    //         ++count;
                    //     }
                    // } else {
                    //     if(G.V[node3].getOutNeighbor(node3_neighbour - node3_in_degree) == node1) {
                    //         std::cout << "Match:" << node1 << "->" << node2 << "->" << node3 << '\n'; 
                    //         ++count;
                    //     }
                    // }
                }
            }
        }
        
    }

    return count;
}
