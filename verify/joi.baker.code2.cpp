// competitive-verifier: LOCALCASE //verify/localcase/joi/baker
#include "assets/stdc++.hpp"
#include "monge/online_range_row_argmin.hpp"

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  long long n,s; int m,q; cin >> n >> m >> s >> q;
  vector<long long> A(m),B; for (auto& a:A) cin >> a;
  vector<pair<long long,long long>> Q(q);
  for (auto& [a,b]:Q) (cin >> a >> b),B.emplace_back(a);
  sort(B.begin(),B.end()),B.erase(unique(B.begin(),B.end()),B.end());
  online_range_row_argmin segtree((int)B.size(),m,guide(value)(
    [&](int i,int k){ return A[k]-B[i]*k; }
  ));
  for (auto [a,b]:Q){
    int l = lower_bound(A.begin(),A.end(),b+a-s)-A.begin();
    int r = upper_bound(A.begin(),A.end(),b)-A.begin();
    if (l>=r){
      cout << 0 << '\n';
      continue;
    }
    int res = segtree.row_argmin(lower_bound(B.begin(),B.end(),a)-B.begin(),l,r);
    cout << min((A[res]+s-b)/a+r-1-res,(long long)r-l) << '\n';
  }
}