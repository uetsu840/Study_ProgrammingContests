#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef unsigned long ulong;


int check(char *param_1, unsigned int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  size_t sVar4;
  char *__dest;
  
  sVar4 = strlen(param_1);
  iVar3 = (int)sVar4;
  if (iVar3 == 1) {
#if 0
    if (table[(int)param_2] != *param_1) {
      return 1;
    }
#endif
    printf("pos: %d, str[%d]\n", param_2, (unsigned int)(*param_1 - '0'));
    ;
  }
  else {
    iVar1 = iVar3 / 2;
//    printf("------------->%d %d [%s][%s]\n", iVar3, iVar1, param_1, param_1 + iVar1);
    __dest = (char *)malloc((long)iVar1 + 1);
    strncpy(__dest,param_1,(long)iVar1);
    __dest[iVar1] = '\0';
    iVar2 = check(__dest,(ulong)param_2);
    if (iVar2 == 1) {
      return 1;
    }
//    printf("<<\n");
    int iLen2 = iVar3 - iVar1;
    __dest = (char *)malloc((long)iLen2 + 1);
    strncpy(__dest,param_1 + iVar1,(long)(iVar3 - iVar1));
    __dest[iLen2] = '\0';
    param_2 = iVar1 * iVar1 + param_2;
    iVar3 = check(__dest,(ulong)param_2);
    if (iVar3 == 1) {
      return 1;
    }
  }
  return 0;
}

int main(void)
{
    char s_acCharTest[0x27];
    for (int i = 0; i< 0x26; i++) {
        s_acCharTest[i] = '0' + i;
    }
    s_acCharTest[0x26] = '\0';
    check(s_acCharTest, 0);

    return 0;
}