# Number of edges in the original dataset
MAX_EDGE_NUM=5533214

# Array of batch sizes
BATCH_SIZE_ARRAY=(1000 10000 100000 1000000)

# Number of batches in a stream
BATCH_NUM=10

# Path to input original dataset (SNAP format)
BASIC_GRAPH="../../../../../data2/ztguan/graphbolt/roadNet-CA.txt"

# Path to output initial graph (SNAP format)
INIT_GRAPH="../../../../../data2/ztguan/graphbolt/CA_roadnet/init_graph.snap"

# Path to output idle graph (SNAP format)
IDLE_GRAPH="../../../../../data2/ztguan/graphbolt/CA_roadnet/idle_graph.snap"

# Path to output initial graph (adjacency format)
INIT_GRAPH_FILE_ADJ="../../../../../data2/ztguan/graphbolt/CA_roadnet/init_graph.adj"


# Iterate over the batch sizes
for batch_size in ${BATCH_SIZE_ARRAY[@]}
do
    echo "Generating with batch size : $batch_size, Number of batches : $BATCH_NUM"

    # Set the paths to streaming dataset and snapshots
    STREAM_GRAPH="../../../../../data2/ztguan/graphbolt/CA_roadnet/${batch_size}/stream_graph.txt"
    SNAPSHOT_PATH="../../../../../data2/ztguan/graphbolt/CA_roadnet/${batch_size}/"

    # Make directory if it doesn't exist
    if [ ! -d "../../../../../data2/ztguan/graphbolt/CA_roadnet/" ];
    then
        mkdir ../../../../../data2/ztguan/graphbolt/CA_roadnet/
    fi

    if [ ! -d "../../../../../data2/ztguan/graphbolt/CA_roadnet/$batch_size/" ];
    then
        mkdir ../../../../../data2/ztguan/graphbolt/CA_roadnet/$batch_size/
    fi

    # If there exists a initial graph and idle graph
    # -> set "-readfile" argument, to generate streams based on them
    if [ -f $INIT_GRAPH -a -f $IDLE_GRAPH ];
    then
        ./stream_generator -maxEdgeNum $MAX_EDGE_NUM -batchSize $batch_size -batchNum $BATCH_NUM \
        -initGraphFile $INIT_GRAPH -idleGraphFile $IDLE_GRAPH -streamGraphFile $STREAM_GRAPH \
        -snapshotPath $SNAPSHOT_PATH -readFile
    # If there is no initial graph or idle graph
    # -> don't set "-readfile" argument
    else
        ./stream_generator -maxEdgeNum $MAX_EDGE_NUM -batchSize $batch_size -batchNum $BATCH_NUM -basicGraphFile $BASIC_GRAPH \
        -initGraphFile $INIT_GRAPH -idleGraphFile $IDLE_GRAPH -streamGraphFile $STREAM_GRAPH \
        -snapshotPath $SNAPSHOT_PATH
    fi

    echo "Generated"
done


# Convert SNAP format to adjacency format
cd ../converters/
pwd
./SNAPtoAdjConverter ${INIT_GRAPH} ${INIT_GRAPH_FILE_ADJ}
for batch_size in ${BATCH_SIZE_ARRAY[@]}
do
    for batch_num in {1..10..1}
    do
        SNAP="../../../../../data2/ztguan/graphbolt/CA_roadnet/${batch_size}/snapshot_${batch_num}.snap"
        ADJ="../../../../../data2/ztguan/graphbolt/CA_roadnet/${batch_size}/snapshot_${batch_num}.adj"
        echo "Converting with batch size : ${batch_size}, batch number : ${batch_num}"
        ./SNAPtoAdjConverter $SNAP $ADJ
        echo "Converted"
    done
done