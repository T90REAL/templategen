// '#' + a + '#' + b + '#'...
// 1-based
vector<int> p(n + 1);
for (int i = 1, m = 0, r = 0; i <= n; i++) {
  if (i > r) {
    p[i] = 1;
  } else {
    p[i] = min(p[2 * m - i], r - i + 1);
  }
  while (i - p[i] > 0 and i + p[i] <= n and s[i - p[i]] == s[i + p[i]]) {
    ++p[i];
  }
  if (i + p[i] - 1 > r) {
    m = i;
    r = i + p[i] - 1;
  }
}