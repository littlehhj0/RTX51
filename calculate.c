//#include "calculate.h"
#include <string.h>
#define maxn 12

unsigned char infixList[maxn][3];
unsigned char suffix[maxn][3];
int p = 0;
char tmp[maxn];
unsigned char p1 = 0;
unsigned char ii=0,j=0;
int infixNum;
unsigned char tot = 0, n;
//把字符串根据操作数、运算符、括号拆开，存储到infixList数组中
int toInfixExpression(char *s)
{
    tot = 0;
		n = strlen(s);
    for (ii = 0; ii < n; ii++) {
        if (s[ii] >= '0' && s[ii] <= '9') {
            p               = 0;
            infixList[tot][p++] = s[ii];
            while (ii + 1 < n && s[ii + 1] >= '0' && s[ii + 1] <= '9') {
                infixList[tot][p++] = s[++ii];
            }
            tot++;
        } else {
            infixList[tot++][0] = s[ii];
        }
    }
    return tot;
}

 //获得优先级
int getPriority(char c)
{
    if (c == 13 || c == 14)
        return 1;
    else if (c == 15 || c == 16)
        return 2;
    else
        return 0; // 处理'('的情况
}

 //转换成后缀表达式
 void parseSuffixExpression(char s[][10], int n)
 {

     for (ii = 0; ii < n; ii++) {
         if (s[ii][0] >= '0' && s[ii][0] <= '9') {
             strcpy(suffix[p++], s[ii]);
         } else if (s[ii][0] == 11) {
             tmp[p1++] = s[ii][0];
         } else if (s[ii][0] == 12) {
             while (tmp[p1 - 1] != 11) {
                 suffix[p++][0] = tmp[--p1];
             }
             p1--;
         } else {
             while (p1 != 0 && getPriority(tmp[p1 - 1]) >= getPriority(s[ii][0])) {
                 suffix[p++][0] = tmp[--p1];
             }
             tmp[p1++] = s[ii][0];
         }
     }
     while (p1 != 0) {
         suffix[p++][0] = tmp[--p1];
     }
 }
int k;
int ans = 0;
// 计算一个以字符串形式存储的数的值
int parseInt(char *s)
{
      k= strlen(s);
     ans = 0;
    for (ii = 0; ii < k; ii++) {
        ans = ans * 10 + (s[ii] - '0');
    }
    return ans;
}

char b[11];
int point = 0;
// 将数k转化成字符串形式存储到指针s开始的地方
void parseString(char *s, int k)
{
    point = 0;
    do {
        b[point++] = (char)('0' + k % 10);
        k /= 10;
    } while (k != 0);
    for (ii = point - 1, j = 0; ii >= 0; ii--, j++) {
        *(s + j) = b[ii];
    }
    *(s + point) = '\0';
}
char t[maxn][5];
int m = 1;
int k;
// 计算后缀表达式的值
int calculate()
{
    point = 0;
    for (ii = 0; ii < p; ii++) {
        k = strlen(suffix[ii]);
        if (suffix[ii][0] >= '0' && suffix[ii][0] <= '9') {
            strcpy(t[point++], suffix[ii]);
            t[point - 1][k] = '\0';
        } else {
            int b   = parseInt(t[--point]);
            int a   = parseInt(t[--point]);
            int res = 0;
            if (suffix[ii][0] == 13) {
                res = a + b;
                // printf("(%d) %d + %d = %d\n", m++, a, b, res);
            } else if (suffix[ii][0] == 14) {
                res = a - b;
                // printf("(%d) %d - %d = %d\n", m++, a, b, res);
            } else if (suffix[ii][0] == 15) {
                res = a * b;
                // printf("(%d) %d * %d = %d\n", m++, a, b, res);
            } else if (suffix[ii][0] == 16) {
                if (b == 0) {
                    // printf("除数为零出错！！！\n");
                    return -1;
                }
                res = a / b;
                // printf("(%d) %d / %d = %d\n", m++, a, b, res);
            }
            parseString(t[point], res);
            point++;
        }
    }
    return parseInt(t[0]);
}

 int ans_calculate(char *s)
 {
     // 输入字符串
     p = p1 = 0;
     // printf("------------------------------\n");
     // printf("请输入待计算的表达式:");
     // scanf("%s", s);
     infixNum = toInfixExpression(s);
     parseSuffixExpression(infixList, infixNum);

     // printf("运算过程如下所示：\n");
     // int ans = calculate();
     // printf("运算结果为: %d\n", ans);
     // printf("------------------------------\n");
     // printf("请选择是否继续(输入0表示退出,否则继续):");
     return calculate();
     // printf("\n");
 }
