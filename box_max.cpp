#include <graphics.h>
#include <stdio.h>
#include <conio.h>	//_getch
#include <stdlib.h>	//system
#include "database.h"
#include <iostream>


#define MAX_TIME 3
static int step = 0;
/*
	模块化:按照功能去划分程序员
		遵循原则:单一性原则 一个函数只做一个事情，或者只实现一个功能
*/
//设计地图  --->全局变量
int cas = 0;
int map[ROW][COL] = { 0 };  //地图
userinfo user;    //用来获取得到用户信息
levelinfo level;  // 用来获取得到关卡信息
bool u_register();

//{
//	0,0,0,1,1,1,1,0,
//	0,1,1,1,0,5,1,0,
//	0,1,0,0,4,0,1,1,
//	1,1,0,1,0,1,1,1,
//	1,0,0,1,0,1,3,1,
//	1,0,1,0,0,4,3,1,
//	1,0,4,0,0,0,3,1,
//	1,1,1,1,1,1,1,1
//	//0,1,1,1,1,1,1,1,1,1,0|0,1,0,0,1,1,0,0,0,1,0|0,1,0,0,0,4,0,0,0,1,0|0,1,4,0,1,1,1,0,4,1,0|0,1,0,1,3,3,3,1,0,1,0|0,1,0,1,3,3,3,1,0,1,1|1,0,4,0,0,4,0,0,4,0,1|1,0,0,0,0,0,1,5,0,0,1|1,1,1,1,1,1,1,1,1,1,1
//};
IMAGE img[6];  //背景图片
IMAGE mm;
IMAGE WW;
int timee = 0;
//循环去描述名字
void loadResource()
{
	int fileIndex[] = { 0,1,3,4,5,7 };
	for (int i = 0; i < 6; i++)
	{
		char fileName[20] = "";
		sprintf(fileName, "%d.bmp", fileIndex[i]);
		loadimage(img + i, fileName, 64, 64);
	}
}
//根据map[i][j]去画地图
/***********************************
* 0:空地 1:墙  3:篮子  4:球   5:人  8:人在篮子上  7:球到篮子里面变成鸡
************************************/
void drawMap()
{
	//画地图
	for (int i = 0; i < level.map_row ; i++)
	{
		for (int j = 0; j < level.map_col; j++)
		{
			int x = 64 * j;
			int y = 64 * i;
			switch (map[i][j])
			{
			case 0:
				//printf("  ");	//汉字符号占用两个字节
				putimage(x, y, img + 0);
				break;
			case 1:
				//printf("■");
				putimage(x, y, img + 1);
				break;
			case 3:
				//printf("☆");
				putimage(x, y, img + 2);
				break;
			case 4:
				//printf("★");
				putimage(x, y, img + 3);
				break;
			case 5:
			case 8:
				putimage(x, y, img + 4);
				//printf("♀");
				break;
			case 7:
				putimage(x, y, img + 5);
				//printf("●");
				break;
			}
		}
		//printf("\n");
		
	}
	loadimage(&WW, "E:\\101.jpg");
	putimage(0, (level.map_row) * 64, &WW);

	setbkmode(0);
	settextcolor(BLUE);
	settextstyle(23, 0, "华文行楷");
	char s[20];
	sprintf(s, "步数:%d", step);
	outtextxy(0, level.map_row * 64 + 20, s);

	settextstyle(25, 0, "华文行楷");
	outtextxy(100, level.map_row * 64 + 20, "按下Backspace键可以重新开始本关哦！");
}
//找人物的i坐标
int searchPos_i()
{
	int i, j;
	for (i = 0; i < level.map_row; i++)
	{
		for (j = 0; j < level.map_col; j++)
		{
			if (map[i][j] == 5 || map[i][j] == 8)
			{
				return i;
			}
		}
	}
	return -1;
}  
//找人物的j坐标
int searchPos_j()
{
	int i, j;
	for (i = 0; i < level.map_row; i++)
	{
		for (j = 0; j < level.map_col; j++)
		{
			if (map[i][j] == 5 || map[i][j] == 8)
			{
				return j;
			}
		}
	}
	return -1;
}
void keyDown()
{
	int i = searchPos_i();
	int j = searchPos_j();
	//按键控制的框架
	char userKey = _getch();
	switch (userKey)
	{
	case 'W':
	case 'w':
	case 72:
		if (map[i - 1][j] == 0 || map[i - 1][j] == 3)
		{
			map[i - 1][j] += 5;  //新的地方人来
			map[i][j] -= 5;		 //原来的地方人走了
		}
		if (map[i - 1][j] == 4 || map[i - 1][j] == 7)
		{
			if (map[i - 2][j] == 0 || map[i - 2][j] == 3)
			{
				map[i][j] -= 5;
				map[i - 1][j] += 1;
				map[i - 2][j] += 4;
			}
		}
		step++;
		break;
	case 's':
	case 'S':
	case 80:
		if (map[i + 1][j] == 0 || map[i + 1][j] == 3)
		{
			map[i + 1][j] += 5;  //新的地方人来
			map[i][j] -= 5;		 //原来的地方人走了
		}
		if (map[i + 1][j] == 4 || map[i + 1][j] == 7)
		{
			if (map[i + 2][j] == 0 || map[i + 2][j] == 3)
			{
				map[i][j] -= 5;
				map[i + 1][j] += 1;
				map[i + 2][j] += 4;
			}
		}
		step++;
		break;
	case 'A':
	case 'a':
	case 75:
		if (map[i][j - 1] == 0 || map[i][j - 1] == 3)
		{
			map[i][j - 1] += 5;  //新的地方人来
			map[i][j] -= 5;		 //原来的地方人走了
		}
		if (map[i][j - 1] == 4 || map[i][j - 1] == 7)
		{
			if (map[i][j - 2] == 0 || map[i][j - 2] == 3)
			{
				map[i][j] -= 5;
				map[i][j - 1] += 1;
				map[i][j - 2] += 4;
			}
		}
		step++;
		break;
	case 'D':
	case 'd':
	case 77:
		if (map[i][j + 1] == 0 || map[i][j + 1] == 3)
		{
			map[i][j + 1] += 5;  //新的地方人来
			map[i][j] -= 5;		 //原来的地方人走了
		}
		if (map[i][j + 1] == 4 || map[i][j + 1] == 7)
		{
			if (map[i][j + 2] == 0 || map[i][j + 2] == 3)
			{
				map[i][j] -= 5;
				map[i][j + 1] += 1;
				map[i][j + 2] += 4;
			}
		}
		step++;
		break;
	case 8:
		step = 0;
		transform_map(level, map);
		drawMap();
		break;
	}
}
//判断游戏结束  如果整个地图没有一个球 则结束
int gameOver()
{
	for (int i = 0; i < level.map_row; i++)
	{
		for (int j = 0; j < level.map_col; j++)
		{
			if (map[i][j] == 4)
				return 0;
		}
	}
	return 1;
}
//根据输入的用户名和密码去数据库查找是否存在此用户

