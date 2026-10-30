//
// Created by Timana on 30/10/2026.
//
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// Cada estado es una mascara de n*m bits (bit = lampara encendida).
// Presionar la lampara p equivale a hacer XOR con efecto[p].
void recorrer(vector<long long> &distancias, vector<long long> &efecto, long long inicio) {
    queue<long long> q;
    distancias[inicio] = 0;
    q.push(inicio);
    while (!q.empty()) {
        long long u = q.front();
        q.pop();
        for (int p = 0; p < efecto.size(); p++) {
            long long v = u ^ efecto[p];
            if (distancias[v] == -1) {
                distancias[v] = distancias[u] + 1;
                q.push(v);
            }
        }
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(false);
    long long n, m;
    cin >> n >> m;
    long long inicio = 0;
    for (long long i = 0; i < n; i++) {
        for (long long j = 0; j < m; j++) {
            char element;
            cin >> element;
            if (element == '1') inicio |= (1LL << (i * m + j));
        }
    }

    vector<int> mov1{1, -1, 0, 0};
    vector<int> mov2{0, 0, 1, -1};
    vector<long long> efecto(n * m, 0);
    for (long long i = 0; i < n; i++) {
        for (long long j = 0; j < m; j++) {
            long long p = i * m + j;
            efecto[p] ^= (1LL << p);
            for (int k = 0; k < 4; k++) {
                long long ni = i + mov1[k], nj = j + mov2[k];
                if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
                    efecto[p] ^= (1LL << (ni * m + nj));
                }
            }
        }
    }

    vector<long long> distancias(1LL << (n * m), -1);
    recorrer(distancias, efecto, inicio);
    cout << distancias[0] << endl;
}
