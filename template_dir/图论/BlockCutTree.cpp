struct BlockCutTree {
    int N;
    int block, tot;

    std::vector<std::vector<int>> edge;
    std::vector<int> dfn, low, stk;
    std::vector<std::vector<int>> T;
    
    BlockCutTree(int N) {
        this->N = N;
        edge.assign(N, {});
        dfn.assign(N, -1);
        low.resize(N);
        stk.clear();
        block = tot = 0;
        T.assign(N * 2, {});
    }
    
    void addEdge(int u, int v) {
        edge[u].push_back(v);
        edge[v].push_back(u);
    }
    
    void dfs(int u) {
        stk.push_back(u);
        dfn[u] = low[u] = tot++;
        
        for (auto v : edge[u]) {
            if (dfn[v] == -1) {
                dfs(v);
                low[u] = std::min(low[u], low[v]);
                if (low[v] == dfn[u]) {
                    int x;
                    do {
                        x = stk.back();
                        stk.pop_back();
                        T[N + block].push_back(x);
                        T[x].push_back(N + block);
                    } while (x != v);
                    T[u].push_back(N + block);
                    T[N + block].push_back(u);
                    block++;
                }
            } else {
                low[u] = std::min(low[u], dfn[v]);
            }
        }
    }
    
    std::pair<int, std::vector<std::vector<int>>> work() {
        for (int i = 0;i < N;i++) {
            if (dfn[i] == -1) {
                stk.clear();
                dfs(i);
            }
        }
        return {block, T};
    }
};