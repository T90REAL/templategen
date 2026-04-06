struct DOT {
    int n, tot = 0;
    vi a;
    vi L, R, id, sz, hs;
    vector<ll> ans;
    vector<vi> g;
    Trie trie;
    DOT(int _n, vector<int>& _a) {
        a = _a;
        n = _n;
        g.resize(n + 1);
        L.resize(n + 1, 0);
        R.resize(n + 1, 0);
        id.resize(n + 1, 0);
        sz.resize(n + 1, 0);
        hs.resize(n + 1, 0);
        ans.resize(n + 1, 0);
    }
    void addEdge(int u, int v) {
        g[u].emplace_back(v);
        g[v].emplace_back(u);
    }
    void dfs(int u, int pre) {
        L[u] = ++tot;
        id[tot] = u;
        sz[u] = 1;
        hs[u] = -1;
        for (int v : g[u]) if (v != pre) {
            dfs(v, u);
            sz[u] += sz[v];
            if (hs[u] == -1 || sz[v] > sz[hs[u]]) hs[u] = v;
        }
        R[u] = tot;
    }
    void dfs2(int u, int pre, bool keep) {
        // TODO: add ans
        auto add = [&](int p) {
            trie.insert(a[p]);
        };
        // TODO: del ans
        auto del = [&](int p) {
            trie.remove(a[p]);
        };

        for (int v : g[u]) if (v != pre && v != hs[u]) dfs2(v, u, 0);
        if (hs[u] != -1) dfs2(hs[u], u, 1);
        for (int v : g[u]) if (v != pre && v != hs[u]) {
            rep(x, L[v], R[v] + 1) add(id[x]);
        }
        add(u);

        // TODO: calculate answer
        if (sz[u] == 1) {
            // leaf
            ans[u] = -1;
        } else {
            ans[u] = trie.findMin(a[u]);
        }

        if (!keep) {
            rep(x, L[u], R[u] + 1) del(id[x]);
        }
    }
    void init() {
        dfs(1, 0);
        dfs2(1, 0, 0);
    }
};