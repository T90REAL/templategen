using vi = vector<int>;
vi norm(vi a) {
  while (a.size() > 1 && a.back() == 0) a.pop_back();
  for (int i = 0; i + 1 < a.size(); i++) {
    a[i + 1] += a[i] / 10;
    a[i] %= 10;
  }
  while (a.back() >= 10) {
    int t = a.back() / 10;
    a.back() %= 10;
    a.push_back(t);
  }
  // assert(a.back()>0||(a.size()==1&&a.back()==0));
  return a;
}

vi operator+(vi a, vi b) {
  vi c;
  int t = max(a.size(), b.size());
  a.resize(t);
  b.resize(t);
  c.resize(t);
  for (int i = 0; i < t; i++) c[i] = a[i] + b[i];
  // return norm(c);
  return c;
}

bool operator<(vi a, vi b) {
  vi A = norm(a), B = norm(b);
  if (A.size() < B.size())
    return 1;
  else if (A.size() > B.size())
    return 0;
  else {
    for (int i = (int)A.size() - 1; i >= 0; i--)
      if (A[i] < B[i])
        return 1;
      else if (A[i] > B[i])
        return 0;
    return 0;
  }
}

vi operator*(vi a, vi b) {
  vi c(a.size() + b.size() - 1);
  for (int i = 0; i < a.size(); i++)
    for (int j = 0; j < b.size(); j++) c[i + j] += a[i] * b[j];
  return norm(c);
}

pair<vi, int> operator/(vi a, int b) {
  vi c;
  int x = 0;
  for (int i = (int)a.size() - 1; i >= 0; i--) {
    x = x * 10 + a[i];
    c.push_back(x / b);
    x %= b;
  }
  reverse(c.begin(), c.end());
  return make_pair(norm(c), x);
}
