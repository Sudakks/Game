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
#define Bomb_Robot 7
#define Bomb_player 10
const long long ONE_SECOND = 6000;
enum director
{
	UP,
	LEFT,
	DOWN,
	RIGHT
};
void display();
//
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
//
class Player
{
private:
	pair<int, int> location;//现在的位置
	char symbol;//玩家代表的字符
	int bomb_range = 1;
	int score = 0;//玩家得分
	bool alive = 1;//以及判断游戏是否结束
	int speed = 1;//玩家的行走速度
	long long wait_time = 0;//玩家炸弹的爆炸时间
	long long bomb_time = 0;//表示玩家没有投放炸弹
public:
	void init_Player(char character);//初始化位置及玩家符号
	void update_location(int type, int speed);//更新位置
	pair<int, int> get_location();//获取位置
	void change_score(int x)//动态分数
	{
		this->score += x;
	}
	char get_symbol()//获取玩家的字符表示
	{
		return this->symbol;
	}
	int get_bomb_rang()
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
	long long get_wait_time()
	{
		return this->wait_time;
	}
	long long get_bomb_time()
	{
		return this->bomb_time;
	}
	void change_bomb_time(long long x)
	{
		this->bomb_time += x;
	}
	void change_wait_time(long long x)
	{
		this->wait_time += x;//当等待时间为0时，设置爆炸时间
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
void Player::update_location(int type, int speed)
{
	//只是用来修改玩家的位置
	switch (type)
	{
	case UP:
		this->location.first-=speed;
		break;
	case LEFT:
		this->location.second-=speed;
		break;
	case DOWN:
		this->location.first+=speed;
		break;
	case RIGHT:
		this->location.second+=speed;
		break;
	default:
		break;
	}
}
	//这里是模拟炸弹爆炸的过程
	//包括产生光束，然后一秒钟恢复，判断有没有东西要被消除
void process_of_explode(int x, int y, int range, int type)
{
	//相应的加分和处理游戏是否已经结束
	char ord = map.get_MYmap(x, y);//获取玩家的符号
	//呈现十字形的爆炸轨迹
	for (int i = y - range; i <= y + range; i++)
	{
		int ran = rand() % 10;
		char now = map.get_MYmap(x, i);
		if (now != '#')
		{
			if (ord == '@')
			{
				//说明是1号玩家的操作
				if (now == '*')
				{
					player1.change_score(Bomb_Wall);
					if (ran == 9)//此时可获得加速
						player1.change_speed(2);
					if (ran == 4)//此时获得炸弹威力增强
						player1.change_bomb_range(2);
				}				
				else if (now == '+')
				{
					player1.change_score(Bomb_player);
					player2.change_alive(0);
				}
				else if(now <= '9'&& now >= '1')
					player1.change_score(Bomb_Robot);
			}
			else if(ord == '+')
			{
				//玩家2的操作
				if (now == '*')
				{
					player2.change_score(Bomb_Wall);
					if (ran == 9)//此时可获得加速
						player2.change_speed(2);
					if (ran == 4)//此时获得炸弹威力增强
						player2.change_bomb_range(2);
				}				
				else if (now == '@')
				{
					player2.change_score(Bomb_player);
					player1.change_alive(0);
				}
				else if (now <= '9' && now >= '1')
					player2.change_score(Bomb_Robot);
			}
			map.change_MYmap('O', x, i);
		}
	}//横条
	for (int i = x - range; i <= x + range; i++)
	{
		int ran = rand() % 10;
		char now = map.get_MYmap(i, y);
		if (now != '#')
		{
			if (ord == '@')
			{
				//说明是1号玩家的操作
				if (now == '*')
				{
					player1.change_score(Bomb_Wall);
					if (ran == 9)//此时可获得加速
						player1.change_speed(2);
					if (ran == 4)//此时获得炸弹威力增强
						player1.change_bomb_range(2);
				}				
				else if (now == '+')
				{
					player1.change_score(Bomb_player);
					player2.change_alive(0);
				}
				else if (now <= '9' && now >= '1')
					player1.change_score(Bomb_Robot);
			}
			else if (ord == '+')
			{
				//玩家2的操作
				if (now == '*')
				{
					player2.change_score(Bomb_Wall);
					if (ran == 9)//此时可获得加速
						player2.change_speed(2);
					if (ran == 4)//此时获得炸弹威力增强
						player2.change_bomb_range(2);
				}				
				else if (now == '@')
				{
					player2.change_score(Bomb_player);
					player1.change_alive(0);
				}
				else if (now <= '9' && now >= '1')
					player2.change_score(Bomb_Robot);
			}
			map.change_MYmap('O', i, y);
		}
	}//竖条
	display();//然后先显示
}//既可以玩家用，也可以机器人用
void process_of_show(int x, int y, int range)
{
	char ord = map.get_MYmap(x, y);//获取玩家的符号
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
class ROBOT
{
private:
	pair<int, int> location;//现在的位置
	char symbol;//玩家代表的字符
public:
	void init_robot(char character);
	pair<int, int> get_location()//获取位置
	{
		return this->location;
	}
	char get_symbol()//获取玩家的字符表示
	{
		return this->symbol;
	}
	void walk();
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
	int x = this->get_location().first;
	int y = this->get_location().second;
	map.change_MYmap(' ', x, y);
	while (1)
	{
		int direct = rand() % 4;
		if (direct == UP && map.get_MYmap(x - 1, y) == ' ')
		{
			this->location.first--;			
			x--;
			map.change_MYmap(character, x, y);
			display();
			break;
		}
		else if (direct == DOWN && map.get_MYmap(x + 1, y) == ' ')
		{
			this->location.first++;
			x++;
			map.change_MYmap(character, x, y);
			display();
			break;
		}
		else if (direct == LEFT && map.get_MYmap(x, y - 1) == ' ')
		{
			this->location.second--;
			y--;
			map.change_MYmap(character, x, y);
			display();
			break;
		}
		else if (direct == RIGHT && map.get_MYmap(x, y + 1) == ' ')
		{
			this->location.second++;
			y++;
			map.change_MYmap(character, x, y);
			display();
			break;
		}
	}
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
	}
}
void display()
{
	system("cls");
	map.show_MYmap();
	show_scores();
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
		//需要将玩家之前在的位置变为空，然后将现在所在的位置，在地图上标为符号
		switch (ch)
		{
		case 'w':
			if (map.get_MYmap(Px1-1, Py1) == ' ')//判断要走的位置是否为空
			{
				map.change_MYmap(' ',Px1, Py1);//将现在的位置变为空
				player1.update_location(UP, player1.get_speed());//更改玩家的位置
				map.change_MYmap(player1.get_symbol(), Px1 - 1, Py1);//显示玩家现在的位置
				display();
			}
			break;
		case 'a':
			if (map.get_MYmap(Px1, Py1-1) == ' ')
			{
				map.change_MYmap(' ', Px1, Py1);
				player1.update_location(LEFT, player1.get_speed());
				map.change_MYmap(player1.get_symbol(), Px1, Py1-1);
				display();
			}
			break;
		case 's':
			if (map.get_MYmap(Px1+1, Py1) == ' ')
			{
				map.change_MYmap(' ', Px1, Py1);
				player1.update_location(DOWN, player1.get_speed());
				map.change_MYmap(player1.get_symbol(), Px1 + 1, Py1);
				display();
			}
			break;
		case 'd':
			if (map.get_MYmap(Px1, Py1+1) == ' ')
			{
				map.change_MYmap(' ', Px1, Py1);
				player1.update_location(RIGHT, player1.get_speed());
				map.change_MYmap(player1.get_symbol(), Px1, Py1 + 1);
				display();
			}
				break;
		case 'i':
			if (map.get_MYmap(Px2-1, Py2) == ' ')
			{
				map.change_MYmap(' ',Px2, Py2);
				player2.update_location(UP, player2.get_speed());
				map.change_MYmap(player2.get_symbol(), Px2 - 1, Py2);				
				display();
			}
				break;
		case 'k':
			if (map.get_MYmap(Px2+1,Py2) == ' ')
			{
				map.change_MYmap(' ', Px2, Py2);
				player2.update_location(DOWN, player2.get_speed());
				map.change_MYmap(player2.get_symbol(), Px2 + 1, Py2);
				display();
			}
				break;
		case 'j':
			if (map.get_MYmap(Px2, Py2-1) == ' ')
			{
				map.change_MYmap(' ',Px2, Py2);
				player2.update_location(LEFT, player2.get_speed());
				map.change_MYmap(player2.get_symbol(), Px2, Py2 - 1);			
				display();
			}
				break;
		case 'l':
			if (map.get_MYmap(Px2,Py2+1) == ' ')
			{
				map.change_MYmap(' ', Px2, Py2);
				player2.update_location(RIGHT, player2.get_speed());
				map.change_MYmap(player2.get_symbol(), Px2, Py2 + 1);
				display();
			}
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
	//定时事件：炸弹定时3s爆炸，光束显示1s
	if (player1.get_bomb_time())
	{
		player1.change_bomb_time(-1 * ONE_SECOND);
		if (player1.get_bomb_time() == 0)//说明此时要爆炸了
		{
			process_of_show(px1, py1, player1.get_bomb_rang());
		}
	}
	if (player2.get_bomb_time())
	{
		player2.change_bomb_time(-1 * ONE_SECOND);
		if (player2.get_bomb_time() == 0)
		{		
			process_of_show(px2, py2, player2.get_bomb_rang());
		}
	}
	if (player1.get_wait_time())
	{
		player1.change_wait_time(-1 * ONE_SECOND);//这个表示等待的时间，即3s
		if (player1.get_wait_time() == 0)
		{
			process_of_explode(px1, py1, player1.get_bomb_rang(), 1);
			player1.change_bomb_time(1 * ONE_SECOND);//炸弹爆炸了再显示它的光线1s			
		}
	}
	if (player2.get_wait_time())
	{
		player2.change_wait_time(-1 * ONE_SECOND);
		if (player2.get_wait_time() == 0)
		{
			process_of_explode(px2, py2, player2.get_bomb_rang(), 2);
			player2.change_bomb_time(1 * ONE_SECOND);			
		}
	}
}
int main()
{
	srand((unsigned)time(NULL));//随机数种子
	init();//作为初始化地图等东西
	map.show_MYmap();
	long long cnt = 1;
	while (1)
	{
		//robot1.walk();		
		//robot2.walk();
		//robot3.walk();
		deal_with_input();
		cnt++;
		if (cnt == ONE_SECOND)
		{
			deal_with_timer();
			cnt = 0;
		}
	}
	return 0;
}
