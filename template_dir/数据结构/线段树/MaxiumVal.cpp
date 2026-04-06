template <typename T>
struct SegmentTree {
  struct Node {
    int l, r;
    T maxVal = 0;
  };
  int n;
  vector<int> leaf;
  vector<Node> tr;
  SegmentTree(vector<T>& a) {
    n = a.size() - 1;
    tr.resize(4 * (n + 1), {0});
    leaf.resize(n + 1, 0);

    auto build = [&](auto build, int p, int l, int r) {
      tr[p].l = l;
      tr[p].r = r;
      if (l == r) {
        tr[p].maxVal = a[l];
        leaf[l] = p;

        return;
      }
      int mid = (l + r) >> 1;
      build(build, p * 2, l, mid);
      build(build, p * 2 + 1, mid + 1, r);
      update(p);
    };
    build(build, 1, 1, n);
  }
  void update(int p) {
    tr[p].maxVal = max(tr[p * 2].maxVal, tr[p * 2 + 1].maxVal);
  }
  void change(int p, int val) {
    p = leaf[p];
    setmax(tr[p].maxVal, val);
    // tr[p].maxVal = val;
    while (p /= 2) tr[p].maxVal = max(tr[p * 2].maxVal, tr[p * 2 + 1].maxVal);
  }
  int query(int p, int l, int r, int ql, int qr) {
    if (ql == l && qr == r) {
      return tr[p].maxVal;
    }
    int mid = (l + r) >> 1;
    if (qr <= mid)
      return query(p * 2, l, mid, ql, qr);
    else if (ql > mid)
      return query(p * 2 + 1, mid + 1, r, ql, qr);
    else
      return max(query(p * 2, l, mid, ql, mid),
                 query(p * 2 + 1, mid + 1, r, mid + 1, qr));
  }
};