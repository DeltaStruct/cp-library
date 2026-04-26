// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/range_parallel_unionfind
#include "assets/stdc++.hpp"
#include "graph/range_parallel_union_find.hpp"

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  int n,q; cin >> n >> q;
  vector<int> A(n); for (int& a:A) cin >> a;
  int res = 0;(((())))
  range_parallel_union_find UF(A,guide(merge)(
    [&](int& a,int b){
      long long t = a; (t *= b) %= 998244353;
      res += t; if (res>998244352) res -= 998244353;
      a += b; if (a>998244352) a -= 998244353;
    }
  ));
  while(q--){
    int c,a,b; cin >> c >> a >> b;
    UF.connect(a,b,c);
    cout << res << '\n';
  }
}