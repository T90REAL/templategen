// Sample implementation of extended Euclidean algorithm from atcoder
pair<long long, long long> extgcd(long long a, long long b) {
  if (b == 0) return make_pair(1, 0);
  long long x, y;
  tie(y, x) = extgcd(b, a % b);
  y -= a / b * x;
  return make_pair(x, y);
}
