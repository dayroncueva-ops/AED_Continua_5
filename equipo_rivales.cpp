//
// Created by Dayron Cueva on 27/10/2026.
//
#include <vector>
#include <queue>
#include <iostream>
using namespace std;


const int MOD= 1e9+7;
bool BipartiteCheck(int src, vector<int> &color, vector<vector<int>> &G) {
    color[src] = 0;
    queue<int> Q;
    Q.emplace(src);
    while (not Q.empty()) {
        int u = Q.front();
        Q.pop();
        for (int &v : G[u]) {
            if (color[v] !=-1) {
                if (color[u] == color[v]) return false;
                continue;
            }
            color[v] = 1-color[u];
            Q.emplace(v);
        }
    }
    return true;
}
int main () {
    cin.tie(0)->sync_with_stdio(false);
    int n, m;
    cin>>n>>m;
    vector<vector<int>> G(n, vector<int>());
    for (int i = 0; i < m; i++) {
        int u, v;
        cin>>u>>v;
        --u, --v;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    int res=1;
    vector<int> color(n,-1);
    for (int i = 0; i < n; i++) {
        if (color[i] != -1) continue;
        if (not BipartiteCheck(i,color,G)) {
            cout<<"NO"<<'\n';
            return 0;
        }
        res  =(res*2)%MOD;
    }
    cout<<"SI "<<res<<'\n';
    return 0;
}
