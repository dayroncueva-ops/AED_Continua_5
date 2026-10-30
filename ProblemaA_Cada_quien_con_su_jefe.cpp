#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int main() {
    cin.tie(0)->sync_with_stdio(false);
    int n, m;
    cin >> n >> m;

    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<bool> visitado(n + 1, false);

    for (int s = 1; s <= n; s++) {
        if (visitado[s]) continue;

        long long V = 0, sumaGrados = 0;
        queue<int> q;
        visitado[s] = true;
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            V++;
            sumaGrados += adj[u].size();
            for (int v : adj[u]) {
                if (!visitado[v]) {
                    visitado[v] = true;
                    q.push(v);
                }
            }
        }

        long long E = sumaGrados / 2;
        if (E > V) {
            cout << "NO\n";
            return 0;
        }
    }
    cout << "SI\n";
}