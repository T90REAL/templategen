// 普通莫队
sort(all(que), [&](array<int, 3> a, array<int, 3> b) {
  if (a[0] / B != b[0] / B) return a[0] / B < b[0] / B;
  return a[0] / B % 2 ? a[1] < b[1] : a[1] > b[1];
  // return a[1] < b[1];
});
auto add = [&](int pos) {};
auto del = [&](int pos) {};
vector<ll> ans(q);
int l = 1, r = 0;
for (auto [ql, qr, id] : que) {
  // 先扩后缩
  while (r < qr) add(++r);
  while (l > ql) add(--l);
  while (r > qr) del(r--);
  while (l < ql) del(l++);
  ans[id] = cur;
}

// Mo's algorithm on tree
vector<vector<int>> g(n + 1);
LCA lca(n + 1);
for (int i = 0; i < n - 1; i++) {
  int u, v;
  cin >> u >> v;
  g[u].emplace_back(v);
  g[v].emplace_back(u);
  lca.addEdge(u, v);
}

lca.init();
vector<int> es{0};
auto dfs = [&](auto dfs, int u, int pre) -> void {
  es.emplace_back(u);
  for (int v : g[u])
    if (v != pre) dfs(dfs, v, u);
  es.emplace_back(u);
};
dfs(dfs, 1, 0);

vector<int> L(n + 1), R(n + 1);
for (int i = 1; i < sz(es); i++) {
  if (!L[es[i]]) L[es[i]] = i;
}
for (int i = sz(es) - 1; i >= 1; i--) {
  if (!R[es[i]]) R[es[i]] = i;
}

vector<array<int, 5>> que(q);
for (int i = 0; i < q; i++) {
  int u, v;
  cin >> u >> v;
  if (L[u] > L[v]) swap(u, v);
  int l, r;
  if (lca.query(u, v) == u) {
    l = L[u];
    r = L[v];
  } else {
    l = R[u];
    r = L[v];
  }
  que[i] = {l, r, i, u, v};
}

sort(all(que), [&](array<int, 5> a, array<int, 5> b) {
  if (a[0] / B != b[0] / B) return a[0] / B < b[0] / B;
  return a[0] / B % 2 ? a[1] < b[1] : a[1] > b[1];
});

vector<int> use(n + 1);
vector<int> cnt(int(1e6) + 1);
int cur = 0;
auto add = [&](int p) {
  int u = es[p];
  if (use[u]) {
    if (--cnt[c[u]] == 0) --cur;
    use[u] = 0;
  } else {
    use[u] = 1;
    if (++cnt[c[u]] == 1) ++cur;
  }
};

int l = 1, r = 0;
vector<int> ans(q);
for (auto [ql, qr, id, u, v] : que) {
  // work
  while (r < qr) add(++r);
  while (l > ql) add(--l);
  while (r > qr) add(r--);
  while (l < ql) add(l++);
  int fa = lca.query(u, v);

  if (fa != u && fa != v) {
    // extra point
    add(L[fa]);
  }
  ans[id] = cur;

  if (fa != u && fa != v) {
    // extra point
    add(L[fa]);
  }
}