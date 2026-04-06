// change index based
template <typename vector_T>
vector_T gen_dvec(vector_T &a) {
  int n = (int)a.size() - 1;
  VP b(n + 1);
  rep(i, 1, n + 1) b[i].fi = a[i], b[i].se = i;
  vector_T res(n + 1);
  // no same number
  sort(b.begin() + 1, b.end(), [](PI u, PI v) {
    if (u.fi != v.fi) return u.fi < v.fi;
    return u.se < v.se;
  });
  rep(i, 1, n + 1) res[b[i].se] = i;

  // exist same number
  auto b = a;
  sort(a.begin() + 1, a.end());
  rep(i, 1, n + 1) res[i] =
      lower_bound(a.begin() + 1, a.end(), b[i]) - a.begin();

  return res;
}
