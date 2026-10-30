#include <iostream>
#include <vector>
#include <string>
#include <deque>

using namespace std;

const int df[] = {-1, 1, 0, 0};
const int dc[] = {0, 0, -1, 1};
const int INF = 1e9;

int n_pared_romper(int n, int m, const vector<string>& grilla) {
    // Matriz de distancias inicializada en infinito
    vector<vector<int>> dist(n, vector<int>(m, INF));
    deque<pair<int, int>> dq;

    // La casilla inicial (0, 0) empieza con costo 0
    dist[0][0] = 0;
    dq.push_back({0, 0});

    while (!dq.empty()) {
        auto [f, c] = dq.front();
        dq.pop_front();

        // Si llegamos a la esquina inferior derecha, terminamos
        if (f == n - 1 && c == m - 1) {
            return dist[f][c];
        }

        for (int k = 0; k < 4; ++k) {
            int nf = f + df[k];
            int nc = c + dc[k];

            // 1. Validar bordes de la grilla
            if (nf >= 0 && nf < n && nc >= 0 && nc < m) {
                // Costo: 0 si es '.', 1 si es '#'
                int peso = (grilla[nf][nc] == '#') ? 1 : 0;

                // 2. Relajación de arista: ¿mejora la distancia?
                if (dist[f][c] + peso < dist[nf][nc]) {
                    dist[nf][nc] = dist[f][c] + peso;

                    // Si pesó 0 va ADELANTE, si pesó 1 va ATRÁS
                    if (peso == 0) {
                        dq.push_front({nf, nc});
                    } else {
                        dq.push_back({nf, nc});
                    }
                }
            }
        }
    }

    return dist[n - 1][m - 1];
}

int main() {

    cin.tie(0)->sync_with_stdio(false);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<string> grilla(n);
    for (int i = 0; i < n; ++i) {
        cin >> grilla[i];
    }

    cout << n_pared_romper(n, m, grilla) << "\n";

    return 0;
}
