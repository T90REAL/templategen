const int N = 2e5 + 10;
int lch[64 * N], rch[64 * N], root[N];
ll sum[64 * N], tag[64 * N];
int tot = 0;
void update(int p) { sum[p] = sum[lch[p]] + sum[rch[p]]; }
void build(int &now, int l, int r) {
  now = ++tot;
  if (l == r) {
    sum[now] = 0;
    return;
  }
  int mid = (l + r) >> 1;
  build(lch[now], l, mid);
  build(rch[now], mid + 1, r);
  update(now);
}
void cpy(int pre, int &now) {
  lch[now] = lch[pre];
  rch[now] = rch[pre];
  sum[now] = sum[pre];
  tag[now] = tag[pre];
}
void modify(int &now, int pre, int l, int r, int ql, int qr, int val) {
  now = ++tot;
  cpy(pre, now);
  if (ql == l && qr == r) {
    sum[now] += 1ll * (r - l + 1) * val;
    tag[now] += val;
    return;
  }
  int mid = (l + r) >> 1;
  if (qr <= mid)
    modify(lch[now], lch[now], l, mid, ql, qr, val);
  else if (ql > mid)
    modify(rch[now], rch[now], mid + 1, r, ql, qr, val);
  else {
    modify(lch[now], lch[now], l, mid, ql, mid, val);
    modify(rch[now], rch[now], mid + 1, r, mid + 1, qr, val);
  }
  update(now);
}
ll query(int p, int l, int r, int ql, int qr) {
  if (ql == l && qr == r) {
    return sum[p];
  }
  int mid = (l + r) >> 1;
  ll tagVal = tag[p];
  if (qr <= mid)
    return tagVal + query(lch[p], l, mid, ql, qr);
  else if (ql > mid)
    return tagVal + query(rch[p], mid + 1, r, ql, qr);
  else
    return tagVal + query(lch[p], l, mid, ql, mid) +
           query(rch[p], mid + 1, r, mid + 1, qr);
}