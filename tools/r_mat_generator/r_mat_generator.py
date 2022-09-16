import snap

Rnd = snap.TRnd()
Graph = snap.GenRMat(1000, 2000, .6, .1, .15, Rnd)
for EI in Graph.Edges():
    print("edge: (%d, %d)" % (EI.GetSrcNId(), EI.GetDstNId()))