// 值域分块 O(1)操作 O(sqrt(N)) 查询
namespace Block {
constexpr int N = 100000;
constexpr int B = 400;
int valCnt[N + 1];                   // 每个值域块的元素个数
int cnt[N + 1];                      // 单点贡献
int pos[N + 1], s[N + 1], e[N + 1];  // 每个值的值域块编号,每个块的起点和终点
#define forn(i, a, b) for (int i = a; i <= b; ++i)
void Add(int val, int change) {
  int id = pos[val];
  if (change == 1 and ++cnt[val] == 1) ++valCnt[id];
  if (change == -1 and --cnt[val] == 0) --valCnt[id];
}
int Q(int l, int r) {
  int L = pos[l], R = pos[r];
  int ans = 0;
  if (L == R) {
    forn(i, l, r) ans += (cnt[i] ? 1 : 0);
    return ans;
  }
  forn(i, l, e[L]) ans += (cnt[i] ? 1 : 0);
  forn(i, s[R], r) ans += (cnt[i] ? 1 : 0);
  forn(i, L + 1, R - 1) ans += valCnt[i];
  return ans;
}
void Init() {
  for (int i = 1; i <= N; i++) {
    pos[i] = (i + B - 1) / B;
    s[pos[i]] = (pos[i] - 1) * B + 1;
    e[pos[i]] = pos[i] * B;
  }
}
}  // namespace Block