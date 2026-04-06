// 1-index
template<typename T> struct BIT {
    int n;
    vector<T> tr;
    BIT(int _n) {
        n = _n;
        tr.resize(n+1);
    }
    T prefix(int i) {
        T res = 0;
        for (; i; i -= i&(-i)) res += tr[i];
        return res;
    }
    T query(int l, int r) { return prefix(r) - prefix(l-1); }
    void modify(int i, T val) { for (; i <= n; i += i&(-i)) tr[i] += val; }
};

// 0-index (faster construction)
template<typename T> struct BIT {
    int n;
    vector<T> tr;
    BIT(int _n) { n = _n; tr.resize(n); }
    BIT(vector<T> const &a) : BIT(a.size()) { rep(i, 0, (int) a.size()) add(i, a[i]); }
    T prefix(int r) {
        T res = 0;
        for (; r >= 0; r = (r & (r+1))-1) res += tr[r];
        return res;
    }
    T query(int l, int r) { 
        assert(l <= r);
        if (!l) return prefix(r);
        return prefix(r) - prefix(l-1); 
    }
    void add(int i, T val) { for (; i < n; i = i | (i+1)) tr[i] += val; }
};

// Range query
template<typename T> struct BIT {
    private:
    int n;
    vector<T> tr1, tr2;
    void add(vector<T> &tr, int i, T x) { for (; i <= n; i += i&(-i)) tr[i] += x; }
    T sum(vector<T> &tr, int i) {
        T res = 0;
        for (; i; i -= i&(-i)) res += tr[i];
        return res;
    }
    public:
    BIT(int _n) {
        n = _n;
        tr1.resize(n+2);
        tr2.resize(n+2);
    }
    void range_add(int l, int r, T x) {
        add(tr1, l, x); add(tr1, r+1, -x);
        add(tr2, l, x*(l-1)); add(tr2, r+1, -x*r);
    }
    T prefix(int i) { return sum(tr1, i) * i - sum(tr2, i); }
    T range_sum(int l, int r) { return prefix(r) - prefix(l-1); }
};

// Fenwick2D
template<typename T> struct fenwick2D {
    int n, m;
    vector<vector<T>> tr;
    fenwick2D(int _n, int _m) {
        n = _n; m = _m;
        tr.resize(n+1, vector<T>(m+1));
    }
    T query(int x, int y) {
        T res = 0;
        for (int i = x; i; i -= (i&-i))
            for (int j = y; j; j -= (j&-j))
                res += tr[i][j];
        return res;
    }
    T query(int x1, int y1, int x2, int y2) {
        return query(x2, y2) - query(x2, y1-1) - query(x1-1, y2) + query(x1-1, y1-1);
    }
    void add(int x, int y, T val) {
        for (int i = x; i <= n; i += (i&-i))
            for (int j = y; j <= m; j += (j&-j))
                tr[i][j] += val;
    }
};
