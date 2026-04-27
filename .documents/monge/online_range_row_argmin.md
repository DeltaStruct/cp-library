---
title: 区間行最小値クエリ (Totally Monotone, Online)
documentation_of: ./monge/online_range_row_argmin.hpp
---
## Online Range Row-argmin
Totally Monotoneな$H \times W$行列$A$に対して、区間行最小値を求める。  
ここでの区間行最小値とは、$x,l,r$が与えられたときの$\argmin_{l \le y < r} A_{x,y}$である。  
アルゴリズムがHはlogの形でしか依存しないので64bit整数などでも可能。  
比較はlessの形だと最大のインデックス、less_equalの形だと最小のインデックスが求まる。

### Order
構築$\order{W \log H}$、クエリ$\order{\log^2 W}$、空間$\order{W}$。  
クエリでのselect評価回数は$\order{\log W}$に抑えられている。

### Guide
#### Optional
- `value(T i,int k) -> U`: $A_{i,k}$の値を求める。
- `select(T i,int k,int j) -> bool`: $A_{i,k}$と$A_{i,j}$($k < j$)を比較して$A_{i,k}$の方が小さいか。

### Constructor
- `online_range_row_argmin(int H,int W,Guide)`: select版。
- `online_range_row_argmin(int H,int W,Guide)`: value版。
- `online_range_row_argmin(int H,int W,Guide,Comp)`: value版。Compを用いて比較する。

### Member
#### Function
- `build() -> void`: 構築する。
- `row_argmin(T x,int l,int r) -> int`: $x$行目の区間行最小値を求める。
- `row_min_value(T x,int l,int r) -> U`: $x$行目の区間行最小値のときの値を求める。

#### Function Alias
- `row_min(T x,int l,int r)`: `row_min_value(T x,int l,int r)`

### Note