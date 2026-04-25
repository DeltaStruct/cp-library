---
title: Semi-online Deletion
documentation_of: ./query/semionline_deletion.hpp
---
## Semi-online Deletion
addクエリとundoクエリが高速に処理できるデータ構造$F$に対してdeleteクエリを提供する。  
Semi-onlineは、ここではaddクエリを行うタイミングでdeleteクエリが行われるタイミングが与えられていることを指す。  

### Guide
#### Required
- `add(T& v) -> void`: $F$に$v$を追加する。
- `undo()    -> void`: $F$へのaddクエリを1つ取り消す。

### Constructor
- `semionline_deletion(int n,Guide)`: タイミングの数を$n$個として構築する。

### Member
#### Function
- `push(int l,int r,const T& v)    -> void`: タイミング$l$に$add$クエリを実行させる。タイミング$r$に$delete$クエリを実行させる。
- `emplace(int l,int r,Args&... v) -> void`: 直接構築版push。
- `reset(bool end = false)         -> void`: タイミングを$0$もしくは$n-1$に初期化する。
- `reverse()                       -> void`: 順番を反転させ、resetを実行する。
- `increment()                     -> int `: タイミングを1つ進め、現在のタイミングを返す。
- `decrement()                     -> int `: タイミングを1つ戻し、現在のタイミングを返す。

#### Iterator
`for (int i:semionline_deletion(...))`が可能なように簡易的なものを定義している。

### Note
[Offline Deletion - USACO Guide](https://usaco.guide/adv/offline-del?lang=cpp)