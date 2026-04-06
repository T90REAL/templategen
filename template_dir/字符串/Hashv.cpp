// 1-based
namespace Hash {
typedef pair<int, int> Hashv;
#define mp make_pair
const Hashv base1 = mp(13331, 1e9 + 7), base2 = mp(131, 998244353);
const Hashv baseMod = mp(base1.se, base2.se);
const pair<Hashv, Hashv> base = mp(base1, base2);
Hashv operator%(Hashv a, Hashv _baseMod) {
  return mp(a.fi % _baseMod.fi, a.se % _baseMod.se);
}
int operator*(int p, Hashv a) { return 1ll * p * a.fi % a.se; }
Hashv operator*(Hashv a, pair<Hashv, Hashv> _base) {
  return mp(a.fi * _base.fi, a.se * _base.se);
}
Hashv operator*(Hashv a, Hashv b) {
  return mp(1ll * a.fi * b.fi % baseMod.fi, 1ll * a.se * b.se % baseMod.se);
}
Hashv operator+(Hashv a, char s) {
  return mp(1ll * a.fi + s, 1ll * a.se + s) % baseMod;
}
Hashv operator-(Hashv a, Hashv b) {
  a.fi -= b.fi;
  if (a.fi < 0) a.fi += baseMod.fi;
  a.se -= b.se;
  if (a.se < 0) a.se += baseMod.se;
  return a;
}
}  // namespace Hash

vector<Hashv> hash(n + 1), pw(n + 1);
pw[0] = mp(1, 1);
for (int i = 1; i <= n; i++) {
  hash[i] = hash[i - 1] * base;
  hash[i] = hash[i] + s[i];
  pw[i] = pw[i - 1] * base;
}

auto get = [&](int l, int r) -> Hashv {
  // return mp((hash[r].fi - 1ll * hash[l - 1].fi * pw[r - l + 1].fi % base1.se
  // + base1.se) % base1.se,
  //        (hash[r].se - 1ll * hash[l - 1].se * pw[r - l + 1].se % base2.se +
  //        base2.se) % base2.se);
  return hash[r] - hash[l - 1] * pw[r - l + 1];
};
