#include "common.h"

/**
 * KMP法 
 */
class KMP {
public:
    vector<SDWORD> makeTable(const string& s) {
        SDWORD n = s.size();
        vector<SDWORD> ret(n+1);
        ret[0] = -1;
        SDWORD j = -1;
        for (SDWORD i = 0; i < n; i++) {
            while ((0 <= j) && (s[i] != s[j])) {
                j = ret[j];
            } 
            j++;

            /* KMP法にするコード */
            if (s[i+1] == s[j]) {
                ret[i+1] = ret[j];
            } else {
                ret[i+1] = j;
            }
        }
        return ret;
    }

    /**
     *  str の中に word とマッチする場所のリストを返す
     * ret のそれぞれの要素 el は, 「str[el] からの文字列が word と一致する」ことを示す
     */
    vector<SDWORD> wordSearch(const string& str, const string& word) {
        vector<SDWORD> table = makeTable(word), ret;

        SDWORD m = word.size();
        SDWORD i = 0;
        SDWORD j = 0;
        SDWORD n = str.size();
        while (j < n) {
            while ((-1 < i) && (word[i]) != str[j]) {
                i = table[i];
            }
            i++;
            j++;
            if (m <= i) {
                ret.push_back(j - i);
                i = table[i];
            }
        }
        return ret;
    }
};