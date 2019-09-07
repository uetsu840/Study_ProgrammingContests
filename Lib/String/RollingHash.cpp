#include "common.h"

/**
 *  rolling hash 
 */
#define MS  (2)
const long long mod[] = {999999937LL, 1000000007LL}, base = 9973;

struct hash_val {
    SQWORD asqHash[MS];

    const bool operator== (const hash_val &v) {
        return 0 == memcmp(this, &v, sizeof(*this));
    };
};

struct rolling_hash {
    int n;
    vector<long long> hs[MS], pw[MS];
    rolling_hash(){}
    rolling_hash(const string &s) {
        n = s.size();
        for (int i = 0; i < MS; i++) {
            hs[i].assign(n+1,0);
            pw[i].assign(n+1,0);
            hs[i][0] = 0;
            pw[i][0] = 1;
            for (int j = 0; j < n; j++) {
                pw[i][j+1] = pw[i][j]*base%mod[i];
                hs[i][j+1] = (hs[i][j]*base+s[j])%mod[i];
            }
        }
    }
    hash_val hash(int l, int r) { 
        hash_val ret;
        for (int i = 0; i < MS; i++) {
            ret.asqHash[i] = ((hs[i][r]-hs[i][l]*pw[i][r-l])%mod[i]+mod[i])%mod[i]; 
        }
        return ret;
    }

    bool match(int l1, int r1, int l2, int r2) {
        bool ret = false;
        hash_val h1 = hash(l1,r1);
        hash_val h2 = hash(l2,r2);

        return (h1 == h2);
    }

    bool match(int l, int r, hash_val h) {
        bool ret = 1;
        return (h == hash(l,r));
    }
};
