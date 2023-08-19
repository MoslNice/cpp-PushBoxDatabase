#include <graphics.h>
#include <stdio.h>
#include <conio.h>	//_getch
#include <stdlib.h>	//system
#include "database.h"
#include <iostream>


#define MAX_TIME 3
static int step = 0;
/*
	ģ�黯:���չ���ȥ���ֳ���Ա
		��ѭԭ��:��һ��ԭ�� һ������ֻ��һ�����飬����ֻʵ��һ������
*/
//��Ƶ�ͼ  --->ȫ�ֱ���
int cas = 0;
int map[ROW][COL] = { 0 };  //��ͼ
userinfo user;    //������ȡ�õ��û���Ϣ
levelinfo level;  // ������ȡ�õ��ؿ���Ϣ
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
IMAGE img[6];  //����ͼƬ
IMAGE mm;
IMAGE WW;
int timee = 0;
//ѭ��ȥ��������
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
//����map[i][j]ȥ����ͼ
/***********************************
* 0:�յ� 1:ǽ  3:����  4:��   5:��  8:����������  7:�����������ɼ�
************************************/
void drawMap()
{
	//����ͼ
	for (int i = 0; i < level.map_row ; i++)
	{
		for (int j = 0; j < level.map_col; j++)
		{
			int x = 64 * j;
			int y = 64 * i;
			switch (map[i][j])
			{
			case 0:
				//printf("  ");	//���ַ���ռ�������ֽ�
				putimage(x, y, img + 0);
				break;
			case 1:
				//printf("��");
				putimage(x, y, img + 1);
				break;
			case 3:
				//printf("��");
				putimage(x, y, img + 2);
				break;
			case 4:
				//printf("��");
				putimage(x, y, img + 3);
				break;
			case 5:
			case 8:
				putimage(x, y, img + 4);
				//printf("��");
				break;
			case 7:
				putimage(x, y, img + 5);
				//printf("��");
				break;
			}
		}
		//printf("\n");
		
	}
	loadimage(&WW, "E:\\101.jpg");
	putimage(0, (level.map_row) * 64, &WW);

	setbkmode(0);
	settextcolor(BLUE);
	settextstyle(23, 0, "�����п�");
	char s[20];
	sprintf(s, "����:%d", step);
	outtextxy(0, level.map_row * 64 + 20, s);

	settextstyle(25, 0, "�����п�");
	outtextxy(100, level.map_row * 64 + 20, "����Backspace���������¿�ʼ����Ŷ��");
}
//�������i����
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
//�������j����
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
	//�������ƵĿ��
	char userKey = _getch();
	switch (userKey)
	{
	case 'W':
	case 'w':
	case 72:
		if (map[i - 1][j] == 0 || map[i - 1][j] == 3)
		{
			map[i - 1][j] += 5;  //�µĵط�����
			map[i][j] -= 5;		 //ԭ���ĵط�������
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
			map[i + 1][j] += 5;  //�µĵط�����
			map[i][j] -= 5;		 //ԭ���ĵط�������
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
			map[i][j - 1] += 5;  //�µĵط�����
			map[i][j] -= 5;		 //ԭ���ĵط�������
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
			map[i][j + 1] += 5;  //�µĵط�����
			map[i][j] -= 5;		 //ԭ���ĵط�������
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
//�ж���Ϸ����  ���������ͼû��һ���� �����
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
//����������û���������ȥ���ݿ�����Ƿ���ڴ��û�

int login() {
	bool res = false;
	do {
		cout << "�������û���:";
		cin >> user.name;
		cout << "����������:";
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
		//ȥ���ݿ�����Ƿ���ڴ��û�
		if (fetch_user(user) == false) {
			cout << endl << "�û����������������������" << endl;
		}
		else {
			return 1;   //�ɹ���¼
		}
	} while (!res);
	bool ret = u_register();
	if(ret) return 2;  //ע��ɹ�
	if (!ret) return 3;  //��ע��
	return 0;
}
bool u_register() {
	cout << endl << "������ȷʵû��ע���Ŷ����Ҫע��һ���˺���(Y/N)" << endl;
	string s;
	cin >> s;
	if (s[0] == 'y' || s[0] == 'Y') {
		cout << "����������û���:";
		cin >> user.name;
		cout << "�������������:";
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
//��Ϸ������һ������Ե���һ��
void gameOverScene() {
	initgraph(1024, 576);
	loadimage(&mm, "E:\\11.jpg");
	putimage(0, 0,&mm);
	setbkmode(0);
	settextcolor(RGB(250, 250, 250));
	settextstyle(25, 0, "�����п�");
	outtextxy(10, 330, "��ϲͨ���˹ؿ�,���������ת����һ�أ�");
	//::system("pause");
}
//��Ϸû����һ�������
void gameOverScene1() {
	initgraph(1024, 576);
	loadimage(&mm, "E:\\11.jpg");
	putimage(0, 0, &mm);
	setbkmode(0);
	settextcolor(RGB(250, 250, 250));
	settextstyle(25, 0, "�����п�");
	outtextxy(10, 330, "��ϲͨ�����һ��,�������������Ϸ��");
	::system("pause");
}
int main()
{
	//�û������֤
	int res = login();
	if (res == 0) {
		cout << endl << "�������������࣬�����µ�¼��" << endl;
		::system("pause");
		exit(-1);
	}
	else if(res == 1) {
		cout << endl << "������ȷ���ɹ�����" << endl;
		//::system("pause");
	}
	else if (res == 2) {
		cout << endl << "ע��ɹ�!�Ƿ����ڵ�¼��(Y/N)" << endl;
		string s;
		cin >> s;
		if (s[0] == 'y' || s[0] == 'Y') {
			login();
		}
		else {
			cout << "�������ٵ�½Ŷ" << endl;
			exit(-1);
		}
	}
	else if (res == 3) {
		cout << "�´�����Ŷ��" << endl;
		exit(-1);
	}
	//�����û��Ĺؿ��ж��Ƿ������Ϸ
	bool quit = false;
	do {
		//�����û��Ĺؿ�id��ȡ��ͼ����
		bool ret = fetch_level(level, user.level_id);
		if (!ret) {
			cout << "��ȡ�ؿ�����ʧ��" << endl;
			::system("pause");
			exit(-1);
		}

		//�����ݿ�ĵ�ͼ����ת����map��λ������
		ret = transform_map(level, map);
		if (!ret) {
			cout << "��ȡ��ͼ����ʧ��" << endl;
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
				if (level.next_level_id == 1)   //˵���Ѿ������һ�� ���ؾͽ���
				{
					gameOverScene1();
					quit = true;
				}
				else {
					gameOverScene();
					user.level_id++;  //�����û���һ�صĹؿ�
				}

				//���ݿ�����û��Ĺؿ�
				updata_user_level(user, level.next_level_id);
				break;
			}
			keyDown();
		}
	} while (quit == false);
	closegraph();
	return 0;
}