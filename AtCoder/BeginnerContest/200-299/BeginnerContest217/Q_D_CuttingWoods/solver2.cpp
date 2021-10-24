#pragma GCC optimize ("O3")

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <set>
#include <algorithm>
#include <numeric>
#include <list>
using namespace std;

using QWORD  = uint64_t;
using SQWORD = int64_t;
using DWORD  = uint32_t;
using SDWORD = int32_t;
using WORD   = uint16_t;
using SWORD  = int16_t;
using BYTE   = uint8_t;
using SBYTE  = int8_t;
using DOUBLE = double;
using LDOUBLE = long double;
using FLOAT  = float;

#define MIN_SDWORD (-2147483648)
#define MAX_SDWORD (2147483647)
#define MIN_SBYTE (-128)
#define MAX_SBYTE (127)

#define MIN_SQWORD (0x8000000000000000)
#define MAX_SQWORD (0x7FFFFFFFFFFFFFFF)

#define MAX_QWORD  (0xFFFFFFFFFFFFFFFF)
#define MAX_DWORD  (0xFFFFFFFF)
#define MAX_WORD   (0xFFFF)
#define MAX_BYTE   (0xFF)

#define MAX_DOUBLE      (1.0e+308)
#define DOUBLE_EPS      (1.0e-12)
#define MIN_DOUBLE_N    (-1.0e+308)

#define ArrayLength(a)  (sizeof(a) / sizeof(a[0]))

