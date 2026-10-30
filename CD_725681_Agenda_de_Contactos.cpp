//
// Created by Timana on 23/10/2026.
//
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    cin.tie(0)->sync_with_stdio(false);
    long long n, m, q;
    cin >> n >> m >> q;
    vector <vector<long long>> vector(n+1);

    for (long long i = 0; i < m; i++) {
        long long u; long long v;
        cin >> u; cin >> v;
        //agregar a los 2 porque es no dirigido
        vector[u].push_back(v);
        vector[v].push_back(u);
    }
    //se ordena de una vez unicamente
    for (auto &vecinos : vector) {
        sort(vecinos.begin(), vecinos.end());
    }
    for (long long i = 0; i < q; i++) {
        int opcion; long long u, kx;
        cin >> opcion; cin >> u; cin >> kx;
        if (opcion == 1) {
            if (vector[u].size() < kx) {
                cout << "-1" << "\n";
            }
            //se retorna el k enesimo
            else {
                 cout << vector[u][kx-1] << "\n";
            }
        }
        if (opcion == 2) {
            //upper_boun devuleve un iterador no un numero
            auto it = upper_bound(vector[u].begin(), vector[u].end(), kx);
            /*
                como upper_bound devuelve un iterador, se le resta el beign para que vean cuantas
                posiciones de diferencia existen
            */
            cout << it - vector[u].begin() << "\n";
        }
    }
}