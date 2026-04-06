namespace ODT {
struct Node_t {
  int l, r;
  mutable int v;
  Node_t(const int &il, const int &ir, const int &iv) : l(il), r(ir), v(iv) {}
  bool operator<(const Node_t &o) const { return l < o.l; }
};
set<Node_t> odt;
auto split(int x) {
  auto it = odt.lower_bound(Node_t(x, 0, 0));
  if (it != odt.end() && it->l == x) {
    return it;
  }
  --it;
  int l = it->l, r = it->r, v = it->v;
  odt.erase(it);
  odt.insert(Node_t(l, x - 1, v));
  return odt.insert(Node_t(x, r, v)).first;
}
void assign(int l, int r, int v) {
  auto itr = split(r + 1), itl = split(l);
  for (auto it = itl; it != itr; ++it) {
    // Calc ans
  }
  odt.erase(itl, itr);
  odt.insert(Node_t(l, r, v));
}
void perform(int l, int r) {
  auto itr = split(r + 1), itl = split(l);
  for (; itl != itr; ++itl) {
    // Perform Operations here
  }
}
}  // namespace ODT