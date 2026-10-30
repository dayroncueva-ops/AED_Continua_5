//
// Created by Dayron Cueva on 29/10/2026.
//
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

const int df [] = {-1,1,0,0};
const int dc [] = {0,0,-1,1};

int BFS (int n, int m, vector<string> &grilla, int inicio_f, int inicio_c) {
    vector<vector<int>> distancia (n, vector<int>(m,-1));
    queue<pair<int,int>> Q;
    distancia[inicio_f][inicio_c] = 0;
    Q.push({inicio_f, inicio_c});
    while (!Q.empty()) {
        auto [f,c] = Q.front();
        Q.pop();
        if (grilla[f][c]=='E') {
            return distancia[f][c];
        }
        for (int k=0; k<4; k++) {
            int idx_f = f + df[k];
            int idx_c = c + dc[k];
            if (idx_f>=0 && idx_c<m && idx_f<n && idx_c>=0) {
                if (grilla[idx_f][idx_c]!='#' && grilla[idx_f][idx_c]==-1) {
                    distancia[idx_f][idx_c] = distancia[f][c] + 1;
                    Q.push({idx_f,idx_c});
                }
            }
        }
    }
    return -1;

}
int main () {
    cin.tie(0)->sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    queue<pair<int,int>> q;

    vector<string> grilla (n);
    int inicio_f = -1;
    int inicio_c = -1;
    for (int i=0; i<n; i++) {
            cin >> grilla[i];
            for (int j=0; j<m; j++) {
                if (grilla[i][j]=='S') {
                    inicio_f = i;
                    inicio_c = j;
            }
        }
    }
    cout<<BFS(n,m,grilla, inicio_f, inicio_c)<<endl;
    return 0;
}
