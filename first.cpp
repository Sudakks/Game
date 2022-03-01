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
#define row 17//0��16��ǽ��0��20��ǽ
#define col 21//��ͼ�ĳߴ�
#define H_WALL 70//��ʾ�м��Ӳǽ����
#define S_WALL 90//��ʾ�м���ǽ�ĸ���
#define ONE_SECOND 10//����Ҳ�֪���Ǹ�ʲô����
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
MAP map;//����һ����ͼ����
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
	//����������Χǽ�ģ�����Ӳǽ
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
	//�м�����Ӳǽ
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
	//�м�������ǽ
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
	pair<int, int> location;//���ڵ�λ��
	char symbol;//��Ҵ�����ַ�
public:
	void init_Player(char character);//��ʼ��λ�ü���ҷ���
	void update_location(int type);//����λ��
	pair<int, int> get_location();//��ȡλ��
	char get_symbol()//��ȡ��ҵ��ַ���ʾ
	{
		return this->symbol;
	}
};
Player player1;
Player player2;//˫���
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
	//ֻ�������޸���ҵ�λ��
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
	//��ʼ�������ݰ���
	map.init_MYmap();//��ʼ����ͼ
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
		//������Ҫ�Բ�ͬ������������ͬ��ѡ��
		int Px1 = player1.get_location().first, Py1 = player1.get_location().second;
		int Px2 = player2.get_location().first, Py2 = player2.get_location().second;
		//��Ҫ�����֮ǰ�ڵ�λ�ñ�Ϊ�գ�Ȼ���������ڵ�λ�ã��ڵ�ͼ�ϱ�Ϊ����
		switch (ch)
		{
		case 'w':
			if (map.get_MYmap(Px1-1, Py1) == ' ')//�ж�Ҫ�ߵ�λ���Ƿ�Ϊ��
			{
				map.change_MYmap(' ',Px1, Py1);//�����ڵ�λ�ñ�Ϊ��
				player1.update_location(UP);//������ҵ�λ��
				map.change_MYmap(player1.get_symbol(), Px1 - 1, Py1);//��ʾ������ڵ�λ��
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
	srand((unsigned)time(NULL));//���������
	init();//��Ϊ��ʼ����ͼ�ȶ���
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