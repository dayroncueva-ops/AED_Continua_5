
#include<bits/stdc++.h>
using namespace::std;
bool valid_position(int x, int y, int n, int m) {
return x >= 0 and x < n and y >= 0 and y < m;
}
int BFS(int sx, int sy, int tx, int ty, int n, int m, vector<string> &s) {
vector<int> dx = {0, 1, 0, -1};
vector<int> dy = {1, 0, -1, 0};
vector<vector<vector<int>>> D(4, vector<vector<int>>(n, vector<int>(m, -1)));
queue<tuple<int, int, int>> Q;
Q.emplace(0, sx, sy);
D[0][sx][sy] = 0;
while (not Q.empty()) {
int dir, ux, uy;
tie(dir, ux, uy) = Q.front();
if (ux == tx and uy == ty) return D[dir][ux][uy];
Q.pop();
if (valid_position(ux + dx[dir], uy + dy[dir], n, m) and s[ux + dx[dir]][uy
+ dy[dir]] != '#') {
int vx = ux + dx[dir];
int vy = uy + dy[dir];
if (D[dir][vx][vy] == -1) {
D[dir][vx][vy] = D[dir][ux][uy] + 1;
Q.emplace(dir, vx, vy);
}
}
if (D[(dir + 3) % 4][ux][uy] == -1) {
D[(dir + 3) % 4][ux][uy] = D[dir][ux][uy] + 1;
Q.emplace((dir + 3) % 4, ux, uy);
}
if (D[(dir + 1) % 4][ux][uy] == -1) {
D[(dir + 1) % 4][ux][uy] = D[dir][ux][uy] + 1;
Q.emplace((dir + 1) % 4, ux, uy);
}
}
return -1;
}
int main() {
cin.tie(0) -> sync_with_stdio(false);
int n, m;
cin >> n >> m;
vector<string> s(n);
for (int i = 0; i < n; ++i) {
cin >> s[i];
}
cout << BFS(0, 0, n - 1, m - 1, n, m, s) << '\n';
return 0;
}
