#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>
#include "GameUI.h"
#include "GameRun.h"

#define ROW 12 //行
#define COL 16 //列
#define WALL 1
#define SNAKE 2
#define FOOD 3

#define MYUNICODE //宽字节版本
#ifdef MYUNICODE
  #define TCHAR  wchar_t
  #define TRET   65535
#else
  #define TCHAR  char
  #define TRET   -1
#endif //end MYUNICODE

//游戏运行主逻辑代码

TCHAR g_szBackboard[ROW][COL] = { 0 }; //游戏界面
TCHAR g_szSnake[(ROW - 2) * (COL - 2)][2] = { 1 }; //蛇

int g_nSnakeX = 0; //蛇的行坐标
int g_nSnakeY = 0; //蛇的列坐标
int g_nFoodX = 0; //食物的行坐标
int g_nFoodY = 0; //食物的列坐标

int g_nSnakeCnt = 0; //表示蛇的长度,每次吃完食物长度要+1
int g_nScore = 0; //积分,每次都从0开始
int g_nSpeed = 500; //速度,起始500

//初始化游戏界面
int InitGameBackboard()
{
  int i = 0;
  int j = 0;

  while (i < ROW)
  {
    j = 0;
    while (j < COL)
    {
      if (i == 0 || j == 0 || i == ROW - 1 || j == COL - 1)
      {
        g_szBackboard[i][j] = WALL; //用1来表示墙
      }
      j++;
    }
    i++;
  }
  //重新开游戏,积分清零
  g_nScore = 0; 
  //重新开游戏,蛇在游戏界面中要清零
  ClearBuff();
  g_nSnakeCnt = 0; //蛇的长度也置为初始值
  //食物也要在界面中清0
  if (g_nFoodX != 0 || g_nFoodY != 0)
  {
    g_szBackboard[g_nFoodX][g_nFoodY] = 0;
  }
  //速度置为初始值
  g_nSpeed = 500;
  return 0;
}
//初始化蛇
int InitSnake()
{
  g_nSnakeX = (rand() % (ROW - 2)) + 1; //蛇出现的行,此时出现的数字范围就是1-((ROW-2)+1)
  g_nSnakeY = (rand() % (COL - 2)) + 1; //蛇出现的列,此时出现的数字范围就是1-((COL-2)+1)

  //把蛇头初始化(蛇头用于控制方向)
  g_szSnake[0][0] = g_nSnakeX;
  g_szSnake[0][1] = g_nSnakeY;

  //把蛇放进游戏背景中
  g_szBackboard[g_nSnakeX][g_nSnakeY] = SNAKE; //用2表示蛇
  g_nSnakeCnt++;
  return 0;
}

//判断食物的位置是否合法
int CheckFood(int nFoodX, int nFoodY)
{
  int i = 0;
  int nRet = 0; //默认返回值为0,表示无重叠
  //此处需要判断食物的初始化位置和蛇的位置是否有重叠
  for (i = 0; i < g_nSnakeCnt; i++)
  {
    if (g_nFoodX == g_szSnake[i][0] && g_szSnake[i][1])
    {
      nRet = -1; //如果发现与蛇有坐标重叠的情况,返回值变为-1
      break;
    }
  }
  return nRet;
}

//生成食物
int InitFood()
{
    int i = 0;
    int nRet = 0;
    do
    {
      g_nFoodX = (rand() % (ROW - 2)) + 1; //食物出现的行,此时出现的数字范围就是1-((ROW-2)+1)
      g_nFoodY = (rand() % (COL - 2)) + 1; //食物出现的列,此时出现的数字范围就是1-((COL-2)+1)
      
    } while (CheckFood(g_nFoodX, g_nFoodY) == -1); //如果有重叠情况就需要重新生成

  //把食物放进游戏背景中
  g_szBackboard[g_nFoodX][g_nFoodY] = FOOD; //用3表示食物
  return nRet;
}

//增加蛇长度
int AddSnake()
{
  int i = 0;
  int nSnakeX = 0;
  int nSnakeY = 0;
  //增加蛇身
  //把当前位置的下一位置放入当前身体的坐标,身子增长,头部被空出来了
  for (i = g_nSnakeCnt - 1; i >= 0; i--)
  {
    //0 1 2 3 4
    //0 o o o
    //0 o o o o
    g_szSnake[i + 1][0] = g_szSnake[i][0];
    g_szSnake[i + 1][1] = g_szSnake[i][1];
  }
  //蛇头更新,现在的蛇头就是吃到食物的地方
  g_szSnake[0][0] = g_nFoodX;
  g_szSnake[0][1] = g_nFoodY;
  g_nSnakeX = g_nFoodX; //全局的蛇头位置也需要更新
  g_nSnakeY = g_nFoodY;
  //长度+1
  g_nSnakeCnt++;
  //积分加1
  g_nScore++;
  //放入背景
  for (i = 0; i < g_nSnakeCnt; i++)
  {
    nSnakeX = g_szSnake[i][0];
    nSnakeY = g_szSnake[i][1];
    g_szBackboard[nSnakeX][nSnakeY] = SNAKE; //把蛇放入背景
  }
  return 0;
}

