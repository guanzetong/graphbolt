echo "Starting Auto experiment"
cd ../../apps
make PageRank
export LD_PRELOAD=/home/ztguan/graphbolt/lib/mimalloc/out/release/libmimalloc.so
for INIT_GRAPH in ../inputs/init_graph_*.adj
do
    BATCH_SIZE=$(echo $INIT_GRAPH | cut -d '/' -f 3 | cut -d '_' -f 3 | cut -d '.' -f 1)
    STREAM_PATH="../inputs/stream_graph_$BATCH_SIZE.txt"
    echo "Running Test with $INIT_GRAPH and $STREAM_PATH"
    # echo $STREAM_PATH
    ./PageRank -numberOfUpdateBatches 10 -nEdges $BATCH_SIZE -maxIters 1000 -streamPath $STREAM_PATH $INIT_GRAPH >> program.out
done