//
// Created by Timana on 30/10/2026.
//
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

const long long MOD = 1000000007;

// BFS desde s: la paridad de distancias[v] es el color (equipo) de v.
// Devuelve false si alguna arista une dos estudiantes con el mismo color.
bool recorrer(vector<long long> &distancias, long long s, vector<vector<long long>> &lista_adyacencia) {
    queue<long long> q;
    distancias[s] = 0;
    q.push(s);
    while (!q.empty()) {
        long long u = q.front();
        q.pop();
        for (int i = 0; i < lista_adyacencia[u].size(); i++) {
            long long v = lista_adyacencia[u][i];
            if (distancias[v] == -1) {
                distancias[v] = distancias[u] + 1;
                q.push(v);
            } else if (distancias[v] % 2 == distancias[u] % 2) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    cin.tie(0)->sync_with_stdio(false);
    long long n, m;
    cin >> n >> m;

    vector<vector<long long>> lista_adyacencia(n + 1);
    for (long long i = 0; i < m; i++) {
        long long x, y;
        cin >> x >> y;
        lista_adyacencia[x].push_back(y);
        lista_adyacencia[y].push_back(x);
    }

    vector<long long> distancias(n + 1, -1);
    long long componentes = 0;
    for (long long i = 1; i <= n; i++) {
        if (distancias[i] == -1) {
            componentes++;
            if (!recorrer(distancias, i, lista_adyacencia)) {
                cout << "NO" << endl;
                return 0;
            }
        }
    }

    // cada componente conexa se puede repartir de 2 formas
    long long resultado = 1;
    for (long long i = 0; i < componentes; i++) {
        resultado = resultado * 2 % MOD;
    }
    cout << "SI " << resultado << endl;
}
