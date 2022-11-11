echo "Starting Auto experiment"
export LD_PRELOAD=/home/ztguan/graphbolt/lib/mimalloc/out/release/libmimalloc.so

# Compile executable
cd ../../apps/
make COEM

# Specify Paths to graph dataset directory
DATASET_PATH="../../../../data2/ztguan/graphbolt/twitter_follower"

# Set path to initial graph
INIT_GRAPH="$DATASET_PATH/init_graph.adj"

# Set path to partition info
# It is only applicable to some algorithms that require the graph to be partitioned in to 2 parts
# e.g. COEM, CF
PARTITION_FILE="$DATASET_PATH/partition.txt"

# Set path to seed vertices info
# It is only applicable to some algorithms that require explicitly defined seeds
# e.g. LabelPropagation, COEM
SEEDS_FILE="$DATASET_PATH/seeds.txt"

# Iterate over the directory of dataset
# The folder containing streams of different batch size are named the batch size
for DIR in $DATASET_PATH/*/
do
    # Extract the batch size from folder name
    BATCH_SIZE=$(echo $DIR | cut -d '/' -f 9)
    echo $BATCH_SIZE

    # Set path to streaming batches
    STREAM_PATH="$DATASET_PATH/$BATCH_SIZE/stream_graph.txt"

    # Empty stream is for running on snapshots
    EMPTY_STREAM_PATH="../inputs/stream_graph_empty.txt"

    # Set path to snapshot dataset
    SNAPSHOT_PATH="$DATASET_PATH/$BATCH_SIZE/snapshot"

    echo "Running Test with $INIT_GRAPH and $STREAM_PATH"

    # Iterate over different number of cores (threads) if you want
    # Normally the performance gain vanishes above 64 threads.

    for CORE_NUM in {16..64..16}
    do
        CORE_NUM=64
        echo "Running with $CORE_NUM cores ..."

        # Set the path to profiling printout
        PROGRAM_OUT="program_$CORE_NUM.out"
        SNAPSHOT_OUT="snapshot_$CORE_NUM.out"
        
        # Trigger the graph algorithm on initial and streaming dataset
        # Follow the instruction in GraphBolt Readme to run other algorithms
        ./COEM -ae -nWorkers $CORE_NUM -numberOfUpdateBatches 10 -nEdges $BATCH_SIZE -maxIters 100 -seedsFile $SEEDS_FILE -partitionsFile $PARTITION_FILE -streamPath $STREAM_PATH $INIT_GRAPH > $PROGRAM_OUT
        
        # Trigger the graph algorithm on snapshots one by one
        # The printouts are collected in to a single file
        rm -f $SNAPSHOT_OUT
        for BATCH_NUM in {1..10..1}
        do
            echo "Running Snapshot No.$BATCH_NUM ..."
            ./COEM -ae -nWorkers $CORE_NUM -numberOfUpdateBatches 0 -nEdges 0 -maxIters 100 -seedsFile $SEEDS_FILE -partitionsFile $PARTITION_FILE -streamPath ${EMPTY_STREAM_PATH} ${SNAPSHOT_PATH}_$BATCH_NUM.adj >> $SNAPSHOT_OUT
        done

        cd ../tools/auto_test/
        # pwd
        
        # Set the path to printouts
        IN_PATH="../../apps/program_${CORE_NUM}.out"
        SNAPSHOT_OUT_PATH="../../apps/snapshot_${CORE_NUM}.out"

        # Set the path to profiling results
        OUT_PATH="time_analysis_${CORE_NUM}_${BATCH_SIZE}.csv"
        ITER_PATH="iter_analysis_${CORE_NUM}_${BATCH_SIZE}.csv"

        # Extract the profiling results
        ./time_analysis -inPath $IN_PATH -snapshotPath $SNAPSHOT_OUT_PATH -outPath $OUT_PATH -iterPath $ITER_PATH
        
        cd ../../apps/
    done
done