static inline DOUBLE MAX(DOUBLE a, DOUBLE b) { return a > b ? a : b; }
static inline QWORD MAX(QWORD a, QWORD b) { return a > b ? a : b; }
static inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
static inline SQWORD MAX(SQWORD a, SQWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SQWORD MIN(SQWORD a, SQWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }
static inline SQWORD ABS(SQWORD a) {return 0 <= a ? a : -a;}
static inline DOUBLE ABS(DOUBLE a) {return 0 <= a ? a : -a;}

#define BYTE_BITS   (8)
#define WORD_BITS   (16)
#define DWORD_BITS  (32)
#define QWORD_BITS  (64)

static inline void inputStringSpSeparated(char *pcStr)
{
    char *pcCur = pcStr;
    for (;;) {
        char c = getchar();
        if (('\n' == c) || (EOF == c) || (' ' == c)) {
            break;
        }
        *pcCur = c;
        pcCur++;
    }
    *pcCur = '\0';
}

static inline void inputString(char *pcStr)
{
    char *pcCur = pcStr;
    for (;;) {
        char c = getchar();
        if (('\n' == c) || (EOF == c)) {
            break;
        }
        *pcCur = c;
        pcCur++;
    }
    *pcCur = '\0';
}


static inline SQWORD inputSQWORD(void)
{
    SQWORD sqNumber = 0;
    SQWORD sqMultiplier = 1;
    bool bRead = false;
    for (;;) {
        char c = getchar();
        if (!bRead) {
            if ('-' == c) {
                sqMultiplier = -1;
            }
        }
        if (('0' <= c) && (c <= '9')) {
            sqNumber *= 10LL;
            sqNumber += (SQWORD)(c - '0');
            bRead = true;
        } else {
            if (bRead) {
                return sqNumber * sqMultiplier;
            }
        }
    }
}


static inline SDWORD inputSDWORD(void)
{
    SDWORD lNumber = 0;
    SDWORD lMultiplier = 1;
    bool bRead = false;
    for (;;) {
        char c = getchar();
        if (!bRead) {
            if ('-' == c) {
                lMultiplier = -1;
            }
        }
        if (('0' <= c) && (c <= '9')) {
            lNumber *= 10;
            lNumber += (c - '0');
            bRead = true;
        } else {
            if (bRead) {
                return lNumber * lMultiplier;
            }
        }
    }
}

static inline DOUBLE inputFP(void)
{
    DOUBLE dInt = 0.0;
    DOUBLE dFrac = 0.0;
    DOUBLE dMultiplier = 1.0;
    DWORD dwFpCnt = 0;
    DOUBLE *pdCur = &dInt;
    bool bRead = false;
    for (;;) {
        char c = getchar();
        if (!bRead) {
            if ('-' == c) {
                dMultiplier = -1;
            }
        }
        if ('.' == c) {
            pdCur = &dFrac;
        } else if (('0' <= c) && (c <= '9')) {
            (*pdCur) *= 10;
            (*pdCur) += (DOUBLE)(c - '0');
            bRead = true;
            if (pdCur == &dFrac) {
                dwFpCnt++;
            }
        } else {
            if (bRead) {
                return dMultiplier * (dInt + dFrac / (pow((DOUBLE)10.0 , (DOUBLE)dwFpCnt)));
            }
        }
    }
}

/*----------------------------------------------*/
/*-----------------------------------------------------*/
using namespace std;
#include <cstdio>
#include "malloc.h"
#include "assert.h"
#include <iterator>
 
/**
 *  デバッグ用トレース
 */
template <typename ... Args>
void Trace(const char *format, Args const & ... args) {
//    printf(format, args ...);
}


/**
 *  平衡2分木
 */
template <typename T>
class AVLTree {
    struct TREE_NODE  {
        T data;
        int diff;
        TREE_NODE *child[2];
    };

    TREE_NODE *root;

    void rotate(TREE_NODE **node, bool is_right) {
        int indexL, indexR, sign;
        if (is_right) {
            indexL = 0;     /* 「左の子」*/
            indexR = 1;     /* 「右の子」*/
            sign = 1;       /* diff の符号 */
        } else {
            indexL = 1;
            indexR = 0;
            sign = -1;
        }
        
        assert((*node)->child[indexL] != nullptr); /* 回転に必要な子が無かったら死亡 */
        if ((*node)->child[indexL] != nullptr) {
            TREE_NODE* left = (*node)->child[indexL];
            int a = (*node)->diff * sign; 
            int b = left->diff * sign;
            int nextA = 0;
            int nextB = 0;
            /* 高さの差の更新 */
            if (0 <= b) {
                if (0 <= a - 1 - b) {
                    nextB = b - 1;
                } else {
                    nextB = a - 2;
                }
                nextA = a - 1 - b;
            } else {
                if (0 <= a - 1) {
                    nextB = b - 1;
                } else {
                    nextB = a + b - 2;
                }
                nextA = a - 1;
            }
            (*node)->diff = nextA * sign;
            left->diff = nextB * sign;
            /* 回転操作 */
            (*node)->child[indexL] = (*node)->child[indexL]->child[indexR];
            left->child[indexR] = *node;
            *node = left;
        }
    }

    /* ノードの追加の追加 */
    int addNode(TREE_NODE **node, T data) {

        /* 新しい子を追加する */
        *node = (TREE_NODE *)malloc(sizeof(TREE_NODE));
        assert(*node != nullptr);
        if (*node != nullptr) {
            /* 作成したノードを初期化する */
            (*node)->data = data;
            (*node)->diff = 0; /* 子は両方無い */
            (*node)->child[0] = nullptr;
            (*node)->child[1] = nullptr;
        }
        Trace("AVLTree addNode data[%lld] [%08x]\n", data, *node);

        /* 何もない所に子を作ったので、高さが増える */
        return 1;
    }

    /* ノードの追加・削除(子の高さに変化があったときの処理) */
    int procChildHeightChanged(TREE_NODE **node, int delta, int delta_sign) 
    {
        int orig_diff = (*node)->diff; /* 子の高さが変わる前の高さの差 */
        int do_rotate = 0, rotate_l, diff_sign, rotate_right; /* 回転操作の情報 */
        (*node)->diff += delta * delta_sign; /* 差を更新する */
        if ((*node)->diff > 1) {
            /* 左が高すぎるので、右回転する */
            do_rotate = 1;      /* 回転をする */
            rotate_l = 0;       /* 「左の子」の添字 */
            diff_sign = 1;      /* diffの意味 */
            rotate_right = 1;   /* この回転は右回転か */
        } else if ((*node)->diff < -1) {
            /* 右が高すぎるので、左回転する */
            do_rotate = 1;
            rotate_l = 1;
            diff_sign = -1;
            rotate_right = 0;
        }
        if (do_rotate) {
            int child_diff = (*node)->child[rotate_l]->diff;
            /* 回転操作 */
            if ((*node)->child[rotate_l]->diff * diff_sign < 0) {
                /* 
                *   左 -> 右 みたいな感じになっているので、まず子を回転する 
                */
                /* メインの回転の逆方向 */
                rotate(&(*node)->child[rotate_l], !rotate_right);
            }
            /* このノードを回転する */
            rotate(node, rotate_right);
            /* 
            *   子が高くなって回転をした場合は、高くなった分がその方向から減らされるから変わらない
            *   子が低くなって回転をした場合は、低くなった分が反対方向から減らされるから減る
            *   ただし、高い方の子の高さの差が0の場合は、回転によって高さが減らない
            *   挿入後の回転では、「高い方の子の高さの差が0、かつ高い方の子が高くなった」という場合は発生しない 
            */
            if ((delta < 0) && (child_diff != 0)) {
                return (-1);
            } else {
                return 0;
            }
        }
        /* 高さの変化を返す */
        if ((0 < delta) && (orig_diff == 0)) {
            /* 
            *   左右の高さが同じ状態で子の高さが増えたら、高さが増える
            *   左右の高さが違う場合は、平らになるだけで高さは変わらないか、回転が行われここでは扱わない 
            */
            return 1;
        } else if ((delta < 0) && (orig_diff != 0)) {
            /* 
            *   左右の高さが違い、高い方の子の高さが減ったら、高さが減る
            *    低い方の高さが減る場合は、回転操作が行われるのでここでは扱わない
            */
            return -1;
        } else {
            /* それ以外では、高さは変わらない */
            return 0;
        }
    }

    /* ノードの追加・削除 */
    int modifyTree(
        TREE_NODE **node,
        T data, 
        bool bIsAdd) 
    {
        Trace("AVLTree modifyTree node[%08x]\n", *node);

        if (*node == nullptr) {
            if (bIsAdd) {
                return addNode(node, data);
            } else {
                /* 削除対象が見つからなかった */
                return 0;
            }
        } else {
            int delta, delta_sign;
            TREE_NODE *next_node;
            if (data == (*node)->data) {
                /* このノードのデータと同じ */
                if (bIsAdd) {
                    /* 重複したデータの挿入クエリなので、無視する */
                    return 0;
                } else {
                    /* このノードを削除する */
                    if ((*node)->child[1] == nullptr) {
                        /* 右の子が無いので、左の子をこのノードの位置に持ってくる */
                        next_node = (*node)->child[0];
                        free(*node);
                        *node = next_node;
                        /* 高さが1減る */
                        return -1;
                    } else if ((*node)->child[0] == nullptr) {
                        /* 左の子が無いので、右の子をこのノードの位置に持ってくる */
                        next_node = (*node)->child[1];
                        free(*node);
                        *node = next_node;
                        /* 高さが1減る */
                        return -1;
                    } else {
                        /* 両方に子がある */
                        /* 左にある一番右のノードを探す */
                        for (next_node = (*node)->child[0]; next_node->child[1] != nullptr; next_node = next_node->child[1]);
                        /* そのノードの値をこのノードに持ってくる */
                        (*node)->data = next_node->data;
                        /* そのノードを削除する (左に行く) */
                        delta_sign = 1;
                        delta = modifyTree(&(*node)->child[0], next_node->data, bIsAdd);
                    }
                }
            } else {
                /* このノードは対象では無いので、子を処理する */
                int nextChildIndex;
                if (data < (*node)->data) {
                    /* このノードのデータより小さいので、左に行く */
                    nextChildIndex = 0;          /* 「左の子」の添字 */
                    delta_sign = 1; /* diffの変化の方向 */
                } else {
                    /* このノードのデータより大きいので、右に行く */
                    nextChildIndex = 1;
                    delta_sign = -1;
                }
                delta = modifyTree(&(*node)->child[nextChildIndex], data, bIsAdd);
                Trace("-----this %08x\n", (*node));
                Trace("-----R %08x\n", (*node)->child[0]);
                Trace("-----L %08x\n", (*node)->child[1]);
            }
            if (delta != 0) {
                return procChildHeightChanged(node, delta, delta_sign);
            } else {
                /* 子の高さが変わっていないので、高さは変わらない */
                return 0;
            }
        }
    }

    /**
     *  検索(あたり)
     */
    bool findNode(const TREE_NODE *node, T findValue) {
        if (node == nullptr) {
            return false;
        }
        if (node->data == findValue) {
            return true;
        } 
        if (findValue < node->data) {
            return findNode(node->child[0], findValue);
        } else {
            return findNode(node->child[1], findValue);
        }
    }

    /**
     *  検索
     */
    T findUpper(const TREE_NODE *node, T upperValue, T findValue) {
        if (nullptr == node) {
            return upperValue;
        }
        if (findValue < node->data) {
            return findUpper(node->child[0], node->data, findValue);
        } else {
            return findUpper(node->child[1], upperValue, findValue);
        }
    }

    /**
     *  検索
     */
    T findLower(const TREE_NODE *node, T upperValue, T findValue) {
        if (nullptr == node) {
            return upperValue;
        }
        if (node->data == findValue) {
            return node->data;
        }
        if (node->data < findValue) {
            return findLower(node->child[1], node->data, findValue);
        } else {
            return findLower(node->child[0], upperValue, findValue);
        }
    }

    /* 出力 */
    void print_tree(const TREE_NODE* node, int depth) {
        int i;
        if (node != NULL) {
            /* 右の子を出力 (右から出力すると、90度右回転した時にいい感じになる) */
            print_tree(node->child[1], depth + 1);
            /* このノードを出力 */
            for (i = 0; i < depth; i++) {
                printf("  ");
            } 
            printf("%d (%d)\n", node->data, node->diff);
            /* 左の子を出力 */
            print_tree(node->child[0], depth + 1);
        }
    }

public:
    AVLTree() {
        root = nullptr;
    }

    void insert(T addValue) 
    {
        modifyTree(&root, addValue, true);
    }
    
    void erase(T eraseValue) 
    {
        modifyTree(&root, eraseValue, false);
    }

    bool find(T findValue) 
    {
        return findNode(root, findValue);
    }

    T findUpper(T findValue) 
    {
        return findUpper(root, MAX_SQWORD, findValue);
    }

    T findLower(T findValue) 
    {
        return findLower(root, MIN_SQWORD, findValue);
    }

    void debugPrint()
    {
        print_tree(root, 0);
    }
};

/*----------------------------------------------*/
int main(void)
{
    SQWORD sqL = inputSQWORD();
    SQWORD sqQ = inputSQWORD();

    AVLTree<SQWORD> setCuttingPos;
    setCuttingPos.insert(0LL);
    setCuttingPos.insert(sqL);
    for (SQWORD sqQueryIdx = 0; sqQueryIdx < sqQ; sqQueryIdx++) {
        SQWORD sqC = inputSQWORD();
        SQWORD sqX = inputSQWORD();

        switch (sqC) {
        case 1:
            setCuttingPos.insert(sqX);
            break;
        case 2:
            {
                SQWORD sqValUpper = setCuttingPos.findUpper(sqX);
                SQWORD sqValLower = setCuttingPos.findLower(sqX);

                printf("%lld\n", sqValUpper - sqValLower);
            }
            break;
        }
    }
    return 0;
}

