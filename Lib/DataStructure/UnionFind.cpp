#include "common.h"

/*----------------------------------------------*/

typedef struct {
    vector<DWORD>   vdwPar;
    vector<DWORD>   vdwRank;
    vector<DWORD>   vdwCnt;
    DWORD   dwSize;

    void initUnionFind(
        DWORD dwSize)
    {
        dwSize = dwSize;
        vdwPar.resize(dwSize);
        vdwRank.resize(dwSize);
        vdwCnt.resize(dwSize);
    
        for (DWORD dwIdx = 0; dwIdx < dwSize; dwIdx++) {
            vdwPar[dwIdx]  = dwIdx;
            vdwRank[dwIdx] = 0;
            vdwCnt[dwIdx]  = 1;
        }
    }

    DWORD ufGetCnt(DWORD sqIdx) {
        return vdwCnt[ufGetRoot(sqIdx)];
    }


    DWORD ufGetParent(DWORD dwIdx) const
    {
        return vdwPar[dwIdx];
    }

    DWORD ufGetRank(DWORD dwIdx) const
    {
        return vdwRank[dwIdx];
    }

    void ufSetParent(DWORD dwIdx, DWORD dwParent)
    {
        vdwPar[dwIdx] = dwParent; 
        if (ufGetRank(dwIdx) == ufGetRank(dwParent)) {
            (vdwRank[dwParent])++;
        }
    }

    DWORD ufGetRoot(DWORD dwIdx) const
    {
        if (ufGetParent(dwIdx) == dwIdx) {
            return dwIdx;
        } else {
            DWORD dwParent = ufGetParent(dwIdx);
            DWORD dwRoot = ufGetRoot(dwParent);
            return dwRoot;
        }
    }

    bool ufUnite(DWORD dwX, DWORD dwY)
    {
        DWORD dwRootX = ufGetRoot(dwX);
        DWORD dwRootY = ufGetRoot(dwY);

        if (dwRootX == dwRootY) {
            return false;
        }

        if (ufGetRank(dwRootX) < ufGetRank(dwRootY)) {
            ufSetParent(dwRootX, dwRootY);
            (vdwCnt[dwRootY]) += (vdwCnt[dwRootX]);
        } else {
            ufSetParent(dwRootY, dwRootX);
            (vdwCnt[dwRootX]) += (vdwCnt[dwRootY]);
        }

        return true;
    }

    bool ufIsSame(DWORD dwX, DWORD dwY) const
    {
        return (ufGetRoot(dwX)  == ufGetRoot(dwY));
    }
} ST_UNION_FIND;
