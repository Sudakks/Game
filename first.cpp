#define  _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <utility>
#include <conio.h>
#include <windows.h>
class MAP;
class Player;
class BOMB;
class ROBOT;
using namespace std;
#define row 22//0和16是墙，0和20是墙`
#define col 34//地图的尺寸
#define H_WALL 110//表示中间的硬墙数量
#define S_WALL 145//表示中间软墙的个数
#define Bomb_Wall 2//炸软墙2分
#define Bomb_Robot 7//(设定道具有使用时间，均为7s)
#define Bomb_player 10
#define Game_time 540000//规定了游戏结束的时间9min
const int ONE_SECOND = 5000;
enum director
{
	UP,
	LEFT,
	DOWN,
	RIGHT
};
void display();
int px1, py1, px2, py2;//这个每次储存的是player1和2当时炸弹爆炸的位置，这样爆炸的位置就不随着人的移动而移动
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
	char MYmap[22][34];
};
MAP map;//申明一个地图对象
void MAP::show_MYmap()
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			printf("%c", MYmap[i][j]);
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
class ROBOT
{
private:
	pair<int, int> location;//现在的位置
	char symbol;//机器人代表的字符
	int wait_time = 0;
	int bomb_time = 0;//是机器人的炸弹时间
	pair<int, int> bomb_location;//这个是机器人当时放炸弹的地方
	int alive = 1;//表示是否存活
	int flag = 0;//是1的时候表示机器人刚刚放完炸弹，然后一定要先往一个方向移动，0就是已经移开区域了
public:
	int get_flag()
	{
		return this->flag;
	}
	void change_flag(int now)
	{
		flag = now;
	}
	pair<int, int> get_bomb_location()
	{
		return this->bomb_location;
	}
	void change_bomb_location(int x, int y)
	{
		this->bomb_location.first = x, this->bomb_location.second = y;
	}
	int get_wait_time()
	{
		return this->wait_time;
	}
	int get_bomb_time()
	{
		return this->bomb_time;
	}
	void change_wait_time(int x)
	{
		this->wait_time += x;
	}
	void change_bomb_time(int x)
	{
		this->bomb_time += x;
	}
	void init_robot(char character);
	pair<int, int> get_location()//获取位置
	{
		return this->location;
	}
	char get_symbol()//获取机器人的字符表示
	{
		return this->symbol;
	}
	void change_location(int x, int y)
	{
		this->location.first = x, this->location.second = y;
	}
	void walk();
	int get_alive()
	{
		return this->alive;
	}
	void change_alive(int now)
	{
		alive = now;
	}
};
ROBOT robot1;
ROBOT robot2;
ROBOT robot3;
void ROBOT::init_robot(char character)
{
	this->symbol = character;
	while (1)
	{
		this->location.first = (rand() % row);
		this->location.second = (rand() % col);
		if (map.get_MYmap(this->location.first, this->location.second) == ' ')
		{
			map.change_MYmap(this->symbol, this->location.first, this->location.second);
			break;
		}
	}
}
void ROBOT::walk()
{
	//利用随机数使三个机器人能够走动起来
	char character = this->symbol;
	int x = this->location.first, y = this->location.second;
	int bomb_x = this->bomb_location.first, bomb_y = this->bomb_location.second;
	map.change_MYmap(' ', x, y);
	while (1)
	{
		if (map.get_MYmap(x - 1, y) != ' ' && map.get_MYmap(x + 1, y) != ' ' && map.get_MYmap(x, y - 1) != ' ' && map.get_MYmap(x, y + 1) != ' ')
		{
			return;
		}
		//原来是这种情况，一开始一定要先往一个方向走，不然就会死循环，而且还要走两步，所以特判空间！！！	
		int direct = rand() % 4;
		if (direct == UP && map.get_MYmap(x - 1, y) == ' ')
		{
			//分为两种情况，一种是没有炸弹，所以只要能走，机器人就会走
			//第二种是有炸弹要判断是否会走到机器人所在的范围
			if (flag == 1 || (!bomb_time && !wait_time))
			{
				this->location.first--;
				x--, flag = 0;
				map.change_MYmap(character, x, y);
				display();
				return;
			}
			else
			{
				//欸好像是循环出不去的问题，无语无语不知道咋改。。。
				for (int i = bomb_x - 1; i <= bomb_x + 1; i++)
				{
					if (x - 1 == i && y == bomb_y)
						//说明不能走
						break;
					if (i == bomb_x + 1)
					{
						for (int j = bomb_y - 1; j <= bomb_y + 1; j++)
						{
							if (x - 1 == bomb_x && y == j)
								//说明不能走
								break;
							if (j == bomb_y + 1)
							{
								//这里是最终可以走的地方，然后开始改变
								this->location.first--;
								x--;
								map.change_MYmap(character, x, y);
								display();
								return;//走完了直接返回main中	
							}
						}
					}
				}
			}
		}
		else if (direct == DOWN && map.get_MYmap(x + 1, y) == ' ')
		{
			if (flag == 1 || (!wait_time && !bomb_time))
			{
				this->location.first++;
				x++, flag = 0;
				map.change_MYmap(character, x, y);
				display();
				break;
			}
			else
			{
				for (int i = bomb_x - 1; i <= bomb_x + 1; i++)
				{
					if (x + 1 == i && y == bomb_y)
						break;
					if (i == bomb_x + 1)
					{
						for (int j = bomb_y - 1; j <= bomb_y + 1; j++)
						{
							if (x + 1 == bomb_x && y == j)
								break;
							if (j == bomb_y + 1)
							{
								this->location.first++;
								x++;
								map.change_MYmap(character, x, y);
								display();
								return;
							}
						}
					}
				}
			}
		}
		else if (direct == LEFT && map.get_MYmap(x, y - 1) == ' ')
		{
			if (flag == 1 || (!wait_time && !bomb_time))
			{
				this->location.second--;
				y--, flag = 0;
				map.change_MYmap(character, x, y);
				display();
				break;
			}
			else
			{
				for (int i = bomb_x - 1; i <= bomb_x + 1; i++)
				{
					if (x == i && y - 1 == bomb_y)
						break;
					if (i == bomb_x + 1)
					{
						for (int j = bomb_y - 1; j <= bomb_y + 1; j++)
						{
							if (x == bomb_x && y - 1 == j)
								break;
							if (j == bomb_y + 1)
							{
								this->location.second--;
								y--;
								map.change_MYmap(character, x, y);
								display();
								return;
							}
						}
					}
				}
			}
		}
		else if (direct == RIGHT && map.get_MYmap(x, y + 1) == ' ')
		{
			if (flag == 1 || (!wait_time && !bomb_time))
			{
				this->location.second++;
				y++, flag = 0;
				map.change_MYmap(character, x, y);
				display();
				break;
			}
			else
			{
				for (int i = bomb_x - 1; i <= bomb_x + 1; i++)
				{
					if (x == i && y + 1 == bomb_y)
						break;
					if (i == bomb_x + 1)
					{
						for (int j = bomb_y - 1; j <= bomb_y + 1; j++)
						{
							if (x == bomb_x && y + 1 == j)
								break;
							if (j == bomb_y + 1)
							{
								this->location.second++;
								y++;
								map.change_MYmap(character, x, y);
								display();
								return;
							}
						}
					}
				}
			}
		}
	}
}
class Player
{
private:
	pair<int, int> location;//现在的位置
	char symbol;//玩家代表的字符
	int bomb_range = 1;
	int score = 0;//玩家得分
	bool alive = 1;//以及判断游戏是否结束
	int speed = 1;//玩家的行走速度
	int wait_time = 0;//玩家炸弹的爆炸时间
	int bomb_time = 0;//表示玩家没有投放炸弹
	int tool1_time = 0;
	int tool2_time = 0;
public:
	void init_Player(char character);//初始化位置及玩家符号
	void update_location(int x, int y);//更新位置
	pair<int, int> get_location();//获取位置
	void change_score(int x)//动态分数
	{
		this->score += x;
	}
	char get_symbol()//获取玩家的字符表示
	{
		return this->symbol;
	}
	int get_bomb_range()
	{
		return bomb_range;
	}
	void change_bomb_range(int new_range)
	{
		bomb_range = new_range;//这里是在更改玩家所拥有的炸弹爆炸的范围
	}
	int get_score()
	{
		return this->score;
	}
	bool if_alive()
	{
		return this->alive;
	}
	void change_alive(int x)
	{
		this->alive = x;
	}
	int get_speed()
	{
		return this->speed;
	}
	void change_speed(int x)
	{
		this->speed = x;
	}
	int get_wait_time()
	{
		return this->wait_time;
	}
	int get_bomb_time()
	{
		return this->bomb_time;
	}
	void change_bomb_time(int x)
	{
		this->bomb_time += x;
	}
	void change_wait_time(int x)
	{
		this->wait_time += x;//当等待时间为0时，设置爆炸时间
	}
	int get_tool1_time()
	{
		return this->tool1_time;
	}
	int get_tool2_time()
	{
		return this->tool2_time;
	}
	void change_tool1_time(int x)
	{
		tool1_time += x;
	}
	void change_tool2_time(int x)
	{
		tool2_time += x;
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
void Player::update_location(int x, int y)
{
	//只是用来修改玩家的位置
	this->location.first = x, this->location.second = y;
}
//这里是模拟炸弹爆炸的过程
//包括产生光束，然后一秒钟恢复，判断有没有东西要被消除
void process_of_explode(int x, int y, int range, int type)
{
	//相应的加分和处理游戏是否已经结束
	//呈现十字形的爆炸轨迹
	for (int i = y - range; i <= y + range; i++)
	{
		int ran = rand() % 5;
		char now = map.get_MYmap(x, i);
		if (now != '#')
		{
			if (now <= '9' && now >= '1')
			{
				if (now == '1')
					robot1.change_alive(0);
				else if (now == '2')
					robot2.change_alive(0);
				else
					robot3.change_alive(0);
			}
			if (type == 1)
			{
				//说明是1号玩家的操作
				if (now == '*')
				{
					player1.change_score(Bomb_Wall);
					if (ran == 2 && player1.get_speed() == 1)//此时可获得加速并且表示已经获得加速后不能再获得加速
					{
						player1.change_speed(2);
						player1.change_tool1_time(7 * ONE_SECOND);
					}						
					if (ran == 4 && player1.get_bomb_range() == 1)//此时获得炸弹威力增强
					{
						player1.change_bomb_range(2);
						player1.change_tool2_time(7 * ONE_SECOND);
					}						
				}
				else if (now == '+')
				{
					player1.change_score(Bomb_player);
					player2.change_alive(0);
				}
				else if (now == '@')
					player1.change_alive(0);//自己炸自己
				else if (now <= '9' && now >= '1')
				{
					player1.change_score(Bomb_Robot);
				}				
			}
			else if(type == 2)
			{
				//玩家2的操作
				if (now == '*')
				{
					player2.change_score(Bomb_Wall);
					if (ran == 2 && player2.get_speed() == 1)
					{
						player2.change_speed(2);
						player2.change_tool1_time(7 * ONE_SECOND);
					}						
					if (ran == 4 && player2.get_bomb_range() == 1)
					{
						player2.change_bomb_range(2);
						player2.change_tool2_time(7 * ONE_SECOND);
					}						
				}				
				else if (now == '@')
				{
					player2.change_score(Bomb_player);
					player1.change_alive(0);
				}
				else if (now == '+')
					player2.change_alive(0);
				else if (now <= '9' && now >= '1')
				{
					player2.change_score(Bomb_Robot);
				}			
			}
			map.change_MYmap('O', x, i);
		}
	}//横条
	for (int i = x - range; i <= x + range; i++)
	{
		int ran = rand() % 6;
		char now = map.get_MYmap(i, y);
		if (now != '#')
		{
			if (now <= '9' && now >= '1')
			{
				//就是不管对哪一个玩家而言都是一样的操作
				if (now == '1')
					robot1.change_alive(0);
				else if (now == '2')
					robot2.change_alive(0);
				else
					robot3.change_alive(0);
			}
			if (type == 1)
			{
				//说明是1号玩家的操作
				if (now == '*')
				{
					player1.change_score(Bomb_Wall);
					if (ran == 2 && player1.get_speed() == 1)
					{
						player1.change_speed(2);
						player1.change_tool1_time(7 * ONE_SECOND);
					}
					if (ran == 0 && player1.get_bomb_range() == 1)
					{
						player1.change_bomb_range(2);
						player1.change_tool2_time(7 * ONE_SECOND);
					}
				}				
				else if (now == '+')
				{
					player1.change_score(Bomb_player);
					player2.change_alive(0);
				}
				else if (now == '@')
					player1.change_alive(0);
				else if (now <= '9' && now >= '1')
				{
					player1.change_score(Bomb_Robot);
				}				
			}
			else if (type == 2)
			{
				//玩家2的操作
				if (now == '*')
				{
					player2.change_score(Bomb_Wall);
					if (ran == 2 && player2.get_speed() == 1)
					{
						player2.change_speed(2);
						player2.change_tool1_time(7 * ONE_SECOND);
					}
					if (ran == 0 && player2.get_bomb_range() == 1)
					{
						player2.change_bomb_range(2);
						player2.change_tool2_time(7 * ONE_SECOND);
					}
				}				
				else if (now == '@')
				{
					player2.change_score(Bomb_player);
					player1.change_alive(0);
				}
				else if (now == '+')
					player2.change_alive(0); 
				else if (now <= '9' && now >= '1')
				{
					player2.change_score(Bomb_Robot);
				}
					
			}
			map.change_MYmap('O', i, y);
		}
	}//竖条
	display();
}
void process_of_show(int x, int y, int range)
{
	//等光束展示了1s后，即恢复到原来的效果，应该是能发出光线的地方都被变成了空地
	for (int i = x - range; i <= x + range; i++)
	{
		for (int j = y - range; j <= y + range; j++)
		{
			if (map.get_MYmap(i, j) == 'O')
			{
				map.change_MYmap(' ', i, j);
			}
		}
	}
	display();
}
void init()
{
	//初始化的内容包括
	map.init_MYmap();//初始化地图
	player1.init_Player('@');
	player2.init_Player('+');
	robot1.init_robot('1');
	robot2.init_robot('2');
	robot3.init_robot('3');
}
void show_scores()
{
	int score1 = player1.get_score(), score2 = player2.get_score();
	int alive1 = player1.if_alive(), alive2 = player2.if_alive();
	if (alive1 && alive2)
	{
		printf("玩家1的得分为： %d\n", score1);
		printf("玩家2的得分为： %d\n", score2);
	}
	else
	{
		int winner;
		if (!alive1 && !alive2)//两个人都over了
		{
			winner = score1 > score2 ? 1 : 2;
		}
		else
		{
			winner = alive1 == 1 ? 1 : 2;//其中一个人over了，剩下来的为winner
		}
		printf("Game Over!\n");
		printf("胜利者为玩家%d,祝贺!!!", winner);	
		exit(0);
	}
}
void display()
{
	system("cls");
	map.show_MYmap();
	show_scores();
	if(player1.get_tool1_time() == 7 * ONE_SECOND)
		printf("恭喜玩家1获得加速道具!\n");
	if(player1.get_tool2_time() == 7 * ONE_SECOND)
		printf("恭喜玩家1获得炸弹威力翻倍道具!\n");
	if(player2.get_tool1_time() == 7 * ONE_SECOND)
		printf("恭喜玩家2获得加速道具!\n");
	if(player2.get_tool2_time() == 7 * ONE_SECOND)
		printf("恭喜玩家2获得炸弹威力翻倍道具!\n");
}
void deal_with_input()
{
	//感动感动终于把这里改好了，下次写switch一定记得有break！！！
	char ch;
	if (_kbhit())
	{
		ch = _getch();
		//后面需要对不同的输入做出不同的选择
		int Px1 = player1.get_location().first, Py1 = player1.get_location().second;
		int Px2 = player2.get_location().first, Py2 = player2.get_location().second;
		int speed1 = player1.get_speed(), speed2 = player2.get_speed();//分别是玩家1，2的速度
		//需要将玩家之前在的位置变为空，然后将现在所在的位置，在地图上标为符号
		switch (ch)
		{
		case 'w':
			for (int i = 1; i <= speed1; i++)
			{
				if (map.get_MYmap(Px1 - 1, Py1) == ' ')
				{
					Px1--;
				}
			}//利用Px1和Py1分别代表现在玩家应该存在的地方
			map.change_MYmap(' ', player1.get_location().first, player1.get_location().second);//将现在的位置变为空
			player1.update_location(Px1, Py1);
			map.change_MYmap(player1.get_symbol(), Px1, Py1);//显示玩家现在的位置
			display();
			break;
		case 'a':
			for (int i = 1; i <= speed1; i++)
			{
				if (map.get_MYmap(Px1, Py1-1) == ' ')
				{
					Py1--;
				}
			}
			map.change_MYmap(' ', player1.get_location().first, player1.get_location().second);
			player1.update_location(Px1, Py1);
			map.change_MYmap(player1.get_symbol(), Px1, Py1);
			display();
			break;
		case 's':
			for (int i = 1; i <= speed1; i++)
			{
				if (map.get_MYmap(Px1 + 1, Py1) == ' ')
				{
					Px1++;
				}
			}
			map.change_MYmap(' ', player1.get_location().first, player1.get_location().second);//将现在的位置变为空
			player1.update_location(Px1, Py1);
			map.change_MYmap(player1.get_symbol(), Px1, Py1);//显示玩家现在的位置
			display();
			break;
		case 'd':
			for (int i = 1; i <= speed1; i++)
			{
				if (map.get_MYmap(Px1, Py1 + 1) == ' ')
				{
					Py1++;
				}
			}
			map.change_MYmap(' ', player1.get_location().first, player1.get_location().second);
			player1.update_location(Px1, Py1);
			map.change_MYmap(player1.get_symbol(), Px1, Py1);
			display();
			break;
		case 'i':
			for (int i = 1; i <= speed2; i++)
			{
				if (map.get_MYmap(Px2 - 1, Py2) == ' ')
				{
					Px2--;
				}
			}
			map.change_MYmap(' ', player2.get_location().first, player2.get_location().second);
			player2.update_location(Px2, Py2);
			map.change_MYmap(player2.get_symbol(), Px2, Py2);
			display();
			break;
		case 'k':
			for (int i = 1; i <= speed2; i++)
			{
				if (map.get_MYmap(Px2 + 1, Py2) == ' ')
				{
					Px2++;
				}
			}
			map.change_MYmap(' ', player2.get_location().first, player2.get_location().second);
			player2.update_location(Px2, Py2);
			map.change_MYmap(player2.get_symbol(), Px2, Py2);
			display();
			break;
		case 'j':
			for (int i = 1; i <= speed2; i++)
			{
				if (map.get_MYmap(Px2, Py2 - 1) == ' ')
				{
					Py2--;
				}
			}
			map.change_MYmap(' ', player2.get_location().first, player2.get_location().second);
			player2.update_location(Px2, Py2);
			map.change_MYmap(player2.get_symbol(), Px2, Py2);
			display();
			break;
		case 'l':
			for (int i = 1; i <= speed2; i++)
			{
				if (map.get_MYmap(Px2, Py2 + 1) == ' ')
				{
					Py2++;
				}
			}
			map.change_MYmap(' ', player2.get_location().first, player2.get_location().second);
			player2.update_location(Px2, Py2);
			map.change_MYmap(player2.get_symbol(), Px2, Py2);
			display();
			break;
		case ' ':
			//此时释放了炸弹，然后过3000ms才会爆炸，设定了炸弹爆炸的时间
			player1.change_wait_time(3 * ONE_SECOND);
			px1 = Px1, py1 = Py1;
			break;
		case 13:
			player2.change_wait_time(3 * ONE_SECOND);
			px2 = Px2, py2 = Py2;
			break;
		default:
			break;
		}
	}
}
void deal_with_timer()
{
	//...怎么会有这么多重复的复杂的代码就是说
	//定时事件：炸弹定时3s爆炸，光束显示1s
	//printf("机器人的爆炸时间为%d,炸弹等待时间为%d\n", robot1.get_bomb_time(), robot1.get_wait_time());
	if (robot1.get_bomb_time())
	{
		robot1.change_bomb_time(-1 * ONE_SECOND);
		if (robot1.get_bomb_time() == 0)//说明此时要爆炸了
		{
			process_of_show(robot1.get_bomb_location().first, robot1.get_bomb_location().second, 1);
		}
	}
	if (robot2.get_bomb_time())
	{
		robot2.change_bomb_time(-1 * ONE_SECOND);
		if (robot2.get_bomb_time() == 0)//说明此时要爆炸了
		{
			process_of_show(robot2.get_bomb_location().first, robot2.get_bomb_location().second, 1);
		}
	}
	if (robot3.get_bomb_time())
	{
		robot3.change_bomb_time(-1 * ONE_SECOND);
		if (robot3.get_bomb_time() == 0)//说明此时要爆炸了
		{
			process_of_show(robot3.get_bomb_location().first, robot3.get_bomb_location().second, 1);
		}
	}
	if (robot1.get_wait_time())
	{
		robot1.change_wait_time(-1 * ONE_SECOND);
		
		if (robot1.get_wait_time() == 0)
		{
			process_of_explode(robot1.get_bomb_location().first, robot1.get_bomb_location().second, 1, 0);
			robot1.change_bomb_time(1 * ONE_SECOND);
		}
		//printf("机器人的爆炸时间为%d,炸弹等待时间为%d\n", robot1.get_bomb_time(), robot1.get_wait_time());
	}
	if (robot2.get_wait_time())
	{
		robot2.change_wait_time(-1 * ONE_SECOND);
		if (robot2.get_wait_time() == 0)
		{
			process_of_explode(robot2.get_bomb_location().first, robot2.get_bomb_location().second, 1, 0);
			robot2.change_bomb_time(1 * ONE_SECOND);
		}
	}
	if (robot3.get_wait_time())
	{
		robot3.change_wait_time(-1 * ONE_SECOND);
		if (robot3.get_wait_time() == 0)
		{
			process_of_explode(robot3.get_bomb_location().first, robot3.get_bomb_location().second, 1, 0);
			robot3.change_bomb_time(1 * ONE_SECOND);
		}
	}
	if (player1.get_bomb_time())
	{
		player1.change_bomb_time(-1 * ONE_SECOND);
		if (player1.get_bomb_time() == 0)//说明此时要爆炸了
		{
			process_of_show(px1, py1, player1.get_bomb_range());
		}
	}
	if (player2.get_bomb_time())
	{
		player2.change_bomb_time(-1 * ONE_SECOND);
		if (player2.get_bomb_time() == 0)
		{		
			process_of_show(px2, py2, player2.get_bomb_range());
		}
	}
	if (player1.get_wait_time())
	{
		player1.change_wait_time(-1 * ONE_SECOND);//这个表示等待的时间，即3s
		if (player1.get_wait_time() == 0)
		{
			process_of_explode(px1, py1, player1.get_bomb_range(), 1);
			player1.change_bomb_time(1 * ONE_SECOND);//炸弹爆炸了再显示它的光线1s			
		}
	}
	if (player2.get_wait_time())
	{
		player2.change_wait_time(-1 * ONE_SECOND);
		if (player2.get_wait_time() == 0)
		{
			process_of_explode(px2, py2, player2.get_bomb_range(), 2);
			player2.change_bomb_time(1 * ONE_SECOND);			
		}
	}
	//接下来是道具的时限处理
	if (player1.get_tool1_time())
	{
		player1.change_tool1_time(-1 * ONE_SECOND);
		if (player1.get_tool1_time() == 0)
		{
			player1.change_speed(1);
		}
	}
	if (player1.get_tool2_time())
	{
		player1.change_tool2_time(-1 * ONE_SECOND);
		if (player1.get_tool2_time() == 0)
		{
			player1.change_bomb_range(1);
		}
	}
	if (player2.get_tool1_time())
	{
		player2.change_tool1_time(-1 * ONE_SECOND);
		if (player2.get_tool1_time() == 0)
		{
			player2.change_speed(1);
		}
	}
	if (player2.get_tool2_time())
	{
		player2.change_tool2_time(-1 * ONE_SECOND);
		if (player2.get_tool2_time() == 0)
		{
			player2.change_bomb_range(1);
		}
	}
}
int main()
{
	srand((unsigned)time(NULL));//随机数种子
	init();//作为初始化地图等东西
	map.show_MYmap();
	long long cnt = 0, start_t = clock(), end_t;
	while (1)
	{
		deal_with_input();
		cnt++;
		if (cnt == ONE_SECOND)
		{
			deal_with_timer();
			cnt = 0;
			//要先判断机器人是否还存在，若不存在，就不能让他继续走
			if (robot1.get_alive())
				robot1.walk();//所以是walk写的有问题。。。真的找了好久，太绝望了
			if (robot2.get_alive())
				robot2.walk();
			if (robot3.get_alive())
				robot3.walk();
			int random = rand() % 5;
			if (random == 1 && robot1.get_alive() && !robot1.get_bomb_time() && !robot1.get_wait_time())
			{
				printf("机器人1释放炸弹，小心！\n");
				robot1.change_flag(1);
				robot1.change_bomb_location(robot1.get_location().first, robot1.get_location().second);
				robot1.change_wait_time(3 * ONE_SECOND);
			}
			if (random == 2 && robot2.get_alive() && !robot2.get_bomb_time() && !robot2.get_wait_time())
			{
				printf("机器人2释放炸弹，小心！\n");
				robot2.change_flag(1);
				robot2.change_bomb_location(robot2.get_location().first, robot2.get_location().second);
				robot2.change_wait_time(3 * ONE_SECOND);
			}
			if (random == 3 && robot3.get_alive() && !robot3.get_bomb_time() && !robot3.get_wait_time())
			{
				printf("机器人3释放炸弹，小心！\n");
				robot3.change_flag(1);
				robot3.change_bomb_location(robot3.get_location().first, robot3.get_location().second);
				robot3.change_wait_time(3 * ONE_SECOND);
			}
		}
		end_t = clock();
		if (end_t - start_t == Game_time)
		{
			int s1 = player1.get_score(), s2 = player2.get_score();
			if (s1 > s2)
				printf("Game over!\n胜利者为玩家1,祝贺!!!\n");
			else if (s1 < s2)
				printf("Game over!\n胜利者为玩家2,祝贺!!!\n");
			else
				printf("棋逢对手，平局，再来一局吧!\n");
			exit(0);
		}		
	}
	return 0;
}