int login() {
	bool res = false;
	do {
		cout << "请输入用户名:";
		cin >> user.name;
		cout << "请输入密码:";
		//cin >> user.pwd;
		//char password[17] = { 0 };
		int i = 0;
		while ((user.pwd[i] = getch()) != '\r')
		{
			putchar('*');
			i++;
		}
		user.pwd[i] = '\0';
		timee++;
		if (timee == MAX_TIME) break;
		//去数据库查找是否存在此用户
		if (fetch_user(user) == false) {
			cout << endl << "用户名或密码错误，请重新输入" << endl;
		}
		else {
			return 1;   //成功登录
		}
	} while (!res);
	bool ret = u_register();
	if(ret) return 2;  //注册成功
	if (!ret) return 3;  //不注册
	return 0;
}
bool u_register() {
	cout << endl << "可能你确实没有注册过哦！需要注册一个账号吗？(Y/N)" << endl;
	string s;
	cin >> s;
	if (s[0] == 'y' || s[0] == 'Y') {
		cout << "请输入你的用户名:";
		cin >> user.name;
		cout << "请输入你的密码:";
		int i = 0;
		while ((user.pwd[i] = getch()) != '\r')
		{
			putchar('*');
			i++;
		}
		user.pwd[i] = '\0';
		if (user_register(user)) return true;
	}
	return false;
}
//游戏还有下一关则可以到下一关
void gameOverScene() {
	initgraph(1024, 576);
	loadimage(&mm, "E:\\11.jpg");
	putimage(0, 0,&mm);
	setbkmode(0);
	settextcolor(RGB(250, 250, 250));
	settextstyle(25, 0, "华文行楷");
	outtextxy(10, 330, "恭喜通过此关卡,按任意键跳转到下一关！");
	//::system("pause");
}
//游戏没有下一关则结束
void gameOverScene1() {
	initgraph(1024, 576);
	loadimage(&mm, "E:\\11.jpg");
	putimage(0, 0, &mm);
	setbkmode(0);
	settextcolor(RGB(250, 250, 250));
	settextstyle(25, 0, "华文行楷");
	outtextxy(10, 330, "恭喜通过最后一关,按任意键结束游戏！");
	::system("pause");
}
int main()
{
	//用户身份验证
	int res = login();
	if (res == 0) {
		cout << endl << "输入错误次数过多，请重新登录！" << endl;
		::system("pause");
		exit(-1);
	}
	else if(res == 1) {
		cout << endl << "输入正确，成功进入" << endl;
		//::system("pause");
	}
	else if (res == 2) {
		cout << endl << "注册成功!是否现在登录？(Y/N)" << endl;
		string s;
		cin >> s;
		if (s[0] == 'y' || s[0] == 'Y') {
			login();
		}
		else {
			cout << "那你下再登陆哦" << endl;
			exit(-1);
		}
	}
	else if (res == 3) {
		cout << "下次再来哦！" << endl;
		exit(-1);
	}
	//根据用户的关卡判断是否结束游戏
	bool quit = false;
	do {
		//根据用户的关卡id获取地图数据
		bool ret = fetch_level(level, user.level_id);
		if (!ret) {
			cout << "获取关卡数据失败" << endl;
			::system("pause");
			exit(-1);
		}

		//把数据库的地图数据转换到map二位数组中
		ret = transform_map(level, map);
		if (!ret) {
			cout << "获取地图数据失败" << endl;
			exit(-1);
		}
		::system("pause");


		initgraph((level.map_col) * 64, (level.map_row+1) * 64);
		
		loadResource();
		while (1)
		{
			
			drawMap();
			if (gameOver())
			{
				if (level.next_level_id == 1)   //说明已经是最后一关 过关就结束
				{
					gameOverScene1();
					quit = true;
				}
				else {
					gameOverScene();
					user.level_id++;  //更新用户下一关的关卡
				}

				//数据库更新用户的关卡
				updata_user_level(user, level.next_level_id);
				break;
			}
			keyDown();
		}
	} while (quit == false);
	closegraph();
	return 0;
}