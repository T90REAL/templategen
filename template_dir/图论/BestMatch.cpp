const int N = 510;
vector<int> g[N];
int vis[N], match[N];
bool find(int x) {
    vis[x] = 1;
    for (int y : g[x]) {
        if (!match[y] || (!vis[match[y]] && find(match[y]))) {
            match[y] = x;
            return true;
        }
    }
    return false;
}
int main() {
    int n1, n2, m; cin >> n1 >> n2 >> m;
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        g[u].push_back(n1 + v);
    }
    int ans = 0;
    for (int i = 1; i <= n1; i++) {
        memset(vis, 0, sizeof(vis));
        if (find(i))
            ++ans;
    }
    cout << ans << '\n';
}
