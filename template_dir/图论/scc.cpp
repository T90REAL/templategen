// Tarjan
// 1-index
// [1, n] && [1, idx] && [1, cnt]

VI bel(n + 1), low(n + 1), dfn(n + 1), ins(n + 1), stk;
vector<VI> scc{{}};
int cnt = 0, idx = 0;
function<void(int)> dfs = [&](int u) {
  dfn[u] = low[u] = ++cnt;
  stk.pb(u);
  ins[u] = 1;

  for (int v : edge[u]) {
    if (!dfn[v]) {
      dfs(v);
      low[u] = min(low[u], low[v]);
    } else if (ins[v]) {
      low[u] = min(low[u], dfn[v]);
    }
  }

  if (dfn[u] == low[u]) {
    VI cur;
    ++idx;
    while (1) {
      int t = stk.back();
      stk.pop_back();
      ins[t] = 0;
      bel[t] = idx;
      cur.pb(t);
      if (t == u) break;
    }
    scc.pb(cur);
  }
};
rep(i, 1, n + 1) if (!bel[i]) dfs(i);

// Kosaraju
// 0-index
// Proved:
// https://cp-algorithms.com/graph/strongly-connected-components.html#description-of-the-algorithm
VI order, vis(n), comp(n, -1);
function<void(int)> dfs1 = [&](int u) {
  vis[u] = 1;
  for (int v : edge[u])
    if (!vis[v]) dfs1(v);
  order.pb(u);
};
rep(i, 0, n) if (!vis[i]) dfs1(i);
function<void(int, int)> dfs2 = [&](int u, int cnt) {
  comp[u] = cnt;
  for (int v : edge_t[u])
    if (comp[v] == -1) dfs2(v, cnt);
};
int cnt = 0;
rep(i, 0, n) if (comp[order[n - i - 1]] == -1) dfs2(order[n - i - 1], cnt++);