template<typename T> struct SegmentTree {
    struct Node {
        int l, r;
        T minVal = -1;
    };
    int n;
    vector<int> leaf, index;
    vector<Node> tr;
    SegmentTree(vector<T>& a) {
        n = a.size() - 1;
        tr.resize(4 * (n + 1), {0});
        index.resize(4 * (n + 1), 0);
        leaf.resize(n + 1, 0);

        auto build = [&](auto build, int p, int l, int r) {
            tr[p].l = l;
            tr[p].r = r;
            if (l == r) {
                tr[p].l = l;
                tr[p].r = r;
                tr[p].minVal = a[l];
                leaf[l] = p;
                index[p] = l;

                return;
            }
            int mid = (l + r) >> 1;
            build(build, p * 2, l, mid);
            build(build, p * 2 + 1, mid + 1, r);
            update(p);
        }; build(build, 1, 0, n);
    }
    void update(int p) {
        tr[p].minVal = min(tr[p * 2].minVal, tr[p * 2 + 1].minVal);
    }
    void change(int p, int val) {
        p = leaf[p];
        tr[p].minVal = val;
        while (p /= 2) tr[p].minVal = min(tr[p * 2].minVal, tr[p * 2 + 1].minVal);
    }
    int query(int p, int l, int r, int ql, int qr) {
        if (ql == l && qr == r) {
            return tr[p].minVal;
        }
        int mid = (l + r) >> 1;
        if (qr <= mid) return query(p * 2, l, mid, ql, qr);
        else if (ql > mid) return query(p * 2 + 1, mid + 1, r, ql, qr);
        else return min(query(p * 2, l, mid, ql, mid), query(p * 2 + 1, mid + 1, r, mid + 1, qr));
    }
    int gao(int L) {
        int p = 1;
        while (tr[p].l != tr[p].r) {
            if (tr[p * 2].minVal < L) p = p * 2;
            else if (tr[p * 2 + 1].minVal < L) p = p * 2 + 1;
            else return tr[p].r + 1;
        }

        return index[p];
    }
};
const int N = 300010;
int main() {
    int n, q; cin >> n >> q;
    vector<int> a(n + 1);
    int idx = 1e9;
    vector<int> st;
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        if (a[i] > 300000) {
            st.push_back(a[i]);
            setmin(idx, a[i]);
        }
    }
    sort(all(st));
    a.erase(unique(all(st)), st.end());
    for (int i = 1; i <= n; i++) if (a[i] > 300000) a[i] = idx + (lower_bound(all(st), a[i]) - st.begin());

    vector<int> t(N, -1);
    SegmentTree sg = t;

    vector<vector<pii>> que(n + 1);
    for (int i = 0; i < q; i++) {
        int l, r; cin >> l >> r;
        que[r].emplace_back(l, i);
    }
    vector<int> ans(q);
    vector<int> lst(N * 2, -1), pre(N * 2, -1);
    for (int r = 1; r <= n; r++) {
        pre[a[r]] = r;
        lst[a[r]] = pre[a[r]];
        sg.change(a[r], lst[a[r]]);
        for (auto [l, id] : que[r]) ans[id] = sg.gao(l);
    }

    for (int a : ans) cout << a << '\n';

    return 0;
}