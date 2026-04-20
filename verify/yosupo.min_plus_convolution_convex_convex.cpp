// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/min_plus_convolution_convex_convex
#include "assets/stdc++.hpp"
#include "convolution/min_plus_convolution.hpp"

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  int n,m; cin >> n >> m;
  vector<long long> A(n),B(m);
  for (auto& a:A) cin >> a;
  for (auto& a:B) cin >> a;
  for (auto a:min_plus_convolution(A,B)) cout << a << ' ';
  cout << '\n';
}