// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/bitwise_and_convolution
#include "assets/stdc++.hpp"
#include "convolution/bitwise_and_convolution.hpp"

struct modint {
  long long v;
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
  vector<int> A(1<<n); for (int& a:A) cin >> a;
  vector<int> B(1<<n); for (int& a:A) cin >> a;
  for (auto a:bitwise_and_convolution(A,B)) cout << a.v << ' ';
  cout << endl;
}