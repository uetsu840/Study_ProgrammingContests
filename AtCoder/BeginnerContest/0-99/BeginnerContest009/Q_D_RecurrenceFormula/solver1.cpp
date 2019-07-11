#pragma GCC optimize("O3")

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
using namespace std;

using QWORD = uint64_t;
using SQWORD = int64_t;
using DWORD = uint32_t;
using SDWORD = int32_t;
using WORD = uint16_t;
using SWORD = int16_t;
using BYTE = uint8_t;
using SBYTE = int8_t;
using DOUBLE = double;
using FLOAT = float;

#define MIN_SDWORD (-2147483648)
#define MAX_SDWORD (2147483647)
#define MIN_SBYTE (-128)
#define MAX_SBYTE (127)

#define MIN_SQWORD (0x8000000000000000)
#define MAX_SQWORD (0x7FFFFFFFFFFFFFFF)

#define MAX_QWORD (0xFFFFFFFFFFFFFFFF)
#define MAX_DWORD (0xFFFFFFFF)
#define MAX_WORD (0xFFFF)
#define MAX_BYTE (0xFF)

#define ArrayLength(a) (sizeof(a) / sizeof(a[0]))

static inline QWORD MAX(QWORD a, QWORD b)
{
    return a > b ? a : b;
}
static inline DWORD MAX(DWORD a, DWORD b) { return a > b ? a : b; }
static inline SDWORD MAX(SDWORD a, SDWORD b) { return a > b ? a : b; }
static inline QWORD MIN(QWORD a, QWORD b) { return a < b ? a : b; }
static inline DWORD MIN(DWORD a, DWORD b) { return a < b ? a : b; }
static inline SDWORD MIN(SDWORD a, SDWORD b) { return a < b ? a : b; }

#define BYTE_BITS (8)
#define WORD_BITS (16)
#define DWORD_BITS (32)
#define QWORD_BITS (64)

using M_BOOL = bool;
#define M_TRUE (true)
#define M_FALSE (false)
#define DIVISOR (1000000007)

