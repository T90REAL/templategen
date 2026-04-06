#include <bits/stdc++.h>
using namespace std;
#define fi first
#define se second
#define vv vector
#define pb push_back
#define mp make_pair
#define sz(a) ((int)a.size())
#define all(a) a.begin(), a.end()
#define rp(i, r) for (int i = 0; i < r; ++i)
#define pr(i, r) for (int i = r-1; i >= 0; --i)
#define rep(i, l, r) for (int i = l; i < r; ++i)
#define per(i, r, l) for (int i = r-1; i >= l; --i)

typedef double db;
typedef long long ll;
typedef pair<int, int> PI;
template<typename T> bool setmax(T &a, T b) { return (a < b ? a = b, 1 : 0); }
template<typename T> bool setmin(T &a, T b) { return (a > b ? a = b, 1 : 0); }

struct DSU {
    int n;
    vector<int> p, sz;
    DSU(int _n) {
        n = _n;
        p.resize(n);
        sz.resize(n, 1);
        iota(all(p), 0);
    }
    int leader(int u) { return u == p[u] ? u : p[u] = leader(p[u]); }
    bool merge(int u, int v) {
        int a = leader(u), b = leader(v);
        if (a != b) {
            if (sz[a] < sz[b]) swap(a, b);
            p[b] = a;
            sz[a] += sz[b];
            return true;
        }
        return false;
    }
};

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
    ST<PI> st;
    int n, tot, rt = 1;
    vector<int> to, dfn, dep, p;
    vector<vector<int>> g;
    vector<PI> es{{-1, -1}};
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

int A,B,C,P;
inline int rnd(){return A=(A*B+C)%P;}
const int mod = int(1e9) + 7;

void solve() {
    int n, m; cin >> n >> m;
    vector<array<int, 3>> edge(m);
    rp(i, m) cin >> edge[i][1] >> edge[i][2] >> edge[i][0];
    sort(all(edge));

    int tot = 2 * n;
    int curNode = n + 1;
    vector<int> weight(tot + 1);
    DSU dsu(tot + 1);
    LCA tree(tot + 1);
    rp(i, m) {
        int u = edge[i][1], v = edge[i][2], w = edge[i][0];
        int pu = dsu.leader(u), pv = dsu.leader(v);
        if (dsu.merge(u, v)) {
            tree.addEdge(curNode, pu);
            tree.addEdge(curNode, pv);
            weight[curNode] = w;
            dsu.p[pu] = dsu.p[pv] = curNode;
            curNode++;
        }
    }

    tree.setRoot(tot - 1);
    tree.init();

    ll ans = 0;
    int q; cin >> q;
    while (q--) {
        cin >> A >> B >> C >> P;
        int u = rnd() % n + 1, v = rnd() % n + 1;
        if (u == v) continue;
        ans = (ans + weight[tree.query(u, v)]) % mod;
    }

    cout << ans << '\n';
}


int main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);

    int tt = 1;
    // cin >> tt;
    while (tt--) solve();

    return 0;
}
