const int N = 1000010;
struct Node {
    ll sum = 0;
} tr[N * 4];

int a[N], in[N], out[N], leaf[N], to[N];
vector<int> g[N];
void update(int p) {
    tr[p].sum = tr[p * 2].sum  + tr[p * 2 + 1].sum;
}
void build(int p, int l, int r) {
    if (l == r) {
        leaf[l] = p;
        tr[p].sum = a[to[l]];
        return;
    }
    int mid = (l + r) >> 1;
    build(p * 2, l, mid);
    build(p * 2 + 1, mid + 1, r);
    update(p);
}
int query(int p, int l, int r, int ql, int qr) {
    if (ql == l && qr == r) {
        return tr[p].sum;
    }
    int mid = (l + r) >> 1;
    if (qr <= mid) return query(p * 2, l, mid, ql, qr);
    else if (ql > mid) return query(p * 2 + 1, mid + 1, r, ql, qr);
    else return query(p * 2, l, mid, ql, mid) + query(p * 2 + 1, mid + 1, r, mid + 1, qr);
}
void change(int p, int val) {
    tr[p].sum += val;
    while (p /= 2) tr[p].sum += val;
}
int tot;
void dfs(int u, int pre) {
    in[u] = ++tot;
    to[tot] = u;
    for (int v : g[u]) if (v != pre) dfs(v, u);
    out[u] = tot;
}

int main() {
    int n, m, rt; cin >> n >> m >> rt;
    for (int i = 1; i <= n; i++) cin >> a[i];
    for (int i = 0; i < n - 1; i++) {
        int u, v; cin >> u >> v;
        g[u].emplace_back(v);
        g[v].emplace_back(u);
    }
    dfs(rt, 0);
    build(1, 1, n);
    while (m--) {
        int o; cin >> o;
        int x; cin >> x;
        if (o == 1) {
            int v; cin >> v;
            change(leaf[in[x]], v);
        } else {
            cout << query(1, 1, n, in[x], out[x]) << '\n';
        }
    }
    return 0;
}