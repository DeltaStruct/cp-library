// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/two_sat
#include "assets/stdc++.hpp"
#include "sat/two_sat.hpp"

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  string t; cin >> t >> t;
  int n,m,c; cin >> n >> m;
  vector<pair<int,int>> A(m);
  for (auto& [a,b]:A) (cin>>a>>b>>c);
  vector R = two_sat(n,A);
  if (R.empty()) (cout<<"s UNSATISFIABLE"<<'\n'),exit(0);
  cout << "s SATISFIABLE" << '\n';
  cout << "v ";
  for (int i(0);i < n;++i) cout << (R[i]?i+1:-i-1) << ' ';
  cout << "0" << '\n';
}