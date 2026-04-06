const int N = 1000010;
ll a[N];
struct info {
  ll minv;
  info operator+(const info &other) { return {min(minv, other.minv)}; }
} tr[4 * N];
void update(int p) { tr[p] = tr[p * 2] + tr[p * 2 + 1]; }
void build(int p, int l, int r) {
  if (l == r) {
    tr[p] = {a[l]};
    return;
  }
  int mid = (l + r) >> 1;
  build(p * 2, l, mid);
  build(p * 2 + 1, mid + 1, r);
  update(p);
};
void change(int p, int l, int r, int x, ll val) {
  if (l == r) {
    assert(l == x);
    tr[p] = {min(tr[p].minv, val)};
    return;
  }
  int mid = (l + r) >> 1;
  if (x <= mid)
    change(p * 2, l, mid, x, val);
  else
    change(p * 2 + 1, mid + 1, r, x, val);
  update(p);
}
info query(int p, int l, int r, int ql, int qr) {
  if (ql == l && qr == r) {
    return tr[p];
  }
  int mid = (l + r) >> 1;
  if (qr <= mid)
    return query(p * 2, l, mid, ql, qr);
  else if (ql > mid)
    return query(p * 2 + 1, mid + 1, r, ql, qr);
  else
    return query(p * 2, l, mid, ql, mid) +
           query(p * 2 + 1, mid + 1, r, mid + 1, qr);
}