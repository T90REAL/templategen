const int mod = 998244353;
#define int ll
#define ssiz(x) (signed)x.size()
int QPow(int x, int y) {
  int res = 1;
  while (y) {
    if (y & 1) res = res * x % mod;
    x = x * x % mod;
    y >>= 1;
  }
  return res;
}
#define Inv(x) QPow(x, mod - 2)
const int gmod = 3;
const int invg = Inv(gmod);
using Poly = vector<int>;

void NTT(Poly &f, Poly &r, int lim, int flag) {
  for (int i = 0; i < lim; i++)
    if (i < r[i]) swap(f[i], f[r[i]]);
  for (int k = 1; k < lim; k <<= 1) {
    int len = k << 1, gn = QPow(flag > 0 ? gmod : invg, (mod - 1) / len);
    for (int i = 0; i < lim; i += len) {
      for (int j = 0, g = 1; j < k; j++, g = g * gn % mod) {
        int tmp = f[i + j + k] * g % mod;
        f[i + j + k] = f[i + j] - tmp;
        if (f[i + j + k] < 0) f[i + j + k] += mod;
        f[i + j] = f[i + j] + tmp;
        if (f[i + j] >= mod) f[i + j] -= mod;
      }
    }
  }
  if (!~flag) {
    int inv = Inv(lim);
    for (int &x : f) x = x * inv % mod;
  }
}
Poly Mul(Poly f, Poly g) {
  int len = ssiz(f) + ssiz(g) - 1, lim = 1;
  while (lim < len) lim <<= 1;
  f.resize(lim, 0), g.resize(lim, 0);

  Poly rev(lim, 0), h(lim, 0);
  for (int i = 0; i < lim; i++) {
    rev[i] = rev[i >> 1] >> 1;
    if (i & 1) rev[i] |= lim >> 1;
  }
  NTT(f, rev, lim, 1), NTT(g, rev, lim, 1);
  for (int i = 0; i < lim; i++) h[i] = f[i] * g[i] % mod;
  NTT(h, rev, lim, -1);

  h.resize(len, 0);
  return h;
}