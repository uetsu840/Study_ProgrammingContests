//遅延評価セグメント木のライブラリ保存用
#include <bits/stdc++.h>
using namespace std;
#define max_n (200009)
#define rep(i,s,e) for(int i=s;i<(e);i++)
typedef long long int ll;
static ll seg_tree[4*max_n]; // セグメント木の配列
static ll lazy[4*max_n]; // 遅延評価用の配列
static ll n_norm; // 2のべき乗になるやつ

/* セグメント木の配列の初期化 */
void init(ll n)
{
 n_norm = 1;
 // セグツリーで管理するデータ数(入力を超える最小の2のべき乗)
 while(n > n_norm){
   n_norm *=2;
 }
 // 管理するデータ数に対して必要なセグツリー配列の数は，2n-1個
 rep(i,0,2*n_norm-1){
   seg_tree[i] = 0;
   lazy[i] = 0; // 遅延評価用の配列も初期化
 }
}

/* 遅延評価の処理 */
void eval(ll k, ll l, ll r) {
    if(lazy[k] != 0) {
        seg_tree[k] += lazy[k]; // 遅延評価配列の値をセグメント木に反映
        if(r - l > 1) {
            lazy[2*k+1] += lazy[k]; // 左の子に遅延評価配列の値を伝播
            lazy[2*k+2] += lazy[k]; // 右の子に遅延評価配列の値を伝播
        }
        lazy[k] = 0; // 遅延評価配列の値を初期化
    }
}

/* 区間[a, b)にxを加算する。ノードk=[l, r)に着目する。 */
void add(ll a, ll b, ll x, ll k=0, ll l=0, ll r=-1) {
    eval(k, l, r); // 遅延評価の処理
    if(r < 0) r = n_norm;
    if(b <= l || r <= a) return; // クエリの範囲外なら何もしない
    if(a <= l && r <= b) {
        lazy[k] += x; // 遅延評価配列に加算
        eval(k, l, r); // 遅延評価の処理
    } else {
        add(a, b, x, 2*k+1, l, (l+r)/2); // 左の子に対する処理
        add(a, b, x, 2*k+2, (l+r)/2, r); // 右の子に対する処理
        seg_tree[k] = max(seg_tree[2*k+1],seg_tree[2*k+2]); // 親の値を更新
    }
}

// クエリ範囲[a,b)で値を返す(0始まり)　
ll query(ll a,ll b, ll index, ll left, ll right)
{
  eval(index, left, right); // 遅延評価の処理を追加
  if(b<=left || right <=a)return 0;// 範囲外クエリで最弱を返す [a,b)と[left,right)
  if(a<=left && right <=b)return seg_tree[index];// 完全範囲内
  
  // 1段下に行く
  ll val1 = query(a,b,2*index+1,left,(left+right)/2); // 左の子に対するクエリ
  ll val2 = query(a,b,2*index+2,(left+right)/2,right); // 右の子に対するクエリ
  return max(val1,val2); // 子のクエリの結果の最大値を返す
}

struct st_event {
    ll  time;
    ll  pos;
    ll  diff;
};

bool operator< (const st_event &a, const st_event &b)
{
    if (a.time == b.time) {
        return a.diff < b.diff;
    }

    return a.time < b.time;
}

int main(){
  ll N, D, W;
  cin >> N >> D >> W;
  vector<st_event> events;
  for(ll i = 0; i < N; i++) {
    ll time;
    ll pos;
    cin >> time >> pos; // 時刻と座標をペアとして保存
    st_event ev1, ev2;
    ev1.time = time;
    ev1.pos  = pos;
    ev1.diff = 1; 
    events.push_back(ev1);

    ev2.time = time + D;
    ev2.pos  = pos;
    ev2.diff = -1;
    events.push_back(ev2);
  }
  sort(events.begin(), events.end()); // 時刻でソート

  init(max_n); 
  
  ll ans = 0;
  for(auto& event : events) {
    add(event.pos, event.pos+W, event.diff); // りんごが落ちた位置に1を加算
    ll time = event.time;
    ll apples = query(0, n_norm, 0, 0, n_norm);
//    printf("%lld %lld\n", time, apples);
    ans = max(ans, apples);
  }

  cout << ans << endl; // 答えを出力
}
