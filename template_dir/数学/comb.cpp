const int mod = 1e9 + 7;
namespace Comb {
const int N = 100010;
int fact[N], infact[N];
int pw(int a, int b) {
  int res = 1;
  while (b) {
    if (b & 1)
      res = 1ll * res * a % mod;
    a = 1ll * a * a % mod;
    b >>= 1;
  }
  return res;
}
void Init() {
  fact[0] = 1;
  infact[0] = 1;
  for (int i = 1; i < N; i++) {
    fact[i] = 1ll * fact[i - 1] * i % mod;
    // infact[i] = 1ll * infact[i - 1] * pw(i, mod - 2) % mod;
  }
  infact[N - 1] = pw(fact[N - 1], mod - 2);
  for (int i = N - 1; i >= 1; i--)
    infact[i - 1] = 1ll * infact[i] * i % mod;
}
int C(int a, int b) {
  if (b > a) {
    return 0;
  }
  return 1ll * fact[a] * infact[b] % mod * infact[a - b] % mod;
}
} // namespace Comb
