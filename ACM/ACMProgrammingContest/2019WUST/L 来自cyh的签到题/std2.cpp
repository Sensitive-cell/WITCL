#include <bits/stdc++.h>
#define ll long long
using namespace std;

template <class T>
inline bool scan(T &ret){
    char c;
    int sgn;
    if (c = getchar(), c == EOF) return 0; //EOF
    while (c != '-' && (c < '0' || c > '9')) c = getchar();
    sgn = (c == '-') ? -1 : 1;
    ret = (c == '-') ? 0 : (c - '0');
    while (c = getchar(), c >= '0' && c <= '9') ret = ret * 10 + (c - '0');
    ret *= sgn;
    return 1;
}

const ll mod = 1e9+7;
const int maxn = 1e6+50;

ll qp(ll x, ll n) {
    ll res = 1; x %= mod;
    while (n > 0) {
        if (n & 1) res = res * x % mod;
        x = x * x % mod;
        n >>= 1;
    }
    return res;
}

int n;
int x[maxn];

int main(int argc, char* argv[]) {
    int T;
    scanf("%d", &T);
    int has = 0;
    for (int kase = 1; kase <= T; ++kase) {
        scanf("%d", &n);
        assert(n >= 3);
        has += n;
        for (int i = 1; i <= n; ++i) {
            scanf("%d", &x[i]);
            assert(x[i] >= 1 && x[i] <= INT32_MAX);
        }
        sort(x + 1, x + 1 + n);
        printf("%d\n", x[n - 2]);
    }
    assert(has <= 1000000);
    return 0;
}