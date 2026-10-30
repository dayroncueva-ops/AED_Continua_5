#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    cin>>n>>m;
    // Si hay más aristas que vértices en total, es imposible de entrada
    if (m > n) {
        cout << "NO\n";
        return 0;
    }
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u); // Cada arista suma 1 a la lista de cada extremo
    }
    vector<bool> visitado(n + 1, false);
    for (int i = 1; i <= n; ++i) {
        if (!visitado[i]) {
            queue<int> q;
            q.push(i);
            visitado[i] = true;
            int vertices = 0;
            long long suma_grados = 0;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                vertices++;
                suma_grados += adj[u].size();
                for (int v : adj[u]) {
                    if (!visitado[v]) {
                        visitado[v] = true;
                        q.push(v);
                    }
                }
            }
            // Cada arista se cuenta 2 veces en suma_grados (sum deg = 2 * E)
            long long aristas = suma_grados / 2;
            if (aristas > vertices) {
                cout << "NO\n";
                return 0;
            }
        }
    }
    cout << "SI\n";
    return 0;
}
