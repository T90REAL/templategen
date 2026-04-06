const int N = (1 << 20) * 4;
struct info {
  ll minv;
  info operator+(const info& other) { return {min(minv, other.minv)}; }
} tr[N];
void update(int p) { tr[p] = tr[p * 2] + tr[p * 2 + 1]; }
void build(int p, int l, int r) {
  if (l == r) {
    tr[p] = {-1};
    return;
  }
  int mid = (l + r) >> 1;
  build(p * 2, l, mid);
  build(p * 2 + 1, mid + 1, r);
  update(p);
}
int search(int p, int l, int r, int ql, int qr, int d) {
  if (ql == l && qr == r) {
    if (tr[p].minv < d) return -1;
    if (l == r) return l;
    int mid = (l + r) >> 1;
    if (tr[p * 2].minv >= d)
      return search(p * 2, l, mid, ql, mid, d);
    else
      return search(p * 2 + 1, mid + 1, r, mid + 1, qr, d);
  }
  int mid = (l + r) >> 1;
  if (qr <= mid)
    return search(p * 2, l, mid, ql, qr, d);
  else if (ql > mid)
    return search(p * 2 + 1, mid + 1, r, ql, qr, d);
  else {
    int pos = search(p * 2, l, mid, ql, mid, d);
    if (pos != -1) return pos;
    return search(p * 2 + 1, mid + 1, r, mid + 1, qr, d);
  }
}
void change(vector<ll>& a, int p, int l, int r, int x, ll val) {
  if (l == r) {
    assert(x == l);
    tr[p] = {val};
    a[x] = {val};
    return;
  }
  int mid = (l + r) >> 1;
  if (x <= mid)
    change(a, p * 2, l, mid, x, val);
  else
    change(a, p * 2 + 1, mid + 1, r, x, val);
  update(p);
}