// generate random permutations
vector<int> a(n);
iota(all(a), 1)
std::random_device rd;
std::mt19937 g(rd());
std::shuffle(a.begin(), a.end(), g);

// generate random ull
mt19937_64 rng {chrono::steady_clock::now().time_since_epoch().count()};
#define ull unsigned long long