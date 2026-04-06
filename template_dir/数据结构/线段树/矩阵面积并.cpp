typedef long long ll;

const int N = 200010;
struct info {
    int minv, mincnt;
    info operator + (const info &other) {
        int cnt = mincnt;
        if (minv == other.minv)
            cnt += other.mincnt;
        else if (minv > other.minv)
            cnt = other.mincnt;
        return {min(minv, other.minv), cnt};
    }
};
struct node {
    int t;
    info val;
} tr[8 * N];

vector<int> vx;

void update(int p) {
    tr[p].val = tr[p * 2].val + tr[p * 2 + 1].val;
}

void build(int p, int l, int r) {
    if (l == r) {
        tr[p] = {0, {0, vx[r] - vx[r - 1]}};
        return;
    }
    int mid = (l + r) >> 1;
    build(p * 2, l, mid);
    build(p * 2 + 1, mid + 1, r);
    update(p);
}

void settag(int p, int t) {
    tr[p].val.minv = tr[p].val.minv + t;
    tr[p].t += t;
}

void pushdown(int p) {
    if (tr[p].t != 0) {
        settag(p * 2, tr[p].t);
        settag(p * 2 + 1, tr[p].t);
        tr[p].t = 0;
    }
}

void modify(int p, int l, int r, int ql, int qr, int t) {
    if (ql == l && qr == r) {
        settag(p, t);
        return;
    }
    pushdown(p);
    int mid = (l + r) >> 1;
    if (qr <= mid) modify(p * 2, l, mid, ql, qr, t);
    else if (ql > mid) modify(p * 2 + 1, mid + 1, r, ql, qr, t);
    else {
        modify(p * 2, l, mid, ql, mid, t);
        modify(p * 2 + 1, mid + 1, r, mid + 1, qr, t);
    }
    update(p);
}

int main() {
    ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL);
    vector<array<int, 4>> evt;
    for (int i = 1; i <= n; i++) {
        // y, op, x1, x2
        vx.push_back(i);
        vx.push_back(L[i] - 1);
        evt.push_back({i - 1, 1, L[i] - 1, i});
        evt.push_back({R[i], -1, L[i] - 1, i});
    }
    sort(all(evt));
    sort(all(vx));
    vx.erase(unique(all(vx)), vx.end());
    int m = sz(vx) - 1;
    build(1, 1, m);
    int prey = 0;
    int len = tr[1].val.mincnt;
    ll ans = 0;
    for (auto [y, tag, x1, x2] : evt) {
        int cov = len;
        if (tr[1].val.minv == 0) cov -= tr[1].val.mincnt;
        ans += 1ll * (y - prey) * cov;
        prey = y;
        int ql = lower_bound(all(vx), x1) - vx.begin() + 1;
        int qr = lower_bound(all(vx), x2) - vx.begin();
        if (ql > qr) continue;
        modify(1, 1, m, ql, qr, tag);
    };
    cout << ans << '\n';

    return 0;
}
