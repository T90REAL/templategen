#include <bits/stdc++.h>
using namespace std;
#define fi first
#define se second
#define pb push_back
#define mp make_pair
#define sz(a) ((int)a.size())
#define all(a) a.begin(), a.end()
#define rep(i, l, r) for (int i = l; i < r; ++i)
#define per(i, r, l) for (int i = r - 1; i >= l; --i)
typedef double db;
typedef long long ll;
typedef pair<int, int> pii;
template <typename T>
bool setmax(T &a, T b) {
  return (a < b ? a = b, 1 : 0);
}
template <typename T>
bool setmin(T &a, T b) {
  return (a > b ? a = b, 1 : 0);
}

const int N = 1000010;
struct Node {
  ll sum = 0;
} tr[N * 4];

int a[N], in[N], out[N], leaf[N], to[N];
vector<int> g[N];

void update(int p) { tr[p].sum = tr[p * 2].sum + tr[p * 2 + 1].sum; }

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
  if (qr <= mid)
    return query(p * 2, l, mid, ql, qr);
  else if (ql > mid)
    return query(p * 2 + 1, mid + 1, r, ql, qr);
  else
    return query(p * 2, l, mid, ql, mid) +
           query(p * 2 + 1, mid + 1, r, mid + 1, qr);
}
void change(int p, int val) {
  tr[p].sum += val;
  while (p /= 2) tr[p].sum += val;
}

int tot;
void dfs(int u, int pre) {
  in[u] = ++tot;
  to[tot] = u;
  for (int v : g[u])
    if (v != pre) dfs(v, u);
  out[u] = tot;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  cout.tie(NULL);

  int n, m, rt;
  cin >> n >> m >> rt;
  for (int i = 1; i <= n; i++) cin >> a[i];
  for (int i = 0; i < n - 1; i++) {
    int u, v;
    cin >> u >> v;
    g[u].emplace_back(v);
    g[v].emplace_back(u);
  }
  dfs(rt, 0);
  // for (int i = 1; i <= n; i++) cout << i << ' ' << in[i] << ' ' << out[i] <<
  // '\n'; int mx1 = 0, mx2 = 0; for (int i = 1; i <= n; i++) setmax(mx1,
  // in[i]), setmax(mx2, out[i]); cout << n << ' ' << mx1 << ' ' << mx2 << '\n';

  build(1, 1, n);

  while (m--) {
    int o;
    cin >> o;
    int x;
    cin >> x;
    if (o == 1) {
      int v;
      cin >> v;
      change(leaf[in[x]], v);
      // change(1, 1, n, x, v);
    } else {
      // cout << x << ' ' << in[x] << ' ' << out[x] << '\n';
      cout << query(1, 1, n, in[x], out[x]) << '\n';
    }
  }

  return 0;
}