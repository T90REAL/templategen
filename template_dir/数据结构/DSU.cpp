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
      // if (sz[a] < sz[b]) swap(a, b);
      p[b] = a;
      sz[a] += sz[b];
      return true;
    }
    return false;
  }
};
