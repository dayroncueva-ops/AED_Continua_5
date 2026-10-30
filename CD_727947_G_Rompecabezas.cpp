//
// Created by Timana on 30/10/2026.
//
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

// El estado (6 numeros del 0 al 5) se numera en base 6 para poder usar un arreglo
long long codificar(vector<long long> &estado) {
    long long codigo = 0;
    for (int i = 0; i < 6; i++) {
        codigo = codigo * 6 + estado[i];
    }
    return codigo;
}

vector<long long> decodificar(long long codigo) {
    vector<long long> estado(6);
    for (int i = 5; i >= 0; i--) {
        estado[i] = codigo % 6;
        codigo /= 6;
    }
    return estado;
}

void recorrer(vector<long long> &distancias, vector<long long> &inicio) {
    vector<int> mov1{1, -1, 0, 0};
    vector<int> mov2{0, 0, 1, -1};
    queue<long long> q;
    long long c0 = codificar(inicio);
    distancias[c0] = 0;
    q.push(c0);

    while (!q.empty()) {
        long long u = q.front();
        q.pop();
        vector<long long> estado = decodificar(u);
        int pos0 = 0;
        for (int i = 0; i < 6; i++) {
            if (estado[i] == 0) pos0 = i;
        }
        long long f = pos0 / 3, c = pos0 % 3;
        for (int k = 0; k < 4; k++) {
            long long nf = f + mov1[k], nc = c + mov2[k];
            if (nf >= 0 && nf < 2 && nc >= 0 && nc < 3) {
                vector<long long> vecino = estado;
                swap(vecino[pos0], vecino[nf * 3 + nc]);
                long long v = codificar(vecino);
                if (distancias[v] == -1) {
                    distancias[v] = distancias[u] + 1;
                    q.push(v);
                }
            }
        }
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(false);
    vector<long long> inicio(6);
    for (int i = 0; i < 6; i++) {
        cin >> inicio[i];
    }
    vector<long long> distancias(46656, -1); // 6^6 estados posibles
    recorrer(distancias, inicio);

    vector<long long> meta{1, 2, 3, 4, 5, 0};
    cout << distancias[codificar(meta)] << endl;
}
