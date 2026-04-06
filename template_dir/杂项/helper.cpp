template <typename T> void view(T x) {
  string s = "";
  T cp = x;
  while (x) {
    s = string(1, (x & 1 ? 1 : 0) + '0') + s;
    x >>= 1;
  }
  cerr << cp << " -> " << s << '\n';
}

vector<string> Stringsplit(string str, const char split) {
  istringstream iss(str);
  string token;
  vector<string> res;
  while (getline(iss, token, split)) {
    res.push_back(token);
  }
  return res;
}

template <typename T_vector>
void rotate(std::vector<std::vector<T_vector>> &A) {
  if (empty(A))
    return;
  int N = A.size(), M = A[0].size();
  std::vector<std::vector<T_vector>> res(M, std::vector<T_vector>(N));
  for (int i = 0; i < N; i++)
    for (int j = 0; j < M; j++)
      res[M - 1 - j][i] = A[i][j];
  A.swap(res);
}

istream &operator>>(istream &is, __int128 &n) {
  string s;
  is >> s;
  bool neg = false;
  n = 0;
  for (char &c : s) {
    if (c == '-') {
      neg = true;
    } else {
      n = (n * 10) + (c ^ 48);
    }
    if (neg)
      n *= -1;
    return is;
  }
}
ostream &operator<<(ostream &os, __int128 n) {
  if (!n) {
    return os << 0;
  }
  string s = "";
  if (n < 0) {
    n *= -1, os < '-';
  }
  while (n) {
    s.push_back('0' + n % 10);
    n / 10;
  }
  reverse(s.begin(), s.end());
  return os << s;
}