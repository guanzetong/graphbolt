echo "Starting Auto experiment"
cd ../../apps/
make CF
export LD_PRELOAD=/home/ztguan/graphbolt/lib/mimalloc/out/release/libmimalloc.so

INIT_GRAPH="../../../../data2/ztguan/graphbolt/LiveJournal/init_graph.adj"
PARTITION_FILE="../../../../data2/ztguan/graphbolt/LiveJournal/partition.txt"
# PARTITION_FILE="../inputs/partition_empty.txt"
for DIR in ../../../../data2/ztguan/graphbolt/LiveJournal/*/
do
    # DIR=../inputs/20220701/1000/
    BATCH_SIZE=$(echo $DIR | cut -d '/' -f 9)
    echo $BATCH_SIZE
    # if [ $BATCH_SIZE -ne 1000 ]
    # then
        STREAM_PATH="../../../../data2/ztguan/graphbolt/LiveJournal/$BATCH_SIZE/stream_graph.txt"
        EMPTY_STREAM_PATH="../inputs/stream_graph_empty.txt"
        SNAPSHOT_PATH="../../../../data2/ztguan/graphbolt/LiveJournal/$BATCH_SIZE/snapshot"
        echo "Running Test with $INIT_GRAPH and $STREAM_PATH"
        # echo $STREAM_PATH
        # for CORE_NUM in {16..48..16}
        # do
            CORE_NUM=64
            echo "Running with $CORE_NUM cores ..."
            PROGRAM_OUT="program_$CORE_NUM.out"
            SNAPSHOT_OUT="snapshot_$CORE_NUM.out"
            ./CF -ae -s -nWorkers $CORE_NUM -numberOfUpdateBatches 10 -nEdges $BATCH_SIZE -maxIters 100 -partitionsFile $PARTITION_FILE -streamPath $STREAM_PATH $INIT_GRAPH > $PROGRAM_OUT
            
            rm -f $SNAPSHOT_OUT
            for BATCH_NUM in {1..10..1}
            do
                echo "Running Snapshot No.$BATCH_NUM ..."
                ./CF -ae -s -nWorkers $CORE_NUM -numberOfUpdateBatches 0 -nEdges 0 -maxIters 100 -partitionsFile $PARTITION_FILE -streamPath ${EMPTY_STREAM_PATH} ${SNAPSHOT_PATH}_$BATCH_NUM.adj >> $SNAPSHOT_OUT
            done

            cd ../tools/auto_test/
            # pwd
            IN_PATH="../../apps/program_${CORE_NUM}.out"
            SNAPSHOT_OUT_PATH="../../apps/snapshot_${CORE_NUM}.out"
            OUT_PATH="time_analysis_${CORE_NUM}_${BATCH_SIZE}.csv"
            ITER_PATH="iter_analysis_${CORE_NUM}_${BATCH_SIZE}.csv"
            ./time_analysis -inPath $IN_PATH -snapshotPath $SNAPSHOT_OUT_PATH -outPath $OUT_PATH -iterPath $ITER_PATH
            cd ../../apps/
            # pwd
        # done
    # fi
done