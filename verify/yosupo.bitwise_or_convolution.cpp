// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/bitwise_and_convolution
#include "assets/stdc++.hpp"
#include "convolution/bitwise_or_convolution.hpp"

struct modint {
  long long v;
  modint() : v(0) {}
  modint(int _v) : v(_v) {}
  modint& operator+=(modint x){
    (v += x.v) %= 998244353;
    return *this;
  }
  modint& operator-=(modint x){
    v -= x.v;
    if (v<0) v += 998244353;
    return *this;
  }
  modint& operator*=(modint x){
    (v *= x.v) %= 998244353;
    return *this;
  }
};

int main(){
  ios::sync_with_stdio(false),cin.tie(0);
  int n; cin >> n;
  vector<modint> A(1<<n); for (auto& a:A) a = modint(input<int>());
  vector<modint> B(1<<n); for (auto& a:B) a = modint(input<int>());
  reverse(A.begin(),A.end()),reverse(B.begin(),B.end());
  vector R = bitwise_or_convolution(A,B);
  reverse(R.begin(),R.end());
  for (auto a:R) cout << a.v << ' ';
  cout << endl;
}