static inline void inputString(char *pcStr)
{
    char *pcCur = pcStr;
    for (;;)
    {
        char c = getchar();
        if (('\n' == c) || (EOF == c))
        {
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
    M_BOOL bRead = M_FALSE;
    for (;;)
    {
        char c = getchar();
        if (!bRead)
        {
            if ('-' == c)
            {
                sqMultiplier = -1;
            }
        }
        if (('0' <= c) && (c <= '9'))
        {
            sqNumber *= 10LL;
            sqNumber += (SQWORD)(c - '0');
            bRead = M_TRUE;
        }
        else
        {
            if (bRead)
            {
                return sqNumber * sqMultiplier;
            }
        }
    }
}

static inline SDWORD inputSDWORD(void)
{
    SDWORD lNumber = 0;
    SDWORD lMultiplier = 1;
    M_BOOL bRead = M_FALSE;
    for (;;)
    {
        char c = getchar();
        if (!bRead)
        {
            if ('-' == c)
            {
                lMultiplier = -1;
            }
        }
        if (('0' <= c) && (c <= '9'))
        {
            lNumber *= 10;
            lNumber += (c - '0');
            bRead = M_TRUE;
        }
        else
        {
            if (bRead)
            {
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
    M_BOOL bRead = M_FALSE;
    for (;;)
    {
        char c = getchar();
        if (!bRead)
        {
            if ('-' == c)
            {
                dMultiplier = -1;
            }
        }
        if ('.' == c)
        {
            pdCur = &dFrac;
        }
        else if (('0' <= c) && (c <= '9'))
        {
            (*pdCur) *= 10;
            (*pdCur) += (DOUBLE)(c - '0');
            bRead = M_TRUE;
            if (pdCur == &dFrac)
            {
                dwFpCnt++;
            }
        }
        else
        {
            if (bRead)
            {
                return dMultiplier * (dInt + dFrac / (pow((DOUBLE)10.0, (DOUBLE)dwFpCnt)));
            }
        }
    }
}

class Matrix{

  int row;  //行
  int column;  //列

  double** p_top; //配列の最初を指すポインタ

 public:
  Matrix(int i=1, int j=1);//コンストラクタ
  Matrix(const Matrix &cp);//コピーコンストラクタ

  ~Matrix();//デストラクタ

  int row_size(){ return(row); }
  int column_size(){ return(column); }

  void change_size(int i, int j);//行列のサイズを変更する

  //演算子のオーバーロード
  double* &operator[](int i){ return(p_top[i]); }
  Matrix operator=(const Matrix &a);
  Matrix operator+(const Matrix &a);
  Matrix operator-(const Matrix &a);
  Matrix operator*(const Matrix &a);

  friend Matrix operator*(const Matrix &a, double b);
  friend Matrix operator*(double b, const Matrix &a);

  //行列の変換など
  void unit_matrix();//単位行列にする
  Matrix transposed();//転置行列をかえす
};

//---------------------------------
//     通常のコンストラクタ
//---------------------------------
Matrix::Matrix(int i, int j)
{
  //  i,j のチェック
  if( i<1 || j<1 ){
    cerr << "err Matrix::Matrix" <<endl;
    exit(1);
  }

  row = i;
  column = j;

  //  配列のメモリ領域を動的に確保
  p_top = new double*[row+1];
  *p_top = new double[row*column+1]; 
  // +1 ga daiji kore tukenaito bagu ga deta
  for(int k=1; k<=row; k++)
    *(p_top+k) = *p_top+((k-1)*column);

  //  値の初期化
  for(int k1=1; k1<=row; k1++){
    for(int k2=1; k2<=column; k2++){
      p_top[k1][k2] = 0;
    }
  }
}

//---------------------------------
//     コピーコンストラクタ
//---------------------------------
Matrix::Matrix(const Matrix &cp)
{
  row = cp.row;
  column = cp.column;

  //  配列のメモリ領域を動的に確保
  p_top = new double*[row+1];
  *p_top = new double[row*column+1]; 
  // +1 ga daiji kore tukenaito bagu ga deta
  for(int k=1; k<=row; k++)
    *(p_top+k) = *p_top+((k-1)*column);

  //  値のコピー
  for(int k1=1; k1<=row; k1++){
    for(int k2=1; k2<=column; k2++){
      p_top[k1][k2] = cp.p_top[k1][k2];
    }
  }
}

//----------------------
//   デストラクタ
//----------------------
Matrix::~Matrix()
{
  delete [] *p_top; 
  delete [] p_top;
}

//------------------------------
//   行列の大きさを変える  値は０
//------------------------------
void Matrix::change_size(int i, int j)
{
  //  i,j のチェック
  if( i<1 || j<1 ){
    cerr << "err Matrix::change_size" <<endl;
    exit(1);
  }

  delete [] *p_top; 
  delete [] p_top;

  row = i;
  column = j;

  //  配列のメモリ領域を動的に確保
  p_top = new double*[row+1];
  *p_top = new double[row*column+1]; 
  // +1 ga daiji kore tukenaito bagu ga deta
  for(int k=1; k<=row; k++)
    *(p_top+k) = *p_top+((k-1)*column);

  //  値の初期化
  for(int k1=1; k1<=row; k1++){
    for(int k2=1; k2<=column; k2++){
      p_top[k1][k2] = 0;
    }
  }

}

//------------------------------------
//     代入
//------------------------------------
Matrix Matrix::operator=(const Matrix &a)
{
  if( row != a.row || column != a.column ){
    change_size(a.row, a.column);
  }

  for(int i=1; i<=row; i++){
    for(int j=1; j<=column; j++){
      p_top[i][j] = a.p_top[i][j];
    }
  }
  return(*this);
}

//------------------------------------
//       行列の加算
//------------------------------------
Matrix Matrix::operator+(const Matrix &a)
{
  if( row != a.row || column != a.column ){
    cerr << "err Matrix::operator+" <<endl;
    cerr << "  not equal matrix size" <<endl;
    exit(0);
  }

  Matrix r(row, column);
  for(int i=1; i<=row; i++){
    for(int j=1; j<=column; j++){
      r.p_top[i][j] = p_top[i][j] + a.p_top[i][j];
    }
  }
  return(r);
}

//------------------------------------
//       行列の減算
//------------------------------------
Matrix Matrix::operator-(const Matrix &a)
{
  if( row != a.row || column != a.column ){
    cerr << "err Matrix::operator-" <<endl;
    cerr << "  not equal matrix size" <<endl;
    exit(0);
  }

  Matrix r(row, column);
  for(int i=1; i<=row; i++){
    for(int j=1; j<=column; j++){
      r.p_top[i][j] = p_top[i][j] - a.p_top[i][j];
    }
  }
  return(r);
}

//------------------------------------
//       行列の積
//------------------------------------
Matrix Matrix::operator*(const Matrix &a)
{
  if( column != a.row ){
    cerr << "err Matrix::operator*" <<endl;
    cerr << "  not equal matrix size" <<endl;
    exit(0);
  }

  Matrix r(row, a.column);
  for(int i=1; i<=row; i++){
    for(int j=1; j<=a.column; j++){
      for(int k=1; k<=column; k++){
	r.p_top[i][j] += p_top[i][k] * a.p_top[k][j];
      }
    }
  }
  return(r);
}

//--------------------------------------
//       行列の定数倍
//--------------------------------------
Matrix operator*(const Matrix &a, double b)
{
  Matrix r(a.row, a.column);
  for(int i=1; i<=a.row; i++){
    for(int j=1; j<=a.column; j++){
      r[i][j] = b * a.p_top[i][j];
    }
  }
  return(r);
}
Matrix operator*(double b, const Matrix &a)
{
  Matrix r(a.row, a.column);
  for(int i=1; i<=a.row; i++){
    for(int j=1; j<=a.column; j++){
      r[i][j] = b * a.p_top[i][j];
    }
  }
  return(r);
}

//----------------------------------------
//  単位行列にする
//----------------------------------------
void Matrix::unit_matrix()
{
  if(row != column){
    cerr <<"err in Matrix::unit_matrix()" <<endl;
    exit(0);
  }

  int n = row;
  double** a = p_top;
  for(int i=1; i<=n; i++){
    for(int j=1; j<=n; j++){
      a[i][j] = 0;
      if(i == j) a[i][j] = 1;
    }
  }

}

//----------------------------------------
//  転置行列をかえす
//----------------------------------------
Matrix Matrix::transposed()
{
  Matrix t(column, row);
  double** a = p_top;

  for(int i=1; i<=row; i++){
    for(int j=1; j<=column; j++){
      t[j][i] = a[i][j];
    }
  }
  return(t);
}


int main()
{
    Matrix m(3, 2);

    
    return 0;
}