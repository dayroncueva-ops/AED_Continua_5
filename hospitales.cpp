#include <iostream>
#include <vector>
#include <queue>
using namespace std;


void MultiBFS (int n, int m, vector<vector<int>>&nodos, vector<int>&hospitales) {
    vector<int> distancia (n+1, -1);
    queue<int> Q;
    for (int i=0; i<hospitales.size(); i++) {
        Q.push(hospitales[i]);
        distancia[hospitales[i]] = 0;
    }
    while (!Q.empty()) {
        int u = Q.front();
        Q.pop();
        for (auto v: nodos[u]) {
            if (distancia[v]==-1) {
                distancia[v] = distancia[u]+1;
                Q.push(v);
            }
        }
    }
    for (int i=1; i<=n; i++) {
        cout << distancia[i] << " ";
    }
}
int main () {
    cin.tie(0)->sync_with_stdio(false);
    int n, m, k;
    cin >> n >> m >> k;
    vector<int> pueblo_con_hospital (k,0);
    for (int i = 0; i < k; i++) {
        cin >> pueblo_con_hospital[i];
    }
    vector<vector<int>> nodos (n+1);
    for (int i=0; i<m; i++) {
        int u, v;
        cin >> u >> v;
        nodos[u].push_back(v);
        nodos[v].push_back(u);
    }
    MultiBFS(n, m, nodos, pueblo_con_hospital);
    return 0;
}
