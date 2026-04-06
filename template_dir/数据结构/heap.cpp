/*
 * Created time: Fri Jan 26, 2024
 * 对顶堆求第k大值
 * log求变化的k所对应的第k大值
 * pre是大根堆维护前 [0, k-1] 个数，suf是小根对维护 [k, n] 的数字
 */
#include <set>
using namespace std;
typedef long long ll;

multiset<int> pre, suf;
ll pres = 0, sufs = 0;

auto update = [&]() {
  int s = pre.size(), t = suf.size();
  if (t - s > 1) {
    pre.insert(*suf.begin());
    pres += *suf.begin();
    sufs -= *suf.begin();
    suf.erase(suf.begin());
  } else if (s > t) {
    suf.insert(*pre.rbegin());
    sufs += *pre.rbegin();
    pres -= *pre.rbegin();
    pre.erase(prev(pre.end()));
  }
};
auto insert = [&](int x) {
  if (suf.size() && x >= *suf.begin()) {
    suf.insert(x);
    sufs += x;
  } else {
    pre.insert(x);
    pres += x;
  }
  // if (pre.empty() || x <= *pre.begin()) {
  //     pre.insert(x);
  //     pres += x;
  // } else {
  //     suf.insert(x);
  //     sufs += x;
  // }

  update();
};
auto remove = [&](int x) {
  if (suf.count(x)) {
    suf.erase(suf.find(x));
    sufs -= x;
  } else {
    assert(pre.count(x));
    pre.erase(pre.find(x));
    pres -= x;
  }
  update();
};
