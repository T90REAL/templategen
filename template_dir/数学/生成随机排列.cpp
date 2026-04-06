// generate random permutations
vector<int> a(n);
iota(all(a), 1) std::random_device rd;
std::mt19937 g(rd());
std::shuffle(a.begin(), a.end(), g);

// generate random ull
mt19937_64 rng{chrono::steady_clock::now().time_since_epoch().count()};
#define ull unsigned long long

vector<int> prefix_function(string s) {
  int n = (int)s.length();
  vector<int> pi(n);
  for (int i = 1; i < n; i++) {
    int j = pi[i - 1];
    while (j > 0 && s[i] != s[j]) j = pi[j - 1];
    if (s[i] == s[j]) j++;
    pi[i] = j;
  }
  return pi;
}