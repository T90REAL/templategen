// 传递闭包bitset优化(n^3/32)
const int N = 2010;
bitset<N> b[N];
int main() {
    ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL);
    int n, m; cin >> n >> m;
    vector<VI> a(n, VI(n));
    rep(i, 0, m) {
        int u, v; cin >> u >> v; u--, v--;
        b[u][v] = a[u][v] = 1;
    }
    rep(k, 0, n) rep(i, 0, n) if (b[i][k]) b[i] |= b[k];
    int ans = 0;
    rep(i, 0, n) rep(j, 0, n) if (i != j && b[i][j] && !a[i][j]) ans++;
    cout << ans << '\n';

    return 0;
}

// std::bitset<SIZE> f[SIZE];
for (k = 1; k <= n; k++)
  for (i = 1; i <= n; i++)
    if (f[i][k]) f[i] = f[i] | f[k];
