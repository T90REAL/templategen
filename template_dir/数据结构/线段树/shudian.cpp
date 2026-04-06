#include <bits/stdc++.h>
using namespace std;
#define pb push_back
#define all(a) a.begin(), a.end()
#define rep(i, l, r) for (int i = l; i < r; ++i)
typedef long long ll;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  cout.tie(NULL);
  int n, q;
  cin >> n >> q;
  vector<array<int, 4>> evt;
  vector<int> vx;
  vector<int> a(n + 1);
  rep(i, 1, n + 1) {
    int x, y;
    cin >> x >> y;
    vx.pb(x);
    evt.pb({y, 0, x, -1});
  }
  for (int i = 0; i < q; i++) {
    int x1, x2, y1, y2;
    cin >> x1 >> x2 >> y1 >> y2;
    // 1 for add
    evt.pb({y2, 1, x2, i});
    evt.pb({y1 - 1, 1, x1 - 1, i});
    evt.pb({y2, 2, x1 - 1, i});
    evt.pb({y1 - 1, 2, x2, i});
  }
  sort(all(evt));
  sort(all(vx));
  vx.erase(unique(all(vx)), vx.end());
  int m = vx.size();
  vector<long long> bit(m + 1);
  auto add = [&](int x, int v) {
    for (; x <= m; x += x & -x) bit[x] += v;
  };
  auto query = [&](int x) {
    long long res = 0;
    for (; x; x -= x & -x) res += bit[x];
    return res;
  };
  vector<long long> ans(q);
  for (auto e : evt) {
    if (e[1] == 0) {
      int p = lower_bound(all(vx), e[2]) - vx.begin() + 1;
      add(p, 1);
    } else {
      int p = upper_bound(all(vx), e[2]) - vx.begin();
      if (e[1] == 1)
        ans[e[3]] += query(p);
      else
        ans[e[3]] -= query(p);
    }
  }
  for (int it : ans) cout << it << '\n';

  return 0;
}
