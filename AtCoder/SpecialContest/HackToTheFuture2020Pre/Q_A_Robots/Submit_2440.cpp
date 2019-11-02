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
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline DOUBLE MIN(DOUBLE a, DOUBLE b) { return a < b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

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
/**
 *  mod による操作ライブラリ
 */
#define ANS_MOD (1000000007)

class MODINT {
    static SQWORD MOD;
    SQWORD m_x;

public:
    MODINT(SQWORD val) {
        m_x = (val % MOD + MOD) % MOD;
    };
    MODINT() {
        m_x = 0;
    }
    static void Init(SQWORD sqMod) {
        MOD = sqMod;
    }

	MODINT& operator+= (const MODINT a)
    {
        m_x = (m_x + a.m_x) % MOD; 
        return *this;
    };
	MODINT& operator-= (const MODINT a)
    { 
        m_x = (m_x - a.m_x + MOD) % MOD; 
        return *this;
    };
	MODINT& operator*= (const MODINT a)
    {
        m_x = (m_x * a.m_x) % MOD;
        return *this;
    };
    MODINT pow(SQWORD t) const {
        if (!t) return 1;
        MODINT a = pow(t>>1);
        a *= a;
        if (t&1) a *= *this;
        return a;
    }
	MODINT operator+ (const MODINT a) const {
		MODINT res(*this);
		return (res += a);
	}
	MODINT operator- (const MODINT a) const {
		MODINT res(*this);
		return (res -= a);
	}
	MODINT operator* (const MODINT a) const {
		MODINT res(*this);
		return (res *= a);
	}
	MODINT operator/ (const MODINT a) const {
		MODINT res(*this);
		return (res /= a);
	}

    /* 逆元 */
    MODINT inv() const {
        return pow(MOD-2);
    }

    /* 除算 */
    MODINT& operator/=(const MODINT a) {
        return (*this) *= a.inv();
    } 

    /* 整数版 */
	MODINT& operator+= (const SQWORD a) {*this += MODINT(a); return *this;};
	MODINT& operator-= (const SQWORD a) {*this -= MODINT(a); return *this;};
	MODINT& operator*= (const SQWORD a) {*this *= MODINT(a); return *this;};
	MODINT& operator/= (const SQWORD a) {*this /= MODINT(a); return *this;};

    SQWORD getVal() { return m_x; };
};
SQWORD MODINT::MOD = ANS_MOD;


/*----------------------------------------------*/
struct POSITION {
    MODINT sqRow;
    MODINT sqCol;
};

enum DIRECTION {
    DIR_U,
    DIR_D,
    DIR_L,
    DIR_R,
    DIR_NONE,
    DIR_MIXED,
    DIR_NUM
};

struct ROBOT {
    POSITION pos;
    DIRECTION dir;
};

static DIRECTION getDirection(char c)
{
    DIRECTION dir;
    switch (c) {
    case 'U':
        dir = DIR_U;
        break;
    case 'D':
        dir = DIR_D;
        break;
    case 'L':
        dir = DIR_L;
        break;
    case 'R':
        dir = DIR_R;
        break;
    }
    return dir;
}

static char directionToChar(DIRECTION d)
{
    char c;
    switch (d) {
    case DIR_U:
        c = 'U';
        break;
    case DIR_D:
        c = 'D';
        break;
    case DIR_L:
        c = 'L';
        break;
    case DIR_R:
        c = 'R';
        break;
    default:
        c = ' ';
        break;
    }
    return c;
}

static char directionToDisplay(DIRECTION d)
{
    char c;
    switch (d) {
    case DIR_U:
        c = '^';
        break;
    case DIR_D:
        c = '_';
        break;
    case DIR_L:
        c = '<';
        break;
    case DIR_R:
        c = '>';
        break;
    default:
        c = '.';
        break;
    }
    return c;
}

static DIRECTION invertDirection(DIRECTION d)
{
    DIRECTION dir;
    switch (d) {
    case DIR_U:
        dir = DIR_D;
        break;
    case DIR_D:
        dir = DIR_U;
        break;
    case DIR_L:
        dir = DIR_R;
        break;
    case DIR_R:
        dir = DIR_L;
        break;
    }
    return dir;
}

static void turnDirection(DIRECTION d, DIRECTION &d1, DIRECTION &d2)
{
    switch (d) {
    case DIR_U:
    case DIR_D:
        d1 = DIR_R;
        d2 = DIR_L;
        break;
    case DIR_L:
    case DIR_R:
        d1 = DIR_U;
        d2 = DIR_D;
        break;
    }
}



struct SearchQueEntry {
    POSITION    posCur;
    DIRECTION   eCurDir;
};

#define MAX_N   (40)

static vector<vector<DIRECTION>> s_aaeDir(MAX_N, vector<DIRECTION>(MAX_N));
static vector<vector<DIRECTION>> s_aaeInstruction(MAX_N, vector<DIRECTION>(MAX_N));
static vector<vector<DIRECTION>> s_aaeRobotInitDir(MAX_N, vector<DIRECTION>(MAX_N));
static vector<vector<bool>>      s_aabIsBlock(MAX_N, vector<bool>(MAX_N));
static vector<vector<bool>>      s_aabIsRobot(MAX_N, vector<bool>(MAX_N));
static vector<vector<bool>>      s_aabIsNeedTurnDir(MAX_N, vector<bool>(MAX_N));
static vector<vector<bool>>      s_aabIsNeedRobotInitDir(MAX_N, vector<bool>(MAX_N));
static vector<vector<bool>>      s_aabIsNeedRobotNeedForce(MAX_N, vector<bool>(MAX_N));
static vector<DIRECTION>         s_aavecRobotDir[MAX_N][MAX_N];
static queue<SearchQueEntry>    queSearch;

static void getInput(
    SQWORD              sqM,
    SQWORD              sqB,
    vector<ROBOT>       &vecRobot,
    vector<POSITION>    &vecBlock) 
{
    for (SQWORD sqRobotIdx = 0; sqRobotIdx < sqM; sqRobotIdx++) {
        SQWORD sqRow = inputSQWORD();
        SQWORD sqCol = inputSQWORD();
        char c;
        cin >> c;
        DIRECTION dir = getDirection(c);
        vecRobot.push_back(ROBOT{sqRow, sqCol, dir});

        s_aavecRobotDir[sqRow][sqCol].emplace_back(dir);
    }
    for (SQWORD sqBlockIdx = 0; sqBlockIdx < sqB; sqBlockIdx++) {
        SQWORD sqX = inputSQWORD();
        SQWORD sqY = inputSQWORD();
        vecBlock.push_back(POSITION{sqX, sqY});
    }
}

static POSITION GetNextPos(
    POSITION      pos, 
    DIRECTION     eDir)
{
                         /*  U      D       L       R*/
    POSITION posDir[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    POSITION diff = posDir[eDir];

    MODINT sqNextRow = pos.sqRow + diff.sqRow;
    MODINT sqNextCol = pos.sqCol + diff.sqCol;

    POSITION ret = POSITION{sqNextRow, sqNextCol};
    return ret;
}


vector<ROBOT>       s_vecRobot;
vector<POSITION>    s_vecBlock; 
POSITION            s_PosGoal;

static void backtrackRoute(SQWORD sqInitRow, SQWORD sqInitCol, DIRECTION eInitDir)
{
    POSITION posCur = {sqInitRow, sqInitCol};
    DIRECTION eDirCur = eInitDir;
    while(1) {
        POSITION posNext = GetNextPos(posCur, eDirCur);

        SQWORD sqNextRow = posNext.sqRow.getVal();
        SQWORD sqNextCol = posNext.sqCol.getVal();

        if ((s_PosGoal.sqRow.getVal() == sqNextRow) && (s_PosGoal.sqCol.getVal() == sqNextCol)) {
            break;
        }
        if (s_aaeDir[sqNextRow][sqNextCol] != eDirCur) {
            if (s_aabIsNeedTurnDir[sqNextRow][sqNextCol]) {
                break;
            }
            eDirCur = s_aaeDir[sqNextRow][sqNextCol];
            s_aabIsNeedTurnDir[sqNextRow][sqNextCol] = true;
        }
        posCur = posNext;
    }
}


static void search1Layer(
    POSITION    posStart,
    DIRECTION   eDir)
{
//    printf("---\n");
    POSITION posCur = posStart;
    while (1) {
        SQWORD sqCurRow = posCur.sqRow.getVal();
        SQWORD sqCurCol = posCur.sqCol.getVal();

//        printf("loop: %lld %lld\n", sqCurRow, sqCurCol);     

        if (s_aabIsBlock[sqCurRow][sqCurCol]) {
            break;
        }
        if (DIR_NONE != s_aaeDir[sqCurRow][sqCurCol]) {
            /* すでに来ている */
            break;
        }        

        bool bIsRobotExist = false;
        if (s_aabIsRobot[sqCurRow][sqCurCol]) {
            bIsRobotExist = true;
        }

        DIRECTION eInstDir = invertDirection(eDir);;
        s_aaeDir[sqCurRow][sqCurCol] = eInstDir;

        queSearch.push(SearchQueEntry{posCur, eDir});

        s_aabIsNeedRobotInitDir[sqCurRow][sqCurCol] 
                = bIsRobotExist && ((eInstDir != s_aaeRobotInitDir[sqCurRow][sqCurCol])
                                    || s_aabIsNeedRobotNeedForce[sqCurRow][sqCurCol]);
        if (bIsRobotExist) {
            backtrackRoute(sqCurRow, sqCurCol, eInstDir);
        }

        /* 更新 */
        posCur = GetNextPos(posCur, eDir);
    }
}

static void procSetInstruction(SQWORD sqN)
{
    for (SQWORD sqRow = 0; sqRow < sqN; sqRow++) {
        for (SQWORD sqCol = 0; sqCol < sqN; sqCol++) {
            if (s_aabIsNeedTurnDir[sqRow][sqCol] || s_aabIsNeedRobotInitDir[sqRow][sqCol]) {
                s_aaeInstruction[sqRow][sqCol] = s_aaeDir[sqRow][sqCol];
            } 
        }
    }
}


struct INSTRUCTION {
    SQWORD sqRow;
    SQWORD sqCol;
    char c;
};

void generateAnswer(vector<INSTRUCTION> &vecAns, POSITION posGoal, SQWORD sqN)
{
    for (SQWORD sqI = 0; sqI < sqN; sqI++) {
        for (SQWORD sqJ = 0; sqJ < sqN; sqJ++) {
            if (s_aaeInstruction[sqI][sqJ] == DIR_NONE) {
                continue;
            }

            if ((sqI == posGoal.sqRow.getVal()) && (sqJ == posGoal.sqCol.getVal())) {
                continue;
            }
            vecAns.push_back(INSTRUCTION{sqI, sqJ, directionToChar(s_aaeInstruction[sqI][sqJ])});
        }
    }   

}

void printBoard(SQWORD sqN)
{
    for (SQWORD sqI = 0; sqI < sqN; sqI++) {
        for (SQWORD sqJ = 0; sqJ < sqN; sqJ++) {
            printf("%d ", s_aaeDir[sqI][sqJ]);
        }
        printf("\n");
    }
    printf("----\n");
    for (SQWORD sqI = 0; sqI < sqN; sqI++) {
        for (SQWORD sqJ = 0; sqJ < sqN; sqJ++) {
            if (s_aabIsBlock[sqI][sqJ]) {
                printf("#");
            } else {
                printf("%c", directionToDisplay(s_aaeDir[sqI][sqJ]));
            }
        }
        printf("\n");
    }
}

static bool searchChgDirDirection(SQWORD sqStartRow, SQWORD sqStartCol, DIRECTION eDir)
{
    POSITION posCur = {sqStartRow, sqStartCol};
    posCur = GetNextPos(posCur, eDir);
    while (1) {
        SQWORD sqCurRow = posCur.sqRow.getVal();
        SQWORD sqCurCol = posCur.sqCol.getVal();

        if (s_aabIsBlock[sqCurRow][sqCurCol]) {
            return false;
        }
        if (s_aaeInstruction[sqCurRow][sqCurCol] != DIR_NONE) {
            return true;
        }    

        posCur = GetNextPos(posCur, eDir);
    } 
    return false;
}
    
int solve1Pattern(SQWORD sqN, vector<INSTRUCTION> &vecAns, vector<DIRECTION> veceSearch, SQWORD sqIter)
{
    while (0 < queSearch.size()) {
        queSearch.pop();
    }

    for (SQWORD sqI = 0; sqI < sqN; sqI++) {
        for (SQWORD sqJ = 0; sqJ < sqN; sqJ++) {
            s_aaeDir[sqI][sqJ] = DIR_NONE;
            s_aaeInstruction[sqI][sqJ] = DIR_NONE;
            s_aaeRobotInitDir[sqI][sqJ] = DIR_NONE;

            s_aabIsBlock[sqI][sqJ] =false;
            s_aabIsRobot[sqI][sqJ] =false;
            s_aabIsNeedTurnDir[sqI][sqJ] =false;
            s_aabIsNeedRobotInitDir[sqI][sqJ] =false;
            s_aabIsNeedRobotNeedForce[sqI][sqJ] = false;
        }
    }
    for (auto m: s_vecRobot) {
        SQWORD sqRow = m.pos.sqRow.getVal();
        SQWORD sqCol = m.pos.sqCol.getVal();
        s_aabIsRobot[sqRow][sqCol] = true;
        
        DIRECTION prevDir = s_aaeRobotInitDir[sqRow][sqCol];
        if (DIR_NONE == prevDir) {
            s_aaeRobotInitDir[sqRow][sqCol] = m.dir;
        } else if (prevDir != m.dir) {
            s_aabIsNeedRobotNeedForce[sqRow][sqCol] = true;
        } 
    }

    for (auto b: s_vecBlock) {
        s_aabIsBlock[b.sqRow.getVal()][b.sqCol.getVal()] = true;
    }

    /* 初期探索 */
    for (auto d: veceSearch) {
        search1Layer(GetNextPos(s_PosGoal, d), d);    
    }

    while (!queSearch.empty()) {
        SearchQueEntry searchEntry = queSearch.front();
        queSearch.pop();

        DIRECTION d1, d2;
        turnDirection(searchEntry.eCurDir, d1, d2);
        if (sqIter == 0) {
            search1Layer(GetNextPos(searchEntry.posCur, d1), d1);
            search1Layer(GetNextPos(searchEntry.posCur, d2), d2);
        } else {
            search1Layer(GetNextPos(searchEntry.posCur, d2), d2);
            search1Layer(GetNextPos(searchEntry.posCur, d1), d1);
        }
    }

    procSetInstruction(sqN);

#if 0
    /* 方向指示が外せるかをロボットの直下に対して確認 */
    for (SQWORD sqI = 0; sqI < sqN; sqI++) {
        for (SQWORD sqJ = 0; sqJ < sqN; sqJ++) {
            if (0 < s_aavecRobotDir[sqI][sqJ].size()) {
                bool bNeedInstruction = false;
                for (auto dir: s_aavecRobotDir[sqI][sqJ]) {
                    if (!searchChgDirDirection(sqI, sqJ, dir)) {
                        bNeedInstruction = true;
                    }
                }
                if ((!bNeedInstruction) && !s_aabIsNeedTurnDir[sqI][sqJ]) {
//                    printf(".");
                    s_aaeInstruction[sqI][sqJ] = DIR_NONE;
                }
            }
        }
    }
#endif

    generateAnswer(vecAns, s_PosGoal, sqN);

#if 0
    printBoard();
#endif

    return 0;
}

int main(void)
{
    vector<vector<DIRECTION>> vvecdSearch = {
        {DIR_U, DIR_D, DIR_L, DIR_R},
        {DIR_U, DIR_D, DIR_R, DIR_L},
        {DIR_U, DIR_L, DIR_D, DIR_R},
        {DIR_U, DIR_L, DIR_R, DIR_D},
        {DIR_D, DIR_U, DIR_L, DIR_R},
        {DIR_D, DIR_U, DIR_R, DIR_L},
        {DIR_D, DIR_L, DIR_U, DIR_R},
        {DIR_D, DIR_L, DIR_R, DIR_U},
        {DIR_L, DIR_U, DIR_D, DIR_R},
        {DIR_L, DIR_U, DIR_R, DIR_D},
        {DIR_L, DIR_D, DIR_U, DIR_R},
        {DIR_L, DIR_D, DIR_R, DIR_U},
        {DIR_R, DIR_U, DIR_D, DIR_L},
        {DIR_R, DIR_U, DIR_L, DIR_D},
        {DIR_R, DIR_D, DIR_U, DIR_L},
        {DIR_R, DIR_D, DIR_L, DIR_U},
    };

    SQWORD sqN = inputSQWORD();
    SQWORD sqM = inputSQWORD();
    SQWORD sqB = inputSQWORD();

    SQWORD sqGx = inputSQWORD();
    SQWORD sqGy = inputSQWORD();

    MODINT::Init(sqN);
    
    s_PosGoal = {sqGx, sqGy};
    getInput(sqM, sqB, s_vecRobot, s_vecBlock);

    vector<INSTRUCTION> vecAns;
    for (auto vecSearch: vvecdSearch) {
        for (SQWORD sqIter = 0; sqIter < 2; sqIter++) {
            vector<INSTRUCTION> vecAnsTmp;
            solve1Pattern(sqN, vecAnsTmp, vecSearch, sqIter);

            if ((0 == vecAns.size()) || (vecAnsTmp.size() < vecAns.size())) {
                vecAns = vecAnsTmp;
            }
        }
    }
   
    printf("%lld\n", vecAns.size());
    for (auto inst: vecAns) {
        printf("%lld %lld %c\n", inst.sqRow, inst.sqCol, inst.c);
    }

    return 0;
}
