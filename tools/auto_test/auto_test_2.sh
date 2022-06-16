echo "Starting Auto experiment"
cd ../../apps/
make PageRank
export LD_PRELOAD=/home/ztguan/graphbolt/lib/mimalloc/out/release/libmimalloc.so
for INIT_GRAPH in ../inputs/snapshot_pokec_1k/init_graph_*.adj
do
    BATCH_SIZE=$(echo $INIT_GRAPH | cut -d '/' -f 4 | cut -d '_' -f 3 | cut -d '.' -f 1)
    STREAM_PATH="../inputs/snapshot_pokec_1k/stream_graph_$BATCH_SIZE.txt"
    EMPTY_STREAM_PATH="../inputs/stream_graph_empty.txt"
    SNAPSHOT_PATH="../inputs/snapshot_pokec_1k/snapshot_$BATCH_SIZE"
    echo "Running Test with $INIT_GRAPH and $STREAM_PATH"
    # echo $STREAM_PATH
    for CORE_NUM in {16..160..16}
    do
        echo "Running with $CORE_NUM cores ..."
        PROGRAM_OUT="program_$CORE_NUM.out"
        SNAPSHOT_OUT="snapshot_$CORE_NUM.out"
        ./PageRank -nWorkers $CORE_NUM -numberOfUpdateBatches 100 -nEdges $BATCH_SIZE -maxIters 1000 -streamPath $STREAM_PATH $INIT_GRAPH > $PROGRAM_OUT
        
        rm -f $SNAPSHOT_OUT
        for BATCH_NUM in {1..100..1}
        do
            echo "Running Snapshot No.$BATCH_NUM ..."
            ./PageRank -nWorkers $CORE_NUM -numberOfUpdateBatches 0 -nEdges 0 -maxIters 1000 -streamPath ${EMPTY_STREAM_PATH} ${SNAPSHOT_PATH}_$BATCH_NUM.adj >> $SNAPSHOT_OUT
        done

        cd ../tools/auto_test/
        # pwd
        IN_PATH="../../apps/program_$CORE_NUM.out"
        SNAPSHOT_OUT_PATH="../../apps/snapshot_$CORE_NUM.out"
        OUT_PATH="time_analysis_$CORE_NUM.csv"
        ./time_analysis -inPath $IN_PATH -snapshotPath $SNAPSHOT_OUT_PATH -streamPath $OUT_PATH
        cd ../../apps/
        pwd
    done
done