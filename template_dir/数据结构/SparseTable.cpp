// cache misses (TVT)
// int lg[MAXN+1];
// lg[1] = 0;
// for (int i = 2; i <= MAXN; i++)
//     lg[i] = lg[i/2] + 1;
//
// C++20
// #include <bit>
// int lg(unsigned long i) {
//     return std::bit_width(i) - 1;
// }
template<typename T> struct ST {
    int n;
    vector<vector<T>> st;
    ST(vector<T>& a) {
        n = (int) a.size();
        st.resize(lg(n)+1, vector<T>(n+1));
        rep(i, 0, n) st[0][i] = a[i];
        // TODO: change to max / sum
        rep(i, 1, lg(n)+1) for (int j = 0; j + (1<<i)-1 < n; j++) st[i][j] = min(st[i-1][j], st[i-1][j + (1<<(i-1))]);
    };
    T query(int l, int r) {
        int s = lg(r - l + 1);
        // TODO: change to max / sum
        return min(st[s][l], st[s][r - (1<<s)+1]);
    }
    // pre C++20
    int lg(unsigned long long i) {
        return i ? __builtin_clzll(1) - __builtin_clzll(i) : -1;
    }
};

// maintain max && min
template<typename T> struct ST {
    int n;
    vector<vector<T>> st_min;
    vector<vector<T>> st_max;
    ST(vector<T>& a) {
        n = (int) a.size();
        st_min.resize(lg(n)+1, vector<T>(n+1));
        st_max.resize(lg(n)+1, vector<T>(n+1));
        rep(i, 0, n) st_min[0][i] = st_max[0][i] = a[i];
        // TODO: change to max / sum
        rep(i, 1, lg(n)+1) for (int j = 0; j + (1<<i)-1 < n; j++) st_min[i][j] = min(st_min[i-1][j], st_min[i-1][j + (1<<(i-1))]), st_max[i][j] = max(st_max[i-1][j], st_max[i-1][j + (1<<(i-1))]);;
    };
    T query_min(int l, int r) {
        int s = lg(r - l + 1);
        // TODO: change to max / sum
        return min(st_min[s][l], st_min[s][r - (1<<s)+1]);
    }
    T query_max(int l, int r) {
        int s = lg(r - l + 1);
        // TODO: change to max / sum
        return max(st_max[s][l], st_max[s][r - (1<<s)+1]);
    }
    // pre C++20
    int lg(unsigned long long i) {
        return i ? __builtin_clzll(1) - __builtin_clzll(i) : -1;
    }
};
