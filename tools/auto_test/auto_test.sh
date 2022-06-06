echo "Starting Auto experiment"
cd ../../apps/
make PageRank
export LD_PRELOAD=/home/ztguan/graphbolt/lib/mimalloc/out/release/libmimalloc.so
for INIT_GRAPH in ../inputs/init_graph_*.adj
do
    BATCH_SIZE=$(echo $INIT_GRAPH | cut -d '/' -f 3 | cut -d '_' -f 3 | cut -d '.' -f 1)
    STREAM_PATH="../inputs/stream_graph_$BATCH_SIZE.txt"
    echo "Running Test with $INIT_GRAPH and $STREAM_PATH"
    # echo $STREAM_PATH
    for CORE_NUM in {32..160..16}
    do
        # echo "Running with $CORE_NUM cores ..."
        # PROGRAM_OUT="program_$CORE_NUM.out"
        # ./PageRank -nWorkers $CORE_NUM -numberOfUpdateBatches 100 -nEdges $BATCH_SIZE -maxIters 1000 -streamPath $STREAM_PATH $INIT_GRAPH > $PROGRAM_OUT
        cd ../tools/auto_test/
        pwd
        IN_PATH="../../apps/program_$CORE_NUM.out"
        OUT_PATH="time_analysis_$CORE_NUM.csv"
        ./time_analysis -inPath $IN_PATH -streamPath $OUT_PATH
        cd ../../apps/
        pwd
    done
done