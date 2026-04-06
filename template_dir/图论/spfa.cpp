auto spfa = [&](int st, vector<array<int, 3>>& E) {
    vector<int> dist(n + 1, 1e9);
    dist[st] = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            auto [u, v, w] = E[j];
            if (setmin(dist[v], dist[u] + w) && i == n - 1) return true;
        }
    }
    return false;
};
