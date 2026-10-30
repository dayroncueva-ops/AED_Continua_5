//
// Created by Timana on 30/10/2026.
//
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

// Estado = (fila, columna, direccion). Se numera como (fila*m + columna)*4 + direccion.
// Direcciones en sentido horario: 0 = derecha, 1 = abajo, 2 = izquierda, 3 = arriba.
void recorrer_robot(vector<long long> &distancias, vector<vector<char>> &laberinto) {
    long long n = laberinto.size(); long long m = laberinto[0].size();
    vector<int> mov1{0, 1, 0, -1};
    vector<int> mov2{1, 0, -1, 0};
    queue<long long> q;
    distancias[0] = 0; // casilla (0,0) mirando a la derecha
    q.push(0);

    while (!q.empty()) {
        long long u = q.front();
        q.pop();
        long long d = u % 4;
        long long celda = u / 4;
        long long f = celda / m; long long c = celda % m;

        vector<long long> vecinos;
        vecinos.push_back(celda * 4 + (d + 1) % 4); // derecha: gira 90 grados
        vecinos.push_back(celda * 4 + (d + 3) % 4); // izquierda: gira 90 grados
        long long nf = f + mov1[d]; long long nc = c + mov2[d];
        if (nf >= 0 && nf < n && nc >= 0 && nc < m && laberinto[nf][nc] != '#') {
            vecinos.push_back((nf * m + nc) * 4 + d); // avanzar
        }

        for (long long v : vecinos) {
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
    vector<vector<char>> laberinto(n, vector<char>(m));
    for (long long i = 0; i < n; i++) {
        for (long long j = 0; j < m; j++) {
            char element;
            cin >> element;
            laberinto[i][j] = element;
        }
    }
    vector<long long> distancias(n * m * 4, -1);
    recorrer_robot(distancias, laberinto);

    long long respuesta = -1;
    for (long long d = 0; d < 4; d++) {
        long long valor = distancias[((n - 1) * m + (m - 1)) * 4 + d];
        if (valor > -1 && (respuesta == -1 || valor < respuesta)) {
            respuesta = valor;
        }
    }
    cout << respuesta << endl;
}
