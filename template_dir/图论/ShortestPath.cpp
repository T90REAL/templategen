namespace Dijstra {
template <class T>
vector<pair<bool, T> > max_path(const vector<vector<pair<int, T> > > &edges,
                                const int &s) {
  priority_queue<pair<T, int> > h;
  vector<pair<bool, T> > dis(edges.size(), {0, 0});
  dis[s] = {1, 0};
  h.push({0, s});
  while (!h.empty()) {
    auto u = h.top();
    h.pop();
    if (u.first != dis[u.second].second) continue;
    for (auto v : edges[u.second]) {
      pair<bool, T> w = {1, v.second + u.first};
      if (w > dis[v.first]) {
        dis[v.first] = w;
        h.push({w.second, v.first});
      }
    }
  }
  return dis;
}
template <class T>
vector<pair<bool, T> > min_path(const vector<vector<pair<int, T> > > &edges,
                                const int &s) {
  auto _edges = edges;
  int n = edges.size();
  for (int i = 0; i < n; i++)
    for (auto &u : _edges[i]) u.second = -u.second;
  auto dis = max_path(_edges, s);
  for (auto &u : dis) u.second = -u.second;
  return dis;
}
}  // namespace Dijstra
namespace BellmanFord {
template <class T>
pair<bool, vector<pair<bool, T> > > max_path(
    const vector<vector<pair<int, T> > > &edges, const int &s) {
  int n = edges.size();
  vector<pair<bool, T> > dis(n, {0, 0});
  dis[s] = {1, 0};
  for (int i = 1; i < n; i++) {
    for (int j = 0; j < n; j++)
      if (dis[j].first)
        for (auto u : edges[j]) {
          pair<bool, T> w = {1, dis[j].second + u.second};
          if (w > dis[u.first]) dis[u.first] = w;
        }
  }
  bool sig = 1;
  for (int j = 0; j < n; j++)
    if (dis[j].first)
      for (auto u : edges[j]) {
        pair<bool, T> w = {1, dis[j].second + u.second};
        if (w > dis[u.first]) sig = 0;
      }
  return {sig, dis};
}
template <class T>
pair<bool, vector<pair<bool, T> > > min_path(
    const vector<vector<pair<int, T> > > &edges, const int &s) {
  auto _edges = edges;
  int n = edges.size();
  for (int i = 0; i < n; i++)
    for (auto &u : _edges[i]) u.second = -u.second;
  auto result = max_path(_edges, s);
  for (auto &u : result.second) u.second = -u.second;
  return result;
}
}  // namespace BellmanFord
namespace Johnson {
template <class T>
pair<bool, vector<vector<pair<bool, T> > > > min_path(
    vector<vector<pair<int, T> > > edges) {
  int n = edges.size();
  edges.push_back({});
  for (int i = 0; i < n; i++) edges[n].push_back({i, 0});
  auto result = BellmanFord::min_path(edges, n);
  if (!result.first) return {0, {}};
  auto &dis = result.second;
  for (int i = 0; i < n; i++)
    for (auto &u : edges[i]) u.second += (dis[i].second - dis[u.first].second);
  edges.pop_back();
  vector<vector<pair<bool, T> > > _dis;
  for (int i = 0; i < n; i++) {
    _dis.push_back(Dijstra::min_path(edges, i));
    for (int j = 0; j < n; j++)
      if (_dis[i][j].first) _dis[i][j].second += dis[j].second - dis[i].second;
  }
  return {1, _dis};
}
}  // namespace Johnson

namespace DirectedTarjan {
vector<int> dfn, low, s, in_stack, sccid;
vector<vector<int> > scc;
int dfncnt;
vector<vector<int> > edge;
void tarjan(int u) {
  low[u] = dfn[u] = ++dfncnt, s.push_back(u), in_stack[u] = 1;
  for (auto v : edge[u]) {
    if (!dfn[v]) {
      tarjan(v);
      low[u] = min(low[u], low[v]);
    } else if (in_stack[v]) {
      low[u] = min(low[u], dfn[v]);
    }
  }
  if (dfn[u] == low[u]) {
    scc.push_back(vector<int>(0));
    int sc = scc.size() - 1;
    while (*s.rbegin() != u) {
      sccid[*s.rbegin()] = sc;
      scc.rbegin()->push_back(*s.rbegin());
      in_stack[*s.rbegin()] = 0;
      s.pop_back();
    }
    sccid[*s.rbegin()] = sc;
    scc.rbegin()->push_back(*s.rbegin());
    in_stack[*s.rbegin()] = 0;
    s.pop_back();
  }
}
pair<vector<vector<int> >, vector<int> > run(vector<vector<int> > _edge) {
  edge = _edge;
  int n = edge.size();
  dfn = low = s = in_stack = sccid = vector<int>(n, 0);
  scc.clear();
  dfncnt = 0;
  for (int i = 0; i < n; i++)
    if (!dfn[i]) tarjan(i);
  return {scc, sccid};
}
}  // namespace DirectedTarjan