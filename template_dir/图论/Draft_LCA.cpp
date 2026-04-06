    vector<vector<int>> adj(n+1);
    for(int i = 0; i < n-1; i++){
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    vector<array<int, 22>> fa(n+1);
    vector<int> dep(n+1);
    vector<int> sum(n+1);
    auto dfs = [&](auto dfs, int u, int pre)->void{
        fa[u][0] = pre;
        dep[u] = dep[pre]+1;
        sum[u] = sum[pre] + c[u];
        for(int v: adj[u]) if(v != pre){
            dfs(dfs, v, u);
        }
    }; dfs(dfs, 1, 0);
    for(int j = 1; j < 22; j++){
        for(int i = 1; i <= n; i++){
            fa[i][j] = fa[fa[i][j-1]][j-1];
        }
    }
    auto lca = [&](int u, int v){
        if(dep[u] > dep[v]) swap(u, v);
        int d = dep[v] - dep[u];
        for(int i = 0; i < 22; i++) if(d >> i & 1){
            v = fa[v][i];
        }
        if(u == v) return u;
        for(int j = 21; j >= 0; j--) if(fa[u][j] != fa[v][j]) {
            u = fa[u][j];
            v = fa[v][j];
        }
        if(u != v) u = fa[u][0];
        return u;
    };