# CGreedy_Snake
## Use Tec Stack
 Use C and Windows SDK implement Greedy Snake game

## Realization Process
### 1.Create game interface
  Use a two-dimensional array to represent the game interface (the first dimension represents columns and the second dimension represents rows)
  There is a wall (border) in the game interface. Use 1 to represent the wall. The first row\last row\first column\last column are all walls.
  用一个二维数组表示游戏界面(第一维表示列第二维表示行)
  游戏界面有墙(边界),使用1来表示墙,第一行\最后一行\第一列\最后一列都是墙

### 2.Create Snake
  Snakes can also be represented by a two-dimensional array. The second dimension is the coordinates of the snake (expressed in x, y), and the first dimension is the maximum length that the snake can grow (the length   is all interfaces except the wall)
  The initial length of the snake is 1, and it appears randomly in the game interface.
  蛇也可以使用一个二维数组表示,第二维是蛇的坐标(以x,y表示),第一维是蛇最大能生长的长度(长度就是除去墙以外的所有界面)
  蛇的初始长度是1,随机出现在游戏界面中

### 3.Create Food
  Food appears randomly in the game interface with x,y coordinates (can't appear on the wall or outside the wall).
  Food can't overlap with the snake's position, if it does, it needs to be regenerated.
  食物以x,y为坐标随机出现在游戏界面中(不能出现在墙上和墙外)
  食物生成的位置不能和蛇的位置重叠,如果重叠了需要重新生成

### 4.Put in place
  UI display
  UI的显示
  The coordinates of snakes and food should be placed in the game interface.
  Show the screen in real time after each move and any changes.
  蛇和食物的对应坐标要放在游戏界面中
  每次移动之后以及有任何的变动都要实时的显示画面

### 5.Move
  Use w\s\a\d to control up, down, left and right movement respectively.
  The more points you get, the faster you should move.
  At the same time can not move in the opposite direction of the current direction
  以w\s\a\d来分别控制上、下、左、右移动
  得到的积分越多,速度应该越快
  同时不能朝当前方向的相反方向移动

### 6.Crash detection
  Hitting the wall and the snake itself means the game has failed, reminding the user to continue playing or quit.
  Snake length +1 if it hits food
  碰到墙和蛇自身,表示游戏失败,提醒用户继续游玩还是退出
  碰到食物蛇长度+1
