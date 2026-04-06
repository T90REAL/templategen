  // 0-based
  auto Kmp = [](string& s) {
    int n = s.size();
    vector<int> pi(n);
    for(int i = 1; i < n; i++) {
      int j = pi[i - 1];
      while(j > 0 and s[i] != s[j]) {
        j = pi[j - 1];
      }
      if(s[i] == s[j]) {
        j++;
      }
      pi[i] = j;
    }
    return pi;
  };

  // 1-based
  auto Kmp = [](string& s) {
    int n = s.size();
    vector<int> pi(n + 1);
    int j = 0;
    for(int i = 2; i <= n; i++) {
      while(j > 0 and s[i] != s[j + 1]) {
        j = pi[j];
      }
      if(s[i] == s[j]) {
        j++;
      }
      pi[i] = j;
    }
    return pi;
  };

  auto Kmp = [&](string& s) {
    int n = s.size() - 1;
    vector<int> border(n + 1, 0);
    for(int i = 2; i <= n; i++) {
      border[i] = border[i - 1];
      while(border[i] and s[i] != s[border[i] + 1]) border[i] = border[border[i]];
      border[i] += (s[i] == s[border[i] + 1]);
    }
  };