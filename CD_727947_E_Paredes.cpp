//
// Created by Timana on 30/10/2026.
//
#include <iostream>
#include <deque>
#include <vector>
using namespace std;

const long long INF = 1000000000;

// BFS 0-1: entrar a una casilla libre cuesta 0 (push_front), a una pared cuesta 1 (push_back)
void recorrer_paredes(vector<vector<long long>> &distancias, vector<vector<char>> &terreno) {
    distancias[0][0] = 0;
    long long n = terreno.size(); long long m = terreno[0].size();
    vector<int> mov1{1, -1, 0, 0};
    vector<int> mov2{0, 0, 1, -1};
    deque<pair<long long, long long>> d;
    d.push_back({0, 0});

    while (!d.empty()) {
        long long f = d.front().first; long long s = d.front().second;
        d.pop_front();
        for (long long i = 0; i < 4; i++) {
            long long nf = f + mov1[i]; long long ns = s + mov2[i];
            if (nf >= 0 && nf < n && ns >= 0 && ns < m) {
                long long w = (terreno[nf][ns] == '#') ? 1 : 0;
                if (distancias[f][s] + w < distancias[nf][ns]) {
                    distancias[nf][ns] = distancias[f][s] + w;
                    if (w == 0) d.push_front({nf, ns});
                    else d.push_back({nf, ns});
                }
            }
        }
    }
}

int main() {
    cin.tie(0)->sync_with_stdio(false);
    long long n, m;
    cin >> n >> m;
    vector<vector<char>> terreno(n, vector<char>(m));
    for (long long i = 0; i < n; i++) {
        for (long long j = 0; j < m; j++) {
            char element;
            cin >> element;
            terreno[i][j] = element;
        }
    }
    vector<vector<long long>> distancias(n, vector<long long>(m, INF));
    recorrer_paredes(distancias, terreno);
    cout << distancias[n - 1][m - 1] << endl;
}
