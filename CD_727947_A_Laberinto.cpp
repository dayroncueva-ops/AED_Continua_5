//
// Created by Timana on 30/10/2026.
//
#include <iostream>
#include <queue>
#include <vector>
using namespace std;


void recorrer_laberinto(vector<vector<long long>> &distancias, vector<vector<char>> &laberinto, long long me, long long ne) {
    distancias[ne][me] = 0;
    long long n= laberinto.size(); long long m= laberinto[0].size();
    vector<int> mov1{1, -1,0,0};
    vector<int> mov2{0,0,1,-1};
    queue<pair<long long, long long>> q;
    vector<pair<long long, long long>> elementos;
    q.push({ne, me});

    while (!q.empty()) {
        long long f = q.front().first; long long s = q.front().second;
        q.pop();
        for (long long i = 0; i < 4; i++) {
            ne = f + mov1[i]; me = s + mov2[i];
            if ( ne >= 0 && ne < n && me >= 0 && me < m && laberinto[ne][me] != '#' && distancias[ne][me] == -1) {
                distancias[ne][me] = distancias[f][s] + 1;
                q.push({ne, me});
            }
        }
    }

}


int main() {
    long long n, m;
    cin>>n>>m;
    long long ne, me, ns, ms;
    vector<vector<char>> laberinto(n, vector<char>(m));
    for (long long i = 0; i < n; i++) {
        for (long long j = 0; j < m; j++) {
            char element;
            cin >> element;
            if (element == 'S') { ns = i; ms = j; }
            if (element == 'E') { ne = i; me = j; }
            laberinto[i][j] = element;
        }
    }
    vector<vector<long long>> distancias(n, vector<long long>(m, -1));
    recorrer_laberinto(distancias, laberinto, me, ne);
    cout << distancias[ns][ms] << endl;
}