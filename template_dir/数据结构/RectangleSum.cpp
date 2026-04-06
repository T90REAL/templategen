template <typename T>
class default_srecs {
 public:
  using value_type = T;
  inline static constexpr value_type ex() { return value_type(); }
  inline static constexpr value_type fx(const value_type &a,
                                        const value_type &b) {
    return a + b;
  }
  inline static constexpr value_type ix(const value_type &a,
                                        const value_type &b) {
    return a - b;
  }
};
template <class X = default_srecs<int64_t>, typename area_type = int>
class static_rectangle_sum {
 private:
  using operator_class = X;
  using value_type = typename X::value_type;

  class internal_BIT {
   private:
    int N;
    value_type *data;

   public:
    internal_BIT(const int N) : N(N), data(new value_type[N + 1]) {
      std::fill(data, data + N + 1, operator_class::ex());
    }

    value_type sum(int i) const {
      value_type s = operator_class::ex();
      while (i > 0) {
        s = operator_class::fx(s, data[i]);
        i -= i & -i;
      }
      return s;
    }

    void add(int i, const value_type &x) {
      ++i;
      while (i <= N) {
        data[i] = operator_class::fx(data[i], x);
        i += i & -i;
      }
    }
  };

  struct point {
    area_type x, y;
    value_type w;
    point() {}
    point(area_type x, area_type y, const value_type &w) : x(x), y(y), w(w) {}
  };

  struct rectangle {
    int id;
    area_type x2, y2;
    bool sign;
    rectangle() {}
    rectangle(const int id, area_type x2, area_type y2, bool sign)
        : id(id), x2(x2), y2(y2), sign(sign) {}
  };

 public:
  struct answer_type {
    int id;
    area_type x1, y1, x2, y2;
    value_type w;
    answer_type() {}
    answer_type(const int id, area_type x1, area_type y1, area_type x2,
                area_type y2)
        : id(id), x1(x1), y1(y1), x2(x2), y2(y2), w(operator_class::ex()) {}
  };

 private:
  int pi, ri;
  std::vector<area_type> CX, CY;
  std::vector<point> P;
  std::vector<rectangle> R;
  std::vector<answer_type> ans;

 public:
  static_rectangle_sum() : pi(0), ri(0) {}

  void init() {
    pi = ri = 0;
    CX.clear();
    CY.clear();
    P.clear();
    R.clear();
    ans.clear();
  }

  inline int add_point(const area_type x, const area_type y,
                       const value_type &w) {
    CX.emplace_back(x);
    CY.emplace_back(y);
    P.emplace_back(point(x, y, w));
    return pi++;
  }

  inline int add_query_rectangle(area_type x1, area_type y1, area_type x2,
                                 area_type y2) {
    ans.emplace_back(answer_type(ri, x1, y1, x2, y2));
    if (x1 == x2 || y1 == y2) return ri++;
    --x1, --y1, --x2, --y2;
    if (0 <= x1) CX.emplace_back(x1);
    CX.emplace_back(x2);
    if (0 <= y1) CY.emplace_back(y1);
    CY.emplace_back(y2);
    if (0 <= x1 && 0 <= y1) R.emplace_back(rectangle(ri, x1, y1, true));
    if (0 <= x1) R.emplace_back(rectangle(ri, x1, y2, false));
    if (0 <= y1) R.emplace_back(rectangle(ri, x2, y1, false));
    R.emplace_back(rectangle(ri, x2, y2, true));
    return ri++;
  }

  void build() {
    CX.emplace_back(0);
    CY.emplace_back(0);
    std::sort(CX.begin(), CX.end());
    CX.erase(std::unique(CX.begin(), CX.end()), CX.end());
    std::sort(CY.begin(), CY.end());
    CY.erase(std::unique(CY.begin(), CY.end()), CY.end());
    const auto compx = [&](area_type &x) {
      x = std::lower_bound(CX.begin(), CX.end(), x) - CX.begin();
    };
    const auto compy = [&](area_type &y) {
      y = std::lower_bound(CY.begin(), CY.end(), y) - CY.begin();
    };
    for (auto &e : P) compx(e.x), compy(e.y);
    for (auto &e : R) compx(e.x2), compy(e.y2);
    std::sort(P.begin(), P.end(), [](const point &p, const point &q) {
      return p.x < q.x || (p.x == q.x && p.y < q.y);
    });
    std::sort(R.begin(), R.end(), [](const rectangle &r, const rectangle &s) {
      return r.x2 < s.x2 || (r.x2 == s.x2 && r.y2 < s.y2);
    });
    int sx = CX.size(), sy = CY.size();
    auto itrp = P.begin();
    auto itrr = R.begin();
    internal_BIT BIT(sy);
    for (int i = 0; i < sx; ++i) {
      while (itrp != P.end() && (*itrp).x == i) {
        BIT.add((*itrp).y, (*itrp).w);
        ++itrp;
      }
      while (itrr != R.end() && (*itrr).x2 == i) {
        if ((*itrr).sign) {
          ans[(*itrr).id].w =
              operator_class::fx(ans[(*itrr).id].w, BIT.sum((*itrr).y2 + 1));
        } else {
          ans[(*itrr).id].w =
              operator_class::ix(ans[(*itrr).id].w, BIT.sum((*itrr).y2 + 1));
        }
        ++itrr;
      }
    }
  }

  const answer_type &operator[](const int i) const { return ans[i]; }
  answer_type get_ans(const int i) const {
    assert(0 <= i && i < ri);
    return ans[i];
  }

  std::vector<answer_type> copy_ans() const { return ans; }
};
/*
    copy from Library Checker: https://judge.yosupo.jp/submission/137516
    Usage:

    static_rectangle_sum<> S;
    for(int i = 1; i <= n; i++){
        S.add_point(L[i], R[i], weight);
    }
    // l1 <= a < r1
    // l2 <= b < r2
    for(int q : query){
        S.add_query_rectangle(l1, l2, r1, r2);
    }
    S.build();
    for(int i = 0; i < (int)query.size(); i++){
        ans[i] = S[i].w;
    }
*/