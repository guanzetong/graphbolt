echo "Starting Auto experiment"
cd ../../apps/
# make PageRank
export LD_PRELOAD=/home/ztguan/graphbolt/lib/mimalloc/out/release/libmimalloc.so

NUM_BATCH=10
INIT_GRAPH="../inputs/LiveJournal_2/init_graph.adj"
for DIR in ../inputs/LiveJournal_2/*/
do
    # DIR=../inputs/LiveJournal_2/1000/
    BATCH_SIZE=$(echo $DIR | cut -d '/' -f 4)
    echo $BATCH_SIZE
    # if [ $BATCH_SIZE -eq 1000 ]
    # then
        STREAM_PATH="../inputs/LiveJournal_2/$BATCH_SIZE/stream_graph.txt"
        EMPTY_STREAM_PATH="../inputs/stream_graph_empty.txt"
        SNAPSHOT_PATH="../inputs/LiveJournal_2/$BATCH_SIZE/snapshot"
        # INCR_OUTPUT_PATH="output_incr_${BATCH_SIZE}.out"
        # SNAP_OUTPUT_PATH="output_snap_${BATCH_SIZE}.out"
        echo "Running Test with $INIT_GRAPH and $STREAM_PATH"
        # echo $STREAM_PATH
        # for CORE_NUM in {16..160..16}
        # do
            CORE_NUM=64
            echo "Running with $CORE_NUM cores ..."
            PROGRAM_OUT="program_$CORE_NUM.out"
            SNAPSHOT_OUT="snapshot_$CORE_NUM.out"
            ./PageRank -ae -nWorkers $CORE_NUM -numberOfUpdateBatches $NUM_BATCH -nEdges $BATCH_SIZE -maxIters 1000 -streamPath $STREAM_PATH $INIT_GRAPH > $PROGRAM_OUT
            
            rm -f $SNAPSHOT_OUT
            for BATCH_NUM in {1..10..1}
            do
                echo "Running Snapshot No.$BATCH_NUM ..."
                ./PageRank -ae -nWorkers $CORE_NUM -numberOfUpdateBatches 0 -nEdges 0 -maxIters 1000 -streamPath ${EMPTY_STREAM_PATH} ${SNAPSHOT_PATH}_$BATCH_NUM.adj >> $SNAPSHOT_OUT
            done

            cd ../tools/auto_test/
            # pwd
            IN_PATH="../../apps/program_${CORE_NUM}.out"
            SNAPSHOT_OUT_PATH="../../apps/snapshot_${CORE_NUM}.out"
            OUT_PATH="time_analysis_${CORE_NUM}_${BATCH_SIZE}.csv"
            ITER_PATH="iter_analysis_${CORE_NUM}_${BATCH_SIZE}.csv"
            ./time_analysis -inPath $IN_PATH -snapshotPath $SNAPSHOT_OUT_PATH -outPath $OUT_PATH -iterPath $ITER_PATH
            cd ../../apps/
            pwd
        # done
    # fi
done