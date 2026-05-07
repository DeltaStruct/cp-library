// competitive-verifier: PROBLEM https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=0275
#include "assets/stdc++.hpp"
#include "graph/offline_dag_reachability.hpp"
#include "graph/dijkstra.hpp"

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  int n,m; cin >> n >> m;
  graph<int> G(n);
  vector<tuple<int,int,int>> E(m);
  for (auto& [a,b,c]:E) (cin>>a>>b>>c),G.emplace_undirected(--a,--b,c);
  int s,t; cin >> s >> t;
  vector A = dijkstra(G,s-1).dist,B = dijkstra(G,--t).dist;
  graph H(n);
  for (auto [a,b,c]:E){
    if (A[a]+c+B[b]==A[t]) H.emplace_directed(a,b);
    if (A[b]+c+B[a]==A[t]) H.emplace_directed(b,a);
  }
  int q; cin >> q;
  vector<pair<int,int>> Q(q);
  for (auto& [a,b]:Q) (cin>>a>>b),--a,--b;
  for (bool a:offline_dag_reachability(H,Q)) cout << (a?"Yes":"No") << '\n';
}