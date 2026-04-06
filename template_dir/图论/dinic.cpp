template<typename T> struct Flow{
    struct edge{
        int v, nxt;
        T f;
    };
 
    int s, t, vtot, etot;
    std::vector<edge> e;
    std::vector<int> head, dis, cur; // dis -> T
 
    Flow(int N, int M, int _s, int _t){
        s = _s; t = _t;
        vtot = N; etot = 0;
 
        e.resize(M * 2 + 1);
        head.assign(N + 1, -1);
        dis.resize(N + 1);
        cur.resize(N + 1);
    }
 
    void addEdge(int u, int v, T f){
        e[etot] = {v, head[u], f}; head[u] = etot++;
        e[etot] = {u, head[v], 0}; head[v] = etot++;
    }
 
    bool bfs(){
        // TODO: the start node should be cared
        for (int i = 0; i <= vtot; i++){
            dis[i] = 0;
            cur[i] = head[i];
        }
        std::queue<int> q;
        q.push(s); dis[s] = 1;
 
        while (!q.empty()){
            int u = q.front(); q.pop();
            for (int i = head[u]; ~i; i = e[i].nxt){
                if (e[i].f && !dis[e[i].v]) {
                    int v = e[i].v;
                    dis[v] = dis[u] + 1;
                    if (v == t) return true;
                    q.push(v);
                }
            }
        }
 
        return false;
    }
 
    T dfs(int u, T m){
        if (u == t) return m;
        T flow = 0;
        for (int i = cur[u]; ~i; cur[u] = i = e[i].nxt) 
            if (e[i].f && dis[e[i].v] == dis[u] + 1){
                T f = dfs(e[i].v, std::min(m, e[i].f));
                e[i].f -= f;
                e[i ^ 1].f += f;
                m -= f;
                flow += f;
                if (!m) break;
            }
 
        if (!flow) dis[u] = -1;
        return flow;
    }
 
    T Dinic(){
        T flow = 0;
        while (bfs()) flow += dfs(s, std::numeric_limits<T>::max());
        return flow;
    }
};