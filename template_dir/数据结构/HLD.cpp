const int N = 30010;
struct Node {
    int maxVal = -INT_MAX, sum = 0;
} tr[4 * N];
Node operator + (const Node &L, const Node &R) {
    return {max(L.maxVal, R.maxVal), L.sum + R.sum};
};

int a[N], n;
int l[N], r[N], id[N];
int fa[N], sz[N], dep[N], top[N], hs[N];
vector<int> adj[N];

void update(int p) {
    tr[p] = tr[p * 2] + tr[p * 2 + 1];
}
void build(int p, int l, int r) {
    if (l == r) {
        tr[p] = {a[id[l]], a[id[l]]};
        return;
    }

    int mid = (l + r) >> 1;
    build(p * 2, l, mid);
    build(p * 2 + 1, mid + 1, r);
    update(p);
}
void change(int p, int l, int r, int pos, int val) {
    if (l == r) {
        tr[p].maxVal = tr[p].sum = val;
        return;
    }

    int mid = (l + r) >> 1;

    if (pos <= mid)
        change(p * 2, l, mid, pos, val);
    else
        change(p * 2 + 1, mid + 1, r, pos, val);

    update(p);
}
Node query(int p, int l, int r, int ql, int qr) {
    if (ql == l and qr == r) {
        return tr[p];
    }

    int mid = (l + r) >> 1;

    if (qr <= mid)
        return query(p * 2, l, mid, ql, qr);
    else if (ql > mid)
        return query(p * 2 + 1, mid + 1, r, ql, qr);
    else
        return query(p * 2, l, mid, ql, mid) + query(p * 2 + 1, mid + 1, r, mid + 1, qr);
}
Node query(int u, int v) {
    Node ans;

    while (top[u] != top[v]) {
        if (dep[top[u]] > dep[top[v]])
            swap(u, v);

        ans = ans + query(1, 1, n, l[top[v]], l[v]);
        v = fa[top[v]];
    }

    if (dep[u] <= dep[v])
        ans = ans + query(1, 1, n, l[u], l[v]);
    else
        ans = ans + query(1, 1, n, l[v], l[u]);

    return ans;
}
int tot;
void dfs1(int u, int pre) {
    fa[u] = pre;
    sz[u] = 1;
    hs[u] = -1;
    dep[u] = dep[pre] + 1;

    for (int v : adj[u])
        if (v != pre) {
            dfs1(v, u);

            if (hs[u] == -1 or sz[v] > sz[hs[u]]) {
                hs[u] = v;
            }

            sz[u] += sz[v];
        }
}
void dfs2(int u, int head) {
    l[u] = ++tot;
    id[tot] = u;
    top[u] = head;

    if (hs[u] != -1) {
        dfs2(hs[u], head);
    }

    for (int v : adj[u])
        if (v != fa[u] and v != hs[u]) {
            dfs2(v, v);
        }

    r[u] = tot;
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    cin >> n;

    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    for (int i = 1; i <= n; i++) {
        cin >> a[i];
    }

    dfs1(1, 0);
    dfs2(1, 1);

    build(1, 1, n);

    int q;
    cin >> q;

    while (q--) {
        string que;
        cin >> que;
        int u, v;
        cin >> u >> v;

        if (que[0] == 'C') {
            change(1, 1, n, l[u], v);
        } else {
            Node ans = query(u, v);

            if (que[1] == 'S') {
                cout << ans.sum << '\n';
            } else {
                cout << ans.maxVal << '\n';
            }
        }
    }

    return 0;
}