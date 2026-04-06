// 0_index base
// add edge by making sure the CNF such as x_1 and x_2 and x_3.... (x_i should be format as (a or b))
// a xor b <=> (a or b) and (-a or -b) 
// -(a xor b) <=> (-a or b) and (a or -b)
// https://cp-algorithms.com/graph/2SAT.html

struct twosat {
    #define VI vector<int>
    #define pb push_back
    #define rep(i, a, b) for (int i = a; i < b; i++)
    int tot;
    VI ans;
    vector<VI> edge, edge_t;
    twosat(int n) {
        tot = 2*n;
        ans.resize(n);
        edge.resize(2*n), edge_t.resize(2*n);
    }
    void either(int a, bool na, int b, bool nb) {
        a = 2*a ^ na; b = 2*b ^ nb;
        int neg_a = a^1; int neg_b = b^1;
        edge[neg_a].pb(b); edge[neg_b].pb(a);
        edge_t[b].pb(neg_a); edge_t[a].pb(neg_b);
    }
    void add_edge(int u, int v) {
        edge[u].pb(v);
        edge_t[v].pb(u);
    }
    VI valid() {
        VI order, vis(tot), comp(tot, -1);
        function<void(int)> dfs1 = [&](int u) {
            vis[u] = 1;
            for (int v : edge[u]) if (!vis[v]) dfs1(v);
            order.pb(u);
        };
        rep(i, 0, tot) if (!vis[i]) dfs1(i);
        function<void(int, int)> dfs2 = [&](int u, int cnt) {
            comp[u] = cnt;
            for (int v : edge_t[u]) if (comp[v] == -1) dfs2(v, cnt);
        };
        int cnt = 0;
        rep(i, 0, tot) if (comp[order[tot-i-1]] == -1) dfs2(order[tot-i-1], cnt++);
        for (int i = 0; i < 2 * (int) ans.size(); i += 2) {
            if (comp[i] == comp[i+1]) return {};
            ans[i/2] = comp[i] > comp[i+1];
        }
        return ans;
    }
};

int tot = 2*m;
vector<VI> edge(tot), edge_t(tot);
auto add_edge = [&](int a, bool na, int b, bool nb) {
    a = 2*a ^ na; b = 2*b ^ nb;
    int neg_a = a^1, neg_b = b^1;
    edge[neg_a].pb(b); edge[neg_b].pb(a);
    edge_t[b].pb(neg_a); edge_t[a].pb(neg_b);
};
// any solution
VI ans(m);
auto twosat = [&]() {
    VI order, vis(tot), comp(tot, -1);
    function<void(int)> dfs1 = [&](int u) {
        vis[u] = 1;
        for (int v : edge[u]) if (!vis[v]) dfs1(v);
        order.pb(u);
    };
    rep(i, 0, tot) if (!vis[i]) dfs1(i);
    function<void(int, int)> dfs2 = [&](int u, int cnt) {
        comp[u] = cnt;
        for (int v : edge_t[u]) if (comp[v] == -1) dfs2(v, cnt);
    };
    int cnt = 0;
    rep(i, 0, tot) if (comp[order[tot-i-1]] == -1) dfs2(order[tot-i-1], cnt++);
    for (int i = 0; i < tot; i += 2) {
        if (comp[i] == comp[i+1]) return false;
        ans[i/2] = comp[i] > comp[i+1];
    }
    return true;
};

// unique solution
VI ans(tot);
auto twosat = [&]() {
    VI order, vis(tot), comp(tot, -1);
    function<void(int)> dfs1 = [&](int u) {
        vis[u] = 1;
        for (int v : edge[u]) if (!vis[v]) dfs1(v);
        order.pb(u);
    };
    rep(i, 0, tot) if (!vis[i]) dfs1(i);
    function<void(int, int)> dfs2 = [&](int u, int cnt) {
        comp[u] = cnt;
        for (int v : edge_t[u]) if (comp[v] == -1) dfs2(v, cnt);
    };
    int cnt = 0;
    rep(i, 0, tot) if (comp[order[tot-i-1]] == -1) dfs2(order[tot-i-1], cnt++);
    VI stk;
    function<bool(int)> paint = [&](int u) {
        if (ans[u] == 2) return false;
        if (ans[u] == 1) return true;
        stk.pb(u); ans[u] = 1, ans[u^1] = 2;
        for (int v : edge[u]) if (!paint(v)) return false;
        return true;
    };
    for (int i = 0; i < tot; i += 2) {
        if (ans[i]) continue;
        stk.clear();
        if (!paint(i)) {
            while (!stk.empty()) ans[stk.back()] = ans[stk.back()^1] = 0, stk.pop_back();
            if (!paint(i^1)) return false;
        }
    }
    return true;
};
