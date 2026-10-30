//
// Created by Dayron Cueva on 23/10/2026.
//
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int main () {
    cin.tie(0)->sync_with_stdio(false);
    int n, m;
    cin>>n>>m;
    vector<vector<int>> adj (n+1);
    vector<int> in_degree (n+1,0);
    for (int i=0; i<m; i++) {
        int u, v;
        cin>>u>>v;
        adj[u].push_back(v);
        in_degree[v]++;
    }
    for (int i=1; i<=n; i++) {
        sort(adj[i].begin(), adj[i].end());
    }
    long long mutuos = 0;
    for (int u=1; u<=n; u++) {
        for (int v: adj[u]) {
            if (u<v && binary_search(adj[v].begin(), adj[v].end(), u)) {
                mutuos++;
            }
        }
    }
    int mejor_usuario = 1;
    int max_seguidores = in_degree[1];
    for (int i=2; i<=n; i++) {
        if (in_degree[i] > max_seguidores) {
            max_seguidores = in_degree[i];
            mejor_usuario = i;
        }
    }
    cout<<mutuos<<" "<<mejor_usuario<<" "<<max_seguidores<<'\n';
    return 0;
}
