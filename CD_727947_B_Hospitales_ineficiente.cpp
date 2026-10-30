//
// Created by Timana on 30/10/2026.
//
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

void recorrer(vector<vector<long long>> &distancias, long long valor, int turno, vector<vector<long long>>& lista_adyacencia) {
    long long x = valor;
    distancias[x][turno] = 0;
    queue<long long> q;
    q.push(x);
    while (!q.empty()) {
        long long u = q.front();
        q.pop();
        for (int i = 0; i < lista_adyacencia[u].size(); i++) {
            long long v = lista_adyacencia[u][i];
            if (distancias[v][turno] == -1 ) {
                distancias[v][turno] = distancias[u][turno]+1;
                q.push(v);
            }
        }
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(false);
    long long n, m, k;
    cin >> n >> m >> k;
    vector<long long> hospitales(k);
    for (long long i = 0; i < k; i++) {
        cin >> hospitales[i];
    }

    vector<vector<long long>> lista_adyacencia(n+1);
    for (long long i = 0; i < m; i++) {
        long long x, y;
        cin >> x >> y;
        lista_adyacencia[x].push_back(y);
        lista_adyacencia[y].push_back(x);
    }

    vector<vector<long long>> distancias(n+1, vector<long long>(k, -1));

    for (int i = 0; i < hospitales.size(); i++) {
        recorrer(distancias, hospitales[i], i, lista_adyacencia);
    }

    for (long long i = 1; i <=n; i++) {
        long long min_actual = -1;
        for (long long valor: distancias[i]) {
            if (valor > -1 && (min_actual == -1 || valor < min_actual)) {
                min_actual = valor;
            }
        }
        cout << min_actual << " ";
    }
}