MAX_EDGE_NUM=5533214
BATCH_SIZE_ARRAY=(1000 10000 100000 1000000)
BATCH_NUM=10
BASIC_GRAPH="../../../../../data2/ztguan/graphbolt/roadNet-CA.txt"
INIT_GRAPH="../../../../../data2/ztguan/graphbolt/CA_roadnet/init_graph.snap"
IDLE_GRAPH="../../../../../data2/ztguan/graphbolt/CA_roadnet/idle_graph.snap"
INIT_GRAPH_FILE_ADJ="../../../../../data2/ztguan/graphbolt/CA_roadnet/init_graph.adj"

for batch_size in ${BATCH_SIZE_ARRAY[@]}
do
    # batch_size=10000
    echo "Generating with batch size : $batch_size, Number of batches : $BATCH_NUM"

    STREAM_GRAPH="../../../../../data2/ztguan/graphbolt/CA_roadnet/${batch_size}/stream_graph.txt"
    SNAPSHOT_PATH="../../../../../data2/ztguan/graphbolt/CA_roadnet/${batch_size}/"

    if [ ! -d "../../../../../data2/ztguan/graphbolt/CA_roadnet/" ];
    then
        mkdir ../../../../../data2/ztguan/graphbolt/CA_roadnet/
    fi

    if [ ! -d "../../../../../data2/ztguan/graphbolt/CA_roadnet/$batch_size/" ];
    then
        mkdir ../../../../../data2/ztguan/graphbolt/CA_roadnet/$batch_size/
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
./SNAPtoAdjConverter ${INIT_GRAPH} ${INIT_GRAPH_FILE_ADJ}
for batch_size in ${BATCH_SIZE_ARRAY[@]}
do
    # batch_size=10000
    for batch_num in {1..10..1}
    do
        SNAP="../../../../../data2/ztguan/graphbolt/CA_roadnet/${batch_size}/snapshot_${batch_num}.snap"
        ADJ="../../../../../data2/ztguan/graphbolt/CA_roadnet/${batch_size}/snapshot_${batch_num}.adj"
        echo "Converting with batch size : ${batch_size}, batch number : ${batch_num}"
        ./SNAPtoAdjConverter $SNAP $ADJ
        echo "Converted"
    done
done