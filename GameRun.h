#pragma once
#ifndef _GAMERUN_H_
#define _GAMERUN_H_
//游戏界面生成
int InitGameBackboard();
//生成蛇
int InitSnake();
//生成食物
int InitFood();
//游戏开始
int StartGame();
//清除游戏界面内蛇的残留值
void ClearBuff();
#endif //!_GAMERUN_H_
