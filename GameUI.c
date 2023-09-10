#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <locale.h>
//游戏UI显示代码
//宏
#define ROW 12
#define COL 16
#define WALL 1
#define SNAKE 2
#define FOOD 3

#define MYUNICODE //宽字节版本
#ifdef MYUNICODE //将所需要的打印输出函数都换成宽字节版本的
  #define TCHAR   wchar_t
  #define TGETC   getwchar
  #define TPRINT  wprintf
  #define TSCAN   wscanf
  #define TRET    65535
  #define TLOCALE _wsetlocale(LC_ALL, T("chs"));
  #define T(sz)   L ## sz
#else
  #define TCHAR  char
  #define TGETC  getchar
  #define TPRINT printf
  #define TSCAN  scanf
  #define TRET    -1
  #define TLOCALE setlocale(LC_ALL, T("chs"));
  #define T(sz)  sz
#endif //end MYUNICODE

TCHAR g_szBackboard[ROW][COL]; //游戏界面
int g_nScore;
int g_nSpeed;

//实现一个清空缓存区的的函数
void MyFlush()
{
  while (1)
  {
    TCHAR ch = TGETC(); //读取输入缓冲区中的字符(相当于利用一个字符变量进行吸收,丢掉了该字符)

    if (ch == '\n') //直到读取到回车(因为回车意味着一个字符串输入的结束)跳出循环,这样就达到了清空输入缓冲区的目的,不至于妨碍下一次的输入
    {
      break;
    }
  }
}

void ShowUI()
{
  TLOCALE
  int i = 0;
  int j = 0;
  setlocale(LC_ALL, "chs");
  //隐藏光标
  CONSOLE_CURSOR_INFO ccInfo = { sizeof(CONSOLE_CURSOR_INFO), 0 };
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ccInfo);
  //控制光标位置,解决闪屏问题
  COORD coor = { 0, 0 };
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coor);

  TPRINT(T("当前积分:%d    游戏速度:%d\r\n"), g_nScore, g_nSpeed); //显示积分
  //根据产生的值绘制界面
  for (i = 0; i < ROW; i++)
  {
    for (j = 0; j < COL; j++)
    {
      if (g_szBackboard[i][j] == WALL)
      {
        TPRINT(T("+ "));
      }
      else if (g_szBackboard[i][j] == SNAKE)
      {
        TPRINT(T("< "));
      }
      else if (g_szBackboard[i][j] == FOOD)
      {
        TPRINT(T("$ "));
      }
      else
      {
        TPRINT(T("  "));
      }
    }
    TPRINT(T("\r\n"));
  }
}

int LoseUI()
{
  TLOCALE
  TCHAR cRet = 0;
  TPRINT(T("您输了,游戏结束\r\n"));
  TPRINT(T("本次得分: %d\r\n"), g_nScore);
  TPRINT(T("是否重新开始?(N:退出 其他键:继续)\r\n"));
  TSCAN(T("%1c"), &cRet);
  MyFlush();
  //清除输入缓冲区

  switch (cRet)
  {
    case 'N':
    case 'n':
      cRet = TRET;
      TPRINT(T("正在退出\r\n"));
      break;
    default:
      TPRINT(T("重新开始\r\n"));
      break;
  }
  return cRet;
}

int WinnerUI()
{
  TLOCALE
  TCHAR cRet = 0;
  TPRINT(T("您赢了,游戏结束\r\n"));
  TPRINT(T("本次得分: %d\r\n"), g_nScore);
  TPRINT(T("是否重新开始?(N:退出 其他键:继续)\r\n"));
  TSCAN(T("%1c"), &cRet);
  MyFlush();
  switch (cRet)
  {
  case 'N':
  case 'n':
    cRet = TRET;
    TPRINT(T("正在退出\r\n"));
    break;
  default:
    TPRINT(T("重新开始\r\n"));
    break;
  }
  return cRet;
}