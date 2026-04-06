// 单个点的图板子可能有问题
struct DCC {
  int S = 0;
  int n;
  vector<vector<int>> adj;
  vector<bool> isCut;
  vector<int> dfn, low;
  int clk = 0;
  stack<int> stk;
  vector<vector<int>> dcc;

  DCC(int _n, int st){
    S = st;
    n = _n;
    adj.assign(n, {});
    isCut.assign(n, 0);
    dfn.assign(n, 0);
    low.assign(n, 0);
  }

  void add_edge(int u, int v){
    adj[u].push_back(v);
    adj[v].push_back(u);
  };

  void tarjan(int u) {
    dfn[u] = low[u] = ++clk;
    stk.push(u);
    int flag = 0;
    for (int v : adj[u]) {
      if (!dfn[v]) {
        tarjan(v);
        low[u] = min(low[u], low[v]);
        if (low[v] >= dfn[u]) {
          flag++;
          if (u != S || flag > 1)
            isCut[u] = true;
          int t;
          dcc.push_back(vector<int>());
          do {
            t = stk.top();
            stk.pop();
            dcc.back().push_back(t);
          } while (t != v);
          dcc.back().push_back(u);
        }
      } else
        low[u] = min(low[u], dfn[v]);
    }
  }

  vector<vector<int>> work(){
    tarjan(S);
    return dcc;
  };
};


// gedian
const int N = 100010;
int n, m, tot, sz;
int dfn[N], low[N];
vector<int> edge[N];
bool cut[N];

void dfs(int u, int fa) {
    dfn[u] = low[u] = ++tot;
    int childnumber = 0;
    for (auto v : edge[u]) {
        if (!dfn[v]) {
            dfs(v, u);
            childnumber++;
            low[u] = min(low[u], low[v]);
            if (low[v] >= dfn[u]) cut[u] = 1;
        } else if (v != fa) low[u] = min(low[u], dfn[v]);
    }   
    if (u == fa && childnumber <= 1) cut[u] = 0;
    sz += cut[u];
}
int main() {
    gogo;

    cin >> n >> m;
    for (int i = 0;i < m;i ++) {
        int a, b;cin >> a >> b;
        edge[a].push_back(b);
        edge[b].push_back(a);
    }
    // dfs(1, 1);//图是连通的话并且1为根节点
    for (int i = 1;i <= n;i ++) if (!dfn[i]) dfs(i, i);//图是不连通的
    cout << sz << '\n';
    for (int i = 1;i <= n;i ++) if (cut[i]) cout << i << ' ';
    cout << '\n';

    return 0;
}

// bridge
const int N = 100010;
int n, m, tot;
int dfn[N], low[N];
vector<pair<int, int>> edge[N];//记录边的编号防止重边
vector<int> bridge;

//fa -> u的边的编号为id
void dfs(int u, int id) {
    dfn[u] = low[u] = ++tot;
    for (auto [v, id2] : edge[u]) {
        if (!dfn[v]) {
            dfs(v, id2);
            low[u] = min(low[u], low[v]);
            if (dfn[v] == low[v]) bridge.push_back(id2 + 1);
        } else if (id != id2) low[u] = min(low[u], dfn[v]);//如果不是从树边下来的就更新一下low，看最多能往上跳多少
   
    // if (dfn[u] == low[u] && id != -1) bridge.push_back(id + 1);//如果没法再往上跳了，就找到了一条割边
    }
}
int main() {
    gogo;

    cin >> n >> m;
    for (int i = 0;i < m;i ++) {
        int a, b;cin >> a >> b;
        edge[a].push_back({b, i});
        edge[b].push_back({a, i});
    }
    dfs(1, -1);
    sort(bridge.begin(), bridge.end());
    cout << bridge.size() << '\n';
    for (auto b : bridge) cout << b << ' ';
    cout << '\n';

    return 0;
}