//
// Created by Timana on 30/10/2026.
//
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

void recorrer(vector<long long> &distancias, vector<long long>& hospitales, vector<vector<long long>>& lista_adyacencia) {
    queue<long long> q;
    for (int i = 0; i < hospitales.size(); i++) {
        long long v = hospitales[i];
        distancias[v] = 0;
        q.push(v);
    }
    while (!q.empty()) {
        long long u = q.front();
        q.pop();
        for (int i = 0; i < lista_adyacencia[u].size(); i++) {
            long long v = lista_adyacencia[u][i];
            if (distancias[v] == -1 ) {
                distancias[v] = distancias[u]+1;
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

    vector<long long> distancias(n+1, -1);

    recorrer(distancias, hospitales, lista_adyacencia);
    for (long long i = 1; i <= n; i++) {
        cout << distancias[i] << endl;
    }
}