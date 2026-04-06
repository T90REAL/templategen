auto insert = [&](int x1, int y1, int x2, int y2, int c) {
  pref[x1][y1] += c;
  pref[x1][y2 + 1] -= c;
  pref[x2 + 1][y1] -= c;
  pref[x2 + 1][y2 + 1] += c;
};
for (int i = 1; i + k - 1 <= n; i++) {
  for (int j = 1; j + k - 1 <= m; j++) {
    insert(i, j, i + k - 1, j + k - 1, 1);
  }
}
for (int i = 1; i <= n; i++)
  for (int j = 1; j <= m; j++)
    pref[i][j] += pref[i - 1][j] + pref[i][j - 1] - pref[i - 1][j - 1];