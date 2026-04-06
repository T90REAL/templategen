const int N = 10010;
struct Tag {
    ll mul = 1, add = 0;
    void clear() {
        mul = 1;
        add = 0;
    }
};
Tag operator + (Tag &a, Tag &b) {
    return {a.mul * b.mul, a.add * b.mul + b.add};
}
struct Node {
    ll sum[2];
    int l, r;
    Tag tag;
} tr[N * 4];

int a[N];

void update(int p) {
    tr[p].sum[0] = tr[p * 2].sum[0] + tr[p * 2 + 1].sum[0];
    tr[p].sum[1] = tr[p * 2].sum[1] + tr[p * 2 + 1].sum[1];
}

void build(int p, int l, int r) {
    tr[p].l = l;
    tr[p].r = r;
    if (l == r) {
        tr[p] = {{a[l], 1ll * a[l] * a[l]}, l, r, {1, 0}};
        return;
    }
    int mid = (l + r) >> 1;
    build(p * 2, l, mid);
    build(p * 2 + 1, mid + 1, r);
    update(p);
}

void setTag(int p, Tag t) {
    tr[p].sum[1] = t.mul * t.mul * tr[p].sum[1] + 2 * t.mul * t.add * tr[p].sum[0] + t.add * t.add * (tr[p].r - tr[p].l + 1);
    tr[p].sum[0] = tr[p].sum[0] * t.mul + t.add * (tr[p].r - tr[p].l + 1);
    tr[p].tag = tr[p].tag + t;
}

void pushdown(int p) {
    if (!(tr[p].tag.mul == 1 && tr[p].tag.add == 0)) {
        setTag(p * 2, tr[p].tag);
        setTag(p * 2 + 1, tr[p].tag);
        tr[p].tag.clear();
    }
}

void modify(int p, int l, int r, int ql, int qr, Tag t) {
    if (ql == l && qr == r) {
        setTag(p, t);
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

ll query(int p, int l, int r, int ql, int qr) {
    if (ql == l && qr == r) {
        return tr[p].sum[0];
    }
    pushdown(p);
    int mid = (l + r) >> 1;
    if (qr <= mid) return query(p * 2, l, mid, ql, qr);
    else if (ql > mid) return query(p * 2 + 1, mid + 1, r, ql, qr);
    else return query(p * 2, l, mid, ql, mid) + query(p * 2 + 1, mid + 1, r, mid + 1, qr);
    update(p);
}

template<class Info>
struct SegmentTree {
    int n;
    std::vector<Info> info;
    SegmentTree() : n(0) {}
    SegmentTree(int n_, Info v_ = Info()) {
        init(n_, v_);
    }
    template<class T>
    SegmentTree(std::vector<T> init_) {
        init(init_);
    }
    void init(int n_, Info v_ = Info()) {
        init(std::vector(n_, v_));
    }
    template<class T>
    void init(std::vector<T> init_) {
        n = init_.size();
        info.assign(4 << std::__lg(n), Info());
        std::function<void(int, int, int)> build = [&](int p, int l, int r) {
            if (r - l == 1) {
                info[p] = init_[l];
                return;
            }
            int m = (l + r) / 2;
            build(2 * p, l, m);
            build(2 * p + 1, m, r);
            pull(p);
        };
        build(1, 0, n);
    }
    void pull(int p) {
        info[p] = info[2 * p] + info[2 * p + 1];
    }
    void modify(int p, int l, int r, int x, const Info &v) {
        if (r - l == 1) {
            info[p] = v;
            return;
        }
        int m = (l + r) / 2;
        if (x < m) {
            modify(2 * p, l, m, x, v);
        } else {
            modify(2 * p + 1, m, r, x, v);
        }
        pull(p);
    }
    void modify(int p, const Info &v) {
        modify(1, 0, n, p, v);
    }
    Info rangeQuery(int p, int l, int r, int x, int y) {
        if (l >= y || r <= x) {
            return Info();
        }
        if (l >= x && r <= y) {
            return info[p];
        }
        int m = (l + r) / 2;
        return rangeQuery(2 * p, l, m, x, y) + rangeQuery(2 * p + 1, m, r, x, y);
    }
    Info rangeQuery(int l, int r) {
        return rangeQuery(1, 0, n, l, r);
    }
    template<class F>
    int findFirst(int p, int l, int r, int x, int y, F pred) {
        if (l >= y || r <= x || !pred(info[p])) {
            return -1;
        }
        if (r - l == 1) {
            return l;
        }
        int m = (l + r) / 2;
        int res = findFirst(2 * p, l, m, x, y, pred);
        if (res == -1) {
            res = findFirst(2 * p + 1, m, r, x, y, pred);
        }
        return res;
    }
    template<class F>
    int findFirst(int l, int r, F pred) {
        return findFirst(1, 0, n, l, r, pred);
    }
    template<class F>
    int findLast(int p, int l, int r, int x, int y, F pred) {
        if (l >= y || r <= x || !pred(info[p])) {
            return -1;
        }
        if (r - l == 1) {
            return l;
        }
        int m = (l + r) / 2;
        int res = findLast(2 * p + 1, m, r, x, y, pred);
        if (res == -1) {
            res = findLast(2 * p, l, m, x, y, pred);
        }
        return res;
    }
    template<class F>
    int findLast(int l, int r, F pred) {
        return findLast(1, 0, n, l, r, pred);
    }
};
struct Info {
    ll maxSum = -inf;
    ll sum = -inf, maxPre = -inf, maxSuf = -inf;
};
Info operator+(Info a, Info b) {
    Info c;
    c.sum = a.sum + b.sum;
    c.maxPre = max(a.maxPre, a.sum + b.maxPre);
    c.maxSuf = max(b.maxSuf, b.sum + a.maxSuf);
    c.maxSum = max({a.maxSum, b.maxSum, a.maxSuf + b.maxPre});
    return c;
}