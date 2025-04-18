# Set and MST
## Program Output
Output for second part of quizn.air2:
```bash
Subset not fully connected
 SFO->BOS
 BOS->DEN
 SFO->LAX
 BOS->JFK
Total Cost: 1000
```

It seems that MIA is unreachable by a direct connection from any of the other airports in the subgraph. This means that the subset is not fully connected, hence the output of my program. However, it handled the rest of the subset fine.

## MST Function
```c++
void airline_system::prim_mst_subgraph(vector<string>& nodes, double& totalWeight, vector<pair<string, string>>& treeEdges) {
    int n = nodes.size();

    unordered_map<string, int> idx;

    for(int i = 0; i < n; i++) idx[nodes[i]] = i;

    //building subgraph
    vector<vector<pair<int, double>>> adj(n);
    for(int i = 0; i < n; i++) {
        auto it = flights.find(nodes[i]);
        if(it == flights.end()) continue; //not concerning current subgraph
        for (auto &f : it->second) {
            auto jt = idx.find(f.destination);
            if(jt != idx.end()) { //making sure both ends are in subgraph
                adj[i].push_back({jt->second, f.cost});
            }
        }
    }

    set inMST;
    inMST += 0; //start at first node
    totalWeight = 0;

    while(inMST.size() < n) {
        double bestW = numeric_limits<double>::infinity();
        int bu=-1, bv=-1;
        //look for cheapest edge crossing the "cut" (one edge in and one edge not in current tree)
        for(int u = 0; u < n; u++) {
            if (!(u^inMST)) continue; //u not in MST
            for(auto &pr : adj[u]) {
                int v = pr.first;
                double w = pr.second;
                if(!(v^inMST) && w < bestW) {
                    bestW = w;
                    bu = u;
                    bv = v;
                }
            }
        }
        if(bu < 0) {  //disconnected
            cout << "Subset not fully connected" << endl;
            break;
        } 
        totalWeight += bestW;
        inMST += bu;
        inMST += bv;
        treeEdges.emplace_back(nodes[bu], nodes[bv]);
    }
}
```

Explanation: 
1. I make an `idx` map that maps airport codes to indices in the nodes list. This is just to speed up the process of checking if a specific flight is in the graph. Ideally, this would have been done with my self-made `set` class but that only holds integers. In reality, this doesn't end up speeding stuff up significantly because the subgraphs only have a few airports to work through. 
2. Next the subgraph is built, edges are added if and only if both airports on that edge are in the subgraph.
3. Then comes the Prim(ish) algorithm. (see footnote) At this point in the code is where I use the custom `set` class for the set of nodes currently in the MST.
    i. Stop in this step if we've reached a point where we cannot reach a node (subset is disconnected) or if the `inMST` set is the size of the number of nodes in the subgraph. 
    ii. Add the best edge to the `treeEdges` vector that will be used in main to print the tree.
4. Finally, main will print out the `treeEdges` vector.



## Footnote 
I say "ish" because technically this is not Prim's as Prim guarantees optimality for UNdirected graphs, while this is a directed graph. Technically what we've found here is an algorithm for a minimum spanning *arborescence*. The distinction is that trees are, by definition, undirected graphs in which every node has one and only one path to every other node. However, an arborescence is a directed acyclic graph in which one node (the root) has one and only one path to every other node (but not the non-root nodes to each other).