//判断能否移动,以及碰到相应物品后返回相应的值
int CanMove(int SnakeX, int SnakeY) 
{
  int nRet = 0; //为0表示可以移动
  //碰到相应的物品后返回相应的状态
  if (g_szBackboard[SnakeX][SnakeY] == WALL)
  {
    nRet = WALL;
  }
  else if (g_szBackboard[SnakeX][SnakeY] == SNAKE)
  {
    nRet = SNAKE;
  }
  else if (g_szBackboard[SnakeX][SnakeY] == FOOD)
  {
    nRet = FOOD;
  }
  return nRet;
}

//更新蛇
int UpdateSnake() //更新蛇
{
  int i = 0;
  int nSnakeX = 0;
  int nSnakeY = 0;

  //清除之前的数据(在更新前清掉)
  ClearBuff();

  //蛇移动时,从尾巴开始,把当前位置前一个的坐标移动到当前位置上,从后往前覆盖以此类推
  //更新蛇身
  for (i = g_nSnakeCnt - 1; i > 0; i--)
  {
    //例:
   // 0 1 2 3
    //0 o o o
  //0 1 2 3 
  //  o o o 可以看到原先的头不见了空出来了,现在的尾巴是之前的倒数第二的位置
    g_szSnake[i][0] = g_szSnake[i - 1][0];
    g_szSnake[i][1] = g_szSnake[i - 1][1];
  }

  //得到新的蛇头,新的蛇头坐标放在蛇数组中
  g_szSnake[0][0] = g_nSnakeX; 
  g_szSnake[0][1] = g_nSnakeY;
  //把新的蛇放在游戏背景中
  for (i = 0; i < g_nSnakeCnt; i++)
  {
    nSnakeX = g_szSnake[i][0];
    nSnakeY = g_szSnake[i][1];
    g_szBackboard[nSnakeX][nSnakeY] = SNAKE;
  }
  ShowUI(); //显示

  return 0;
}

//向上移动
int MoveUp()
{
  int nRet = 0;
  nRet = CanMove(g_nSnakeX - 1, g_nSnakeY);
  if (nRet == 0)
  {
    g_nSnakeX--; //如果可以向上移动,行就-1
    //还需要将更新后的坐标放在背景中
    UpdateSnake();
  }
  else if (nRet == FOOD)
  {
    //碰到食物,蛇长度+1
    AddSnake();
    //生成食物
    InitFood();
    ShowUI();
  }
  else if (nRet == WALL || (g_nSnakeCnt != (ROW-2) * (COL-2) && nRet == SNAKE) )
  {
    //碰到墙或者自身(当蛇还没有生长到最长的时候),游戏判定为失败
    nRet = TRET;
  }
  else if (g_nSnakeCnt == (ROW - 2) * (COL - 2))
  {
    nRet = 1; //当蛇生长到最大时,表示无法移动也证明游戏胜利
  }
  return nRet;
}

int MoveDown()
{
  int nRet = 0;
  nRet = CanMove(g_nSnakeX + 1, g_nSnakeY);
  if (nRet == 0)
  {
    g_nSnakeX++; //如果可以向下移动,行就+1
    //还需要将更新后的坐标放在背景中
    UpdateSnake();
  }
  else if (nRet == FOOD)
  {
    //碰到食物,蛇长度+1
    AddSnake();
    //生成食物
    InitFood();
    ShowUI();
  }
  else if (nRet == WALL || (g_nSnakeCnt != (ROW - 2) * (COL - 2) && nRet == SNAKE))
  {
    //碰到墙或者自身,游戏判定为失败
    nRet = TRET;
  }
  else if (g_nSnakeCnt == (ROW - 2) * (COL - 2))
  {
    nRet = 1; //当蛇生长到最大时,表示无法移动也证明游戏胜利
  }
  return nRet;
}

int MoveLeft()
{
  int nRet = 0;
  nRet = CanMove(g_nSnakeX, g_nSnakeY - 1);
  if (nRet == 0)
  {
    g_nSnakeY--; //如果可以向左移动,列就-1
    //还需要将更新后的坐标放在背景中
    UpdateSnake();
  }
  else if (nRet == FOOD)
  {
    //碰到食物,蛇长度+1
    AddSnake();
    //生成食物
    InitFood();
    ShowUI();
  }
  else if (nRet == WALL || (g_nSnakeCnt != (ROW - 2) * (COL - 2) && nRet == SNAKE))
  {
    //碰到墙或者自身,游戏判定为失败
    nRet = TRET;
  }
  else if (g_nSnakeCnt == (ROW - 2) * (COL - 2))
  {
    nRet = 1; //当蛇生长到最大时,表示无法移动也证明游戏胜利
  }
  return nRet;
}

