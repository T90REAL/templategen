// Pi(N) -> O(N / lnN)
namespace Primes {
const int N = 1000010;
vector<int> primes;
bool st[N];
void init(int n) {
    for (int i = 2; i <= n; i ++ ) {
        if (!st[i]) primes.push_back(i);
        for (int j = 0; primes[j] <= n / i; j ++ ) {
            st[primes[j] * i] = true;
            if (i % primes[j] == 0) break;
        }
    }
}
}


const int N = 5e6 + 1;
bool is_prime[N];
void Sieve(int n) {
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i <= n; ++i) is_prime[i] = true;
    for (int i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            if ((long long)i * i > n) continue;
            for (int j = i * i; j <= n; j += i)
                is_prime[j] = false;
        }
    }
}
