// competitive-verifier: LOCALCASE //verify/localcase/joi/baker
#include "assets/stdc++.hpp"
#include "monge/online_range_row_argmin.hpp"

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  long long n,s; int m,q; cin >> n >> m >> s >> q;
  vector<long long> A(m); for (auto& a:A) cin >> a;
  vector<pair<long long,long long>> Q(q);
  for (auto& [a,b]:Q) cin >> a >> b;
  online_range_row_argmin segtree((long long)(4e12)+1,m,guide(value)(
    [&](long long i,int k){ return A[k]-i*k; }
  ));
  for (auto [a,b]:Q){
    int l = lower_bound(A.begin(),A.end(),b+a-s)-A.begin();
    int r = upper_bound(A.begin(),A.end(),b)-A.begin();
    if (l>=r){
      cout << 0 << '\n';
      continue;
    }
    int res = segtree.row_argmin(a,l,r);
    cout << min((A[res]+s-b)/a+r-1-res,(long long)r-l) << '\n';
  }
}