#define  _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <utility>
#include <conio.h>
#include <windows.h>
class MAP;
class Player;
using namespace std;
#define row 17//0和16是墙，0和20是墙
#define col 21//地图的尺寸
#define H_WALL 70//表示中间的硬墙数量
#define S_WALL 90//表示中间软墙的个数
#define ONE_SECOND 10//这个我不知道是干什么的嗯
enum director
{
	UP,
	LEFT,
	DOWN,
	RIGHT
};
//
class MAP
{
public:
	void show_MYmap();
	void init_MYmap();
	void change_MYmap(char character, int x, int y)
	{
		MYmap[x][y] = character;
	}
	char get_MYmap(int x, int y)
	{
		return MYmap[x][y];
	}
private:
	char MYmap[17][21];
};
MAP map;//申明一个地图对象
void MAP::show_MYmap()
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			printf("%c ", MYmap[i][j]);
		}
		printf("\n");
	}
}
void MAP::init_MYmap()
{
	//首先四周是围墙的，都是硬墙
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (i == 0 || i == row - 1 || j == 0 || j == col - 1)
			{
				MYmap[i][j] = '#';
			}
			else
			{
				MYmap[i][j] = ' ';
			}
		}
	}
	//中间设置硬墙
	for (int i = 0; i < H_WALL; )
	{
		int r = rand() % row;
		int c = rand() % col;
		if ( MYmap[r][c] == ' ')
		{
			MYmap[r][c] = '#';
			i++;
		}
	}
	//中间设置软墙
	for (int i = 0; i < S_WALL; )
	{
		int r = rand() % row;
		int c = rand() % col;
		if (MYmap[r][c] == ' ')
		{
			MYmap[r][c] = '*';
			i++;
		}
	}
}
//
class Player
{
private:
	pair<int, int> location;//现在的位置
	char symbol;//玩家代表的字符
public:
	void init_Player(char character);//初始化位置及玩家符号
	void update_location(int type);//更新位置
	pair<int, int> get_location();//获取位置
	char get_symbol()//获取玩家的字符表示
	{
		return this->symbol;
	}
};
Player player1;
Player player2;//双玩家
void Player::init_Player(char character)
{
	this->symbol = character;
	while (1)
	{
		this->location.first = (rand() % row);
		this->location.second = (rand() % col);
		if (map.get_MYmap(this->location.first,this->location.second) == ' ')
		{
			map.change_MYmap(character, this->location.first, this->location.second);
			break;
		}

	}
}
pair<int, int> Player::get_location()
{
	return this->location;
}
void Player::update_location(int type)
{
	//只是用来修改玩家的位置
	cout << player2.get_location().first << " v "<<player2.get_location().second << endl;
	switch (type)
	{
	case UP:
		this->location.first--;
	case LEFT:
		this->location.second--;
	case DOWN:
		this->location.first++;
	case RIGHT:
		this->location.second++;
	default:
		break;
	}
	//cout << player2.get_location().first << " l "<<player2.get_location().second << endl;
}
void init()
{
	//初始化的内容包括
	map.init_MYmap();//初始化地图
	player1.init_Player('@');
	player2.init_Player('+');
}
void display()
{
	system("cls");
	map.show_MYmap();
	//cout << player1.get_location().first << "  " << player1.get_location().second << "  "<<map.MYmap[player1.get_location().first][player1.get_location().second]<<endl;
	//cout << player2.get_location().first << "  " << player2.get_location().second << endl;
}
void deal_with_input()
{
	char ch;
	if (_kbhit())
	{
		ch = _getch();
		//后面需要对不同的输入做出不同的选择
		int Px1 = player1.get_location().first, Py1 = player1.get_location().second;
		int Px2 = player2.get_location().first, Py2 = player2.get_location().second;
		//需要将玩家之前在的位置变为空，然后将现在所在的位置，在地图上标为符号
		switch (ch)
		{
		case 'w':
			if (map.get_MYmap(Px1-1, Py1) == ' ')//判断要走的位置是否为空
			{
				map.change_MYmap(' ',Px1, Py1);//将现在的位置变为空
				player1.update_location(UP);//更改玩家的位置
				map.change_MYmap(player1.get_symbol(), Px1 - 1, Py1);//显示玩家现在的位置
				display();
			}
		case 'a':
			if (map.get_MYmap(Px1, Py1-1) == ' ')
			{
				map.change_MYmap(' ', Px1, Py1);
				player1.update_location(LEFT);
				map.change_MYmap(player1.get_symbol(), Px1, Py1-1);
				display();
			}
		case 's':
			if (map.get_MYmap(Px1+1, Py1) == ' ')
			{
				map.change_MYmap(' ', Px1, Py1);
				player1.update_location(DOWN);
				map.change_MYmap(player1.get_symbol(), Px1 + 1, Py1);
				display();
			}
		case 'd':
			if (map.get_MYmap(Px1, Py1+1) == ' ')
			{
				map.change_MYmap(' ', Px1, Py1);
				player1.update_location(RIGHT);
				map.change_MYmap(player1.get_symbol(), Px1, Py1 + 1);
				display();
			}
		case 'i':
			if (map.get_MYmap(Px2-1, Py2) == ' ')
			{
				map.change_MYmap(' ',Px2, Py2);
				player2.update_location(UP);
				map.change_MYmap(player2.get_symbol(), Px2 - 1, Py2);
				
				display();
			}
		case 'k':
			if (map.get_MYmap(Px2+1,Py2) == ' ')
			{
				map.change_MYmap(' ', Px2, Py2);
				player2.update_location(DOWN);
				map.change_MYmap(player2.get_symbol(), Px2 + 1, Py2);

				display();
			}
		case 'j':
			if (map.get_MYmap(Px2, Py2-1) == ' ')
			{
				map.change_MYmap(' ',Px2, Py2);
				player2.update_location(LEFT);
				map.change_MYmap(player2.get_symbol(), Px2, Py2 - 1);
				
				display();
			}
		case 'l':
			if (map.get_MYmap(Px2,Py2+1) == ' ')
			{
				map.change_MYmap(' ', Px2, Py2);
				player2.update_location(RIGHT);
				map.change_MYmap(player2.get_symbol(), Px2, Py2 + 1);

				display();
			}
		default:
			break;
		}
	}
}
int main()
{
	srand((unsigned)time(NULL));//随机数种子
	init();//作为初始化地图等东西
	map.show_MYmap();
	while (1)
	{
		deal_with_input();
		//count += 1;
		/*if (count == ONE_SECOND)
		{
			//deal_with_timer();
			count = 0;
		}*/	
	}
	return 0;
}