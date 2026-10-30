//
// Created by Timana on 23/10/2026.
//
#include <iostream>
using namespace std;
#include <bits/stdc++.h>

int main () {
    cin.tie(0)->sync_with_stdio(false);
    int n, m, q;
    cin >> n >> m >> q;
    map<pair <int, int>, int> freq;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        //intercambia para no isnertar 2 veces
        if (u > v) swap(u, v);
        freq[{u, v}]++;
    }
    for (int i=0; i<q; i++) {
        int u, v;
        cin >> u >> v;
        // intercambia para no buscar 2 veces, ay que asi se inserto
        if (u > v) swap(u, v);
        //devulve la frecuencia de u a v, si es 0 lo devuelve en itempo logaritmico
        cout << (freq.count({u,v}) ? freq[{u,v}]: 0 ) << "\n";
    }
}