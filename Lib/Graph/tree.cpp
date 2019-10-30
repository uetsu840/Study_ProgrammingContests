#include "common.h"

class TREE {
public: 
    TREE(SQWORD sqNodeNum)
    {
        m_vvEdge.resize(sqNodeNum + 1);
    }
private:
    vector<vector<SQWORD>> m_vvEdge;
    /**
     *  target までのパスを探す
     */
    bool searchPathExec(
        SQWORD sqParent, 
        SQWORD sqCur,
        SQWORD sqTarget,
        vector<SQWORD> &vecsqPath) 
    {
        if (sqTarget == sqCur) {
            vecsqPath.emplace_back(sqCur);
            return true;
        }

        for (auto next: m_vvEdge[sqCur]) {
            if (sqParent != next) {
                if (searchPathExec(sqCur, next, sqTarget, vecsqPath)) {
                    vecsqPath.emplace_back(sqCur);
                    return true;
                }
            }
        }

        return false;
    }

    /**
     *  ノードの数を数える
     */
    SQWORD countNodeExec(
        SQWORD sqParent, 
        SQWORD sqCur)
    {
        SQWORD sqCnt = 1;
        for (auto next: m_vvEdge[sqCur]) {
            if (sqParent != next) {
                sqCnt += countNodeExec(sqCur, next);
            }
        }
        return sqCnt;
    }
public:
    void AddEdge(SQWORD sqA, SQWORD sqB)
    {
        m_vvEdge[sqA].emplace_back(sqB);
        m_vvEdge[sqB].emplace_back(sqA);
    }

    /* 
    * 辺を消す。エッジは隣接していること。遅いので注意。
    * また、当たり前だけど消した後は木じゃなくなる。
    */
    void EraseEdge(SQWORD sqA, SQWORD sqB)
    {
        m_vvEdge[sqA].erase(std::find(m_vvEdge[sqA].begin(), m_vvEdge[sqA].end(), sqB));
        m_vvEdge[sqB].erase(std::find(m_vvEdge[sqB].begin(), m_vvEdge[sqB].end(), sqA));

    }

    void SearchPath(SQWORD sqFrom, SQWORD sqTo, vector<SQWORD> &vecsqPath)
    {
        searchPathExec(-1, sqFrom, sqTo, vecsqPath);
    }

    SQWORD CountNode(SQWORD sqRoot)
    {
        return countNodeExec(-1, sqRoot);
    }
};
