// O(NlogN) for building
// O(1) for each query
template<typename T> struct ST {
    int n;
    vector<vector<T>> st;
    ST() { }
    void load(vector<T>& a) {
        n = (int) a.size();
        st.resize(lg(n)+1, vector<T>(n+1));
        rep(i, 0, n) st[0][i] = a[i];
        rep(i, 1, lg(n)+1) for (int j = 0; j + (1<<i)-1 < n; j++) st[i][j] = min(st[i-1][j], st[i-1][j + (1<<(i-1))]);
    }
    T query(int l, int r) {
        assert(r >= l);
        int s = lg(r - l + 1);
        return min(st[s][l], st[s][r - (1<<s)+1]);
    }
    // pre C++20
    int lg(unsigned long long i) {
        return i ? __builtin_clzll(1) - __builtin_clzll(i) : -1;
    }
};
struct LCA {
    ST<pii> st;
    int n, tot, rt = 1;
    vector<int> to, dfn, dep, p;
    vector<vector<int>> g;
    vector<pii> es{{-1, -1}};
    LCA(int _n) {
        n = _n;
        tot = 0;
        g.resize(n + 1);
        p.resize(n + 1);
        to.resize(n + 1);
        dfn.resize(n + 1);
        dep.resize(n + 1);
    };
    // TODO: Set root of the tree
    void setRoot(int _rt) {
        rt = _rt;
    }
    void init() {
        auto dfs = [&](auto dfs, int u, int pre) -> void {
            dep[u] = dep[pre] + 1;
            dfn[u] = ++tot;
            to[dfn[u]] = u;
            es.push_back({dep[u], dfn[u]});
            for (int v : g[u]) if (v != pre) {
                dfs(dfs, v, u);
                es.push_back({dep[u], dfn[u]});
            }
        }; dfs(dfs, rt, 0);

        for (int i = 1; i < sz(es); i++) {
            if (!p[es[i].se])
                p[es[i].se] = i;
        }
        st.load(es);
    }
    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }
    int query(int u, int v) {
        int l = p[dfn[u]], r = p[dfn[v]];
        if (l > r) swap(l, r);
        return to[st.query(l, r).se];
    }
};
