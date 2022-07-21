MAX_EDGE_NUM=1468364884
BATCH_SIZE_ARRAY=(10000 100000 1000000)
BATCH_NUM=10
BASIC_GRAPH="../../inputs/twitter-2010.txt"
INIT_GRAPH="../../inputs/20220701/init_graph.snap"
IDLE_GRAPH="../../inputs/20220701/idle_graph.snap"
INIT_GRAPH_FILE_ADJ="../../inputs/20220701/init_graph.adj"

for batch_size in ${BATCH_SIZE_ARRAY[@]}
do
    # batch_size=10000
    echo "Generating with batch size : $batch_size, Number of batches : $BATCH_NUM"

    STREAM_GRAPH="../../inputs/20220701/${batch_size}/stream_graph.txt"
    SNAPSHOT_PATH="../../inputs/20220701/${batch_size}/"

    if [ ! -d "../../inputs/20220701/" ];
    then
        mkdir ../../inputs/20220701/
    fi

    if [ ! -d "../../inputs/20220701/$batch_size/" ];
    then
        mkdir ../../inputs/20220701/$batch_size/
    fi

    if [ -f $INIT_GRAPH -a -f $IDLE_GRAPH ];
    then
        ./stream_generator -maxEdgeNum $MAX_EDGE_NUM -batchSize $batch_size -batchNum $BATCH_NUM \
        -initGraphFile $INIT_GRAPH -idleGraphFile $IDLE_GRAPH -streamGraphFile $STREAM_GRAPH \
        -snapshotPath $SNAPSHOT_PATH -readFile
    else
        ./stream_generator -maxEdgeNum $MAX_EDGE_NUM -batchSize $batch_size -batchNum $BATCH_NUM -basicGraphFile $BASIC_GRAPH \
        -initGraphFile $INIT_GRAPH -idleGraphFile $IDLE_GRAPH -streamGraphFile $STREAM_GRAPH \
        -snapshotPath $SNAPSHOT_PATH
    fi

    echo "Generated"
done

cd ../converters/
pwd
# ./SNAPtoAdjConverter ${INIT_GRAPH_FILE} ${INIT_GRAPH_FILE_ADJ}
for batch_size in ${BATCH_SIZE_ARRAY[@]}
do
    # batch_size=10000
    for batch_num in {1..10..1}
    do
        SNAP="../../inputs/20220701/${batch_size}/snapshot_${batch_num}.snap"
        ADJ="../../inputs/20220701/${batch_size}/snapshot_${batch_num}.adj"
        echo "Converting with batch size : ${batch_size}, batch number : $batch_num"
        ./SNAPtoAdjConverter $SNAP $ADJ
        echo "Converted"
    done
done