int MoveRight()
{
  int nRet = 0;
  nRet = CanMove(g_nSnakeX, g_nSnakeY + 1);
  if (nRet == 0)
  {
    g_nSnakeY++; //如果可以向右移动,列就+1
    //还需要将更新后的坐标放在背景中
    UpdateSnake();
  }
  else if (nRet == FOOD)
  {
    //碰到食物,蛇长度+1
    AddSnake();
    //生成食物
    InitFood();
    ShowUI();
  }
  else if (nRet == WALL || (g_nSnakeCnt != (ROW - 2) * (COL - 2) && nRet == SNAKE))
  {
    //碰到墙或者自身,游戏判定为失败
    nRet = TRET;
  }
  else if (g_nSnakeCnt == (ROW - 2) * (COL - 2))
  {
    nRet = 1; //当蛇生长到最大时,表示无法移动也证明游戏胜利
  }
  return nRet;
}

//控制游戏
int ControlGame()
{
  int nRet = 0;
  int nKey = 0; //获取输入
  int nDirection = 0; //表示方位
  int nScore = 0; //记录当前分数
  int nForward = 0; //记录当前前进方向 1上 2下 3左 4右
  clock_t clockBegin = clock();
  clock_t clockEnd = 0;

  while (1)
  {
    clockEnd = clock();
    //如果长时间未输入开始自动朝指定方向前进(前提需要确定一个方向)
    //速度可调整
    if (clockEnd - clockBegin >= g_nSpeed)
    {
      nScore = g_nScore; //保存一下之前的分数情况
      clockBegin = clockEnd; //需要更新开始时间
      if ((nDirection == 'w' || nDirection == 'W') && (nForward != 2)) //还需要判定是否为当前方向的相反方向
      {
        nRet = MoveUp();
        if (nRet == TRET || nRet == 1)
        {
          break; //表示撞到墙或者蛇自己了,退出循环
        }
      }
      else if ((nDirection == 's' || nDirection == 'S')  && (nForward != 1))
      {
        nRet = MoveDown();
        if (nRet == TRET || nRet == 1)
        {
          break;
        }
      }
      else if ((nDirection == 'a' || nDirection == 'A') && (nForward != 4))
      {
        nRet = MoveLeft();
        if (nRet == TRET || nRet == 1)
        {
          break;
        }
      }
      else if ((nDirection == 'd' || nDirection == 'D') && (nForward != 3))
      {
        nRet = MoveRight();
        if (nRet == TRET || nRet == 1)
        {
          break;
        }
      }

      if (g_nScore != nScore && g_nScore % 10 == 0 && g_nScore != 0) //没得到10分可以增加一下速度(同时还要确认分数有变化,否则可能会一直增长)
      {
        g_nSpeed += 100;
      }
    }
    if (_kbhit()) //查看有无键盘输入
    {
      nKey = _getch(); //不回显接收字符

      switch (nKey)
      {
        case 'w':
        case 'W':
          nDirection = nKey;
          if(nForward != 2) //查看是否与当前前进方向相反
          {
            nForward = 1; //如果不相反,就把方向更新为当前输入相应的方向
          }
          else
          {
            nDirection = 's'; //如果是相反方向,那么前进方向不变,同时把表示方位的变量变成和前进方向相同的按键
          }
          break;
        case 's':
        case 'S':
          nDirection = nKey;
          if(nForward != 1)
          {
            nForward = 2;
          }
          else
          {
            nDirection = 'w';
          }
          break;
        case 'a':
        case 'A':
          nDirection = nKey;
          if(nForward != 4)
          {
            nForward = 3;
          }
          else
          {
            nDirection = 'd';
          }
          break;
        case 'd':
        case 'D':
          nDirection = nKey;
          if(nForward != 3)
          {
            nForward = 4;
          }
          else
          {
            nDirection = 'a';
          }
          break;
      }
    }
  } //end while1

  return nRet;
}
//游戏开始
int StartGame()
{
  int nRet = 0;
  srand((unsigned int)time(NULL)); //初始化随机种子
  while (nRet != TRET)
  {
    InitGameBackboard(); //1.生成游戏界面
    InitSnake(); //2.生成蛇
    InitFood(); //3.生成食物
    ShowUI(); //4.显示界面
    nRet = ControlGame(); //5.控制游戏
    //如果返回值为-1,表示您输了当前这局游戏已结束
    if (nRet == TRET)
    {
      nRet = LoseUI(); //如果不为-1表示继续游戏
    }
    else if (nRet == 1)//如果返回值为1,表示您赢了当前这局游戏已结束
    {
      nRet = WinnerUI(); //如果不为-1表示继续游戏
    }
  }

  return nRet;
}

//清除游戏界面内蛇的残留值
void ClearBuff()
{
  int i = 0;
  int nSnakeX = 0;
  int nSnakeY = 0;
  for (i = 0; i < g_nSnakeCnt; i++)
  {
    nSnakeX = g_szSnake[i][0];
    nSnakeY = g_szSnake[i][1];
    g_szBackboard[nSnakeX][nSnakeY] = 0; //把原先在游戏背景中有蛇的地方清掉
  }
}

