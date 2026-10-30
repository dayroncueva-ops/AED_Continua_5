//
// Created by Timana on 23/10/2026.
//
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;



int main() {
    cin.tie(0)->sync_with_stdio(false);
    long long n, m;
    cin >> n >> m;
    unordered_set<long long> edge_hashes; //Almacena datos
    vector<pair<int, int>> edges; //grados
    vector<long long> in_degrees(n+1,0); //grado de entrada
    for (int j = 0; j < m; j++) {
        int u, v;
        cin >> u >> v;
        edges.emplace_back(u, v);
        edge_hashes.insert(1ll * u * (n+1)+ v); //La clave al valor
        ++in_degrees[v]; // le agrega al grado
    }
    int res = 0;
    for (auto &e : edges) {
        int u, v;
        tie(u, v) = e;
        if (edge_hashes.find(1ll * v * (n+1)+ u) != edge_hashes.end()) {
            res++;
        }
    }
    int best = max_element(in_degrees.begin()+1, in_degrees.end()) - in_degrees.begin();
    cout << (res >> 1) << " " << best << " " << in_degrees[best] << '\n';
}